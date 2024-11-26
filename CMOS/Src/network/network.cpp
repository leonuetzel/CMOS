#include "../../Inc/network/network.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/

Array<Network*> Network::m_networks;
uint8 Network::m_networkCounter = 0;



/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

Network::Network(I_PhysicalLayer& physicalLayer)
	:	m_networkID(networkID_invalid),
		m_physicalLayer(physicalLayer),
		m_ethernet(m_networkID),
		m_arp(m_networkID),
		m_ip(m_networkID),
		m_udp(m_networkID),
		m_tcp(m_networkID),
		m_dhcp(m_networkID),
		m_dns(m_networkID)
{
	
}


Network::~Network()
{
	detach();
}







Network& Network::get(uint8 networkID)
{
	for(auto& i: m_networks)
	{
		if(i->m_networkID == networkID)
		{
			return(*i);
		}
	}
	return((Network&) *((Network*) nullptr));
}


uint32 Network::get_numberOfNetworks()
{
	return(m_networks.get_size());
}







uint8 Network::attach()
{
	//	Network already attached
	if(m_networkID != networkID_invalid)
	{
		return(m_networkID);
	}
	
	
	//	Assign new Network ID
	m_networkID = m_networkCounter;
	
	
	//	Attach Network
	m_networks += this;
	
	
	//	Increment Network Counter for next Attachement
	m_networkCounter++;
	
	
	return(m_networkID);
}


feedback Network::detach()
{
	const uint32 numberOfNetworks = m_networks.get_size();
	for(uint32 i = 0; i < numberOfNetworks; i++)
	{
		const Network& network(*m_networks[i]);
		if(network.m_networkID == m_networkID)
		{
			m_networkID = networkID_invalid;
			m_networks.erase(i);
			return(OK);
		}
	}
	return(FAIL);
}







void Network::setup_DHCP(const String& deviceName)
{
	//	Set Device Name for DHCP
	m_dhcp.setDeviceName(deviceName);
	
	
	//	Start DHCP State Machine to obtain an IP Address from the DHCP Server
	DHCP_Frame& dummyFrame = *new DHCP_Frame;
	m_dhcp.handlePacket(dummyFrame);
	
	
	//	Wait for DHCP Process to get an IP Address
	CMOS& cmos = CMOS::get();
	while(m_dhcp.m_state != DHCP::e_state::BOUND)
	{
		cmos.sleep_ms(100);
	}
}


const Array<uint8> Network::getIpAddressByDNS(const String& domainName)
{
	return(m_dns.resolve(domainName));
}







const Array<uint8>& Network::getIpAddress() const
{
	return(m_ip.get_ipAddress());
}


TCP& Network::get_tcp()
{
	return(m_tcp);
}







void Network::handlePacket(const Array<uint8>& packet)
{
	EthernetFrame ethernetFrame(packet);
	m_ethernet.handlePacket(ethernetFrame);
}