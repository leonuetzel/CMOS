#include "../../Inc/network/dnsFrame.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

String DNS_Frame::decodeDomainName(const Array<uint8>& data, uint32& index) const
{
	String name;
	while(1)
	{
		const uint32 domainNameLength = data[index];
		index++;
		
		
		if(domainNameLength == 0)
		{
			return(name);
		}
		
		
		//	Domain Name Lenghts are allowed to be maximum 63 Byte
		if(domainNameLength < 64)
		{
			//	No DNS Compression used
			
			
			
			//	Reserve Memory for Domain Name
			String domainName;
			domainName.set_size(domainNameLength);
			
			
			//	Examine Domain Name
			for(uint32 i = 0; i < domainNameLength; i++)
			{
				domainName[i] = data[index + i];
			}
			index += domainNameLength;
			if(name.get_size() > 0)
			{
				name += '.';
			}
			name += domainName;
		}
		if(domainNameLength >= 192)
		{
			//	DNS Compression used
			//	Look up the Domain Name at Index indicated by the 2 Byte Field
			uint32 indexNew = ((domainNameLength & 0x3F) << 8) | data[index];
			name += decodeDomainName(data, indexNew);
			index++;
			return(name);
		}
	}
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

DNS_Frame::DNS_Frame()
	:	ID(),
		query(),
		opCode(),
		authoritativeAnswer(),
		truncation(),
		recursionDesired(),
		recursionAvailable(),
		responseCode(),
		questions(),
		answerRecords(),
		authorityRecords(),
		additionalRecords()
{
	
}


DNS_Frame::DNS_Frame(const Array<uint8>& data)
{
	//	Minimum Packet Length is 12 Byte for Header + 4 Byte for one Question
	if(data.get_size() < 16)
	{
		return;
	}
	
	
	//	ID
	ID = (data[0] << 8) | data[1];
	
	
	//	Query / Response Flag
	query = bit::isCleared(data[2], 7);
	
	
	//	Operation Code
	opCode = (e_opCode) ((data[2] & 0x78) >> 3);
	
	
	//	Authoritative Answer Flag
	authoritativeAnswer = bit::isSet(data[2], 2);
	
	
	//	Truncation Flag
	truncation = bit::isSet(data[2], 1);
	
	
	//	Recursion desired Flag
	recursionDesired = bit::isSet(data[2], 0);
	
	
	//	Recursion available Flag
	recursionAvailable = bit::isSet(data[3], 7);
	
	
	//	Response Code
	responseCode = (e_responseCode) (data[3] & 0x0F);
	
	
	//	Number of Questions
	const uint16 numberOfQuestions = (data[4] << 8) | data[5];
	
	
	//	Number of Answer Records
	const uint16 numberOfAnswerRecords = (data[6] << 8) | data[7];
	
	
	//	Number of Authoritative Records
	const uint16 numberOfAuthoritativeRecords = (data[8] << 8) | data[9];
	
	
	//	Number of additional Records
	const uint16 numberOfAdditionalRecords = (data[10] << 8) | data[11];
	
	
	//	Questions
	uint32 index = 12;
	for(uint32 i = 0; i < numberOfQuestions; i++)
	{
		s_question question;
		question.name = decodeDomainName(data, index);
		question.type = (e_qType) ((data[index] << 8) | data[index + 1]);
		question.questionClass = (data[index + 2] << 8) | data[index + 3];
		index += 4;
		
		questions += question;
	}
	
	
	//	Answer Records
	for(uint32 i = 0; i < numberOfAnswerRecords; i++)
	{
		s_commonResourceRecord answerRecord;
		answerRecord.name = decodeDomainName(data, index);
		answerRecord.type = (e_qType) ((data[index] << 8) | data[index + 1]);
		answerRecord.resourceClass = (data[index + 2] << 8) | data[index + 3];
		answerRecord.timeToLive = (data[index + 4] << 24) | (data[index + 5] << 16) | (data[index + 6] << 8) | data[index + 7];
		answerRecord.length = (data[index + 8] << 8) | data[index + 9];
		answerRecord.data.set_size(answerRecord.length);
		index += 10;
		for(uint32 j = 0; j < answerRecord.length; j++)
		{
			answerRecord.data[j] = data[j + index];
		}
		index += answerRecord.length;
		
		answerRecords += answerRecord;
	}
	
	
	//	Authoritative Records
	for(uint32 i = 0; i < numberOfAuthoritativeRecords; i++)
	{
		s_commonResourceRecord authoritativeRecord;
		authoritativeRecord.name = decodeDomainName(data, index);
		authoritativeRecord.type = (e_qType) ((data[index] << 8) | data[index + 1]);
		authoritativeRecord.resourceClass = (data[index + 2] << 8) | data[index + 3];
		authoritativeRecord.timeToLive = (data[index + 4] << 24) | (data[index + 5] << 16) | (data[index + 6] << 8) | data[index + 7];
		authoritativeRecord.length = (data[index + 8] << 8) | data[index + 9];
		authoritativeRecord.data.set_size(authoritativeRecord.length);
		index += 10;
		for(uint32 j = 0; j < authoritativeRecord.length; j++)
		{
			authoritativeRecord.data[j] = data[j + index];
		}
		index += authoritativeRecord.length;
		
		authorityRecords += authoritativeRecord;
	}
	
	
	//	Additional Records
	for(uint32 i = 0; i < numberOfAdditionalRecords; i++)
	{
		s_commonResourceRecord additionalRecord;
		additionalRecord.name = decodeDomainName(data, index);
		additionalRecord.type = (e_qType) ((data[index] << 8) | data[index + 1]);
		additionalRecord.resourceClass = (data[index + 2] << 8) | data[index + 3];
		additionalRecord.timeToLive = (data[index + 4] << 24) | (data[index + 5] << 16) | (data[index + 6] << 8) | data[index + 7];
		additionalRecord.length = (data[index + 8] << 8) | data[index + 9];
		additionalRecord.data.set_size(additionalRecord.length);
		index += 10;
		for(uint32 j = 0; j < additionalRecord.length; j++)
		{
			additionalRecord.data[j] = data[j + index];
		}
		index += additionalRecord.length;
		
		additionalRecords += additionalRecord;
	}
}


DNS_Frame::~DNS_Frame()
{
	
}







Array<uint8> DNS_Frame::convertToByteStream()
{
	//	Calculate Packet Size
	uint32 packetSize = 12;
	for(auto& i: questions)
	{
		packetSize += 4;
		packetSize += i.name.get_size() + 2;
	}
	for(auto& i: answerRecords)
	{
		packetSize += 10;
		packetSize += i.name.get_size() + 2;
		packetSize += i.length;
	}
	for(auto& i: authorityRecords)
	{
		packetSize += 10;
		packetSize += i.name.get_size() + 2;
		packetSize += i.length;
	}
	for(auto& i: additionalRecords)
	{
		packetSize += 10;
		packetSize += i.name.get_size() + 2;
		packetSize += i.length;
	}
	
	
	//	Reserve Memory for Packet
	Array<uint8> data(0, packetSize);
	
	
	//	ID
	data[0] = (ID & 0xFF00) >> 8;
	data[1] = (ID & 0xFF00);
	
	
	//	Query / Response Flag
	if(query == false)
	{
		bit::set(data[2], 7);
	}
	
	
	//	Operation Code
	data[2] |= (((uint8) opCode) << 3);
	
	
	//	Authoritative Answer Flag
	if(authoritativeAnswer == true)
	{
		bit::set(data[2], 2);
	}
	
	
	//	Truncation Flag
	if(truncation == true)
	{
		bit::set(data[2], 1);
	}
	
	
	//	Recursion desired Flag
	if(recursionDesired == true)
	{
		bit::set(data[2], 0);
	}
	
	
	//	Recursion available Flag
	if(recursionAvailable == true)
	{
		bit::set(data[3], 7);
	}
	
	
	//	Response Code
	data[3] |= (uint8) responseCode;
	
	
	//	Question Count
	const uint16 numberOfQuestions = questions.get_size();
	data[4] = (numberOfQuestions & 0xFF00) >> 8;
	data[5] = (numberOfQuestions & 0x00FF);
	
	
	//	Answer Record Count
	const uint16 numberOfAnswerRecords = answerRecords.get_size();
	data[6] = (numberOfAnswerRecords & 0xFF00) >> 8;
	data[7] = (numberOfAnswerRecords & 0x00FF);
	
	
	//	Authority Record Count
	const uint16 numberOfAuthorityRecords = authorityRecords.get_size();
	data[8] = (numberOfAuthorityRecords & 0xFF00) >> 8;
	data[9] = (numberOfAuthorityRecords & 0x00FF);
	
	
	//	Additional Record Count
	const uint16 numberOfAdditionalRecords = additionalRecords.get_size();
	data[10] = (numberOfAdditionalRecords & 0xFF00) >> 8;
	data[11] = (numberOfAdditionalRecords & 0x00FF);
	
	
	//	Questions
	uint32 index = 12;
	for(auto& i: questions)
	{
		//	Name Field in DNS Name Notation
		Array<String> name(i.name.explode('.'));
		for(auto& j: name)
		{
			//	Domain Name Length in Byte
			data[index] = j.get_size();
			index++;
			
			
			//	Domain Name
			for(auto& k: j)
			{
				data[index] = k;
				index++;
			}
		}
		//	Trailing Zero for Domain Name (not needed to explicitly write, since Array is initialized with 0's)
		index++;
		
		
		//	Question Type Field
		data[index    ] = (((uint16) i.type) & 0xFF00) >> 8;
		data[index + 1] = (((uint16) i.type) & 0x00FF);
		index += 2;
		
		
		//	Question Class Field
		data[index    ] = (i.questionClass & 0xFF00) >> 8;
		data[index + 1] = (i.questionClass & 0x00FF);
		index += 2;
	}
	
	
	//	Answers
	for(auto& i: answerRecords)
	{
		//	Name Field in DNS Name Notation
		Array<String> name(i.name.explode('.'));
		for(auto& j: name)
		{
			//	Domain Name Length in Byte
			data[index] = j.get_size();
			index++;
			
			
			//	Domain Name
			for(auto& k: j)
			{
				data[index] = k;
				index++;
			}
		}
		//	Trailing Zero for Domain Name (not needed to explicitly write, since Array is initialized with 0's)
		index++;
		
		
		//	Type Field
		data[index    ] = (((uint16) i.type) & 0xFF00) >> 8;
		data[index + 1] = (((uint16) i.type) & 0x00FF);
		index += 2;
		
		
		//	Class Field
		data[index    ] = (i.resourceClass & 0xFF00) >> 8;
		data[index + 1] = (i.resourceClass & 0x00FF);
		index += 2;
		
		
		//	Time to Live Field
		data[index    ] = (i.timeToLive & 0xFF000000) >> 24;
		data[index + 1] = (i.timeToLive & 0x00FF0000) >> 16;
		data[index + 2] = (i.timeToLive & 0x0000FF00) >> 8;
		data[index + 3] = (i.timeToLive & 0x000000FF);
		index += 4;
		
		
		//	Resource Data Length Field
		data[index    ] = (i.length & 0xFF00) >> 8;
		data[index + 1] = (i.length & 0x00FF);
		index += 2;
		
		
		//	Resource Data Field
		for(auto& j: i.data)
		{
			data[index] = j;
			index++;
		}
	}
	
	
	//	Authority Records
	for(auto& i: authorityRecords)
	{
		//	Name Field in DNS Name Notation
		Array<String> name(i.name.explode('.'));
		for(auto& j: name)
		{
			//	Domain Name Length in Byte
			data[index] = j.get_size();
			index++;
			
			
			//	Domain Name
			for(auto& k: j)
			{
				data[index] = k;
				index++;
			}
		}
		//	Trailing Zero for Domain Name (not needed to explicitly write, since Array is initialized with 0's)
		index++;
		
		
		//	Type Field
		data[index    ] = (((uint16) i.type) & 0xFF00) >> 8;
		data[index + 1] = (((uint16) i.type) & 0x00FF);
		index += 2;
		
		
		//	Class Field
		data[index    ] = (i.resourceClass & 0xFF00) >> 8;
		data[index + 1] = (i.resourceClass & 0x00FF);
		index += 2;
		
		
		//	Time to Live Field
		data[index    ] = (i.timeToLive & 0xFF000000) >> 24;
		data[index + 1] = (i.timeToLive & 0x00FF0000) >> 16;
		data[index + 2] = (i.timeToLive & 0x0000FF00) >> 8;
		data[index + 3] = (i.timeToLive & 0x000000FF);
		index += 4;
		
		
		//	Resource Data Length Field
		data[index    ] = (i.length & 0xFF00) >> 8;
		data[index + 1] = (i.length & 0x00FF);
		index += 2;
		
		
		//	Resource Data Field
		for(auto& j: i.data)
		{
			data[index] = j;
			index++;
		}
	}
	
	
	//	Additional Records
	for(auto& i: additionalRecords)
	{
		//	Name Field in DNS Name Notation
		Array<String> name(i.name.explode('.'));
		for(auto& j: name)
		{
			//	Domain Name Length in Byte
			data[index] = j.get_size();
			index++;
			
			
			//	Domain Name
			for(auto& k: j)
			{
				data[index] = k;
				index++;
			}
		}
		//	Trailing Zero for Domain Name (not needed to explicitly write, since Array is initialized with 0's)
		index++;
		
		
		//	Type Field
		data[index    ] = (((uint16) i.type) & 0xFF00) >> 8;
		data[index + 1] = (((uint16) i.type) & 0x00FF);
		index += 2;
		
		
		//	Class Field
		data[index    ] = (i.resourceClass & 0xFF00) >> 8;
		data[index + 1] = (i.resourceClass & 0x00FF);
		index += 2;
		
		
		//	Time to Live Field
		data[index    ] = (i.timeToLive & 0xFF000000) >> 24;
		data[index + 1] = (i.timeToLive & 0x00FF0000) >> 16;
		data[index + 2] = (i.timeToLive & 0x0000FF00) >> 8;
		data[index + 3] = (i.timeToLive & 0x000000FF);
		index += 4;
		
		
		//	Resource Data Length Field
		data[index    ] = (i.length & 0xFF00) >> 8;
		data[index + 1] = (i.length & 0x00FF);
		index += 2;
		
		
		//	Resource Data Field
		for(auto& j: i.data)
		{
			data[index] = j;
			index++;
		}
	}
	
	
	return(data);
}