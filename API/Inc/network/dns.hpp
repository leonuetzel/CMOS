#pragma once

#include "../cmos.hpp"
#include "dnsFrame.hpp"





class DNS
{
	public:
		
		
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		const uint8& m_networkID;
		uint8 m_threadID;
		UniquePairArray<String, Pair<Array<uint8>, uint32>> m_table;
		uint16 dnsHeaderIDCounter;
		
		
		//	Constructor and Destructor
		DNS(const uint8& networkID);
		DNS(const DNS& dns) = delete;
		~DNS();
		
		
		//	Member Functions
		void timeToLiveCounter();
		
		
		//	Friends
		friend class Network;
		
		
		
		
		
	public:
		
		void handlePacket(const DNS_Frame& dnsFrame);
		
		Array<uint8> resolve(const String& domainName, uint32 timeoutInMilliseconds = 5000);
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

