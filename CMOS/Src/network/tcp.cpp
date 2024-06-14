#include "../../Inc/network/network.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

TCP::TCP(const uint8& networkID)
	:	m_networkID(networkID)
{
	if(CMOS::get().semaphore_create(this) != OK)
	{
		return;
	}
}


TCP::~TCP()
{
	
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

void TCP::handlePacket(const TCP_Frame& tcpFrame)
{
	//	Search the Socket that the Packet is addressed to
	bool socketFound = false;
	for(auto& i: m_sockets)
	{
		if(i->m_sourcePort == tcpFrame.destinationPort)
		{
			if(i->m_destinationPort == tcpFrame.sourcePort)
			{
				i->handlePacket(tcpFrame);
				socketFound = true;
				break;
			}
		}
	}
	
	
	//	If Socket wasn't found, do some Error Handling
	if(socketFound == false)
	{
		//	ToDo
	}
}







TCP_Socket* TCP::createSocket(const Array<uint8>& destinationIpAddress, uint16 destinationPort, uint16 sourcePort, uint32 maximumSegmentSize, uint32 receiveBufferSize)
{
	//	Dont proceed if Network isnt attached
	if(m_networkID == Network::networkID_invalid)
	{
		return(nullptr);
	}
	
	
	CMOS& cmos = CMOS::get();
	cmos.semaphore_lock(this);
	
	
	//	Check if Socket already exists
	for(auto& i: m_sockets)
	{
		if(i->m_destinationIpAddress == destinationIpAddress)
		{
			if(i->m_destinationPort == destinationPort)
			{
				if(i->m_sourcePort == sourcePort)
				{
					return(nullptr);
				}
			}
		}
	}
	
	
	cmos.semaphore_unlock(this);
	
	
	//	Create TCP Socket Instance to return
	const Array<uint8>& sourceIpAddress = Network::get(m_networkID).getIpAddress();
	TCP_Socket* socket = new TCP_Socket(m_networkID, sourceIpAddress, sourcePort, destinationIpAddress, destinationPort, maximumSegmentSize, receiveBufferSize);
	cmos.semaphore_lock(this);
	m_sockets += socket;
	cmos.semaphore_unlock(this);
	
	return(socket);
}