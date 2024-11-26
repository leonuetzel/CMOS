#include "../../Inc/network/network.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

TCP_Socket::TCP_Socket(const uint8& networkID, const Array<uint8>& sourceIpAddress, uint16 sourcePort, const Array<uint8>& destinationIpAddress, uint16 destinationPort, uint32 maximumSegmentSize, uint32 receiveBufferSize)
	:	m_networkID(networkID),
		m_state(e_state::CLOSED),
		
		m_sourceIpAddress(sourceIpAddress),
		m_destinationIpAddress(destinationIpAddress),
		m_sourcePort(sourcePort),
		m_destinationPort(destinationPort),
		
		m_receiveBuffer(receiveBufferSize),
		m_ownMaximumSegmentSize(maximumSegmentSize),
		m_foreignMaximumSegmentSize(0),
		m_sequenceNumber(0),
		m_lastForeignSequenceNumberACKed(0),
		m_sequenceNumbersAndTransmittedPackets()
{
	//	Create Semaphore for thread-safe Access
	if(CMOS::get().semaphore_create(this) != OK)
	{
		return;
	}
	
	
	//	Get Maximum Transmission Unit (MTU) of underlying Layer
	//	Maximum Segment Size (MSS) = MTU - 20 Byte IP Header - 20 Byte TCP Header - 8 Byte Point-to-Point Protocol (PPP)
	//	In Case of Ethernet (Payload Length = 1500 Byte max): MSS = 1452 Byte
	Network& network = Network::get(m_networkID);
	const uint32 maximumTransmissionUnit = network.m_physicalLayer.get_maximumTransmissionUnit();
	const uint32 maximumSegmentSizeMaximum = (maximumTransmissionUnit - 20 - 20 - 8);
	if(m_ownMaximumSegmentSize > maximumSegmentSizeMaximum)
	{
		m_ownMaximumSegmentSize = maximumSegmentSizeMaximum;
	}
}


TCP_Socket::~TCP_Socket()
{
	if(m_state != e_state::CLOSED)
	{
		close();
	}
}







feedback TCP_Socket::process_connectionEstablishment(const TCP_Frame& tcpFrame)
{
	//	Connection Establishment has already started but not yet been finished
	if(tcpFrame.synchronize == true && tcpFrame.ack == true)
	{
		//	Lock Semaphore for thread-safe Access
		CMOS& cmos = CMOS::get();
		cmos.semaphore_lock(this);
		
		
		//	Check for ACK Number
		if(m_sequenceNumbersAndTransmittedPackets.contains(tcpFrame.ackNumber) == true)
		{
			m_sequenceNumbersAndTransmittedPackets.erase(tcpFrame.ackNumber);
			
			
			//	SYN/ACK Packet received as Answer to a SYN Packet for connecting the TCP Socket
			//	Extract foreign Maximum Segment Size if present, so we never send too large Datagrams
			for(auto& i: tcpFrame.options)
			{
				if(i.kind == TCP_Frame::e_optionKind::MAXIMUM_SEGMENT_SIZE)
				{
					if(i.data.get_size() == 2)
					{
						m_foreignMaximumSegmentSize = (i.data[0] << 8) | i.data[1];
						break;
					}
				}
			}
			
			
			//	Save last foreign ACKed Sequence Number
			m_lastForeignSequenceNumberACKed = tcpFrame.sequenceNumber + 1;
			
			
			//	Send an ACK Packet to confirm the SYN/ACK Packet
			TCP_Frame tcpFrame_reply;
			tcpFrame_reply.sourcePort											= m_sourcePort;
			tcpFrame_reply.destinationPort								= m_destinationPort;
			tcpFrame_reply.sequenceNumber									= m_sequenceNumber;
			tcpFrame_reply.ackNumber											= m_lastForeignSequenceNumberACKed;
			tcpFrame_reply.ack														= true;
			tcpFrame_reply.receiveWindow									= m_receiveBuffer.get_size() - m_receiveBuffer.get_numberOfUnread();
			
			
			//	Unlock Semaphore
			cmos.semaphore_unlock(this);
			
			
			if(Network::get(m_networkID).m_ip.tx(tcpFrame_reply.convertToByteStream(m_sourceIpAddress, m_destinationIpAddress), IP_Frame::e_protocol::TCP, m_destinationIpAddress) == OK)
			{
				return(OK);
			}
		}
		
		
		//	Unlock Semaphore if not done yet
		if(cmos.semaphore_isLocked(this) == true)
		{
			cmos.semaphore_unlock(this);
		}
	}
	return(FAIL);
}


feedback TCP_Socket::process_connectionClosingByForeign(const TCP_Frame& tcpFrame)
{
	//	Lock Semaphore for thread-safe Access
	CMOS& cmos = CMOS::get();
	cmos.semaphore_lock(this);
	
	
	//	Save last foreign ACKed Sequence Number
	m_lastForeignSequenceNumberACKed = tcpFrame.sequenceNumber + 1;
	
	
	//	Send an ACK Packet to confirm the FIN Packet
	TCP_Frame tcpFrame_reply;
	tcpFrame_reply.sourcePort											= m_sourcePort;
	tcpFrame_reply.destinationPort								= m_destinationPort;
	tcpFrame_reply.sequenceNumber									= m_sequenceNumber;
	tcpFrame_reply.ackNumber											= m_lastForeignSequenceNumberACKed;
	tcpFrame_reply.ack														= true;
	tcpFrame_reply.receiveWindow									= m_receiveBuffer.get_size() - m_receiveBuffer.get_numberOfUnread();
	
	
	//	Unlock Semaphore
	cmos.semaphore_unlock(this);
	
	
	//	Send ACK Packet over IP Layer
	Network& network = Network::get(m_networkID);
	if(network.m_ip.tx(tcpFrame_reply.convertToByteStream(m_sourceIpAddress, m_destinationIpAddress), IP_Frame::e_protocol::TCP, m_destinationIpAddress) != OK)
	{
		return(FAIL);
	}
	
	
	//	Send FIN Packet only if we want to close the Connection too
	//	This is done only from the ESTABLISHED State
	if(m_state == e_state::ESTABLISHED)
	{
		//	Send an ACK Packet to confirm the FIN Packet
		TCP_Frame tcpFrame_fin;
		tcpFrame_fin.sourcePort											= m_sourcePort;
		tcpFrame_fin.destinationPort								= m_destinationPort;
		tcpFrame_fin.sequenceNumber									= m_sequenceNumber;
		tcpFrame_fin.ackNumber											= m_lastForeignSequenceNumberACKed;
		tcpFrame_fin.ack 														= true;
		tcpFrame_fin.finish													= true;
		tcpFrame_fin.receiveWindow									= m_receiveBuffer.get_size() - m_receiveBuffer.get_numberOfUnread();
		
		
		//	Save Sequence Number to be acknowledged together with the transmitted Packet
		Pair<uint32, Array<uint8>> transmissionData(m_sequenceNumber + 1, tcpFrame_fin.convertToByteStream(m_sourceIpAddress, m_destinationIpAddress));
		m_sequenceNumbersAndTransmittedPackets += transmissionData;
		
		
		//	Send FIN Packet over IP Layer
		if(network.m_ip.tx(transmissionData.second(), IP_Frame::e_protocol::TCP, m_destinationIpAddress) != OK)
		{
			return(FAIL);
		}
	}
	return(OK);
}


feedback TCP_Socket::process_connectionClosingByForeignACK(const TCP_Frame& tcpFrame)
{
	//	Lock Semaphore for thread-safe Access
	CMOS& cmos = CMOS::get();
	cmos.semaphore_lock(this);
	
	
	//	Check for expected Sequence Number
	bool ackReceived = false;
	if(m_sequenceNumbersAndTransmittedPackets.contains(tcpFrame.ackNumber) == true)
	{
		m_sequenceNumbersAndTransmittedPackets.erase(tcpFrame.ackNumber);
		ackReceived = true;
	}
	
	
	//	Unlock Semaphore
	cmos.semaphore_unlock(this);
	
	
	if(ackReceived == true)
	{
		return(OK);
	}
	return(FAIL);
}


feedback TCP_Socket::process_connectionClosingByMyself(const TCP_Frame& tcpFrame)
{
	//	Lock Semaphore for thread-safe Access
	CMOS& cmos = CMOS::get();
	cmos.semaphore_lock(this);
	
	
	//	Check for expected Sequence Number
	bool ackReceived = false;
	if(m_sequenceNumbersAndTransmittedPackets.contains(tcpFrame.ackNumber) == true)
	{
		m_sequenceNumbersAndTransmittedPackets.erase(tcpFrame.ackNumber);
		ackReceived = true;
	}
	
	
	//	Unlock Semaphore
	cmos.semaphore_unlock(this);
	
	
	//	Check for foreign Connection closing by FIN/ACK Packet
	if(tcpFrame.finish == true)
	{
		//	Save last foreign ACKed Sequence Number
		m_lastForeignSequenceNumberACKed = tcpFrame.sequenceNumber + 1;
		
		
		//	Send an ACK Packet to confirm the FIN Packet
		TCP_Frame tcpFrame_reply;
		tcpFrame_reply.sourcePort											= m_sourcePort;
		tcpFrame_reply.destinationPort								= m_destinationPort;
		tcpFrame_reply.sequenceNumber 								= m_sequenceNumber;
		tcpFrame_reply.ackNumber											= m_lastForeignSequenceNumberACKed;
		tcpFrame_reply.ack														= true;
		tcpFrame_reply.receiveWindow									= m_receiveBuffer.get_size() - m_receiveBuffer.get_numberOfUnread();
		
		
		//	Send Packet over IP Layer
		Network& network = Network::get(m_networkID);
		network.m_ip.tx(tcpFrame_reply.convertToByteStream(m_sourceIpAddress, m_destinationIpAddress), IP_Frame::e_protocol::TCP, m_destinationIpAddress);
	}
	
	
	if(ackReceived == true)
	{
		return(OK);
	}
	return(FAIL);
}


void TCP_Socket::process_normalData(const TCP_Frame& tcpFrame)
{
	
}







void TCP_Socket::handlePacket(const TCP_Frame& tcpFrame)
{
	//	Check for a valid Receive Buffer
	if(m_receiveBuffer.is_valid() == false)
	{
		return;
	}
	
	
	//	Check for Abortion by a RST Packet from Server
	if(tcpFrame.reset == true)
	{
		m_state = e_state::CLOSED;
		return;
	}
	
	
	switch(m_state)
	{
		case e_state::ESTABLISHING:
		{
			if(process_connectionEstablishment(tcpFrame) != OK)
			{
				m_state = e_state::CLOSED;
			}
			else
			{
				m_state = e_state::ESTABLISHED;
			}
		}
		break;
		
		case e_state::ESTABLISHED:
		{
			//	Check for Connection Close from Server
			if(tcpFrame.finish == true)
			{
				process_connectionClosingByForeign(tcpFrame);
				m_state = e_state::CLOSING_BY_FOREIGN;
			}
			else
			{
				process_normalData(tcpFrame);
			}
		}
		break;
		
		case e_state::CLOSING_BY_FOREIGN:
		{
			if(process_connectionClosingByForeignACK(tcpFrame) == OK)
			{
				m_state = e_state::CLOSED;
			}
		}
		break;
		
		case e_state::CLOSING_BY_MYSELF:
		{
			if(process_connectionClosingByMyself(tcpFrame) == OK)
			{
				if(tcpFrame.finish == false)
				{
					m_state = e_state::CLOSED_BY_MYSELF;
				}
				else
				{
					m_state = e_state::CLOSED;
				}
			}
		}
		break;
		
		case e_state::CLOSED_BY_MYSELF:
		{
			if(process_connectionClosingByForeign(tcpFrame) == OK)
			{
				m_state = e_state::CLOSED;
			}
		}
		break;
		
		case e_state::CLOSED:
		{
			
		}
		break;
		
		default:
		{
			return;
		}
	}
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

feedback TCP_Socket::connect()
{
	//	Dont proceed if Network isnt attached
	if(m_networkID == Network::networkID_invalid)
	{
		return(FAIL);
	}
	
	
	//	Lock Semaphore for thread-safe Access
	CMOS& cmos = CMOS::get();
	cmos.semaphore_lock(this);
	
	
	//	Build Packet
	TCP_Frame tcpFrame;
	tcpFrame.sourcePort											= m_sourcePort;
	tcpFrame.destinationPort								= m_destinationPort;
	tcpFrame.sequenceNumber									= m_sequenceNumber;
	tcpFrame.synchronize										= true;
	tcpFrame.receiveWindow									= m_receiveBuffer.get_size() - m_receiveBuffer.get_numberOfUnread();
	
	TCP_Frame::s_option option_maximumSegmentSize;
	option_maximumSegmentSize.kind = TCP_Frame::e_optionKind::MAXIMUM_SEGMENT_SIZE;
	option_maximumSegmentSize.data += (m_ownMaximumSegmentSize & 0x0000FF00) >> 8;
	option_maximumSegmentSize.data += (m_ownMaximumSegmentSize & 0x000000FF);
	
	tcpFrame.options += option_maximumSegmentSize;
	
	
	//	Increment Sequence Number
	m_sequenceNumber++;
	
	
	//	Save Sequence Number to be acknowledged together with the transmitted Packet
	Pair<uint32, Array<uint8>> transmissionData(m_sequenceNumber, tcpFrame.convertToByteStream(m_sourceIpAddress, m_destinationIpAddress));
	m_sequenceNumbersAndTransmittedPackets += transmissionData;
	
	
	//	Unlock Semaphore
	cmos.semaphore_unlock(this);
	
	
	//	Send Packet over IP Layer
	Network& network = Network::get(m_networkID);
	if(network.m_ip.tx(transmissionData.second(), IP_Frame::e_protocol::TCP, m_destinationIpAddress) != OK)
	{
		return(FAIL);
	}
	
	
	m_state = e_state::ESTABLISHING;
	return(OK);
}


feedback TCP_Socket::close()
{
	//	Dont proceed if Network isnt attached
	if(m_networkID == Network::networkID_invalid)
	{
		return(FAIL);
	}
	
	
	//	Lock Semaphore for thread-safe Access
	CMOS& cmos = CMOS::get();
	cmos.semaphore_lock(this);
	
	
	//	Build Packet
	TCP_Frame tcpFrame;
	tcpFrame.sourcePort											= m_sourcePort;
	tcpFrame.destinationPort								= m_destinationPort;
	tcpFrame.sequenceNumber									= m_sequenceNumber;
	tcpFrame.ackNumber											= m_lastForeignSequenceNumberACKed;
	tcpFrame.ack 														= true;
	tcpFrame.finish													= true;
	tcpFrame.receiveWindow									= m_receiveBuffer.get_size() - m_receiveBuffer.get_numberOfUnread();
	
	
	//	Increment Sequence Number
	m_sequenceNumber++;
	
	
	//	Save Sequence Number to be acknowledged together with the transmitted Packet
	Pair<uint32, Array<uint8>> transmissionData(m_sequenceNumber, tcpFrame.convertToByteStream(m_sourceIpAddress, m_destinationIpAddress));
	m_sequenceNumbersAndTransmittedPackets += transmissionData;
	
	
	//	Unlock Semaphore
	cmos.semaphore_unlock(this);
	
	
	//	Send Packet over IP Layer
	Network& network = Network::get(m_networkID);
	if(network.m_ip.tx(transmissionData.second(), IP_Frame::e_protocol::TCP, m_destinationIpAddress) != OK)
	{
		return(FAIL);
	}
	
	
	m_state = e_state::CLOSING_BY_MYSELF;
	return(OK);
}







feedback TCP_Socket::tx(const Array<uint8>& data)
{
	//	Dont proceed if Network isnt attached
	if(m_networkID == Network::networkID_invalid)
	{
		return(FAIL);
	}
	
	
	//	Check for Connection State
	if(m_state != e_state::ESTABLISHED && m_state != e_state::CLOSED_BY_FOREIGN)
	{
		return(FAIL);
	}
	
	
	//	Lock Semaphore for thread-safe Access
	CMOS& cmos = CMOS::get();
	cmos.semaphore_lock(this);
	
	
	//	Build Packet
	TCP_Frame tcpFrame;
	tcpFrame.sourcePort											= m_sourcePort;
	tcpFrame.destinationPort								= m_destinationPort;
	tcpFrame.sequenceNumber									= m_sequenceNumber;
	tcpFrame.ackNumber											= m_lastForeignSequenceNumberACKed;
	tcpFrame.ack 														= true;
	tcpFrame.receiveWindow									= m_receiveBuffer.get_size() - m_receiveBuffer.get_numberOfUnread();
	tcpFrame.payload 												= data;
	
	
	//	Increment Sequence Number
	m_sequenceNumber++;
	
	
	//	Save Sequence Number to be acknowledged together with the transmitted Packet
	Pair<uint32, Array<uint8>> transmissionData(m_sequenceNumber, tcpFrame.convertToByteStream(m_sourceIpAddress, m_destinationIpAddress));
	m_sequenceNumbersAndTransmittedPackets += transmissionData;
	
	
	//	Unlock Semaphore
	cmos.semaphore_unlock(this);
	
	
	//	Send Packet over IP Layer
	Network& network = Network::get(m_networkID);
	return(network.m_ip.tx(transmissionData.second(), IP_Frame::e_protocol::TCP, m_destinationIpAddress));
}







TCP_Socket::e_state TCP_Socket::get_state() const
{
	return(m_state);
}