#pragma once

#include "defines.hpp"





namespace bit
{
	template<typename dataType>
	constexpr inline void set(dataType& variable, uint32 bit)
	{
		variable |= (((dataType) 1) << bit);
	}
	
	
	template<typename dataType>
	constexpr inline void clear(dataType& variable, uint32 bit)
	{
		variable &= ~(((dataType) 1) << bit);
	}
	
	
	template<typename dataType>
	constexpr inline void clear(dataType& variable, uint32 lowestBitToClear, uint32 numberOfBitsToClear)
	{
		dataType mask = (1 << numberOfBitsToClear) - 1;
		mask = mask << lowestBitToClear;
		
		variable &= ~(mask);
	}
	
	
	template<typename dataType>
	constexpr inline bool isSet(dataType variable, uint32 bit)
	{
		dataType test = variable & (((dataType) 1) << bit);
		
		if(test != 0)
		{
			return(true);
		}
		return(false);
	}
	
	
	template<typename dataType>
	constexpr inline bool isCleared(dataType variable, uint32 bit)
	{
		if(isSet<dataType>(variable, bit) == true)
		{
			return(false);
		}
		return(true);
	}
	
	
	
	
	
	
	
	namespace pack
	{
		constexpr inline uint16 _2x8Bit_to_1x16Bit(const uint8* lowestByte, bool msbFirst = true)
		{
			if(msbFirst == true)
			{
				return((lowestByte[0] << 8) + lowestByte[1]);
			}
			return((lowestByte[1] << 8) + lowestByte[0]);
		}
		
		
		constexpr inline uint32 _4x8Bit_to_1x32Bit(const uint8* lowestByte, bool msbFirst = true)
		{
			if(msbFirst == true)
			{
				return((lowestByte[0] << 24) + (lowestByte[1] << 16) + (lowestByte[2] << 8) + lowestByte[3]);
			}
			return((lowestByte[3] << 24) + (lowestByte[2] << 16) + (lowestByte[1] << 8) + lowestByte[0]);
		}
		
		
		constexpr inline uint64 _8x8Bit_to_1x64Bit(const uint8* lowestByte, bool msbFirst = true)
		{
			if(msbFirst == true)
			{
				uint64 data = 0;
				data |= ((uint64) lowestByte[0]) << 56;
				data |= ((uint64) lowestByte[1]) << 48;
				data |= ((uint64) lowestByte[2]) << 40;
				data |= ((uint64) lowestByte[3]) << 32;
				data |= ((uint64) lowestByte[4]) << 24;
				data |= ((uint64) lowestByte[5]) << 16;
				data |= ((uint64) lowestByte[6]) << 8;
				data |= ((uint64) lowestByte[7]) << 0;
				return(data);
			}
			
			uint64 data = 0;
			data |= ((uint64) lowestByte[0]) << 0;
			data |= ((uint64) lowestByte[1]) << 8;
			data |= ((uint64) lowestByte[2]) << 16;
			data |= ((uint64) lowestByte[3]) << 24;
			data |= ((uint64) lowestByte[4]) << 32;
			data |= ((uint64) lowestByte[5]) << 40;
			data |= ((uint64) lowestByte[6]) << 48;
			data |= ((uint64) lowestByte[7]) << 56;
			return(data);
		}
		
		
		
		
		
		
		
		constexpr inline void _1x16Bit_to_2x8Bit(uint16 data, uint8 outBuffer[2], bool msbFirst = true)
		{
			if(msbFirst == true)
			{
				outBuffer[0] = (data & 0xFF00) >> 8;
				outBuffer[1] = (data & 0x00FF) >> 0;
			}
			else
			{
				outBuffer[0] = (data & 0x00FF) >> 0;
				outBuffer[1] = (data & 0xFF00) >> 8;
			}
		}
		
		
		constexpr inline void _1x32Bit_to_4x8Bit(uint32 data, uint8 outBuffer[4], bool msbFirst = true)
		{
			if(msbFirst == true)
			{
				outBuffer[0] = (data & 0xFF000000) >> 24;
				outBuffer[1] = (data & 0x00FF0000) >> 16;
				outBuffer[2] = (data & 0x0000FF00) >> 8;
				outBuffer[3] = (data & 0x000000FF) >> 0;
			}
			else
			{
				outBuffer[0] = (data & 0x000000FF) >> 0;
				outBuffer[1] = (data & 0x0000FF00) >> 8;
				outBuffer[2] = (data & 0x00FF0000) >> 16;
				outBuffer[3] = (data & 0xFF000000) >> 24;
			}
		}
		
		
		constexpr inline void _1x64Bit_to_8x8Bit(uint64 data, uint8 outBuffer[8], bool msbFirst = true)
		{
			if(msbFirst == true)
			{
				outBuffer[0] = (data & 0xFF00000000000000) >> 56;
				outBuffer[1] = (data & 0x00FF000000000000) >> 48;
				outBuffer[2] = (data & 0x0000FF0000000000) >> 40;
				outBuffer[3] = (data & 0x000000FF00000000) >> 32;
				outBuffer[4] = (data & 0x00000000FF000000) >> 24;
				outBuffer[5] = (data & 0x0000000000FF0000) >> 16;
				outBuffer[6] = (data & 0x000000000000FF00) >> 8;
				outBuffer[7] = (data & 0x00000000000000FF) >> 0;
			}
			else
			{
				outBuffer[0] = (data & 0x00000000000000FF) >> 0;
				outBuffer[1] = (data & 0x000000000000FF00) >> 8;
				outBuffer[2] = (data & 0x0000000000FF0000) >> 16;
				outBuffer[3] = (data & 0x00000000FF000000) >> 24;
				outBuffer[4] = (data & 0x000000FF00000000) >> 32;
				outBuffer[5] = (data & 0x0000FF0000000000) >> 40;
				outBuffer[6] = (data & 0x00FF000000000000) >> 48;
				outBuffer[7] = (data & 0xFF00000000000000) >> 56;
			}
		}
		
		
		
		
		
		
		
		constexpr inline void double_to_8x8Bit(double data, uint8 outBuffer[8], bool msbFirst = true)
		{
			union convert
			{
				double d;
				uint8 u[8];
			};
			
			convert c = {0};
			c.d = data;
			
			if(msbFirst == false)
			{
				for(uint32 i = 0; i < 8; i++)
				{
					outBuffer[i] = c.u[i];
				}
			}
			else
			{
				for(uint32 i = 0; i < 8; i++)
				{
					outBuffer[i] = c.u[7 - i];
				}
			}
		}
		
		
		constexpr inline double _8x8Bit_to_double(const uint8* lowestByte, bool msbFirst = true)
		{
			union convert
			{
				double d;
				uint8 u[8];
			};
			
			convert c = {0};
			
			if(msbFirst == false)
			{
				for(uint32 i = 0; i < 8; i++)
				{
					c.u[i] = lowestByte[i];
				}
			}
			else
			{
				for(uint32 i = 0; i < 8; i++)
				{
					c.u[i] = lowestByte[7 - i];
				}
			}
			
			return(c.d);
		}
		
		
		constexpr inline void float_to_4x8Bit(float data, uint8 outBuffer[4], bool msbFirst = true)
		{
			union convert
			{
				float d;
				uint8 u[4];
			};
			
			convert c = {0};
			c.d = data;
			
			if(msbFirst == false)
			{
				for(uint32 i = 0; i < 4; i++)
				{
					outBuffer[i] = c.u[i];
				}
			}
			else
			{
				for(uint32 i = 0; i < 4; i++)
				{
					outBuffer[i] = c.u[3 - i];
				}
			}
		}
		
		
		constexpr inline float _4x8Bit_to_float(const uint8* lowestByte, bool msbFirst = true)
		{
			union convert
			{
				float d;
				uint8 u[4];
			};
			
			convert c = {0};
			
			if(msbFirst == false)
			{
				for(uint32 i = 0; i < 4; i++)
				{
					c.u[i] = lowestByte[i];
				}
			}
			else
			{
				for(uint32 i = 0; i < 4; i++)
				{
					c.u[i] = lowestByte[3 - i];
				}
			}
			
			return(c.d);
		}
	};
};