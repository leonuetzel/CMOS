#pragma once

#include "defines.hpp"
#include "math.h"





namespace Math
{
	template<typename dataType>
	constexpr dataType pi = (dataType) 3.14159265358979323846;
	
	constexpr inline float nanFloat();
	constexpr inline double nanDouble();
	constexpr inline float infFloat();
	constexpr inline double infDouble();
	constexpr inline bool is_nan(float number);
	constexpr inline bool is_nan(double number);
	constexpr inline bool is_inf(float number);
	constexpr inline bool is_inf(double number);
	constexpr inline float round(float number, uint32 postDotLetters);
	constexpr inline double round(double number, uint32 postDotLetters);
	
	template<typename dataType>
	constexpr inline dataType radToDeg(dataType radiant);
	
	template<typename dataType>
	constexpr inline dataType degToRad(dataType degree);
	
	constexpr inline float log(uint32 base, float number);
	constexpr inline double log(uint32 base, double number);
	constexpr inline float log2(float number);
	constexpr inline double log2(double number);
	constexpr inline uint32 log2(uint32 number);
	
	
	
	template<typename dataType> 
	constexpr inline dataType absolute(dataType value);
	
	template<typename dataType_base, typename dataType_result>
	constexpr dataType_result power(dataType_base base, int32 exponent);
	
	template<typename dataType>
	constexpr uint32 numberOfDigits(dataType number);
	
	template<typename dataType>
	constexpr inline int8 sign(dataType number);
	
	
	template<typename dataType>
	constexpr bool isAngleBetween(dataType angle, dataType angleStart, dataType angleEnd);
	
	
	template<typename dataType>
	constexpr uint8 digit(dataType number, int32 digitPosition);
	
	
	template<typename dataType>
	constexpr inline dataType min(dataType number1, dataType number2);
	
	template<typename dataType>
	constexpr inline dataType min(dataType number1, dataType number2, dataType number3);
	
	template<typename dataType>
	constexpr inline dataType min(dataType number1, dataType number2, dataType number3, dataType number4);
	
	template<typename dataType>
	constexpr inline dataType min(dataType number1, dataType number2, dataType number3, dataType number4, dataType number5);
	
	
	template<typename dataType>
	constexpr inline dataType max(dataType number1, dataType number2);
	
	template<typename dataType>
	constexpr inline dataType max(dataType number1, dataType number2, dataType number3);
	
	template<typename dataType>
	constexpr inline dataType max(dataType number1, dataType number2, dataType number3, dataType number4);
	
	template<typename dataType>
	constexpr inline dataType max(dataType number1, dataType number2, dataType number3, dataType number4, dataType number5);
};









constexpr inline float Math::nanFloat()
{
	return(0.0f / 0.0f);
}


constexpr inline double Math::nanDouble()
{
	return(0.0 / 0.0);
}


constexpr inline float Math::infFloat()
{
	return(1.0f / 0.0f);
}


constexpr inline double Math::infDouble()
{
	return(1.0 / 0.0);
}


constexpr inline bool Math::is_nan(float number)
{
	union floatConversion
	{
		uint32 u;
		float f;
	};
	
	floatConversion nan({.f = 0.0f / 0.0f});
	floatConversion num({.f = number});
	
	if(nan.u == num.u)
	{
		return(true);
	}
	return(false);
}


constexpr inline bool Math::is_nan(double number)
{
	union doubleConversion
	{
		uint64 u;
		double f;
	};
	
	doubleConversion nan({.f = 0.0 / 0.0});
	doubleConversion num({.f = number});
	
	if(nan.u == num.u)
	{
		return(true);
	}
	return(false);
}


constexpr inline bool Math::is_inf(float number)
{
	union floatConversion
	{
		uint32 u;
		float f;
	};
	
	floatConversion inf_pos({.f =  1.0f / 0.0f});
	floatConversion inf_neg({.f = -1.0f / 0.0f});
	floatConversion num({.f = number});
	
	if(inf_pos.u == num.u || inf_neg.u == num.u)
	{
		return(true);
	}
	return(false);
}


constexpr inline bool Math::is_inf(double number)
{
	union doubleConversion
	{
		uint64 u;
		double f;
	};
	
	doubleConversion inf_pos({.f =  1.0 / 0.0});
	doubleConversion inf_neg({.f = -1.0 / 0.0});
	doubleConversion num({.f = number});
	
	if(inf_pos.u == num.u || inf_neg.u == num.u)
	{
		return(true);
	}
	return(false);
}


constexpr inline float Math::round(float number, uint32 postDotLetters)
{
	float factor = power<uint32, float>(10.0, postDotLetters);
	float value = number * factor;
	if(number > 0)
	{
		value += 0.5f;
	}
	else
	{
		value -= 0.5f;
	}
	int64 temp = value;
	float result = temp;
	result /= factor;
	return(result);
}


constexpr inline double Math::round(double number, uint32 postDotLetters)
{
	double factor = power<uint32, double>(10, postDotLetters);
	double value = number * factor;
	if(number > 0)
	{
		value += 0.5;
	}
	else
	{
		value -= 0.5;
	}
	int64 temp = value;
	double result = temp;
	result /= factor;
	return(result);
}


template<typename dataType>
constexpr inline dataType Math::radToDeg(dataType radiant)
{
	return(((dataType) 180) * radiant / pi<dataType>);
}


template<typename dataType>
constexpr inline dataType Math::degToRad(dataType degree)
{
	return(pi<dataType> * degree / ((dataType) 180));
}







constexpr inline float Math::log(uint32 base, float number)
{
	if(number <= 0)
	{
		return(nanFloat());
	}
	return(log2(number) / log2((float) base));
}


constexpr inline double Math::log(uint32 base, double number)
{
	if(number <= 0)
	{
		return(nanDouble());
	}
	return(log2(number) / log2((float) base));
}


constexpr inline float Math::log2(float number)
{
	if(number <= 0)
	{
		return(nanFloat());
	}
	return(std::log2(number));
}


constexpr inline double Math::log2(double number)
{
	if(number <= 0)
	{
		return(nanDouble());
	}
	return(std::log2(number));
}


constexpr inline uint32 Math::log2(uint32 number)
{
	if(number == 0)
	{
		return(0);
	}
	
	
	//	Bit Shift to right Side equals Divison by 2
	uint32 result = 0;
	while(number > 1)
	{
		number = number >> 1;
		result++;
	}
	return(result);
}







template<typename dataType>
constexpr inline dataType Math::absolute(dataType value)
{
	if(value < 0)
	{
		value *= -1;
	}
	return(value);
}


template<typename dataType_base, typename dataType_result>
constexpr dataType_result Math::power(dataType_base base, int32 exponent)
{
	if(base == 0)
	{
		return(0);
	}
	if(exponent == 0)
	{
		return(1);
	}
	
	
	dataType_result result = base;
	const uint32 c_absolute = absolute(exponent);
	for(uint32 i = 0; i < c_absolute - 1; i++)
	{
		result *= base;
	}
	if(exponent < 0)
	{
		return(1 / result);
	}
	return(result);
}


template<typename dataType>
constexpr uint32 Math::numberOfDigits(dataType number)
{
	number = absolute(number);
	
	
	uint32 digits = 1;
	while(number >= 10)
	{
		number /= 10;
		digits++;
	}
	return(digits);
}


template<typename dataType>
constexpr inline int8 Math::sign(dataType number)
{
	if(number < 0)
	{
		return(-1);
	}
	return(1);
}


template<typename dataType>
constexpr bool Math::isAngleBetween(dataType angle, dataType angleStart, dataType angleEnd)
{
	while(angle >= 360)
	{
		angle -= 360;
	}
	while(angleEnd >= 360)
	{
		angleEnd -= 360;
	}
	while(angleStart >= 360)
	{
		angleStart -= 360;
	}
	
	
	if(angleStart == angleEnd)
	{
		if(angle == angleStart)
		{
			return(true);
		}
		else
		{
			return(false);
		}
	}
	if(angleEnd > angleStart)
	{
		if(angle >= angleStart && angle <= angleEnd)
		{
			return(true);
		}
		else
		{
			return(false);
		}
	}
	else
	{
		if(angle >= angleStart || angle <= angleEnd)
		{
			return(true);
		}
		else
		{
			return(false);
		}
	}
	
	return(false);
}


template<typename dataType>
constexpr uint8 Math::digit(dataType number, int32 digitPosition)
{
	//	Only use absolute Values, because no negative Digits are desired
	number = absolute(number);
	
	
	//	Shift Number to the Left or to the Right if necessary
	if(digitPosition != 0)
	{
		const uint32 factor = power<uint32, uint32>(10, absolute(digitPosition));
		
		if(digitPosition < 0)
		{
			number *= factor;
		}
		else
		{
			number /= factor;
		}
	}
	
	
	//	Cast to integer, so we are able to use Modulo Operator
	uint64 number_asInt = number;
	return(number_asInt % 10);
}







template<typename dataType>
constexpr inline dataType Math::min(dataType number1, dataType number2)
{
	if(number1 <= number2)
	{
		return(number1);
	}
	return(number2);
}


template<typename dataType>
constexpr inline dataType Math::min(dataType number1, dataType number2, dataType number3)
{
	if(number1 <= number2 && number1 <= number3)
	{
		return(number1);
	}
	if(number2 <= number3)
	{
		return(number2);
	}
	return(number3);
}


template<typename dataType>
constexpr inline dataType Math::min(dataType number1, dataType number2, dataType number3, dataType number4)
{
	if(number1 <= number2 && number1 <= number3 && number1 <= number4)
	{
		return(number1);
	}
	if(number2 <= number3 && number2 <= number4)
	{
		return(number2);
	}
	if(number3 <= number4)
	{
		return(number3);
	}
	return(number4);
}


template<typename dataType>
constexpr inline dataType Math::min(dataType number1, dataType number2, dataType number3, dataType number4, dataType number5)
{
	if(number1 <= number2 && number1 <= number3 && number1 <= number4 && number1 <= number5)
	{
		return(number1);
	}
	if(number2 <= number3 && number2 <= number4 && number2 <= number5)
	{
		return(number2);
	}
	if(number3 <= number4 && number3 <= number5)
	{
		return(number3);
	}
	if(number4 <= number5)
	{
		return(number4);
	}
	return(number5);
}







template<typename dataType>
constexpr inline dataType Math::max(dataType number1, dataType number2)
{
	if(number1 >= number2)
	{
		return(number1);
	}
	return(number2);
}


template<typename dataType>
constexpr inline dataType Math::max(dataType number1, dataType number2, dataType number3)
{
	if(number1 >= number2 && number1 >= number3)
	{
		return(number1);
	}
	if(number2 >= number3)
	{
		return(number2);
	}
	return(number3);
}


template<typename dataType>
constexpr inline dataType Math::max(dataType number1, dataType number2, dataType number3, dataType number4)
{
	if(number1 >= number2 && number1 >= number3 && number1 >= number4)
	{
		return(number1);
	}
	if(number2 >= number3 && number2 >= number4)
	{
		return(number2);
	}
	if(number3 >= number4)
	{
		return(number3);
	}
	return(number4);
}


template<typename dataType>
constexpr inline dataType Math::max(dataType number1, dataType number2, dataType number3, dataType number4, dataType number5)
{
	if(number1 >= number2 && number1 >= number3 && number1 >= number4 && number1 >= number5)
	{
		return(number1);
	}
	if(number2 >= number3 && number2 >= number4 && number2 >= number5)
	{
		return(number2);
	}
	if(number3 >= number4 && number3 >= number5)
	{
		return(number3);
	}
	if(number4 >= number5)
	{
		return(number4);
	}
	return(number5);
}