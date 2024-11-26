#include "../../Inc/network/network.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

IP::IP(const uint8& networkID)
	:	m_networkID(networkID),
		m_ipAddress(0, 4),
		m_datagramIdentificationCounter(0),
		m_packetBuffer()
{
	
}


IP::~IP()
{
	
}







const IP_Frame* IP::handleFragmentedPacket(uint16 datagramIdentification)
{
	//	Search for the last Packet of the IP Datagram
	//	It has the "More Fragments" Bit cleared indicating being the last Packet of that Datagram
	uint32 packetSize = 0;
	for(auto& i: m_packetBuffer)
	{
		if(i->identification == datagramIdentification)
		{
			if(i->moreFragments == false)
			{
				packetSize = i->fragmentOffset * 8 + i->totalLength - i->internetHeaderLength * 4;
			}
		}
	}
	if(packetSize == 0)
	{
		return(nullptr);
	}
	
	
	//	Search the Buffer for all Packets belonging to the Datagram Identification
	//	Place these Packets in an Array in ascending Fragment Offset Order
	//	Triplet Format:
	//	(1) Index of Packet Position in m_packetBuffer
	//	(2) Byte Offset of Packet (not Fragment Offset)
	//	(3) Payload Length of Fragment
	Array<Triplet<uint32, uint16, uint16>> fragments;
	for(uint32 i = 0; i < m_packetBuffer.get_size(); i++)
	{
		const IP_Frame& ipFrame = *m_packetBuffer[i];
		if(ipFrame.identification == datagramIdentification)
		{
			const uint32 payloadLength = ipFrame.totalLength - ipFrame.internetHeaderLength * 4;
			Triplet<uint32, uint16, uint16> fragment(i, ipFrame.fragmentOffset * 8, payloadLength);
			
			
			//	Calculate Position in Array where to insert the Triplet
			uint32 insertIndex = 0;
			for(uint32 j = 0; j < fragments.get_size(); j++)
			{
				if(fragments[j].second() < fragment.second())
				{
					insertIndex++;
				}
			}
			
			
			//	Insert Triplet
			if(insertIndex >= fragments.get_size())
			{
				fragments += fragment;
			}
			else
			{
				fragments.insert(insertIndex, fragment);
			}
		}
	}
	
	
	//	Check if all Fragments have arrived and are plausible
	const uint32 numberOfFragments = fragments.get_size();
	if(numberOfFragments == 0)
	{
		return(nullptr);
	}
	
	
	//	First Fragment has to start at 0
	if(fragments[0].second() != 0)
	{
		return(nullptr);
	}
	
	
	for(uint32 i = 1; i < numberOfFragments; i++)
	{
		if(fragments[i - 1].second() + fragments[i - 1].third() != fragments[i].second())
		{
			return(nullptr);
		}
	}
	
	
	//	All Fragments have arrived and are plausible
	//	Now, assemble them to on IP_Frame
	//	Copy the first Fragment to get the IP Header
	IP_Frame* ipFrame = new IP_Frame(*m_packetBuffer[fragments[0].first()]);
	
	
	//	Clear the "More Fragments" Bit and adjust the "Total Length" Field
	ipFrame->moreFragments = false;
	ipFrame->totalLength = ipFrame->internetHeaderLength * 4 + packetSize;
	
	
	//	Reserve Memory for the whole Payload
	ipFrame->payload.set_size(packetSize);
	
	
	//	Copy the Payload
	uint32 index = 0;
	for(auto& i: fragments)
	{
		const IP_Frame& fragment = *m_packetBuffer[i.first()];
		for(uint32 j = 0; j < i.third(); j++)
		{
			ipFrame->payload[j + index] = fragment.payload[j];
		}
		index += i.third();
	}
	
	
	//	Delete Fragments from Packet Buffer
	for(uint32 i = 0; i < m_packetBuffer.get_size(); i++)
	{
		if(m_packetBuffer[i]->identification == datagramIdentification)
		{
			delete m_packetBuffer[i];
			m_packetBuffer.erase(i);
			i--;
		}
	}
	
	
	return(ipFrame);
}


void IP::packetHandler(const IP_Frame& ipFrame)
{
	Network& network = Network::get(m_networkID);
	switch(ipFrame.protocol)
	{
		case IP_Frame::e_protocol::UDP:
		{
			UDP_Frame udpFrame(ipFrame.payload);
			network.m_udp.handlePacket(udpFrame);
		}
		break;
		
		case IP_Frame::e_protocol::TCP:
		{
			//	Extract Source IP Address
			Array<uint8> sourceIpAddress(0, 4);
			for(uint32 i = 0; i < 4; i++)
			{
				sourceIpAddress[i] = ipFrame.sourceAddress[i];
			}
			
			
			//	Handle the TCP Packet
			TCP_Frame tcpFrame(ipFrame.payload);
			network.m_tcp.handlePacket(tcpFrame, sourceIpAddress);
		}
		
		default:
		{
			
		}
		break;
	}
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

void IP::handlePacket(const IP_Frame& ipFrame)
{
	//	If Packet is fragmented, wait for all Fragments to arrive
	if(ipFrame.moreFragments == true || ipFrame.fragmentOffset != 0)
	{
		//	Add Fragment to the internal Fragment Buffer
		m_packetBuffer += new IP_Frame(ipFrame);
		
		
		//	Try to assemble a full IP Datagram from the available Fragments if possible
		const IP_Frame* ipFrameAssembled = handleFragmentedPacket(ipFrame.identification);
		if(ipFrameAssembled != nullptr)
		{
			//	Handle the assembled IP Datagram
			packetHandler(*ipFrameAssembled);
			delete ipFrameAssembled;
		}
	}
	else
	{
		//	Packet is not fragmented and can be handled immediately
		packetHandler(ipFrame);
	}
}







feedback IP::set_ipAddress(const Array<uint8>& ipAddress)
{
	if(ipAddress.get_size() == 4)
	{
		m_ipAddress = ipAddress;
		return(OK);
	}
	return(FAIL);
}







feedback IP::tx(const Array<uint8>& data, IP_Frame::e_protocol protocol, const Array<uint8>& destinationIpAddress)
{
	//	Dont proceed if Network isnt attached
	if(m_networkID == Network::networkID_invalid)
	{
		return(FAIL);
	}
	
	
	//	Check destinationIpAddress to be 4 Byte long
	if(destinationIpAddress.get_size() != 4)
	{
		return(FAIL);
	}
	
	
	//	Get MTU of underlying Layer in the Network Stack
	Network& network = Network::get(m_networkID);
	const uint32 maximumTransmissionUnit = network.m_physicalLayer.get_maximumTransmissionUnit();
	
	
	//	Determine Ethernet Destination MAC Address
	//	If Broadcast IP Address, Ethernet Destination MAC Address is Broadcast too
	Array<uint8> destinationMacAddress(0xFF, 6);
	if(destinationIpAddress != Array<uint8>(0xFF, 4))
	{
		//	Get IP Address of the Router from DHCP
		const Array<uint8>& ipAddressRouter = network.m_dhcp.get_ipAddressRouter();
		
		
		//	Get MAC Address of the Router from ARP
		destinationMacAddress = network.m_arp.get_hardwareAddress(ipAddressRouter, true);
	}
	
	
	//	Send the Datagram without Fragmentation if possible
	if(data.get_size() <= maximumTransmissionUnit)
	{
		IP_Frame ipFrame;
		ipFrame.version = 4;
		ipFrame.ecn = IP_Frame::e_ECN::NOT_CAPABLE;
		ipFrame.identification = m_datagramIdentificationCounter;
		m_datagramIdentificationCounter++;
		ipFrame.dontFragment = false;
		ipFrame.moreFragments = false;
		ipFrame.fragmentOffset = 0;
		ipFrame.timeToLive = 64;
		ipFrame.protocol = protocol;
		for(uint32 i = 0; i < 4; i++)
		{
			ipFrame.sourceAddress[i] = m_ipAddress[i];
		}
		for(uint32 i = 0; i < 4; i++)
		{
			ipFrame.destinationAddress[i] = destinationIpAddress[i];
		}
		ipFrame.payload = data;
		
		
		//	Send Packet to underlying Layer in the Network Stack
		return(network.m_ethernet.tx(ipFrame.convertToByteStream(), destinationMacAddress, EthernetFrame::e_etherType::IPv4));
	}
	
	
	//	Fragmentation is needed
	
	//	Calculate the Maximum Fragment Size = MTU - 20 Byte IP Header - IP Options Field Size
	//	Since we dont use the Options Field (by now), IP Options Field Size = 0
	//	Fragment Size has to be a Multiple of 8 because of the Fragment Offset Field in the IP Header being in Units of 8 Byte
	uint32 maximumFragmentSize = maximumTransmissionUnit - 20;
	if(maximumFragmentSize % 8 != 0)
	{
		maximumFragmentSize -= maximumFragmentSize % 8;
	}
	
	
	//	Calculate the Number of IP Frames needed
	const uint32 payloadLength = data.get_size();
	uint32 numberOfIpFrames = payloadLength / maximumFragmentSize;
	if(payloadLength % maximumFragmentSize != 0)
	{
		numberOfIpFrames++;
	}
	
	
	//	Create the IP Frames
	for(uint32 i = 0; i < numberOfIpFrames; i++)
	{
		IP_Frame ipFrame;
		ipFrame.version = 4;
		ipFrame.ecn = IP_Frame::e_ECN::NOT_CAPABLE;
		ipFrame.identification = m_datagramIdentificationCounter;
		ipFrame.dontFragment = false;
		if(i < numberOfIpFrames - 1)
		{
			ipFrame.moreFragments = true;
		}
		else
		{
			ipFrame.moreFragments = false;
		}
		ipFrame.fragmentOffset = i * (maximumFragmentSize / 8);
		ipFrame.timeToLive = 64;
		ipFrame.protocol = protocol;
		for(uint32 i = 0; i < 4; i++)
		{
			ipFrame.sourceAddress[i] = m_ipAddress[i];
		}
		for(uint32 i = 0; i < 4; i++)
		{
			ipFrame.destinationAddress[i] = destinationIpAddress[i];
		}
		uint32 fragmentSize = maximumFragmentSize;
		if(i >= numberOfIpFrames - 1)
		{
			fragmentSize = payloadLength - i * maximumFragmentSize;
		}
		ipFrame.payload = data.sub(i * maximumFragmentSize, fragmentSize);
		
		if(network.m_ethernet.tx(ipFrame.convertToByteStream(), destinationMacAddress, EthernetFrame::e_etherType::IPv4) != OK)
		{
			//	Increment the Datagram Identification Counter
			m_datagramIdentificationCounter++;
			return(FAIL);
		}
	}
	
	
	//	Increment the Datagram Identification Counter
	m_datagramIdentificationCounter++;
	
	
	return(OK);
}