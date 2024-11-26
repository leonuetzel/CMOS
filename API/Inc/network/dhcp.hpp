#pragma once

#include "../cmos.hpp"
#include "dhcpFrame.hpp"





class DHCP
{
	public:
		
		
		
		
		
		
		
	private:
		
		//	Static Member
		enum class e_state
		{
			INIT,
			SELECTING,
			REQUESTING,
			BOUND,
			RENEWING,
			REBINDING,
			INIT_REBOOT,
			REBOOTING
		};
		
		
		//	Non-static Member
		const uint8& m_networkID;
		String m_deviceName;
		e_state m_state;
		uint8 m_threadID;
		uint8 m_threadID_leaseTimer;
		Array<uint8> m_ipAddress;
		Array<uint8> m_subnetMask;
		Array<uint8> m_ipAddressRouter;
		Array<uint8> m_ipAddressDhcpServer;
		Array<Array<uint8>> m_dnsServer;
		uint32 m_ipAddressLeaseTime;
		uint32 m_ipAddressLeaseRenewalTimeT1;
		uint32 m_ipAddressLeaseRebindingTimeT2;
		
		
		//	Constructor and Destructor
		DHCP(const uint8& networkID);
		DHCP(const DHCP& dhcp) = delete;
		~DHCP();
		
		
		//	Member Functions
		void manager();
		void leaseTimer();
		
		void discover();
		feedback process_offer(const DHCP_Frame& dhcpOffer);
		void request(const Array<uint8>& yourIpAddress, const Array<uint8>& serverIdentifier, bool broadcast = true);
		feedback process_ack(const DHCP_Frame& dhcpOffer);
		void decline();
		
		void setDeviceName(const String& deviceName);
		
		
		//	Friends
		friend class Network;
		
		
		
		
		
	public:
		
		void handlePacket(const DHCP_Frame& dhcpFrame);
		inline const Array<uint8>& get_ipAddressRouter() const;
		inline const Array<Array<uint8>>& get_ipAddressDnsServer() const;
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

inline const Array<uint8>& DHCP::get_ipAddressRouter() const
{
	return(m_ipAddressRouter);
}


inline const Array<Array<uint8>>& DHCP::get_ipAddressDnsServer() const
{
	return(m_dnsServer);
}