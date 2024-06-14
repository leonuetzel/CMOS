#include "../Inc/string.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

CODE_RAM String::String(int32 number, uint32 minimumDigits)
	:	String()
{
	if(number < 0)
	{
		Array::operator+=('-');
	}
	
	uint32 numberOfDigits = Math::numberOfDigits(number);
	if(minimumDigits != c_invalid)
	{
		numberOfDigits = Math::max(numberOfDigits, minimumDigits);
	}
	
	for(uint32 i = 0; i < numberOfDigits; i++)
	{
		uint8 digit = Math::digit(number, numberOfDigits - i - 1);
		Array::operator+=(toASCII(digit));
	}
}


CODE_RAM String::String(uint32 number, uint32 minimumDigits)
	:	String()
{
	uint32 numberOfDigits = Math::numberOfDigits(number);
	if(minimumDigits != c_invalid)
	{
		numberOfDigits = Math::max(numberOfDigits, minimumDigits);
	}
	
	for(uint32 i = 0; i < numberOfDigits; i++)
	{
		uint8 digit = Math::digit(number, numberOfDigits - i - 1);
		Array::operator+=(toASCII(digit));
	}
}


CODE_RAM String::String(int64 number, uint32 minimumDigits)
	:	String()
{
	if(number < 0)
	{
		Array::operator+=('-');
	}
	
	uint32 numberOfDigits = Math::numberOfDigits(number);
	if(minimumDigits != c_invalid)
	{
		numberOfDigits = Math::max(numberOfDigits, minimumDigits);
	}
	
	for(uint32 i = 0; i < numberOfDigits; i++)
	{
		uint8 digit = Math::digit(number, numberOfDigits - i - 1);
		Array::operator+=(toASCII(digit));
	}
}


CODE_RAM String::String(uint64 number, uint32 minimumDigits)
	:	String()
{
	uint32 numberOfDigits = Math::numberOfDigits(number);
	if(minimumDigits != c_invalid)
	{
		numberOfDigits = Math::max(numberOfDigits, minimumDigits);
	}
	
	for(uint32 i = 0; i < numberOfDigits; i++)
	{
		uint8 digit = Math::digit(number, numberOfDigits - i - 1);
		Array::operator+=(toASCII(digit));
	}
}


CODE_RAM String::String(Time time, Language::e_language language)
	:	String()
{
	String weekday(time.get_weekday(),	language);
	String day		(time.day,						2);
	String month	(time.month,					2);
	String year		(time.year,						4);
	String hour		(time.hour,						2);
	String minute	(time.minute,					2);
	String second	(time.second,					2);
	
	*this = weekday + ", " + day + "." + month + "." + year + " - " + hour + ":" + minute + ":" + second;
}


CODE_RAM String::String(Time::e_weekday weekday, Language::e_language language)
	:	String()
{
	const char* weekdayString[Language::c_numberOfLanguages][Time::c_numberOfWeekdays] =
	{
		[(uint8) Language::e_language::ENGLISH] =
		{
			[(uint8) Time::e_weekday::INVALID]		= "Unknown",
			[(uint8) Time::e_weekday::MONDAY]			= "Monday",
			[(uint8) Time::e_weekday::TUESDAY]		= "Tuesday",
			[(uint8) Time::e_weekday::WEDNESDAY]	= "Wednesday",
			[(uint8) Time::e_weekday::THURSDAY]		= "Thursday",
			[(uint8) Time::e_weekday::FRIDAY]			= "Friday",
			[(uint8) Time::e_weekday::SATURDAY]		= "Saturday",
			[(uint8) Time::e_weekday::SUNDAY]			= "Sunday"
		},
		[(uint8) Language::e_language::GERMAN] =
		{
			[(uint8) Time::e_weekday::INVALID]		= "Unbekannt",
			[(uint8) Time::e_weekday::MONDAY]			= "Montag",
			[(uint8) Time::e_weekday::TUESDAY]		= "Dienstag",
			[(uint8) Time::e_weekday::WEDNESDAY]	= "Mittwoch",
			[(uint8) Time::e_weekday::THURSDAY]		= "Donnerstag",
			[(uint8) Time::e_weekday::FRIDAY]			= "Freitag",
			[(uint8) Time::e_weekday::SATURDAY]		= "Samstag",
			[(uint8) Time::e_weekday::SUNDAY]			= "Sonntag"
		},
		[(uint8) Language::e_language::FRANCAIS] =
		{
			[(uint8) Time::e_weekday::INVALID]		= "Inconnu",
			[(uint8) Time::e_weekday::MONDAY]			= "Lundi",
			[(uint8) Time::e_weekday::TUESDAY]		= "Mardi",
			[(uint8) Time::e_weekday::WEDNESDAY]	= "Mercredi",
			[(uint8) Time::e_weekday::THURSDAY]		= "Jeudi",
			[(uint8) Time::e_weekday::FRIDAY]			= "Vendredi",
			[(uint8) Time::e_weekday::SATURDAY]		= "Samedi",
			[(uint8) Time::e_weekday::SUNDAY]			= "Dimanche"
		}
	};
	
	Array::operator=(String(weekdayString[(uint32) language][(uint32) weekday]));
}







CODE_RAM const char* String::get_string() const
{
	return(get_data());
}







CODE_RAM uint32 String::findMatchingBracket(uint32 startPosition, char bracketOpening, char bracketClosing) const
{
	uint32 bracketCounter = 0;
	
	const uint32 numberOfCharacters = get_size();
	const char* data = get_data();
	
	for(uint32 i = startPosition; i < numberOfCharacters; i++)
	{
		const char& character(data[i]);
		if(character == bracketOpening)
		{
			bracketCounter++;
		}
		if(character == bracketClosing)
		{
			bracketCounter--;
			if(bracketCounter == 0)
			{
				return(i);
			}
		}
	}
	return(numberOfCharacters);
}







CODE_RAM Array<String> String::explode(const char delimiter) const
{
	const uint32 numberOfFields = count(delimiter) + 1;
	Array<String> stringExploded("", numberOfFields);
	for(uint32 i = 0; i < numberOfFields; i++)
	{
		stringExploded[i] = subWithDelimiter(delimiter, i);
	}
	return(stringExploded);
}


CODE_RAM Array<String> String::explodeIfNotInStringQuotes(const char delimiter) const
{
	Array<String> elements;
	const uint32 numberOfCharacters = get_size();
	if(numberOfCharacters == 0)
	{
		return(elements);
	}
	const char* data = get_data();
	
	
	bool isString = false;
	uint32 start = 0;
	for(uint32 i = 0; i < numberOfCharacters; i++)
	{
		const char& character(data[i]);
		
		if(character == '\"')
		{
			if(isString == false)
			{
				isString = true;
			}
			else
			{
				isString = false;
			}
		}
		else
		{
			if(isString == false)
			{
				if(character == delimiter)
				{
					elements += sub(start, i - start);
					start = i + 1;
				}
			}
		}
	}
	if(start != numberOfCharacters)
	{
		elements += sub(start);
	}
	
	if(elements[elements.get_size() - 1].get_size() == 0)
	{
		elements.eraseFromEnd(1);
	}
	return(elements);
}







CODE_RAM bool String::is_bool() const
{
	if(*this == "true" || *this == "false")
	{
		return(true);
	}
	return(false);
}


CODE_RAM bool String::to_bool() const
{
	if(*this == "true")
	{
		return(true);
	}
	return(false);
}


CODE_RAM bool String::is_int() const
{
	const char* data = get_data();
	if(data == nullptr)
	{
		return(false);
	}
	
	const uint32 length = get_size();
	for(uint32 i = 0; i < length; i++)
	{
		const char& character(data[i]);
		if(is_numeric(character) == false)
		{
			if(i != 0)
			{
				return(false);
			}
			if(character != '-')
			{
				return(false);
			}
		}
	}
	return(true);
}


CODE_RAM int32 String::to_int32() const
{
	if(is_int() == false)
	{
		return(0);
	}
	
	const char* data = get_data();
	
	uint32 startPosition = 0;
	int8 sign = 1;
	if(data[0] == '-')
	{
		sign = -1;
		startPosition = 1;
	}
	
	int32 result = 0;
	const uint32 length = get_size();
	for(uint32 i = startPosition; i < length; i++)
	{
		result += toNumber(data[i]) * Math::power<uint32, uint32>(10, length - i - 1);
	}
	return(result * sign);
}


CODE_RAM uint32 String::to_uint32() const
{
	if(is_int() == false)
	{
		return(0);
	}
	
	const char* data = get_data();
	
	uint32 result = 0;
	const uint32 length = get_size();
	for(uint32 i = 0; i < length; i++)
	{
		const char character = data[i];
		if(is_numeric(character) == false)
		{
			return(0);
		}
		result += toNumber(character) * Math::power<uint32, uint32>(10, length - i - 1);
	}
	return(result);
}


CODE_RAM int64 String::to_int64() const
{
	if(is_int() == false)
	{
		return(0);
	}
	
	const char* data = get_data();
	
	uint32 startPosition = 0;
	int8 sign = 1;
	if(data[0] == '-')
	{
		sign = -1;
		startPosition = 1;
	}
	
	int64 result = 0;
	const uint32 length = get_size();
	for(uint32 i = startPosition; i < length; i++)
	{
		result += toNumber(data[i]) * Math::power<uint64, uint64>(10, length - i - 1);
	}
	return(result * sign);
}


CODE_RAM uint64 String::to_uint64() const
{
	if(is_int() == false)
	{
		return(0);
	}
	
	const char* data = get_data();
	
	uint64 result = 0;
	const uint32 length = get_size();
	for(uint32 i = 0; i < length; i++)
	{
		const char character = data[i];
		if(is_numeric(character) == false)
		{
			return(0);
		}
		result += toNumber(character) * Math::power<uint64, uint64>(10, length - i - 1);
	}
	return(result);
}


CODE_RAM bool String::is_float(char delimiter) const
{
	//	Check for Number of Subarrays (1 for Number without Decimal Places, 2 for Number with Decimal Places)
	Array<String> numbers = explode(delimiter);
	if(numbers.get_size() != 1 && numbers.get_size() != 2)
	{
		return(false);
	}
	
	
	//	Check if Subarrays are numeric
	if(numbers[0].is_int() == false)
	{
		return(false);
	}
	
	if(numbers.get_size() == 2)
	{
		if(numbers[1].is_int() == false)
		{
			return(false);
		}
	}
	
	
	return(true);
}


CODE_RAM float String::to_float(char delimiter) const
{
	if(is_float(delimiter) == false)
	{
		return(Math::nanFloat());
	}
	
	
	//	Get Subarrays
	Array<String> subArrays = explode(delimiter);
	
	
	//	Pre Dot Number
	float number = subArrays[0].to_int32();
	
	
	//	Decimal Places
	const float power = Math::power<uint32, uint32>(10, subArrays[1].get_size());
	number += Math::sign(number) * (subArrays[1].to_int32() / power);
	
	
	return(number);
}


CODE_RAM feedback String::delimitByThousands(char delimiter)
{
	const char* data = get_data();
	if(data == nullptr)
	{
		return(FAIL);
	}
	
	const uint32 c_length = get_size();
	if(c_length == 0 || is_int() == false)
	{
		return(FAIL);
	}
	
	uint32 length_full = c_length;
	uint32 length_withoutMinus = length_full;
	if(data[0] == '-')
	{
		length_withoutMinus--;
	}
	
	for(uint32 i = 0; i < (length_withoutMinus - 1) / 3; i++)
	{
		Array::insert(length_full - 3 * (i + 1), delimiter);
	}
	return(OK);
}







CODE_RAM feedback String::erase_trailing(char sign)
{
	const char* data = get_data();
	if(data == nullptr)
	{
		return(FAIL);
	}
	
	uint32 start = 0;
	const uint32 c_length = get_size();
	for(uint32 i = 0; i < c_length; i++)
	{
		uint32 position = c_length - i - 1;
		if(data[position] != sign)
		{
			start = position + 1;
			break;
		}
	}
	
	if(start < c_length)
	{
		return(eraseToEnd(start));
	}
	return(OK);
}


CODE_RAM void String::lowerCase()
{
	const char* data = get_data();
	if(data != nullptr)
	{
		const uint32 c_length = get_size();
		for(uint32 i = 0; i < c_length; i++)
		{
			(*this)[i] = lowerCase(data[i]);
		}
	}
}


CODE_RAM void String::upperCase()
{
	const char* data = get_data();
	if(data != nullptr)
	{
		const uint32 c_length = get_size();
		for(uint32 i = 0; i < c_length; i++)
		{
			(*this)[i] = upperCase(data[i]);
		}
	}
}







CODE_RAM bool String::compare(const String& string) const
{
	if(get_size() != string.get_size())
	{
		return(false);
	}
	return(compare(string.get_data(), get_size()));
}


CODE_RAM bool String::compare(const String& string, uint32 length) const
{
	return(compare(string.get_data(), length));
}


CODE_RAM bool String::compare(const char* string) const
{
	if(get_size() != get_length(string))
	{
		return(false);
	}
	return(compare(string, get_size()));
}


CODE_RAM bool String::compare(const char* string, uint32 length) const
{
	//	Null Check
	const char* data = get_data();
	if(data == nullptr || string == nullptr)
	{
		if(data == string)
		{
			return(true);
		}
		return(false);
	}
	
	
	//	Comparison
	for(uint32 i = 0; i < length; i++)
	{
		if(data[i] != string[i])
		{
			return(false);
		}
	}
	return(true);
}


CODE_RAM bool String::compare(const char* string_1, const char* string_2)
{
	uint32 index = 0;
	while(1)
	{
		const char& char_1(string_1[index]);
		
		
		//	Compare for Difference
		if(char_1 != string_2[index])
		{
			return(false);
		}
		
		
		//	Check if String End is reached
		if(char_1 == 0)
		{
			return(true);
		}
		
		
		//	Increment Index
		index++;
	}
	return(false);
}







CODE_RAM bool String::operator<(const char* string) const
{
	return(operator<(String(string)));
}


CODE_RAM bool String::operator<(const String& string) const
{
	const uint32 numberOfCharacters = get_size();
	for(uint32 i = 0; i < numberOfCharacters; i++)
	{
		if(string.get_size() > i)
		{
			const char& character_this = (*this)[i];
			const char& character_string = string[i];
			
			
			//	Numbers come first
			if(is_numeric(character_this) == true)
			{
				if(is_numeric(character_string) == false)
				{
					return(true);
				}
				
				
				const uint8 number_this = toNumber(character_this);
				const uint8 number_string = toNumber(character_string);
				if(number_this < number_string)
				{
					return(true);
				}
				if(number_this > number_string)
				{
					return(false);
				}
			}
			else
			{
				if(is_numeric(character_string) == true)
				{
					return(false);
				}
				
				
				const char character_thisUppercase = upperCase(character_this);
				const char character_stringUppercase = upperCase(character_string);
				if(character_thisUppercase < character_stringUppercase)
				{
					return(true);
				}
				if(character_thisUppercase > character_stringUppercase)
				{
					return(false);
				}
			}
		}
		else
		{
			//	The shorter String is alphabetically first if identically so far
			return(false);
		}
	}
	return(true);
}


CODE_RAM bool String::operator==(const String& string) const
{
	return(compare(string));
}


CODE_RAM bool String::operator==(const char* string) const
{
	return(compare(string));
}


CODE_RAM bool String::operator!=(const String& string) const
{
	return(!compare(string));
}


CODE_RAM bool String::operator!=(const char* string) const
{
	return(!compare(string));
}