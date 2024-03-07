#pragma once

#include "../cmos.hpp"
#include "i_physicalLayer.hpp"


//	OSI Layer 2
#include "ethernet.hpp"
#include "arp.hpp"


//	OSI Layer 3
#include "ip.hpp"


//	OSI Layer 4
#include "udp.hpp"
#include "tcp.hpp"


//	OSI Layer 5



//	OSI Layer 6



//	OSI Layer 7
#include "dhcp.hpp"
#include "dns.hpp"





class Network
{
	public:
		
		static constexpr uint8 networkID_invalid = 0xFF;
		
		
		
		
		
	private:
		
		//	Static Member
		static Array<Network*> m_networks;
		static uint8 m_networkCounter;
		
		
		//	Non-static Member
		uint8 m_networkID;
		I_PhysicalLayer& m_physicalLayer;
		Ethernet m_ethernet;
		ARP m_arp;
		IP m_ip;
		UDP m_udp;
		TCP m_tcp;
		DHCP m_dhcp;
		DNS m_dns;
		
		
		//	Constructor and Destructor
		Network(const Network& network) = delete;
		
		
		//	Member Functions
		void test();
		
		
		//	Friends
		friend class Ethernet;
		friend class ARP;
		friend class IP;
		friend class UDP;
		friend class TCP;
		friend class TCP_Socket;
		friend class DHCP;
		friend class DNS;
		
		
		
		
		
	public:
		
		Network(I_PhysicalLayer& physicalLayer);
		~Network();
		
		static Network& get(uint8 networkID);
		static uint32 get_numberOfNetworks();
		
		uint8 attach();
		feedback detach();
		
		void setup_DHCP();
		const Array<uint8> getIpAddressByDNS(const String& domainName);
		
		
		//	Getter
		const Array<uint8>& getIpAddress() const;
		TCP& get_tcp();
		
		void handlePacket(const Array<uint8>& packet);
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

