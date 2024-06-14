#include "../../Inc/network/tcpFrame.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

uint16 TCP_Frame::calculate_checksum(const Array<uint8>& data, const Array<uint8>& sourceIpAddress, const Array<uint8>& destinationIpAddress)
{
	//	The Checksum is calculated over:
	//	-	Pseudo IP Header
	//	-	TCP Header with Checksum Field set to 0
	//	-	TCP Payload
	
	
	//	The Pseudo IP Header consists of:
	//	-	Source IP Address																	 4 Byte
	//	-	Destination IP Address														 4 Byte
	//	-	Fixed Byte (always 0x00)													 1 Byte
	//	-	Protocol Field from IP Header (0x06 for TCP)			 1 Byte
	//	-	TCP Length (TCP Header + TCP Payload Length)			 2 Byte
	//																											-------
	//	Total Pseudo IP Header Size													12 Byte
	Array<uint8> pseudoIPHeader(0, 12);
	for(uint32 i = 0; i < 4; i++)
	{
		pseudoIPHeader[i] = sourceIpAddress[i];
		pseudoIPHeader[i + 4] = destinationIpAddress[i];
	}
	pseudoIPHeader[9] = 0x06;
	
	const uint16 tcpLength = 20 + 4 * options.get_size() + payload.get_size();
	pseudoIPHeader[10] = (tcpLength & 0xFF00) >> 8;
	pseudoIPHeader[11] = (tcpLength & 0x00FF);
	
	
	//	Create the Data Stream over which the Checksum is created
	const uint32 dataStreamLength = 12 + tcpLength;
	Array<uint16> dataStream(0, dataStreamLength / 2);
	
	for(uint32 i = 0; i < 6; i++)
	{
		dataStream[i] = (pseudoIPHeader[i * 2] << 8) | pseudoIPHeader[i * 2 + 1];
	}
	for(uint32 i = 0; i < tcpLength / 2; i++)
	{
		dataStream[i + 6] = (data[i * 2] << 8) | data[i * 2 + 1];
	}
	
	
	//	Do One's Complement by performing 32-Bit Addition
	uint32 sum = 0;
	for(uint32 i = 0; i < dataStreamLength / 2; i++)
	{
		sum += (uint32) dataStream[i];
	}
	
	while(sum > 0x0000FFFF)
	{
		sum = (sum & 0x0000FFFF) + ((sum >> 16) & 0x0000FFFF);
	}
	
	return(~sum);
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

TCP_Frame::TCP_Frame()
	:	sourcePort(),
		destinationPort(),
		sequenceNumber(),
		ackNumber(),
		dataOffset(),
		congestionWindowReduced(),
		explicitCongestionNotification(),
		urgent(),
		ack(),
		push(),
		reset(),
		synchronize(),
		finish(),
		receiveWindow(),
		checksum(),
		urgentPointer(),
		options(),
		payload()
{
	
}


TCP_Frame::TCP_Frame(const Array<uint8>& data)
{
	//	Source Port
	sourcePort = (data[0] << 8) | data[1];
	
	
	//	Destination Port
	destinationPort = (data[2] << 8) | data[3];
	
	
	//	Sequence Number
	sequenceNumber = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];
	
	
	//	Acknowledgement Number
	ackNumber = (data[8] << 24) | (data[9] << 16) | (data[10] << 8) | data[11];
	
	
	//	Data Offset
	dataOffset = (data[12] >> 4) & 0x0F;
	
	
	// Congestion Window Reduced Flag
	congestionWindowReduced = bit::isSet(data[13], 7);
	
	
	//	Explicit Congestion Notification Flag
	explicitCongestionNotification = bit::isSet(data[13], 6);
	
	
	//	Urgent Flag
	urgent = bit::isSet(data[13], 5);
	
	
	//	Acknowledgment Flag
	ack = bit::isSet(data[13], 4);
	
	
	//	Push Flag
	push = bit::isSet(data[13], 3);
	
	
	//	Reset Flag
	reset = bit::isSet(data[13], 2);
	
	
	//	Synchronize Flag
	synchronize = bit::isSet(data[13], 1);
	
	
	//	Finish Flag
	finish = bit::isSet(data[13], 0);
	
	
	//	Receive Window
	receiveWindow = (data[14] << 8) | data[15];
	
	
	//	Checksum
	checksum = (data[16] << 8) | data[17];
	
	
	//	Urgent Pointer
	urgentPointer = (data[18] << 8) | data[19];
	
	
	//	Options
	const uint32 dataStartByte = 4 * dataOffset;
	{
		uint32 index = 20;
		while(index < dataStartByte)
		{
			s_option option;
			
			
			//	Option Kind
			option.kind = (e_optionKind) data[index];
			if(option.kind == e_optionKind::END_OF_OPTIONS || option.kind == e_optionKind::NO_OPERATION)
			{
				//	Add Option to our Options
				options += option;
				
				
				//	Increment Index by 1
				index++;
				
				
				//	Terminate Option Decoding if "End of Options" Option is encountered
				if(option.kind == e_optionKind::END_OF_OPTIONS)
				{
					break;
				}
			}
			else
			{
				//	Every other Option has an additional "Length" and "Data" Field
				
				
				//	Option Length
				const uint8 optionLength = data[index + 1] - 2;
				
				
				//	Option Data
				option.data.set_size(optionLength);
				for(uint32 i = 0; i < optionLength; i++)
				{
					option.data[i] = data[index + 2 + i];
				}
				
				
				//	Add Option to our Options
				options += option;
				
				
				//	Increment Index
				index += 2 + optionLength;
			}
		}
	}
	
	
	//	Payload
	const uint32 payloadLength = data.get_size() - dataStartByte;
	payload.set_size(payloadLength);
	for(uint32 i = 0; i < payloadLength; i++)
	{
		payload[i] = data[i + dataStartByte];
	}
}


TCP_Frame::~TCP_Frame()
{
	
}







Array<uint8> TCP_Frame::convertToByteStream(const Array<uint8>& sourceIpAddress, const Array<uint8>& destinationIpAddress)
{
	//	Calculate Array Size
	//	20 Byte fixed Header Size + Option's Field Size + Payload Size
	
	
	//	Calculate Size of Option's Field
	uint32 optionsFieldSize = 0;
	for(auto& i: options)
	{
		optionsFieldSize += i.data.get_size() + 1;
		
		
		//	Add 1 Byte for the "Length" Field that is not included in the "End of Options" and the "No Operation" Options
		if(i.kind != e_optionKind::END_OF_OPTIONS && i.kind != e_optionKind::NO_OPERATION)
		{
			optionsFieldSize++;
		}
	}
	
	
	//	Align Option's Field to 4-Byte Boundary
	//	Since "End of Options" Option has the Code 0, it is automatically appended to the other Options
	if(optionsFieldSize % 4 != 0)
	{
		optionsFieldSize += 4 - (optionsFieldSize % 4);
	}
	
	
	const uint32 payloadLength = payload.get_size();
	const uint32 packetSize = 20 + optionsFieldSize + payloadLength;
	
	
	//	Reserve Memory for Packet so we dont have to extend the Array later (= expensive Copy Operations)
	Array<uint8> data(0, packetSize);
	
	
	//	Source Port
	data[0] = (sourcePort & 0xFF00) >> 8;
	data[1] = (sourcePort & 0x00FF);
	
	
	//	Destination Port
	data[2] = (destinationPort & 0xFF00) >> 8;
	data[3] = (destinationPort & 0x00FF);
	
	
	//	Sequence Number
	data[4] = (sequenceNumber & 0xFF000000) >> 24;
	data[5] = (sequenceNumber & 0x00FF0000) >> 16;
	data[6] = (sequenceNumber & 0x0000FF00) >> 8;
	data[7] = (sequenceNumber & 0x000000FF);
	
	
	//	Acknowledgement Number
	data[8]		= (ackNumber & 0xFF000000) >> 24;
	data[9]		= (ackNumber & 0x00FF0000) >> 16;
	data[10]	= (ackNumber & 0x0000FF00) >> 8;
	data[11]	= (ackNumber & 0x000000FF);
	
	
	//	Data Offset
	//	Payload starts after fixed 20 Byte of Header + 4 Byte per Option
	//	The Unit of the Data Offset Field is 4 Byte
	dataOffset = 5 + optionsFieldSize / 4;
	data[12] = (dataOffset & 0x0F) << 4;
	
	
	//	Congestion Window Reduced Flag
	if(congestionWindowReduced == true)
	{
		bit::set(data[13], 7);
	}
	
	
	//	Explicit Congestion Notification Flag
	if(explicitCongestionNotification == true)
	{
		bit::set(data[13], 6);
	}
	
	
	//	Urgent Flag
	if(urgent == true)
	{
		bit::set(data[13], 5);
	}
	
	
	//	Acknowledgement Flag
	if(ack == true)
	{
		bit::set(data[13], 4);
	}
	
	
	//	Push Flag
	if(push == true)
	{
		bit::set(data[13], 3);
	}
	
	
	//	Reset Flag
	if(reset == true)
	{
		bit::set(data[13], 2);
	}
	
	
	//	Synchronize Flag
	if(synchronize == true)
	{
		bit::set(data[13], 1);
	}
	
	
	//	Finish Flag
	if(finish == true)
	{
		bit::set(data[13], 0);
	}
	
	
	//	Receive Window
	data[14] = (receiveWindow & 0xFF00) >> 8;
	data[15] = (receiveWindow & 0x00FF);
	
	
	//	Urgent Pointer
	data[18] = (urgentPointer & 0xFF00) >> 8;
	data[19] = (urgentPointer & 0x00FF);
	
	
	//	Options
	{
		uint32 index = 20;
		for(auto& i: options)
		{
			data[index] = (uint8) i.kind;
			
			if(i.kind != e_optionKind::END_OF_OPTIONS && i.kind != e_optionKind::NO_OPERATION)
			{
				const uint32 optionLength = i.data.get_size();
				data[index + 1] = optionLength + 2;
				for(uint32 j = 0; j < optionLength; j++)
				{
					data[j + index + 2] = i.data[j];
				}
				
				index += optionLength + 1;
			}
			index++;
		}
	}
	
	
	//	Payload
	for(uint32 i = 0; i < payloadLength; i++)
	{
		data[dataOffset * 4 + i] = payload[i];
	}
	
	
	//	Checksum
	const uint16 checksum = calculate_checksum(data, sourceIpAddress, destinationIpAddress);
	data[16] = (checksum & 0xFF00) >> 8;
	data[17] = (checksum & 0x00FF);
	
	
	return(data);
}