#include "../../Inc/network/network.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

DHCP::DHCP(const uint8& networkID)
	:	m_networkID(networkID),
		m_deviceName()
{
	m_threadID = CMOS::get().thread_create(&DHCP::manager, this, "DHCP", 100, 2048);
}


DHCP::~DHCP()
{
	
}







void DHCP::manager()
{
	//	Init State Machine
	m_state = e_state::INIT;
	
	
	//	Wait for Network to be attached
	CMOS& cmos = CMOS::get();
	while(m_networkID == Network::networkID_invalid)
	{
		cmos.sleep_ms(100);
	}
	
	
	//	Run State Machine
	while(1)
	{
		//	Wait for a new Packet to handle by Function "handlePacket"
		while(cmos.is_mailAvailable() == false)
		{
			cmos.sleep();
		}
		
		
		//	Get DHCP Frame
		DHCP_Frame& dhcpFrame = (DHCP_Frame&) *((DHCP_Frame*) cmos.read_mail().data);
		
		
		//	State Machine
		switch(m_state)
		{
			case e_state::INIT:
			{
				cmos.thread_shutdown(m_threadID_leaseTimer, true);
				m_threadID_leaseTimer = CMOS::threadID_invalid;
				m_ipAddress.erase();
				m_subnetMask.erase();
				m_ipAddressRouter.erase();
				m_ipAddressDhcpServer.erase();
				m_dnsServer.erase();
				m_ipAddressLeaseTime = 0;
				m_ipAddressLeaseRenewalTimeT1 = 0;
				m_ipAddressLeaseRebindingTimeT2 = 0;
				
				discover();
				m_state = e_state::SELECTING;
			}
			break;
			
			case e_state::SELECTING:
			{
				if(process_offer(dhcpFrame) != OK)
				{
					m_state = e_state::INIT;
				}
				else
				{
					m_state = e_state::REQUESTING;
				}
			}
			break;
			
			case e_state::REQUESTING:
			{
				if(process_ack(dhcpFrame) != OK)
				{
					decline();
					m_state = e_state::INIT;
				}
				else
				{
					m_state = e_state::BOUND;
				}
			}
			break;
			
			case e_state::BOUND:
			{
				if(m_ipAddressLeaseRenewalTimeT1 == 0)
				{
					request(m_ipAddress, m_ipAddressDhcpServer, false);
					m_state = e_state::RENEWING;
				}
			}
			break;
			
			case e_state::RENEWING:
			{
				if(m_ipAddressLeaseRebindingTimeT2 == 0)
				{
					request(m_ipAddress, m_ipAddressDhcpServer);
					m_state = e_state::REBINDING;
					break;
				}
				if(process_ack(dhcpFrame) != OK)
				{
					m_state = e_state::INIT;
					break;
				}
				m_state = e_state::BOUND;
			}
			break;
			
			case e_state::REBINDING:
			{
				if(m_ipAddressLeaseTime == 0)
				{
					m_state = e_state::INIT;
					break;
				}
				if(process_ack(dhcpFrame) != OK)
				{
					m_state = e_state::INIT;
					break;
				}
				m_state = e_state::BOUND;
			}
			break;
			
			case e_state::INIT_REBOOT:
			{
				//	Tbd
			}
			break;
			
			case e_state::REBOOTING:
			{
				//	Tbd
			}
			break;
			
			default:
			{
				m_state = e_state::INIT;
			}
			break;
		}
		
		
		//	Free Memory of DHCP Frame (reserved by UDP::handlePacket)
		delete &dhcpFrame;
	}
}


void DHCP::leaseTimer()
{
	CMOS& cmos = CMOS::get();
	
	while(1)
	{
		//	Rebinding Timer T2
		if(m_ipAddressLeaseRebindingTimeT2 > 0)
		{
			m_ipAddressLeaseRebindingTimeT2--;
			if(m_ipAddressLeaseRebindingTimeT2 == 0)
			{
				cmos.send_mail(m_threadID, 2);
			}
		}
		
		
		//	Renewal Timer T1
		if(m_ipAddressLeaseRenewalTimeT1 > 0)
		{
			m_ipAddressLeaseRenewalTimeT1--;
			if(m_ipAddressLeaseRenewalTimeT1 == 0)
			{
				cmos.send_mail(m_threadID, 1);
			}
		}
		
		
		//	Lease Timer
		if(m_ipAddressLeaseTime > 0)
		{
			m_ipAddressLeaseTime--;
			if(m_ipAddressLeaseTime == 0)
			{
				cmos.send_mail(m_threadID, 0);
				return;
			}
		}
		
		
		//	Sleep 1 Second
		cmos.sleep_sec(1);
	}
}







void DHCP::discover()
{
	//	Get own MAC Address
	Network& network = Network::get(m_networkID);
	I_PhysicalLayer& physicalLayer = network.m_physicalLayer;
	const Array<uint8>& macAddress = physicalLayer.get_macAddress();
	
	
	//	DHCP Frame for DHCP Discovery Message
	DHCP_Frame dhcpFrame;
	dhcpFrame.operationCode					= 0x01;
	dhcpFrame.hardwareType					= DHCP_Frame::e_hardwareType::ETHERNET_10MB;
	dhcpFrame.hardwareAddressLength	= 6;
	dhcpFrame.xID										= 0x3903F326;
	for(uint32 i = 0; i < 6; i++)
	{
		dhcpFrame.clientHardwareAddress[i] = macAddress[i];
	}
	//	DHCP Message Type
	{
		DHCP_Frame::s_option* option = new DHCP_Frame::s_option;
		option->code = DHCP_Frame::e_option::DHCP_MESSAGE_TYPE;
		option->data += (uint8) DHCP_Frame::e_dhcpMessageType::DISCOVER;
		dhcpFrame.options += option;
	}
	//	DHCP Parameter Request List
	{
		DHCP_Frame::s_option* option = new DHCP_Frame::s_option;
		option->code = DHCP_Frame::e_option::PARAMETER_REQUEST_LIST;
		option->data += 0x01;
		option->data += 0x03;
		option->data += 0x06;
		option->data += 0x0F;
		dhcpFrame.options += option;
	}
	//	DHCP Client Identifier
	{
		DHCP_Frame::s_option* option = new DHCP_Frame::s_option;
		option->code = DHCP_Frame::e_option::CLIENT_IDENTIFIER;
		option->data += 0x01;
		for(uint32 i = 0; i < 6; i++)
		{
			option->data += macAddress[i];
		}
		dhcpFrame.options += option;
	}
	//	DHCP Host Name
	{
		DHCP_Frame::s_option* option = new DHCP_Frame::s_option;
		option->code = DHCP_Frame::e_option::HOST_NAME;
		for(auto& i: m_deviceName)
		{
			option->data += i;
		}
		dhcpFrame.options += option;
	}
	
	
	network.m_udp.tx(dhcpFrame.convertToByteStream(), 68, 67, Array<uint8>(0xFF, 4));
}


feedback DHCP::process_offer(const DHCP_Frame& dhcpOffer)
{
	//	Check Operation Code
	if(dhcpOffer.operationCode != 2)
	{
		return(FAIL);
	}
	
	
	//	Check for DHCP Option "Message Type (0x35)" to be "DHCPOFFER (0x02)"
	bool messageTypeCorrect = false;
	for(auto& i: dhcpOffer.options)
	{
		if(i->code == DHCP_Frame::e_option::DHCP_MESSAGE_TYPE)
		{
			if(i->data.get_size() == 0)
			{
				return(FAIL);
			}
			if(i->data[0] != (uint8) DHCP_Frame::e_dhcpMessageType::OFFER)
			{
				return(FAIL);
			}
			messageTypeCorrect = true;
			break;
		}
	}
	if(messageTypeCorrect == false)
	{
		return(FAIL);
	}
	
	
	//	Examine offered IP Address
	Array<uint8> yourIpAddress(0, 4);
	for(uint32 i = 0; i < 4; i++)
	{
		yourIpAddress[i] = dhcpOffer.yourIPaddress[i];
	}
	
	
	//	Do an ARP Request to check, if the offered IP Address is already in Use
	Network& network = Network::get(m_networkID);
	if(network.m_arp.probe(yourIpAddress) == true)
	{
		//	Offered IP Address is already in Use
		return(FAIL);
	}
	
	
	//	Examine Server Identifier from Option 54
	Array<uint8> serverIdentifier(0, 4);
	bool ipAddressFound = false;
	for(auto& i: dhcpOffer.options)
	{
		if(i->code == DHCP_Frame::e_option::SERVER_IDENTIFIER)
		{
			for(uint32 j = 0; j < 4; j++)
			{
				serverIdentifier[j] = i->data[j];
			}
			ipAddressFound = true;
		}
	}
	if(ipAddressFound == false)
	{
		return(FAIL);
	}
	
	
	//	Accept Offer by sending a DHCP Frame with DHCP Option "Message Type (0x35)" set to "DHCPREQUEST (0x03)"
	request(yourIpAddress, serverIdentifier);
	
	
	return(OK);
}


void DHCP::request(const Array<uint8>& yourIpAddress, const Array<uint8>& serverIdentifier, bool broadcast)
{
	//	Get own MAC Address
	Network& network = Network::get(m_networkID);
	I_PhysicalLayer& physicalLayer = network.m_physicalLayer;
	const Array<uint8>& macAddress = physicalLayer.get_macAddress();
	
	
	//	DHCP Frame for DHCP Discovery Message
	DHCP_Frame dhcpFrame;
	dhcpFrame.operationCode					= 0x01;
	dhcpFrame.hardwareType					= DHCP_Frame::e_hardwareType::ETHERNET_10MB;
	dhcpFrame.hardwareAddressLength	= 6;
	dhcpFrame.xID										= 0x3903F326;
	for(uint32 i = 0; i < 6; i++)
	{
		dhcpFrame.clientHardwareAddress[i] = macAddress[i];
	}
	//	DHCP Message Type
	{
		DHCP_Frame::s_option* option = new DHCP_Frame::s_option;
		option->code = DHCP_Frame::e_option::DHCP_MESSAGE_TYPE;
		option->data += (uint8) DHCP_Frame::e_dhcpMessageType::REQUEST;
		dhcpFrame.options += option;
	}
	//	DHCP Requested IP Address
	{
		DHCP_Frame::s_option* option = new DHCP_Frame::s_option;
		option->code = DHCP_Frame::e_option::REQUESTED_IP_ADDRESS;
		option->data = yourIpAddress;
		dhcpFrame.options += option;
	}
	//	DHCP Server Identifier
	{
		DHCP_Frame::s_option* option = new DHCP_Frame::s_option;
		option->code = DHCP_Frame::e_option::SERVER_IDENTIFIER;
		option->data = serverIdentifier;
		dhcpFrame.options += option;
	}
	
	
	if(broadcast == true)
	{
		network.m_udp.tx(dhcpFrame.convertToByteStream(), 68, 67, Array<uint8>(0xFF, 4));
	}
	else
	{
		network.m_udp.tx(dhcpFrame.convertToByteStream(), 68, 67, serverIdentifier);
	}
}


feedback DHCP::process_ack(const DHCP_Frame& dhcpAck)
{
	//	Check Operation Code
	if(dhcpAck.operationCode != 2)
	{
		return(FAIL);
	}
	
	
	//	Check for DHCP Option "Message Type (0x35)" to be "DHCPACK (0x05)"
	bool messageTypeCorrect = false;
	for(auto& i: dhcpAck.options)
	{
		if(i->code == DHCP_Frame::e_option::DHCP_MESSAGE_TYPE)
		{
			if(i->data.get_size() == 0)
			{
				return(FAIL);
			}
			if(i->data[0] != (uint8) DHCP_Frame::e_dhcpMessageType::ACK)
			{
				return(FAIL);
			}
			messageTypeCorrect = true;
			break;
		}
	}
	if(messageTypeCorrect == false)
	{
		return(FAIL);
	}
	
	
	//	Do an ARP Request to check, if the acknowledged IP Address is already in Use
	Array<uint8> ipAddress(0, 4);
	for(uint32 i = 0; i < 4; i++)
	{
		ipAddress[i] = dhcpAck.yourIPaddress[i];
	}
	
	Network& network = Network::get(m_networkID);
	if(network.m_arp.probe(ipAddress) == true)
	{
		//	Offered IP Address is already in Use
		return(FAIL);
	}
	
	
	//	Save IP Address
	if(network.m_ip.set_ipAddress(ipAddress) != OK)
	{
		return(FAIL);
	}
	m_ipAddress = ipAddress;
	
	
	//	Save Subnet Mask
	{
		m_subnetMask.set_size(4);
		bool found = false;
		for(auto& i: dhcpAck.options)
		{
			if(i->code == DHCP_Frame::e_option::SUBNET_MASK)
			{
				if(i->data.get_size() != 4)
				{
					return(FAIL);
				}
				m_subnetMask = i->data;
				found = true;
			}
		}
		if(found == false)
		{
			return(FAIL);
		}
	}
	
	
	//	Save Router's IP Address
	{
		m_ipAddressRouter.set_size(4);
		bool found = false;
		for(auto& i: dhcpAck.options)
		{
			if(i->code == DHCP_Frame::e_option::ROUTER)
			{
				if(i->data.get_size() != 4)
				{
					return(FAIL);
				}
				m_ipAddressRouter = i->data;
				found = true;
			}
		}
		if(found == false)
		{
			return(FAIL);
		}
	}
	
	
	//	Save DHCP Server's IP Address
	{
		m_ipAddressDhcpServer.set_size(4);
		bool found = false;
		for(auto& i: dhcpAck.options)
		{
			if(i->code == DHCP_Frame::e_option::SERVER_IDENTIFIER)
			{
				if(i->data.get_size() != 4)
				{
					return(FAIL);
				}
				m_ipAddressDhcpServer = i->data;
				found = true;
			}
		}
		if(found == false)
		{
			return(FAIL);
		}
	}
	
	
	//	Save DNS Server's IP Address(es)
	{
		bool found = false;
		for(auto& i: dhcpAck.options)
		{
			if(i->code == DHCP_Frame::e_option::DNS_NAME_SERVER)
			{
				const uint32 length = i->data.get_size();
				if(length == 0 || length % 4 != 0)
				{
					return(FAIL);
				}
				for(uint32 j = 0; j < length / 4; j++)
				{
					m_dnsServer += i->data.sub(j * 4, 4);
				}
				found = true;
			}
		}
		if(found == false)
		{
			return(FAIL);
		}
	}
	
	
	//	Save IP Address Lease Time (Option Code 51)
	{
		bool found = false;
		for(auto& i: dhcpAck.options)
		{
			if(i->code == DHCP_Frame::e_option::IP_ADDRESS_LEASE_TIME)
			{
				if(i->data.get_size() != 4)
				{
					return(FAIL);
				}
				m_ipAddressLeaseTime = (i->data[0] << 24) | (i->data[1] << 16) | (i->data[2] << 8) | i->data[3];
				found = true;
			}
		}
		if(found == false)
		{
			return(FAIL);
		}
	}
	
	
	//	Save IP Address Renewal Time T1 (Option Code 58)
	{
		bool found = false;
		for(auto& i: dhcpAck.options)
		{
			if(i->code == DHCP_Frame::e_option::RENEWAL_TIME_VALUE)
			{
				if(i->data.get_size() != 4)
				{
					return(FAIL);
				}
				m_ipAddressLeaseRenewalTimeT1 = (i->data[0] << 24) | (i->data[1] << 16) | (i->data[2] << 8) | i->data[3];
				found = true;
			}
		}
		if(found == false)
		{
			return(FAIL);
		}
	}
	
	
	//	Save IP Address Rebinding Time T2 (Option Code 59)
	{
		bool found = false;
		for(auto& i: dhcpAck.options)
		{
			if(i->code == DHCP_Frame::e_option::REBINDING_TIME_VALUE)
			{
				if(i->data.get_size() != 4)
				{
					return(FAIL);
				}
				m_ipAddressLeaseRebindingTimeT2 = (i->data[0] << 24) | (i->data[1] << 16) | (i->data[2] << 8) | i->data[3];
				found = true;
			}
		}
		if(found == false)
		{
			return(FAIL);
		}
	}
	
	
	//	Start Lease Timer Thread to observe the Lease Time
	CMOS& cmos = CMOS::get();
	cmos.thread_shutdown(m_threadID_leaseTimer, true);
	m_threadID_leaseTimer = cmos.thread_create(&DHCP::leaseTimer, this, "DHCP Lease Timer", 50, 256);
	
	
	return(OK);
}


void DHCP::decline()
{
	//	Tbd
}







void DHCP::setDeviceName(const String& deviceName)
{
	m_deviceName = deviceName;
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

void DHCP::handlePacket(const DHCP_Frame& dhcpFrame)
{
	CMOS::get().send_mail(m_threadID, (uint32) &dhcpFrame);
}