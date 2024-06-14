#include "../../Inc/network/network.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

UDP::UDP(const uint8& networkID)
	:	m_networkID(networkID)
{
	
}


UDP::~UDP()
{
	
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

void UDP::handlePacket(const UDP_Frame& udpFrame)
{
	Network& network = Network::get(m_networkID);
	switch(udpFrame.destinationPort)
	{
		case 53:
		{
			DNS_Frame dnsFrame(udpFrame.payload);
			network.m_dns.handlePacket(dnsFrame);
		}
		break;
		
		case 68:
		{
			DHCP_Frame& dhcpFrame = *new DHCP_Frame(udpFrame.payload);
			network.m_dhcp.handlePacket(dhcpFrame);
		}
		break;
		
		default:
		{
			
		}
		break;
	}
}







feedback UDP::tx(const Array<uint8>& data, uint16 sourcePort, uint16 destinationPort, const Array<uint8>& destinationIpAddress)
{
	//	Dont proceed if Network isnt attached
	if(m_networkID == Network::networkID_invalid)
	{
		return(FAIL);
	}
	
	
	//	Assemble UDP Frame
	UDP_Frame udpFrame;
	udpFrame.sourcePort								= sourcePort;
	udpFrame.destinationPort					= destinationPort;
	udpFrame.payload									= data;
	
	
	//	Send to underlying Layer in the Network Stack
	Network& network = Network::get(m_networkID);
	return(network.m_ip.tx(udpFrame.convertToByteStream(), IP_Frame::e_protocol::UDP, destinationIpAddress));
}