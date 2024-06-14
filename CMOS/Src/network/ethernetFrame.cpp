#include "../../Inc/network/ethernetFrame.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

uint32 EthernetFrame::calculate_checksum(const Array<uint8>& data)
{
	//	CRC is calculated over all Fields except the CRC Field
	const uint32 length = data.get_size() - 4;
	
	
	constexpr uint32 crc_table[] =
  {
    0x4DBDF21C, 0x500AE278, 0x76D3D2D4, 0x6B64C2B0,
    0x3B61B38C, 0x26D6A3E8, 0x000F9344, 0x1DB88320,
    0xA005713C, 0xBDB26158, 0x9B6B51F4, 0x86DC4190,
    0xD6D930AC, 0xCB6E20C8, 0xEDB71064, 0xF0000000
  };
	
	
	uint32 crcValue = 0;
	for(uint32 i = 0; i < length; i++)
  {
    crcValue = (crcValue >> 4) ^ crc_table[(crcValue ^ (data[i] >> 0)) & 0x0F];
    crcValue = (crcValue >> 4) ^ crc_table[(crcValue ^ (data[i] >> 4)) & 0x0F];
  }
	
	uint32 crcReflected = ((crcValue & 0xFF000000) >> 24) | ((crcValue & 0x00FF0000) >> 8) | ((crcValue & 0x0000FF00) << 8) | ((crcValue & 0x000000FF) << 24);
	
	return(crcReflected);
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

EthernetFrame::EthernetFrame()
	:	etherType(e_etherType::UNKNOWN)
{
	
}


EthernetFrame::EthernetFrame(const Array<uint8>& data)
{
	//	MTU (Minimum Transmission Unit) is 64 Byte for an Ethernet Frame
	//	Maxmimum Ethernet Frame Size is 1522 Byte
	const uint32 ethernetFrameSize = data.get_size();
	if(ethernetFrameSize < 64 || ethernetFrameSize > 1522)
	{
		return;
	}
	
	
	
	//	Destination MAC Address
	for(uint32 i = 0; i < 6; i++)
	{
		MAC_destination[i] = data[i];
	}
	
	
	
	//	Source MAC Address
	for(uint32 i = 0; i < 6; i++)
	{
		MAC_source[i] = data[i + 6];
	}
	
	
	
	//	Ether Type
	//	Values < 1536 are considered the Payload's Size
	//	Values >= 1536 are an Ether Type ID
	uint32 payloadLength = 0;
	const uint16 type = ((data[12] << 8) | data[13]);
	if(type < 1536)
	{
		etherType = e_etherType::SIZE;
		payloadLength = type;
	}
	else
	{
		const e_etherType etherType_1 = (e_etherType) type;
		
		
		//	If Ether Type is VLAN, the VLAN tag is inserted instead of the Ether Type of the original Frame 
		//	This Way the original Ether Type is shifted by 4 Bytes
		if(etherType_1 == e_etherType::VLAN_IEEE802_1Q_IEEE802_1AQ_NNI)
		{
			//	Read the VLAN Tag
			for(uint32 i = 0; i < 4; i++)
			{
				VLAN_Tag[i] = data[i + 12];
			}
			
			
			//	Read the Ether Type of the original Frame
			etherType = (e_etherType) ((data[16] << 8) | data[17]);
		}
		else
		{
			//	Normal Ether Type
			etherType = etherType_1;
		}
	}
	
	
	
	//	Payload
	//	Calculate Start Byte of the Payload
	uint32 payloadStartByte = 14;
	if(etherType == e_etherType::VLAN_IEEE802_1Q_IEEE802_1AQ_NNI)
	{
		payloadStartByte += 4;
	}
	
	
	//	Payload Length only needs to be calculated, if not already provided by the Ether Type Field
	if(payloadLength == 0)
	{
		//	Payload Length is the Packet Size - 6 Byte Destination MAC Address - 6 Byte Source MAC Address - 2 Byte Ether Type ID - 4 Byte CRC
		//	If VLAN Tagging is used, an additional 4 Byte have to be subtracted
		payloadLength = data.get_size() - 6 - 6 - 2 - 4;
		if(etherType == e_etherType::VLAN_IEEE802_1Q_IEEE802_1AQ_NNI)
		{
			payloadLength -= 4;
		}
	}
	payload.set_size(payloadLength);
	
	
	
	//	Read Payload
	for(uint32 i = 0; i < payloadLength; i++)
	{
		payload[i] = data[i + payloadStartByte];
	}
	
	
	
	//	CRC
	crc = (data[ethernetFrameSize - 4] << 24) | (data[ethernetFrameSize - 3] << 16) | (data[ethernetFrameSize - 2] << 8) | data[ethernetFrameSize - 1];
}


EthernetFrame::~EthernetFrame()
{
	
}







Array<uint8> EthernetFrame::convertToByteStream()
{
	//	Calculate Packet Size
	uint32 packetSize = 18 + payload.get_size();
	
	
	//	If VLAN Tagging is used, the Packet Size increases by 4 Byte
	e_etherType vlanTagTest = (e_etherType) ((VLAN_Tag[0] << 8) | VLAN_Tag[1]);
	bool vlanTaggingUsed = false;
	if(vlanTagTest == e_etherType::VLAN_IEEE802_1Q_IEEE802_1AQ_NNI)
	{
		packetSize += 4;
		vlanTaggingUsed = true;
	}
	
	
	//	Check for maximum Packet Sizes
	if(packetSize > 1522)
	{
		return(Array<uint8>());
	}
	
	
	//	Pad with 0's if below the minimum Packet Size
	if(packetSize < 64)
	{
		const uint32 numberOfPaddingBytes = 64 - packetSize;
		payload.set_size(payload.get_size() + numberOfPaddingBytes, false);
		packetSize = 64;
	}
	
	
	//	Reserve Memory for the Packet to avoid extending the Array afterwards (= expensive Copy Operations)
	Array<uint8> data(0, packetSize);
	
	
	//	Fill Array
	//	Destination MAC Address
	for(uint32 i = 0; i < 6; i++)
	{
		data[i] = MAC_destination[i];
	}
	
	
	//	Source MAC Address
	for(uint32 i = 0; i < 6; i++)
	{
		data[i + 6] = MAC_source[i];
	}
	
	
	//	Optional VLAN Tagging
	uint32 index = 12;
	if(vlanTaggingUsed == true)
	{
		for(uint32 i = 0; i < 4; i++)
		{
			data[i + index] = VLAN_Tag[i];
		}
		index += 4;
	}
	
	
	//	Ether Type
	data[index + 0] = (((uint16) etherType) & 0xFF00) >> 8;
	data[index + 1] = (((uint16) etherType) & 0x00FF);
	index += 2;
	
	
	//	Payload
	const uint32 payloadLength = payload.get_size();
	for(uint32 i = 0; i < payloadLength; i++)
	{
		data[i + index] = payload[i];
	}
	index += payloadLength;
	
	
	//	32-Bit CRC Checksum
	crc = calculate_checksum(data);
	data[index + 0] = (crc & 0xFF000000) >> 24;
	data[index + 1] = (crc & 0x00FF0000) >> 16;
	data[index + 2] = (crc & 0x0000FF00) >> 8;
	data[index + 3] = (crc & 0x000000FF);
	
	
	return(data);
}