#include "../../Inc/network/dhcpFrame.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

DHCP_Frame::DHCP_Frame()
	:	operationCode(),
		hardwareType(),
		hardwareAddressLength(),
		hops(),
		xID(),
		seconds(),
		broadcast(),
		clientIPaddress(),
		yourIPaddress(),
		serverIPaddress(),
		gatewayIPaddress(),
		clientHardwareAddress(),
		serverName(),
		bootFileName(),
		magicCookie(),
		options()
{
	
}


DHCP_Frame::DHCP_Frame(const Array<uint8>& data)
{
	//	Check minimum required Size for DHCP Frame
	if(data.get_size() < 236)
	{
		return;
	}
	
	
	//	Operation Code
	operationCode = data[0];
	
	
	//	Hardware Type
	hardwareType = (e_hardwareType) data[1];
	
	
	//	Hardware Address Length
	hardwareAddressLength = data[2];
	
	
	//	Hops
	hops = data[3];
	
	
	//	X ID
	xID = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];;
	
	
	//	Seconds
	seconds = (data[8] << 8) | data[9];
	
	
	//	Broadcast Flag
	broadcast = bit::isSet(data[10], 7);
	
	
	//	Client IP Address
	for(uint32 i = 0; i < 4; i++)
	{
		clientIPaddress[i] = data[i + 12];
	}
	
	
	//	"Your" IP Address
	for(uint32 i = 0; i < 4; i++)
	{
		yourIPaddress[i] = data[i + 16];
	}
	
	
	//	Server IP Address
	for(uint32 i = 0; i < 4; i++)
	{
		serverIPaddress[i] = data[i + 20];
	}
	
	
	//	Gateway IP Address
	for(uint32 i = 0; i < 4; i++)
	{
		gatewayIPaddress[i] = data[i + 24];
	}
	
	
	//	Client Hardware Address
	for(uint32 i = 0; i < 16; i++)
	{
		clientHardwareAddress[i] = data[i + 28];
	}
	
	
	//	Server Name
	for(uint32 i = 0; i < 64; i++)
	{
		serverName[i] = data[i + 44];
	}
	
	
	//	Boot File Name
	for(uint32 i = 0; i < 128; i++)
	{
		bootFileName[i] = data[i + 108];
	}
	
	
	//	Options
	if(data.get_size() > 236)
	{
		//	Magic Cookie
		//	Value: 0x63 0x82 0x53 0x63
		for(uint32 i = 0; i < 4; i++)
		{
			magicCookie[i] = data[i + 236];
		}
		
		if(magicCookie[0] != 0x63 || magicCookie[1] != 0x82 || magicCookie[2] != 0x53 || magicCookie[3] != 0x63)
		{
			return;
		}
		
		
		//	Read all Options
		constexpr uint32 optionsStartIndex = 240;
		uint32 offset = 0;
		while(1)
		{
			//	Start Index of this particular Option
			const uint32 optionStartIndex = optionsStartIndex + offset;
			
			
			//	Read Option Code
			//	If End of Options is reached, stop reading Options
			e_option code = (e_option) data[optionStartIndex];
			if(code == e_option::END_OF_OPTIONS)
			{
				break;
			}
			
			
			//	Skip reading this Option if its a Padding Option
			if(code == e_option::PADDING)
			{
				//	Increment Option Offset by 1 Byte
				//	If Option is a padding Option, the Option Length and Option Data Fields are omitted, reducing the Option's Size Usage to 1 Byte
				offset++;
			}
			else
			{
				//	Allocate Memory for the Option
				s_option& option = *new s_option;
				
				
				//	Option Code
				option.code = code;
				
				
				//	Read Option Length
				//	This Value does not include the 2 Byte used for the Option Code and Option Length
				const uint32 length = data[optionStartIndex + 1];
				
				
				//	Read Option Data
				option.data.set_size(length);
				for(uint32 i = 0; i < length; i++)
				{
					option.data[i] = data[i + optionStartIndex + 2];
				}
				
				
				//	Append the read Option to the Options Array
				options += &option;
				
				
				//	Increment Option Offset by the 2 Byte (Option Code + Option Length) and <length> Byte (Option Data)
				offset += 2 + length;
			}
		}
	}
}


DHCP_Frame::~DHCP_Frame()
{
	for(auto& i: options)
	{
		delete i;
	}
}







Array<uint8> DHCP_Frame::convertToByteStream()
{
	//	Delete padding Options and "End of Options" Option
	for(uint32 i = 0; i < options.get_size(); i++)
	{
		s_option& option = *options[i];
		if(option.code == e_option::PADDING || option.code == e_option::END_OF_OPTIONS)
		{
			options.erase(i);
			i--;
		}
	}
	
	
	//	Sort Options so maximum one Padding Option is required
	sortOptionsForBetterAlignment();
	
	
	//	Reserve Memory for Packets
	//	Minimum Size is 236 Byte
	uint32 packetSize = 236;
	
	
	//	Optional Options
	bool paddingOptionNeeded = false;
	if(options.get_size() > 0)
	{
		//	If Options are specified, the Magic Cookie has to be included, consisting of 4 Byte
		packetSize += 4;
		
		//	Go through the Options and sum up the total Size
		for(uint32 i = 0; i < options.get_size(); i++)
		{
			packetSize += 2 + options[i]->data.get_size();
		}
		
		
		//	Packetsize has to be even after the "End of Options" Option has been appended
		//	Since the "End of Options" Options only has 1 Byte in Size (Option Length and Option Data are omitted), we have to check for an even Number of Bytes now
		if(packetSize % 2 == 0)
		{
			//	Create Padding Option then (Option Length and Option Data are omitted) so the Option Size is 1 Byte
			//	-> Used for aligning the Option Field to the required 2-Byte Boundary
			paddingOptionNeeded = true;
			
			
			//	Update the Packet Size
			packetSize++;
		}
		
		
		//	Add the "End of Options" Option Size to the Packet Size
		packetSize++;
	}
	
	
	//	Reserve Memory for the whole DHCP Frame, so we dont have to extend (= expensive Copy-Operation) the Array afterwards
	Array<uint8> data(0, packetSize);
	
	
	//	Fill Data Array now
	//	Fixed Fields first
	data[0] = (uint8) operationCode;
	data[1] = (uint8) hardwareType;
	data[2] = hardwareAddressLength;
	data[3] = hops;
	data[4] = (xID & 0xFF000000) >> 24;
	data[5] = (xID & 0x00FF0000) >> 16;
	data[6] = (xID & 0x0000FF00) >> 8;
	data[7] = (xID & 0x000000FF);
	data[8] = (seconds & 0xFF00) >> 8;
	data[9] = (seconds & 0x00FF);
	if(broadcast == true)
	{
		bit::set(data[10], 7);
	}
	for(uint32 i = 0; i < 4; i++)
	{
		data[i + 12] = clientIPaddress[i];
	}
	for(uint32 i = 0; i < 4; i++)
	{
		data[i + 16] = yourIPaddress[i];
	}
	for(uint32 i = 0; i < 4; i++)
	{
		data[i + 20] = serverIPaddress[i];
	}
	for(uint32 i = 0; i < 4; i++)
	{
		data[i + 24] = gatewayIPaddress[i];
	}
	for(uint32 i = 0; i < 16; i++)
	{
		data[i + 28] = clientHardwareAddress[i];
	}
	for(uint32 i = 0; i < 64; i++)
	{
		data[i + 44] = serverName[i];
	}
	for(uint32 i = 0; i < 128; i++)
	{
		data[i + 108] = bootFileName[i];
	}
	
	
	//	Options then
	if(options.get_size() > 0)
	{
		//	Magic Cookie
		//	Value: 0x63 0x82 0x53 0x63
		data[236] = 0x63;
		data[237] = 0x82;
		data[238] = 0x53;
		data[239] = 0x63;
		
		
		uint32 index = 240;
		for(auto& i: options)
		{
			//	Option Code
			data[index] = (uint8) i->code;
			
			
			//	Option Length
			const uint32 optionLength = i->data.get_size();
			data[index + 1] = optionLength;
			
			
			//	Option Data
			for(uint32 j = 0; j < optionLength; j++)
			{
				data[j + index + 2] = i->data[j];
			}
			
			
			//	Increment Index
			index += 2 + optionLength;
		}
		
		
		//	Append a Padding Option if needed
		if(paddingOptionNeeded == true)
		{
			//	Padding Option consists of only the Option Code - Option Length and Option Data are omitted
			data[index] = (uint8) e_option::PADDING;
			index++;
		}
		
		
		//	Append the "End of Options" Option
		//	This Option too consists only of the Option Code
		data[index] = (uint8) e_option::END_OF_OPTIONS;
	}
	
	
	return(data);
}


void DHCP_Frame::sortOptionsForBetterAlignment()
{
	//	Aligns the Options
	//	Options with an even Number of Byte comes first
	//	Options with an odd Number of Byte will be put to the End of the Array
	const uint32 numberOfOptions = options.get_size();
	uint32 indexOfLastOptionWithEvenNumberOfByte = numberOfOptions - 1;
	for(uint32 i = 0; i < numberOfOptions; i++)
	{
		//	Search in ascending Order for Options with an odd Number of Byte
		s_option* optionOdd = options[i];
		if(optionOdd->data.get_size() % 2 != 0)
		{
			//	Flag to check if the Swap was possible
			bool swapSuccessful = false;
			
			
			//	Now search in descending Order for an Option with an even Number of Byte to swap with
			for(uint32 j = indexOfLastOptionWithEvenNumberOfByte; j > i; j--)
			{
				s_option* optionEven = options[j];
				if(optionEven->data.get_size() % 2 == 0)
				{
					//	Swap both Options
					options[i] = optionEven;
					options[j] = optionOdd;
					
					
					//	Save Index of Option with even Number of Byte so we dont have to search the whole Array next Time again
					indexOfLastOptionWithEvenNumberOfByte = j - 1;
					
					
					//	Set Flag to indicate that the Swap was successful
					swapSuccessful = true;
					
					
					//	Terminate if both Indices meet in the Middle (this means there is nothing more to swap)
					if(indexOfLastOptionWithEvenNumberOfByte <= i + 1)
					{
						return;
					}
				}
			}
			
			
			//	Terminate if no Option could be swapped
			if(swapSuccessful == false)
			{
				return;
			}
		}
	}
}