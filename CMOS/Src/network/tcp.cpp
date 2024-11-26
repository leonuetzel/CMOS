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

void TCP::handlePacket(const TCP_Frame& tcpFrame, const Array<uint8>& sourceIpAddress)
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
	
	
	//	If Socket wasn't found, check if it is a Request to close a previous Connection or an unwanted Connection Attempt
	if(socketFound == false)
	{
		if(tcpFrame.synchronize == true)
		{
			//	This is an unwanted Connection Attempt and we will simply send a RST Packet to abort it
			TCP_Frame tcpFrameResponse;
			tcpFrameResponse.sourcePort											= tcpFrame.destinationPort;
			tcpFrameResponse.destinationPort								= tcpFrame.sourcePort;
			tcpFrameResponse.sequenceNumber									= 0;
			tcpFrameResponse.ackNumber											= 0;
			tcpFrameResponse.ack 														= false;
			tcpFrameResponse.finish													= false;
			tcpFrameResponse.reset													= true;
			tcpFrameResponse.receiveWindow									= 0;
			
			
			//	Send Packet over IP Layer
			Network& network = Network::get(m_networkID);
			network.m_ip.tx(tcpFrameResponse.convertToByteStream(network.getIpAddress(), sourceIpAddress), IP_Frame::e_protocol::TCP, sourceIpAddress);
			return;
		}
		if(tcpFrame.finish == true)
		{
			//	There is no Socket to close, but since the foreign Host wants to close the Connection, we simply act like there was a Socket and we close it now
			TCP_Frame tcpFrameResponse;
			tcpFrameResponse.sourcePort											= tcpFrame.destinationPort;
			tcpFrameResponse.destinationPort								= tcpFrame.sourcePort;
			tcpFrameResponse.sequenceNumber									= tcpFrame.ackNumber;
			tcpFrameResponse.ackNumber											= tcpFrame.sequenceNumber + 1;
			tcpFrameResponse.ack 														= true;
			tcpFrameResponse.finish													= true;
			tcpFrameResponse.receiveWindow									= 0;
			
			
			//	Send Packet over IP Layer
			Network& network = Network::get(m_networkID);
			network.m_ip.tx(tcpFrameResponse.convertToByteStream(network.getIpAddress(), sourceIpAddress), IP_Frame::e_protocol::TCP, sourceIpAddress);
			return;
		}
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


feedback TCP::deleteSocket(TCP_Socket* tcpSocket)
{
	//	Dont proceed if Network isnt attached
	if(m_networkID == Network::networkID_invalid)
	{
		return(FAIL);
	}
	
	
	CMOS& cmos = CMOS::get();
	cmos.semaphore_lock(this);
	
	
	//	Search for Socket to delete
	const uint16 numberOfSockets = m_sockets.get_size();
	for(uint32 i = 0; i < numberOfSockets; i++)
	{
		TCP_Socket* socket = m_sockets[i];
		if(socket == tcpSocket)
		{
			//	Only delete Socket if it is in CLOSED State
			if(socket->get_state() != TCP_Socket::e_state::CLOSED)
			{
				//	Unlock Semaphore
				cmos.semaphore_unlock(this);
				return(FAIL);
			}
			
			
			//	Delete Socket
			m_sockets.erase(i);
			delete socket;
			
			
			//	Unlock Semaphore
			cmos.semaphore_unlock(this);
			return(OK);
		}
	}
	
	
	//	Unlock Semaphore
	cmos.semaphore_unlock(this);
	return(OK);
}