#pragma once

#include "../cmos.hpp"
#include "tcpFrame.hpp"
#include "tcpSocket.hpp"





class TCP
{
	public:
		
		
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		const uint8& m_networkID;
		Array<TCP_Socket*> m_sockets;
		
		
		//	Constructor and Destructor
		TCP(const uint8& networkID);
		TCP(const TCP& tcp) = delete;
		~TCP();
		
		
		//	Member Functions
		
		
		
		//	Friends
		friend class Network;
		
		
		
		
		
	public:
		
		void handlePacket(const TCP_Frame& tcpFrame);
		
		TCP_Socket* createSocket(const Array<uint8>& destinationIpAddress, uint16 destinationPort, uint16 sourcePort, uint32 maximumSegmentSize, uint32 receiveBufferSize);
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

