#include "../../Inc/network/network.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

ARP::ARP(const uint8& networkID)
	:	m_networkID(networkID)
{
	CMOS& cmos = CMOS::get();
	cmos.semaphore_create(this);
}







bool ARP::probe(const Array<uint8>& protocolAddress, uint8 numberOfRetries, uint32 intervalInMilliSeconds)
{
	//	Get own MAC Address
	Network& network = Network::get(m_networkID);
	I_PhysicalLayer& physicalLayer = network.m_physicalLayer;
	const Array<uint8>& macAddress =  physicalLayer.get_macAddress();
	
	
	ARP_Frame arpFrame;
	
	arpFrame.hardwareType						= ARP_Frame::e_hardwareType::ETHERNET_10MB;
	arpFrame.protocolType						= ARP_Frame::e_protocolType::IPv4;
	arpFrame.hardwareAddressLength	= macAddress.get_size();
	arpFrame.protocolAddressLength	= protocolAddress.get_size();
	arpFrame.operationCode					= ARP_Frame::e_operationCode::REQUEST;
	
	for(uint32 i = 0; i < macAddress.get_size(); i++)
	{
		arpFrame.senderHardwareAddress[i] = macAddress[i];
	}
	
	for(uint32 i = 0; i < protocolAddress.get_size(); i++)
	{
		arpFrame.targetProtocolAddress[i] = protocolAddress[i];
	}
	
	
	EthernetFrame ethernetFrame;
	for(uint32 i = 0; i < 6; i++)
	{
		ethernetFrame.MAC_destination[i] = 0xFF;
	}
	for(uint32 i = 0; i < 6; i++)
	{
		ethernetFrame.MAC_source[i] = macAddress[i];
	}
	ethernetFrame.etherType = EthernetFrame::e_etherType::ARP;
	ethernetFrame.payload = arpFrame.convertToByteStream();
	
	
	//	If ARP Table already contains the requested IP Address, erase it to renew it
	if(contains_protocolAddress(protocolAddress) == true)
	{
		CMOS& cmos = CMOS::get();
		cmos.semaphore_lock(this);
		m_table.erase(protocolAddress);
		cmos.semaphore_unlock(this);
	}
	
	
	//	Send ARP Request maximum <numberOfRetries> Times with a <intervalInMilliSeconds> MilliSeconds Timeout
	//	Stop either on ARP Reply or Timeout
	CMOS& cmos = CMOS::get();
	const Array<uint8> byteStream(ethernetFrame.convertToByteStream());
	for(uint32 i = 0; i < numberOfRetries; i++)
	{
		physicalLayer.tx(byteStream);
		cmos.sleep_ms(intervalInMilliSeconds);
		
		//	Check for ARP Reply (indicated through updated ARP Table)
		if(contains_protocolAddress(protocolAddress) == true)
		{
			return(true);
		}
	}
	return(false);
}


void ARP::update(const Array<uint8>& protocolAddress, const Array<uint8>& hardwareAddress)
{
	CMOS& cmos = CMOS::get();
	if(contains_protocolAddress(protocolAddress) == true)
	{
		cmos.semaphore_lock(this);
		m_table[protocolAddress] = hardwareAddress;
		cmos.semaphore_unlock(this);
	}
	else
	{
		Pair<Array<uint8>, Array<uint8>> entry(protocolAddress, hardwareAddress);
		cmos.semaphore_lock(this);
		m_table += entry;
		cmos.semaphore_unlock(this);
	}
}







bool ARP::contains_protocolAddress(const Array<uint8>& protocolAddress) const
{
	CMOS& cmos = CMOS::get();
	cmos.semaphore_lock(this);
	const bool result = m_table.contains(protocolAddress);
	cmos.semaphore_unlock(this);
	return(result);
}


bool ARP::contains_hardwareAddress(const Array<uint8>& hardwareAddress) const
{
	CMOS& cmos = CMOS::get();
	cmos.semaphore_lock(this);
	for(auto& i: m_table)
	{
		if(i.second() == hardwareAddress)
		{
			cmos.semaphore_unlock(this);
			return(true);
		}
	}
	cmos.semaphore_unlock(this);
	return(false);
}


Array<uint8> ARP::get_hardwareAddress(const Array<uint8>& protocolAddress, bool doRequestIfNotInCache, uint32 timeoutInMilliseconds)
{
	//	Dont proceed if Network isnt attached
	if(m_networkID == Network::networkID_invalid)
	{
		return(Array<uint8>());
	}
	
	
	if(contains_protocolAddress(protocolAddress) == true)
	{
		CMOS& cmos = CMOS::get();
		cmos.semaphore_lock(this);
		Array<uint8> result = m_table[protocolAddress];
		cmos.semaphore_unlock(this);
		return(result);
	}
	
	
	if(doRequestIfNotInCache == false || protocolAddress.get_size() != 4)
	{
		return(Array<uint8>());
	}
	
	
	//	Send Request
	Network& network = Network::get(m_networkID);
	ARP_Frame arpRequest;
	arpRequest.hardwareType						= ARP_Frame::e_hardwareType::ETHERNET_10MB;
	arpRequest.protocolType						= ARP_Frame::e_protocolType::IPv4;
	arpRequest.hardwareAddressLength	= 6;
	arpRequest.protocolAddressLength	= 4;
	arpRequest.operationCode					= ARP_Frame::e_operationCode::REQUEST;
	
	const Array<uint8>& macAddress = network.m_physicalLayer.get_macAddress();
	for(uint32 i = 0; i < 6; i++)
	{
		arpRequest.senderHardwareAddress[i] = macAddress[i];
	}
	
	const Array<uint8>& ipAddress = network.m_ip.get_ipAddress();
	for(uint32 i = 0; i < 4; i++)
	{
		arpRequest.senderProtocolAddress[i] = ipAddress[i];
	}
	
	for(uint32 i = 0; i < 6; i++)
	{
		arpRequest.targetHardwareAddress[i] = 0;
	}
	
	for(uint32 i = 0; i < 4; i++)
	{
		arpRequest.targetProtocolAddress[i] = protocolAddress[i];
	}
	
	
	//	Send Packet over Ethernet
	network.m_ethernet.tx(arpRequest.convertToByteStream(), Array<uint8>(0xFF, 6), EthernetFrame::e_etherType::ARP);
	
	
	//	Wait for ARP Reply to update the Cache or Timeout
	uint32 timeoutCounter = 0;
	CMOS& cmos = CMOS::get();
	while(contains_protocolAddress(protocolAddress) == false)
	{
		cmos.sleep_ms(50);
		timeoutCounter += 50;
		if(timeoutCounter > timeoutInMilliseconds)
		{
			return(Array<uint8>());
		}
	}
	
	
	//	Return the Hardware Address
	cmos.semaphore_lock(this);
	Array<uint8> result = m_table[protocolAddress];
	cmos.semaphore_unlock(this);
	
	return(result);
}


Array<uint8> ARP::get_protocolAddress(const Array<uint8>& hardwareAddress) const
{
	if(contains_hardwareAddress(hardwareAddress) == true)
	{
		CMOS& cmos = CMOS::get();
		cmos.semaphore_lock(this);
		Array<uint8> result = m_table[hardwareAddress];
		cmos.semaphore_unlock(this);
		return(result);
	}
	return(Array<uint8>());
}







void ARP::handlePacket(const ARP_Frame& arpFrame)
{
	//	Dont proceed if Network isnt attached
	if(m_networkID == Network::networkID_invalid)
	{
		return;
	}
	
	
	//	Only serve IPv4 Protocol
	if(arpFrame.protocolType != ARP_Frame::e_protocolType::IPv4)
	{
		return;
	}
	
	
	//	Request
	switch(arpFrame.operationCode)
	{
		case ARP_Frame::e_operationCode::REQUEST:
		{
			//	Examine Target Protocol Address
			Array<uint8> targetProtocolAddress(0, 4);
			for(uint32 i = 0; i < 4; i++)
			{
				targetProtocolAddress[i] = arpFrame.targetProtocolAddress[i];
			}
			
			
			//	Check if we are the Target of the Request
			Network& network = Network::get(m_networkID);
			if(network.m_ip.get_ipAddress() != targetProtocolAddress)
			{
				return;
			}
			
			
			//	Reply to the Request
			ARP_Frame arpReply;
			arpReply.hardwareType						= ARP_Frame::e_hardwareType::ETHERNET_10MB;
			arpReply.protocolType						= ARP_Frame::e_protocolType::IPv4;
			arpReply.hardwareAddressLength	= 6;
			arpReply.protocolAddressLength	= 4;
			arpReply.operationCode					= ARP_Frame::e_operationCode::REPLY;
			
			const Array<uint8>& macAddress = network.m_physicalLayer.get_macAddress();
			for(uint32 i = 0; i < 6; i++)
			{
				arpReply.senderHardwareAddress[i] = macAddress[i];
			}
			
			for(uint32 i = 0; i < 4; i++)
			{
				arpReply.senderProtocolAddress[i] = arpFrame.targetProtocolAddress[i];
			}
			
			for(uint32 i = 0; i < 6; i++)
			{
				arpReply.targetHardwareAddress[i] = arpFrame.senderHardwareAddress[i];
			}
			
			for(uint32 i = 0; i < 4; i++)
			{
				arpReply.targetProtocolAddress[i] = arpFrame.senderProtocolAddress[i];
			}
			
			
			//	Send Packet over Ethernet
			Array<uint8> targetMacAddress(0, 6);
			for(uint32 i = 0; i < 6; i++)
			{
				targetMacAddress[i] = arpReply.targetHardwareAddress[i];
			}
			network.m_ethernet.tx(arpReply.convertToByteStream(), targetMacAddress, EthernetFrame::e_etherType::ARP);
		}
		break;
		
		case ARP_Frame::e_operationCode::REPLY:
		{
			//	Update ARP Table on ARP Reply
			if(arpFrame.operationCode == ARP_Frame::e_operationCode::REPLY)
			{
				//	Examine Protocol Address
				Array<uint8> protocolAddress(0, arpFrame.protocolAddressLength);
				for(uint32 i = 0; i < protocolAddress.get_size(); i++)
				{
					protocolAddress[i] = arpFrame.senderProtocolAddress[i];
				}
				
				
				//	Examine Hardware Address
				Array<uint8> hardwareAddress(0, arpFrame.hardwareAddressLength);
				for(uint32 i = 0; i < hardwareAddress.get_size(); i++)
				{
					hardwareAddress[i] = arpFrame.senderHardwareAddress[i];
				}
				
				
				//	Update ARP Table
				update(protocolAddress, hardwareAddress);
			}
		}
		break;
		
		case ARP_Frame::e_operationCode::REQUEST_REVERSE:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::REPLY_REVERSE:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::DRARP_REQUEST:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::DRARP_REPLY:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::DRARP_ERROR:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::InARP_REQUEST:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::InARP_REPLY:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::ARP_NAK:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::MARS_REQUEST:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::MARS_MULTI:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::MARS_MSERV:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::MARS_JOIN:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::MARS_LEAVE:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::MARS_NAK:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::MARS_UNSERV:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::MARS_SJOIN:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::MARS_SLEAVE:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::MARS_GROUPLIST_REQUEST:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::MARS_GROUPLIST_REPLY:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::MARS_REDIRECT_MAP:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::MAPOS_UNARP:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::OP_EXP1:
		{
			
		}
		break;
		
		case ARP_Frame::e_operationCode::OP_EXP2:
		{
			
		}
		break;
		
		default:
		{
			
		}
		return;
	}
}