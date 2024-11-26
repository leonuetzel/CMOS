#pragma once

#include "../cmos.hpp"
#include "ethernetFrame.hpp"
#include "ethernetPhy.hpp"





class Ethernet
{
	public:
		
		
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		const uint8& m_networkID;
		
		
		//	Constructor and Destructor
		Ethernet(const uint8& networkID);
		Ethernet(const Ethernet& ethernet) = delete;
		~Ethernet();
		
		
		//	Member Functions
		
		
		
		//	Friends
		friend class Network;
		
		
		
		
		
	public:
		
		void handlePacket(const EthernetFrame& ethernetFrame);
		
		feedback tx(const Array<uint8>& data, const Array<uint8>& destinationMacAddress, EthernetFrame::e_etherType etherType, bool VLAN = false, uint16 VLAN_tag = 0x0000);
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

