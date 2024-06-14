#include "../../Inc/network/arpFrame.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

ARP_Frame::ARP_Frame()
{
	for(uint32 i = 0; i < 6; i++)
	{
		senderHardwareAddress[i] = 0;
		targetHardwareAddress[i] = 0;
	}
	for(uint32 i = 0; i < 4; i++)
	{
		senderProtocolAddress[i] = 0;
		targetProtocolAddress[i] = 0;
	}
}


ARP_Frame::ARP_Frame(const Array<uint8>& data)
	:	protocolType(e_protocolType::UNKNOWN)
{
	//	Hardware Type
	hardwareType = (e_hardwareType) ((data[0] << 8) | data[1]);
	
	
	//	Protocol Type
	//	We only handle IPv4 ARP Requests here
	protocolType = (e_protocolType) ((data[2] << 8) | data[3]);
	if(protocolType != e_protocolType::IPv4)
	{
		return;
	}
	
	
	//	Hardware Address Length
	hardwareAddressLength = data[4];
	
	
	//	Protocol Address Length
	protocolAddressLength = data[5];
	
	
	//	Operation Code
	operationCode = (e_operationCode) ((data[6] << 8) | data[7]);
	
	
	//	Sender Hardware Address
	for(uint32 i = 0; i < 6; i++)
	{
		senderHardwareAddress[i] = data[i + 8];
	}
	
	
	//	Sender Protocol Address
	for(uint32 i = 0; i < 4; i++)
	{
		senderProtocolAddress[i] = data[i + 14];
	}
	
	
	//	Target Hardware Address
	for(uint32 i = 0; i < 6; i++)
	{
		targetHardwareAddress[i] = data[i + 18];
	}
	
	
	//	Target Protocol Address
	for(uint32 i = 0; i < 4; i++)
	{
		targetProtocolAddress[i] = data[i + 24];
	}
}







Array<uint8> ARP_Frame::convertToByteStream()
{
	Array<uint8> data(0, 28);
	
	
	//	Hardware Type
	data[0] = (((uint16) hardwareType) & 0xFF00) >> 8;
	data[1] = (((uint16) hardwareType) & 0x00FF);
	
	
	//	Protocol Type
	data[2] = (((uint16) protocolType) & 0xFF00) >> 8;
	data[3] = (((uint16) protocolType) & 0x00FF);
	
	
	//	Hardware Address Length
	data[4] = hardwareAddressLength;
	
	
	//	Protocol Address Length
	data[5] = protocolAddressLength;
	
	
	//	Operation Code
	data[6] = (((uint16) operationCode) & 0xFF00) >> 8;
	data[7] = (((uint16) operationCode) & 0x00FF);
	
	
	//	Sender Hardware Address
	for(uint32 i = 0; i < 6; i++)
	{
		data[i + 8] = senderHardwareAddress[i];
	}
	
	
	//	Sender Protocol Address
	for(uint32 i = 0; i < 4; i++)
	{
		data[i + 14] = senderProtocolAddress[i];
	}
	
	
	//	Target Hardware Address
	for(uint32 i = 0; i < 6; i++)
	{
		data[i + 18] = targetHardwareAddress[i];
	}
	
	
	//	Target Protocol Address
	for(uint32 i = 0; i < 4; i++)
	{
		data[i + 24] = targetProtocolAddress[i];
	}
	
	
	return(data);
}