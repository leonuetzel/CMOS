#pragma once

#include "../cmos.hpp"
#include "udpFrame.hpp"





class UDP
{
	public:
		
		
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		const uint8& m_networkID;
		
		
		//	Constructor and Destructor
		UDP(const uint8& networkID);
		UDP(const UDP& udp) = delete;
		~UDP();
		
		
		//	Member Functions
		
		
		
		//	Friends
		friend class Network;
		
		
		
		
		
	public:
		
		void handlePacket(const UDP_Frame& udpFrame);
		
		feedback tx(const Array<uint8>& data, uint16 sourcePort, uint16 destinationPort, const Array<uint8>& destinationIpAddress);
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

