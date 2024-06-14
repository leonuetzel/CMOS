#include "../../Inc/network/ipFrame.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

uint16 IP_Frame::calculate_checksum(const Array<uint8>& data)
{
	//	The Checksum is only calculated over the Header, not the Payload
	const uint32 length = 4 * internetHeaderLength;
	
	
	//	Sum up all 16-Bit Words of the Data (except Checksum Field)
	uint32 sum = 0;
	for(uint32 i = 0; i < length; i++)
	{
		//	Byte 10 and 11 are the Checksum
		if(i != 10 && i != 11)
		{
			sum += (data[2 * i] << 8) | data[2 * i + 1];
			
			if(bit::isSet(sum, 16) == true)
			{
				bit::clear(sum, 16);
				sum++;
			}
		}
	}
	
	
	//	Invert all Bits to do the One's Complement
	return(~sum);
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

IP_Frame::IP_Frame()
	:	version(),
		internetHeaderLength(),
		dscp(),
		ecn(),
		totalLength(),
		identification(),
		dontFragment(),
		moreFragments(),
		fragmentOffset(),
		timeToLive(),
		protocol(),
		headerChecksum(),
		sourceAddress(),
		destinationAddress(),
		options(),
		payload()
{
	
}


IP_Frame::IP_Frame(const Array<uint8>& data)
{
	//	Version
	version = (data[0] & 0xF0) >> 4;
	
	
	//	Internet Header Length
	//	Indicates Length of the IP Header in Units of 32 Bit
	//	Minimum Value is 5 -> Minimum of 5 x 32 Bit = 5 x 4 Byte = 20 Byte per Header
	internetHeaderLength = data[0] & 0x0F;
	if(internetHeaderLength < 5)
	{
		return;
	}
	
	
	//	Differentiated Services Code Point (DSCP)
	dscp = (e_DSCP) ((data[1] & 0xFC) >> 2);
	
	
	//	Explicit Congestion Notification (ECN)
	ecn = (e_ECN) (data[1] & 0x03);
	
	
	//	Total Length
	//	Total Length = Header Length + Payload Length
	totalLength = (data[2] << 8) | data[3];
	
	
	//	Identification
	//	Used for uniquely identifying the Group of Fragments of a single IP Datagram
	identification = (data[4] << 8) | data[5];
	
	
	//	Flags
	dontFragment = bit::isSet(data[6], 6);
	moreFragments = bit::isSet(data[6], 5);
	
	
	//	Fragment Offset
	//	ID for every Fragment, the first Fragment has ID 0
	//	Fragments are in Units of 8 Bytes
	fragmentOffset = ((data[6] & 0x1F) << 8) | data[7];
	
	
	//	Time to live
	timeToLive = data[8];
	
	
	//	Protocol
	protocol = (e_protocol) data[9];
	
	
	//	Header Checksum
	headerChecksum = (data[10] << 8) | data[11];
	
	
	//	Source Address
	for(uint32 i = 0; i < 4; i++)
	{
		sourceAddress[i] = data[i + 12];
	}
	
	
	//	Destination Address
	for(uint32 i = 0; i < 4; i++)
	{
		destinationAddress[i] = data[i + 16];
	}
	
	
	//	Options
	uint32 index = 20;
	while(1)
	{
		s_option option;
		
		
		//	Copied Bit
		option.copied = bit::isSet(data[index], 7);
		
		
		//	Option Class
		option.optionClass = (e_optionClass) ((data[index] & 0x60) >> 5);
		
		
		//	Option Number
		//	Terminate Option Parsing when "End of Option List" Option appears
		option.option = (e_option) (data[index] & 0x1F);
		if(option.option == e_option::END_OF_OPTIONS)
		{
			break;
		}
		
		
		//	Option Length
		//	Includes all Option Fields
		const uint32 optionLength = data[index + 1] - 2;
		
		
		//	Option Data
		if(optionLength > 0)
		{
			option.data.set_size(optionLength);
			for(uint32 i = 0; i < optionLength; i++)
			{
				option.data[i] = data[i + index + 2];
			}
		}
		
		
		//	Increment Index for next Option
		index++;
		if(optionLength > 0)
		{
			index += 1 + optionLength;
		}
	}
	
	
	//	Payload
	//	Starts after 4 x "Internet Header Length" Byte
	const uint32 payloadLength = totalLength - internetHeaderLength;
	payload.set_size(payloadLength);
	for(uint32 i = 0; i < payloadLength; i++)
	{
		payload[i] = data[i + 4 * internetHeaderLength];
	}
}


IP_Frame::~IP_Frame()
{
	
}







Array<uint8> IP_Frame::convertToByteStream()
{
	//	Calculate Internet Header Length
	//	Fixed Header Length is 20 Byte
	uint32 headerSize = 20;
	
	
	//	Optional Options Field has to be added
	if(options.get_size() > 0)
	{
		for(auto& i: options)
		{
			//	Option Type Field
			headerSize++;
			
			
			//	If Option has no Data, the Option Length Field and the Option Data Field can be omitted
			const uint32 optionLength = i.data.get_size();
			if(optionLength > 0)
			{
				headerSize += 1 + optionLength;
			}
		}
	}
	
	
	//	Align Header to 4 Byte Boundary
	if(headerSize % 4 != 0)
	{
		headerSize += 4 - (headerSize % 4);
	}
	
	
	//	Reserve Memory for the Packet so we dont have to extend the Array later (= expensive Copy Operations)
	const uint32 payloadLength = payload.get_size();
	const uint32 packetSize = headerSize + payloadLength;
	Array<uint8> data(0, packetSize);
	
	
	//	Set Internet Header Length Value
	//	It indicates the Length of the Header without the Payload in Units of 4 Byte
	//	Since it is a 4-Bit Field, the maximum Value is 15 (equals 60 Byte)
	internetHeaderLength = headerSize / 4;
	if(internetHeaderLength > 15)
	{
		return(Array<uint8>());
	}
	
	
	//	Total Length
	//	Total Length = Header Length + Payload Length
	totalLength = packetSize;
	
	
	//	Fill Packet
	data[0] = (version << 4) | internetHeaderLength;
	data[1] = (((uint8) dscp) << 2) | ((uint8) ecn);
	data[2] = (totalLength & 0xFF00) >> 8;
	data[3] = (totalLength & 0x00FF);
	data[4] = (identification & 0xFF00) >> 8;
	data[5] = (identification & 0x00FF);
	data[6] = (fragmentOffset & 0x1F00) >> 8;
	if(dontFragment == true)
	{
		bit::set(data[6], 6);
	}
	if(moreFragments == true)
	{
		bit::set(data[6], 5);
	}
	data[7] = (fragmentOffset & 0x00FF);
	data[8] = timeToLive;
	data[9] = (uint8) protocol;
	for(uint32 i = 0; i < 4; i++)
	{
		data[i + 12] = sourceAddress[i];
	}
	for(uint32 i = 0; i < 4; i++)
	{
		data[i + 16] = destinationAddress[i];
	}
	
	
	//	Options
	uint32 index = 20;
	for(auto& i: options)
	{
		//	Option Type
		data[index] = (((uint8) i.optionClass) << 5) | (uint8) i.option;
		if(i.copied == true)
		{
			bit::set(data[index], 7);
		}
		
		
		//	Option Length and Option Data can be omitted, if Option has no Data
		const uint32 optionLength = i.data.get_size();
		if(optionLength > 0)
		{
			//	Option Length
			//	Includes all Option Fields
			data[index + 1] = optionLength + 2;
			
			
			//	Option Data
			for(uint32 j = 0; j < optionLength; j++)
			{
				data[j + index + 2] = i.data[j];
			}
		}
		
		
		//	Increment Index
		index ++;
		if(optionLength > 0)
		{
			index += 1 + optionLength;
		}
	}
	
	
	//	Header Checksum (is only calculated over the Header)
	headerChecksum = calculate_checksum(data);
	data[10] = (headerChecksum & 0xFF00) >> 8;
	data[11] = (headerChecksum & 0x00FF);
	
	
	//	Payload
	for(uint32 i = 0; i < payloadLength; i++)
	{
		data[i + 20] = payload[i];
	}
	
	
	return(data);
}