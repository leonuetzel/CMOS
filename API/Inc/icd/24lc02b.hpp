#pragma once

#include "../defines.hpp"
#include "../i_i2c.hpp"





namespace ICD
{
	class EEPROM_24LC02B
	{
		public:
			
			static constexpr uint32 sizeInBytes = 2048;
			
			
			
			
			
		private:
			
			//	Static Member
			static constexpr uint8 slaveAddress = 0x50;
			
			
			//	Non-static Member
			I_I2C& m_i2c;
			
			
			//	Constructor and Destructor
			
			
			
			//	Member Functions
			
			
			
			//	Friends
			
			
			
			
			
			
		public:
			
			EEPROM_24LC02B(I_I2C& i2c);
			EEPROM_24LC02B(const EEPROM_24LC02B& eeprom_24LC02B) = delete;
			~EEPROM_24LC02B();
			
			uint8 operator[](uint32 address) const;
			feedback write(uint32 address, uint8 data);
	};
	
	
	
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
	
	
};