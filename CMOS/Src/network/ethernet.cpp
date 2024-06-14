#include "../../Inc/network/network.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

Ethernet::Ethernet(const uint8& networkID)
	:	m_networkID(networkID)
{
	
}


Ethernet::~Ethernet()
{
	
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

void Ethernet::handlePacket(const EthernetFrame& ethernetFrame)	
{
	Network& network = Network::get(m_networkID);
	
	
	//	Interprete the Ethernet Frame
	if(ethernetFrame.payload.get_size() > 0)
	{
		//	Decide based on Ether Type what Protocol to invoke
		switch(ethernetFrame.etherType)
		{
			case EthernetFrame::e_etherType::ARP:
			{
				//	Interprete the ARP Frame
				ARP_Frame arpFrame(ethernetFrame.payload);
				network.m_arp.handlePacket(arpFrame);
			}
			break;
			
			case EthernetFrame::e_etherType::IPv4:
			{
				IP_Frame ipFrame(ethernetFrame.payload);
				network.m_ip.handlePacket(ipFrame);
			}
			break;
			
			default:
			{
				
			}
			break;
		}
	}
}







feedback Ethernet::tx(const Array<uint8>& data, const Array<uint8>& destinationMacAddress, EthernetFrame::e_etherType etherType, bool VLAN, uint16 VLAN_tag)
{
	//	Dont proceed if Network isnt attached
	if(m_networkID == Network::networkID_invalid)
	{
		return(FAIL);
	}
	
	
	//	Check destinationMacAddress to be 6 Byte long
	if(destinationMacAddress.get_size() != 6)
	{
		return(FAIL);
	}
	
	
	//	Check Maximum Transmission Unit (MTU)
	Network& network = Network::get(m_networkID);
	const uint32 maximumTransmissionUnit = network.m_physicalLayer.get_maximumTransmissionUnit();
	if(data.get_size() > maximumTransmissionUnit)
	{
		return(FAIL);
	}
	
	
	//	Assemble Ethernet Frame
	EthernetFrame ethernetFrame;
	for(uint32 i = 0; i < 6; i++)
	{
		ethernetFrame.MAC_destination[i] = destinationMacAddress[i];
	}
	
	const Array<uint8>& macAddress = network.m_physicalLayer.get_macAddress();
	for(uint32 i = 0; i < 6; i++)
	{
		ethernetFrame.MAC_source[i] = macAddress[i];
	}
	ethernetFrame.etherType = etherType;
	
	if(VLAN == true)
	{
		ethernetFrame.VLAN_Tag[0] = (((uint16) EthernetFrame::e_etherType::VLAN_IEEE802_1Q_IEEE802_1AQ_NNI) & 0xFF00) >> 8;
		ethernetFrame.VLAN_Tag[1] = (((uint16) EthernetFrame::e_etherType::VLAN_IEEE802_1Q_IEEE802_1AQ_NNI) & 0x00FF);
		ethernetFrame.VLAN_Tag[2] = (VLAN_tag & 0xFF00) >> 8;
		ethernetFrame.VLAN_Tag[3] = (VLAN_tag & 0x00FF);
	}
	ethernetFrame.payload = data;
	
	
	network.m_physicalLayer.tx(ethernetFrame.convertToByteStream());
	return(OK);
}