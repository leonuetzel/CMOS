#include "../../Inc/network/udpFrame.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

UDP_Frame::UDP_Frame()
	:	sourcePort(),
		destinationPort(),
		checkSum(),
		payload()
{
	
}


UDP_Frame::UDP_Frame(const Array<uint8>& data)
{
	//	Source Port
	sourcePort = (data[0] << 8) | data[1];
	
	
	//	Destination Port
	destinationPort = (data[2] << 8) | data[3];
	
	
	//	Length (in Byte)
	//	Minimum Length is 8 (Length of Header)
	const uint16 length = (data[4] << 8) | data[5];
	if(length < 8)
	{
		return;
	}
	
	
	//	Checksum
	checkSum = (data[6] << 8) | data[7];
	
	
	//	Payload
	const uint32 payloadLength = length - 8;
	payload.set_size(payloadLength);
	for(uint32 i = 0; i < payloadLength; i++)
	{
		payload[i] = data[i + 8];
	}
}


UDP_Frame::~UDP_Frame()
{
	
}







Array<uint8> UDP_Frame::convertToByteStream() const
{
	//	Calculate Packet Size
	//	4 x 2 Byte for the Header + Payload Length
	const uint16 payloadLength = payload.get_size();
	const uint16 length = 8 + payloadLength;
	uint32 packetSize = length;
	
	
	//	Payload must consist of an even Number of Byte
	//	Padding Bytes at the End of the Payload are filled with 0's
	if(packetSize % 2 != 0)
	{
		packetSize++;
	}
	
	
	//	Reserve Memory for Packet, so we dont have to extend it later (= expensive Copy Operations)
	Array<uint8> data(0, packetSize);
	
	
	//	Fill Packet
	data[0] = (sourcePort & 0xFF00) >> 8;
	data[1] = (sourcePort & 0x00FF);
	
	data[2] = (destinationPort & 0xFF00) >> 8;
	data[3] = (destinationPort & 0x00FF);
	
	data[4] = (length & 0xFF00) >> 8;
	data[5] = (length & 0x00FF);
	
	data[6] = (checkSum & 0xFF00) >> 8;
	data[7] = (checkSum & 0x00FF);
	
	for(uint32 i = 0; i < payloadLength; i++)
	{
		data[i + 8] = payload[i];
	}
	
	
	return(data);
}