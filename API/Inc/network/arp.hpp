#pragma once

#include "../cmos.hpp"
#include "i_physicalLayer.hpp"
#include "ethernetFrame.hpp"
#include "arpFrame.hpp"





class ARP
{
	public:
		
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		const uint8& m_networkID;
		UniquePairArray<Array<uint8>, Array<uint8>> m_table;
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		ARP(const uint8& networkID);
		ARP(const ARP& arp) = delete;
		inline ~ARP();
		
		bool probe(const Array<uint8>& protocolAddress, uint8 numberOfRetries = 3, uint32 intervalInMilliSeconds = 500);
		void update(const Array<uint8>& protocolAddress, const Array<uint8>& hardwareAddress);
		
		bool contains_protocolAddress(const Array<uint8>& protocolAddress) const;
		bool contains_hardwareAddress(const Array<uint8>& hardwareAddress) const;
		Array<uint8> get_hardwareAddress(const Array<uint8>& protocolAddress, bool doRequestIfNotInCache = false, uint32 timeoutInMilliseconds = 3000);
		Array<uint8> get_protocolAddress(const Array<uint8>& hardwareAddress) const;
		
		void handlePacket(const ARP_Frame& arpFrame);
};



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

inline ARP::~ARP()
{
	
}



/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

