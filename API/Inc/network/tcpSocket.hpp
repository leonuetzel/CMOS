#pragma once

#include "../cmos.hpp"





class TCP_Socket
{
	public:
		
		enum class e_state
		{
			CLOSED,
			CLOSING_BY_FOREIGN,
			CLOSED_BY_FOREIGN,
			CLOSING_BY_MYSELF,
			CLOSED_BY_MYSELF,
			ESTABLISHING,
			ESTABLISHED
		};
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		const uint8& m_networkID;
		e_state m_state;
		
		const Array<uint8> m_sourceIpAddress;
		const Array<uint8> m_destinationIpAddress;
		const uint16 m_sourcePort;
		const uint16 m_destinationPort;
		
		RingbufferDynamic<uint8> m_receiveBuffer;
		uint32 m_ownMaximumSegmentSize;
		uint32 m_foreignMaximumSegmentSize;
		uint32 m_sequenceNumber;
		uint32 m_lastForeignSequenceNumberACKed;
		UniquePairArray<uint32, Array<uint8>> m_sequenceNumbersAndTransmittedPackets;
		
		
		//	Constructor and Destructor
		TCP_Socket(const uint8& networkID, const Array<uint8>& sourceIpAddress, uint16 sourcePort, const Array<uint8>& destinationIpAddress, uint16 destinationPort, uint32 maximumSegmentSize, uint32 receiveBufferSize);
		TCP_Socket(const TCP_Socket& tcpSocket) = delete;
		~TCP_Socket();
		
		
		//	Member Functions
		feedback process_connectionEstablishment(const TCP_Frame& tcpFrame);
		feedback process_connectionClosingByForeign(const TCP_Frame& tcpFrame);
		feedback process_connectionClosingByForeignACK(const TCP_Frame& tcpFrame);
		feedback process_connectionClosingByMyself(const TCP_Frame& tcpFrame);
		void process_normalData(const TCP_Frame& tcpFrame);
		
		void handlePacket(const TCP_Frame& tcpFrame);
		
		
		//	Friends
		friend class TCP;
		
		
		
		
		
	public:
		
		feedback connect();
		feedback close();
		
		feedback tx(const Array<uint8>& data);
		
		e_state get_state() const;
};



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

