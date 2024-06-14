#include "../../Inc/network/network.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

DNS::DNS(const uint8& networkID)
	:	m_networkID(networkID),
		m_threadID(CMOS::threadID_invalid),
		m_table(),
		dnsHeaderIDCounter(0)
{
	//	Create Semaphore for Thread safe Programming
	CMOS& cmos = CMOS::get();
	if(cmos.semaphore_create(this) != OK)
	{
		return;
	}
	
	
	//	Create Time to live Thread
	m_threadID = cmos.thread_create(&DNS::timeToLiveCounter, this, "DNS Time to Live", 50, 512);
}


DNS::~DNS()
{
	
}







void DNS::timeToLiveCounter()
{
	CMOS& cmos = CMOS::get();
	while(1)
	{
		//	Lock Semaphore prior to updating the Cache
		if(cmos.semaphore_lock(this) != OK)
		{
			return;
		}
		
		
		//	Count down Time to live Counter and erase Entries if outdated
		for(auto& i: m_table)
		{
			uint32& timeToLive = i.second().second();
			if(timeToLive > 0)
			{
				timeToLive--;
				if(timeToLive == 0)
				{
					m_table.erase(i.first());
				}
			}
			else
			{
				m_table.erase(i.first());
			}
		}
		
		
		//	Unlock Semaphore
		if(cmos.semaphore_unlock(this) != OK)
		{
			return;
		}
		
		
		//	Sleep 1 Second
		cmos.sleep_sec(1);
	}
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

void DNS::handlePacket(const DNS_Frame& dnsFrame)
{
	if(dnsFrame.query == false && dnsFrame.opCode == DNS_Frame::e_opCode::STANDARD_QUERY)
	{
		if(dnsFrame.responseCode == DNS_Frame::e_responseCode::NO_ERROR && dnsFrame.truncation == false)
		{
			if(dnsFrame.answerRecords.get_size() > 0)
			{
				for(auto& i: dnsFrame.answerRecords)
				{
					if(i.type == DNS_Frame::e_qType::A && i.resourceClass == 0x0001 && i.timeToLive > 0 && i.length == 4)
					{
						Pair<String, Pair<Array<uint8>, uint32>> entry;
						entry.first() = i.name;
						entry.second().first() = i.data;
						entry.second().second() = i.timeToLive;
						m_table += entry;
					}
				}
			}
		}
	}
}







Array<uint8> DNS::resolve(const String& domainName, uint32 timeoutInMilliseconds)
{
	//	Dont proceed if Network isnt attached
	if(m_networkID == Network::networkID_invalid)
	{
		return(Array<uint8>());
	}
	
	
	//	Thread Safe Programming
	//	Lock Semaphore first
	CMOS& cmos = CMOS::get();
	if(cmos.semaphore_lock(this) != OK)
	{
		return(Array<uint8>());
	}
	
	
	//	Lookup in Cache
	if(m_table.contains(domainName) == true)
	{
		//	Get IP Address
		Array<uint8> ipAddress = m_table[domainName].first();
		
		
		//	Unlock Semaphore
		cmos.semaphore_unlock(this);
		
		
		return(ipAddress);
	}
	
	
	//	If not found in Cache, start a DNS Query to the DNS Server
	//	Get DNS Server IP Address from DHCP Protocol
	Network& network = Network::get(m_networkID);
	const Array<Array<uint8>>& dnsServerIpAddresses = network.m_dhcp.get_ipAddressDnsServer();
	if(dnsServerIpAddresses.get_size() == 0)
	{
		//	Unlock Semaphore
		cmos.semaphore_unlock(this);
		
		
		return(Array<uint8>());
	}
	
	
	//	Build DNS Query Frame
	DNS_Frame dnsQuery;
	dnsQuery.ID = dnsHeaderIDCounter;
	dnsQuery.query = true;
	dnsQuery.opCode = DNS_Frame::e_opCode::STANDARD_QUERY;
	dnsQuery.recursionDesired = true;
	DNS_Frame::s_question question;
	question.name = domainName;
	question.type = DNS_Frame::e_qType::A;
	question.questionClass = 0x0001;
	dnsQuery.questions += question;
	
	
	dnsHeaderIDCounter++;
	
	
	//	Send DNS Packet over UDP
	if(network.m_udp.tx(dnsQuery.convertToByteStream(), 53, 53, dnsServerIpAddresses[0]) != OK)
	{
		//	Unlock Semaphore
		cmos.semaphore_unlock(this);
		
		
		return(Array<uint8>());
	}
	
	
	//	Wait for DNS Entry in Table (DNS Reply Packet Handling is performed by another Thread via "handlePacket" Function)
	uint32 timeoutCounter = 0;
	while(1)
	{
		//	Give the other Threads Time to update the Cache
		cmos.semaphore_unlock(this);
		cmos.sleep_ms(50);
		cmos.semaphore_lock(this);
		
		
		//	Lookup the desired Domain Name in the (now hopefully updated) Cache
		if(m_table.contains(domainName) == true)
		{
			break;
		}
		
		
		//	Timeout Counter
		timeoutCounter += 50;
		if(timeoutCounter > timeoutInMilliseconds)
		{
			cmos.semaphore_unlock(this);
			return(Array<uint8>());
		}
	}
	
	
	//	Get IP Address from Cache
	Array<uint8> ipAddress = m_table[domainName].first();
	
	
	//	Unlock Semaphore
	cmos.semaphore_unlock(this);
	
	
	return(ipAddress);
}