#include "../../Inc/cmos.hpp"





namespace ICD
{
	/*****************************************************************************/
	/*                    Globals and Static Initialization					 						 */
	/*****************************************************************************/
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Private	  			 						 						 */
	/*****************************************************************************/
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Public	  			 						 						 */
	/*****************************************************************************/
	
	EEPROM_24LC02B::EEPROM_24LC02B(I2C& i2c)
		:	m_i2c(i2c)
	{
		
	}
	
	
	EEPROM_24LC02B::~EEPROM_24LC02B()
	{
		
	}
	
	
	
	
	
	
	
	uint8 EEPROM_24LC02B::operator[](uint32 address) const
	{
		//	Boundary Check
		if(address >= sizeInBytes)
		{
			return(0);
		}
		
		
		//	Write Base Address and upper 3 Bits of Memory Address
		const uint8 addressAssembled = slaveAddress | ((address & 0x00000700) >> 8);
		if(m_i2c.start(addressAssembled, true) != OK)
		{
			return(0);
		}
		
		
		//	Write lower 8 Bit of Memory Address
		m_i2c << (uint8) (address & 0x000000FF);
		if(m_i2c.tx() != OK)
		{
			return(0);
		}
		
		
		//	Send another Start Condition but read Data this Time
		if(m_i2c.start(addressAssembled, false) != OK)
		{
			return(0);
		}
		
		
		//	Read the Data
		return(m_i2c.rx());
	}
	
	
	feedback EEPROM_24LC02B::write(uint32 address, uint8 data)
	{
		//	Boundary Check
		if(address >= sizeInBytes)
		{
			return(FAIL);
		}
		
		
		//	Write Base Address and upper 3 Bits of Memory Address
		const uint8 addressAssembled = slaveAddress | ((address & 0x00000700) >> 8);
		if(m_i2c.start(addressAssembled, true) != OK)
		{
			return(FAIL);
		}
		
		
		//	Write lower 8 Bit of Memory Address and Data to save
		m_i2c << (uint8) (address & 0x000000FF);
		m_i2c << data;
		
		
		//	Transmit Data
		if(m_i2c.tx() != OK)
		{
			return(FAIL);
		}
		
		
		//	ACK Polling to wait until EEPROM has finished Write Cycle
		CMOS& cmos = CMOS::get();
		while(m_i2c.start(slaveAddress, true) != OK)
		{
			cmos.sleep_ms(5);
		}
		
		
		return(OK);
	}
};