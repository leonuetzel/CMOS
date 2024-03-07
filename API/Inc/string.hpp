#pragma once

#include "defines.hpp"
#include "language.hpp"
#include "time.hpp"
#include "math.hpp"
#include "heap.hpp"
#include "array.hpp"





class String: public Array<char>
{
	public:
		
		
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		//	Constructors
		constexpr inline String();
		inline String(const Array<char>& string);
		inline String(char character);
		inline String(const char* string);
		inline String(const String& string);
		inline String(bool boolean);
		inline String(int8 number, uint32 minimumDigits = c_invalid);
		inline String(uint8 number, uint32 minimumDigits = c_invalid);
		inline String(int16 number, uint32 minimumDigits = c_invalid);
		inline String(uint16 number, uint32 minimumDigits = c_invalid);
		String(int32 number, uint32 minimumDigits = c_invalid);
		String(uint32 number, uint32 minimumDigits = c_invalid);
		String(int64 number, uint32 minimumDigits = c_invalid);
		String(uint64 number, uint32 minimumDigits = c_invalid);
		
		template<typename dataType>
		String(dataType number, uint32 decimalPlaces, String unit = "", char decimalPoint = '.');
		
		template<typename dataType>
		String(dataType number, uint32 decimalPlaces, int32 exponent, char decimalPoint = '.');
		
		String(Time time, Language::e_language language);
		String(Time::e_weekday weekday, Language::e_language language);
		inline ~String();
		
		
		//	Getter
		const char* get_string() const;
		
		
		//	Informational
		inline uint32 find(char data, uint32 start = 0, uint32 occurence = 1) const;
		inline uint32 find(const char* data, uint32 start = 0, uint32 occurence = 1) const;
		uint32 findMatchingBracket(uint32 startPosition, char bracketOpening, char bracketClosing) const;
		
		
		//	Explode
		Array<String> explode(const char delimiter) const;
		Array<String> explodeIfNotInStringQuotes(const char delimiter) const;
		
		
		//	Numeric
		bool is_bool() const;
		bool to_bool() const;
		bool is_int() const;
		int32 to_int32() const;
		uint32 to_uint32() const;
		int64 to_int64() const;
		uint64 to_uint64() const;
		bool is_float(char delimiter = '.') const;
		float to_float(char delimiter = '.') const;
		feedback delimitByThousands(char delimiter);
		
		
		//	Editing
		inline feedback insert(uint32 index, const char* string);
		inline feedback insert(uint32 index, const String& string);
		feedback erase_trailing(char sign);
		inline feedback expandLeading(uint32 newSize, char fill);
		void lowerCase();
		void upperCase();
		
		
		//	Comparison
		bool compare(const String& string) const;
		bool compare(const String& string, uint32 length) const;
		bool compare(const char* string) const;
		bool compare(const char* string, uint32 length) const;
		static bool compare(const char* string_1, const char* string_2);
		
		
		//	Operators
		inline String& operator=(const String& string);
		
		template<typename dataType>
		inline String operator+(const dataType& data) const;
		
		template<typename dataType>
		inline String& operator+=(const dataType& data);
		
		bool operator==(const String& string) const;
		bool operator==(const char* string) const;
		bool operator!=(const String& string) const;
		bool operator!=(const char* string) const;
		
		
		//	Static Functions
		constexpr inline static uint32 get_length(const char* string);
		constexpr inline static char lowerCase(char letter);
		constexpr inline static char upperCase(char letter);
		constexpr inline static bool isLowerCase(char letter);
		constexpr inline static bool isUpperCase(char letter);
		constexpr inline static bool is_numeric(char character);
		constexpr inline static bool isLetter(char character);
		constexpr inline static bool isPrintable(char character);
		constexpr inline static uint8 toNumber(char number);
		constexpr inline static char toASCII(uint8 number);
};



inline String operator+(const char* string1, const String& string2);
inline String operator+(char string1, const String& string2);



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/

inline String operator+(const char* string1, const String& string2)
{
	String string(string1);
	string += string2;
	return(string);
}


inline String operator+(char string1, const String& string2)
{
	String string(string1);
	string += string2;
	return(string);
}



/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

constexpr inline String::String()
{
	
}


inline String::String(const Array<char>& string)
	:	Array(string)
{
	
}


inline String::String(char character)
	:	String()
{
	Array::operator+=(character);
}


inline String::String(const char* string)
	:	String()
{
	if(string == nullptr)
	{
		return;
	}
	
	for(uint32 i = 0; string[i] != 0; i++)
	{
		Array::operator+=(string[i]);
	}
}


inline String::String(const String& string)
	:	String()
{
	Array::operator=(string);
}


inline String::String(bool boolean)
	:	String()
{
	if(boolean == true)
	{
		Array::operator=(String("true"));
	}
	else
	{
		Array::operator=(String("false"));
	}
}


inline String::String(int8 number, uint32 minimumDigits)
	:	String((int32) number, minimumDigits)
{
	
}


inline String::String(uint8 number, uint32 minimumDigits)
	:	String((uint32) number, minimumDigits)
{
	
}


inline String::String(int16 number, uint32 minimumDigits)
	:	String((int32) number, minimumDigits)
{
	
}


inline String::String(uint16 number, uint32 minimumDigits)
	:	String((uint32) number, minimumDigits)
{
	
}







template<typename dataType>
CODE_RAM String::String(dataType number, uint32 decimalPlaces, String unit, char decimalPoint)
	:	String()
{
	//	Limit this Function to floating Point only
	static_assert(std::is_same<dataType, float>::value || std::is_same<dataType, double>::value, "This String Constructor is limited to floating Point Numbers (float or double) only");
	
	
	//	Check Special Cases
	if(Math::is_nan(number) == true)
	{
		operator=("NaN");
		return;
	}
	if(Math::is_inf(number) == true)
	{
		operator=("Inf");
		return;
	}
	
	
	//	Sign
	if(number < 0)
	{
		Array::operator+=('-');
		number *= -1;
	}
	
	
	//	Get Exponent of Number and add Offset, so it can be rounded by Integer Casting (else there would be a Problem rounding Numbers < 0)
	//	Offset must be a Multiple of 3
	constexpr int32 c_exponentOffset = 900;
	int32 exponentActual = Math::log(10, number) + c_exponentOffset;
	
	
	//	Get Exponent that is a Multiple of 3
	int32 exponentTarget = (exponentActual / 3) * 3 - c_exponentOffset;
	
	
	//	Correct exponent for future Calculations (dont correct before exponentTarget Calculation because of negative Value Division)
	exponentActual -= c_exponentOffset;
	
	
	//	Shift Number so that exponentTarget is the correct Exponent
	uint64 factor = Math::power<uint32, uint64>(10, Math::absolute(exponentTarget));
	if(exponentTarget <= 0)
	{
		number *= factor;
	}
	else
	{
		number /= factor;
	}
	
	
	//	Convert Digits left of Decimal Point to String and append them to this
	const uint32 numberOfPreDotDigits = exponentActual - exponentTarget + 1;
	for(uint32 i = 0; i < numberOfPreDotDigits; i++)
	{
		const uint8 digit = Math::digit(number, numberOfPreDotDigits - i - 1);
		const char character = toASCII(digit);
		Array::operator+=(character);
	}
	
	
	//	Decimal Point
	Array::operator+=(decimalPoint);
	
	
	//	Convert Digits right of Decimal Point to String and append them to this
	for(uint32 i = 0; i < decimalPlaces; i++)
	{
		//	Shift number one Decimal Point to the left
		number *= 10;
		uint64 numberAsInt = number;
		Array::operator+=(toASCII(numberAsInt % 10));
	}
	
	
	//	Space after numeric Value
	Array::operator+=(' ');
	
	
	//	Get Exponent String
	constexpr char c_exponentString[] =
	{
		'y',
		'z',
		'a',
		'f',
		'p',
		'n',
		'u',
		'm',
		'\0',
		'k',
		'M',
		'G',
		'T',
		'P',
		'E',
		'Z',
		'Y'
	};
	constexpr int32 c_numberOfExponentStrings = sizeof(c_exponentString) / sizeof(c_exponentString[0]);
	constexpr int32 c_exponentStringZero = 8;
	
	
	//	Only access c_exponentString Array if index is valid
	const int32 exponentStringIndex = exponentTarget / 3 + c_exponentStringZero;
	if(exponentStringIndex < c_numberOfExponentStrings && exponentStringIndex >= 0)
	{
		const char& exponentCharacter = c_exponentString[exponentStringIndex];
		if(exponentCharacter != '\0')
		{
			Array::operator+=(exponentCharacter);
		}
		else
		{
			//	Erase Space Character added above, if no Unit String and no Exponent String is added
			if(unit.get_size() == 0)
			{
				eraseFromEnd(1);
			}
		}
	}
	
	
	//	Add Unit
	operator+=(unit);
}


template<typename dataType>
CODE_RAM String::String(dataType number, uint32 decimalPlaces, int32 exponent, char decimalPoint)
	:	String()
{
	//	Limit this Function to floating Point only
	static_assert(std::is_same<dataType, float>::value || std::is_same<dataType, double>::value, "This String Constructor is limited to floating Point Numbers (float or double) only");
	
	
	//	Check Special Cases
	if(Math::is_nan(number) == true)
	{
		operator=("NaN");
		return;
	}
	if(Math::is_inf(number) == true)
	{
		operator=("Inf");
		return;
	}
	
	
	//	Sign
	if(number < 0)
	{
		Array::operator+=('-');
		number *= -1;
	}
	
	
	//	Get Exponent of Number and add Offset, so it can be rounded by Integer Casting (else there would be a Problem rounding Numbers < 0)
	constexpr int32 c_exponentOffset = 900;
	int32 exponentActual = Math::log(10, number) + c_exponentOffset;
	
	
	//	Correct exponentActual for future Calculations (dont correct before exponentTarget Calculation because of negative Value Division)
	exponentActual -= c_exponentOffset;
	
	
	//	Shift Number so that exponent is the correct Exponent
	uint64 factor = Math::power<uint32, uint64>(10, Math::absolute(exponent));
	if(exponent <= 0)
	{
		number *= factor;
	}
	else
	{
		number /= factor;
	}
	
	
	//	Convert Digits left of Decimal Point to String and append them to this
	if(exponentActual >= exponent)
	{
		const uint32 numberOfPreDotDigits = exponentActual - exponent + 1;
		for(uint32 i = 0; i < numberOfPreDotDigits; i++)
		{
			const uint8 digit = Math::digit(number, numberOfPreDotDigits - i - 1);
			const char character = toASCII(digit);
			Array::operator+=(character);
		}
	}
	else
	{
		Array::operator+=('0');
	}
	
	
	//	Decimal Point
	Array::operator+=(decimalPoint);
	
	
	//	Convert Digits right of Decimal Point to String and append them to this
	for(uint32 i = 0; i < decimalPlaces; i++)
	{
		//	Shift number one Decimal Point to the left
		number *= 10;
		uint64 numberAsInt = number;
		const uint8 digit = numberAsInt % 10;
		const char character = toASCII(digit);
		Array::operator+=(character);
	}
	
	
	//	Exponent Sign e after numeric Value
	Array::operator+=('e');
	
	
	//	Exponent after e
	operator+=(exponent);
}







inline String::~String()
{
	
}







inline uint32 String::find(char data, uint32 start, uint32 occurence) const
{
	return(Array<char>::find(data, start, occurence));
}


inline uint32 String::find(const char* data, uint32 start, uint32 occurence) const
{
	String stringSearched(data);
	return(Array<char>::find(stringSearched, start, occurence));
}







inline feedback String::insert(uint32 index, const char* string)
{
	return(Array::insert(index, String(string)));
}


inline feedback String::insert(uint32 index, const String& string)
{
	return(Array::insert(index, string));
}


inline feedback String::expandLeading(uint32 newSize, char fill)
{
	if(newSize < get_size())
	{
		return(OK);
	}
	
	String fillString(Array<char>(fill, newSize - get_size()));
	return(insert(0, fillString));
}







inline String& String::operator=(const String& data)
{
	Array::operator=(data);
	return(*this);
}


template<typename dataType>
inline String String::operator+(const dataType& data) const
{
	String result(*this);
	result.operator+=(data);
	return(result);
}


template<typename dataType>
inline String& String::operator+=(const dataType& data)
{
	Array::operator+=(String(data));
	return(*this);
}







constexpr inline uint32 String::get_length(const char* string)
{
	if(string == nullptr)
	{
		return(0);
	}
	
	uint32 length = 0;
	while(*string != 0)
	{
		length++;
		string++;
	}
	return(length);
}


constexpr inline char String::lowerCase(char letter)
{
	if(isUpperCase(letter) == true)
	{
		bit::set(letter, 5);
	}
	return(letter);
}


constexpr inline char String::upperCase(char letter)
{
	if(isLowerCase(letter) == true)
	{
		bit::clear(letter, 5);
	}
	return(letter);
}


constexpr inline bool String::isLowerCase(char character)
{
	if(character >= 97 && character <= 122)
	{
		return(true);
	}
	return(false);
}


constexpr inline bool String::isUpperCase(char character)
{
	if(character >= 65 && character <= 90)
	{
		return(true);
	}
	return(false);
}


constexpr inline bool String::is_numeric(const char character)
{
	if(character >= 48 && character <= 57)
	{
		return(true);
	}
	return(false);
}


constexpr inline bool String::isLetter(char character)
{
	if(isLowerCase(character) == true || isUpperCase(character) == true)
	{
		return(true);
	}
	return(false);
}


constexpr inline bool String::isPrintable(char character)
{
	if(character >= 32 && character <= 126)
	{
		return(true);
	}
	return(false);
}


constexpr inline uint8 String::toNumber(char number)
{
	if(is_numeric(number))
	{
		return(number - 48);
	}
	return(0);
}


constexpr inline char String::toASCII(uint8 number)
{
	if(number > 9)
	{
		return(0);
	}
	return(number + 48);
}