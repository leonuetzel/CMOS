#include "../../Inc/cmos.hpp"





namespace ICD
{
	/*****************************************************************************/
	/*                    Globals and Static Initialization					 						 */
	/*****************************************************************************/
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Private	  			 						 						 */
	/*****************************************************************************/
	
	uint8 MB85RC16::operator[](uint32 address) const
	{
		//	Boundary Check
		if(address >= sizeInBytes)
		{
			return(0);
		}
		
		
		//	Write Base Address and upper 3 Bits of Memory Address
		const uint8 addressAssembled = slaveAddress | ((address & 0x00000700) >> 8);
		if(m_i2c.start(addressAssembled, true, 1) != OK)
		{
			return(0);
		}
		
		
		//	Write lower 8 Bit of Memory Address
		if(m_i2c.tx(address & 0x000000FF) != OK)
		{
			return(0);
		}
		
		
		//	Send another Start Condition but read Data this Time
		if(m_i2c.start(addressAssembled, false, 1) != OK)
		{
			return(0);
		}
		
		
		//	Read the Data
		return(m_i2c.rx());
	}
	
	
	feedback MB85RC16::operator()(uint32 address, uint8 data)
	{
		//	Boundary Check
		if(address >= sizeInBytes)
		{
			return(FAIL);
		}
		
		
		//	Write Base Address and upper 3 Bits of Memory Address
		const uint8 addressAssembled = slaveAddress | ((address & 0x00000700) >> 8);
		if(m_i2c.start(addressAssembled, true, 2) != OK)
		{
			return(FAIL);
		}
		
		
		//	Write lower 8 Bit of Memory Address
		if(m_i2c.tx(address & 0x000000FF) != OK)
		{
			return(FAIL);
		}
		
		
		//	Transmit Data
		return(m_i2c.tx(data));
	}
	
	
	
	/*****************************************************************************/
	/*                      						Public	  			 						 						 */
	/*****************************************************************************/
	
	MB85RC16::MB85RC16(I_I2C& i2c)
		:	m_i2c(i2c)
	{
		
	}
	
	
	MB85RC16::~MB85RC16()
	{
		
	}
	
	
	
	
	
	
	
	feedback MB85RC16::write(uint32 address, int8 data)
	{
		return(operator()(address, data));
	}
	
	
	feedback MB85RC16::write(uint32 address, uint8 data)
	{
		return(operator()(address, data));
	}
	
	
	feedback MB85RC16::write(uint32 address, int16 data)
	{
		uint8 dataRaw[sizeof(data)];
		bit::pack::_1x16Bit_to_2x8Bit(data, dataRaw);
		for(uint32 i = 0; i < sizeof(dataRaw) / sizeof(dataRaw[0]); i++)
		{
			if(operator()(address + i, dataRaw[i]) != OK)
			{
				return(FAIL);
			}
		}
		return(OK);
	}
	
	
	feedback MB85RC16::write(uint32 address, uint16 data)
	{
		uint8 dataRaw[sizeof(data)];
		bit::pack::_1x16Bit_to_2x8Bit(data, dataRaw);
		for(uint32 i = 0; i < sizeof(dataRaw) / sizeof(dataRaw[0]); i++)
		{
			if(operator()(address + i, dataRaw[i]) != OK)
			{
				return(FAIL);
			}
		}
		return(OK);
	}
	
	
	feedback MB85RC16::write(uint32 address, int32 data)
	{
		uint8 dataRaw[sizeof(data)];
		bit::pack::_1x32Bit_to_4x8Bit(data, dataRaw);
		for(uint32 i = 0; i < sizeof(dataRaw) / sizeof(dataRaw[0]); i++)
		{
			if(operator()(address + i, dataRaw[i]) != OK)
			{
				return(FAIL);
			}
		}
		return(OK);
	}
	
	
	feedback MB85RC16::write(uint32 address, uint32 data)
	{
		uint8 dataRaw[sizeof(data)];
		bit::pack::_1x32Bit_to_4x8Bit(data, dataRaw);
		for(uint32 i = 0; i < sizeof(dataRaw) / sizeof(dataRaw[0]); i++)
		{
			if(operator()(address + i, dataRaw[i]) != OK)
			{
				return(FAIL);
			}
		}
		return(OK);
	}
	
	
	feedback MB85RC16::write(uint32 address, int64 data)
	{
		uint8 dataRaw[sizeof(data)];
		bit::pack::_1x64Bit_to_8x8Bit(data, dataRaw);
		for(uint32 i = 0; i < sizeof(dataRaw) / sizeof(dataRaw[0]); i++)
		{
			if(operator()(address + i, dataRaw[i]) != OK)
			{
				return(FAIL);
			}
		}
		return(OK);
	}
	
	
	feedback MB85RC16::write(uint32 address, uint64 data)
	{
		uint8 dataRaw[sizeof(data)];
		bit::pack::_1x64Bit_to_8x8Bit(data, dataRaw);
		for(uint32 i = 0; i < sizeof(dataRaw) / sizeof(dataRaw[0]); i++)
		{
			if(operator()(address + i, dataRaw[i]) != OK)
			{
				return(FAIL);
			}
		}
		return(OK);
	}
	
	
	feedback MB85RC16::write(uint32 address, float data)
	{
		uint8 dataRaw[sizeof(data)];
		bit::pack::float_to_4x8Bit(data, dataRaw);
		for(uint32 i = 0; i < sizeof(dataRaw) / sizeof(dataRaw[0]); i++)
		{
			if(operator()(address + i, dataRaw[i]) != OK)
			{
				return(FAIL);
			}
		}
		return(OK);
	}
	
	
	feedback MB85RC16::write(uint32 address, double data)
	{
		uint8 dataRaw[sizeof(data)];
		bit::pack::double_to_8x8Bit(data, dataRaw);
		for(uint32 i = 0; i < sizeof(dataRaw) / sizeof(dataRaw[0]); i++)
		{
			if(operator()(address + i, dataRaw[i]) != OK)
			{
				return(FAIL);
			}
		}
		return(OK);
	}
	
	
	
	
	
	
	
	int8 MB85RC16::read_int8(uint32 address)
	{
		return(operator[](address));
	}
	
	
	uint8 MB85RC16::read_uint8(uint32 address)
	{
		return(operator[](address));
	}
	
	
	int16 MB85RC16::read_int16(uint32 address)
	{
		uint8 dataRaw[2];
		for(uint32 i = 0; i < sizeof(dataRaw) / sizeof(dataRaw[0]); i++)
		{
			dataRaw[i] = operator[](address + i);
		}
		return(bit::pack::_2x8Bit_to_1x16Bit(dataRaw));
	}
	
	
	uint16 MB85RC16::read_uint16(uint32 address)
	{
		uint8 dataRaw[2];
		for(uint32 i = 0; i < sizeof(dataRaw) / sizeof(dataRaw[0]); i++)
		{
			dataRaw[i] = operator[](address + i);
		}
		return(bit::pack::_2x8Bit_to_1x16Bit(dataRaw));
	}
	
	
	int32 MB85RC16::read_int32(uint32 address)
	{
		uint8 dataRaw[4];
		for(uint32 i = 0; i < sizeof(dataRaw) / sizeof(dataRaw[0]); i++)
		{
			dataRaw[i] = operator[](address + i);
		}
		return(bit::pack::_4x8Bit_to_1x32Bit(dataRaw));
	}
	
	
	uint32 MB85RC16::read_uint32(uint32 address)
	{
		uint8 dataRaw[4];
		for(uint32 i = 0; i < sizeof(dataRaw) / sizeof(dataRaw[0]); i++)
		{
			dataRaw[i] = operator[](address + i);
		}
		return(bit::pack::_4x8Bit_to_1x32Bit(dataRaw));
	}
	
	
	int64 MB85RC16::read_int64(uint32 address)
	{
		uint8 dataRaw[8];
		for(uint32 i = 0; i < sizeof(dataRaw) / sizeof(dataRaw[0]); i++)
		{
			dataRaw[i] = operator[](address + i);
		}
		return(bit::pack::_8x8Bit_to_1x64Bit(dataRaw));
	}
	
	
	uint64 MB85RC16::read_uint64(uint32 address)
	{
		uint8 dataRaw[8];
		for(uint32 i = 0; i < sizeof(dataRaw) / sizeof(dataRaw[0]); i++)
		{
			dataRaw[i] = operator[](address + i);
		}
		return(bit::pack::_8x8Bit_to_1x64Bit(dataRaw));
	}
	
	
	float MB85RC16::read_float(uint32 address)
	{
		uint8 dataRaw[4];
		for(uint32 i = 0; i < sizeof(dataRaw) / sizeof(dataRaw[0]); i++)
		{
			dataRaw[i] = operator[](address + i);
		}
		return(bit::pack::_4x8Bit_to_float(dataRaw));
	}
	
	
	double MB85RC16::read_double(uint32 address)
	{
		uint8 dataRaw[8];
		for(uint32 i = 0; i < sizeof(dataRaw) / sizeof(dataRaw[0]); i++)
		{
			dataRaw[i] = operator[](address + i);
		}
		return(bit::pack::_8x8Bit_to_double(dataRaw));
	}
};