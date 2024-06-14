#pragma once

#include "../defines.hpp"
#include "../i2c.hpp"





namespace ICD
{
	class MCP23016
	{
		public:
			
			static constexpr uint8 numberOfPins = 16;
			
			
			
			
			
		private:
			
			//	Static Member
			enum class REG: uint8
			{
				GP0			= 0x00,
				GP1			= 0x01,
				OLAT0		= 0x02,
				OLAT1		= 0x03,
				IPOL0		= 0x04,
				IPOL1		= 0x05,
				IODIR0	= 0x06,
				IODIR1	= 0x07,
				INTCAP0	= 0x08,
				INTCAP1	= 0x09,
				IOCON0	= 0x0A,
				IOCON1	= 0x0B
			};
			
			static constexpr uint8 slaveAddress = 0x20;
			
			
			//	Non-static Member
			I2C& m_i2c;
			uint8 slaveAddressCustomized;
			
			
			//	Constructor and Destructor
			
			
			
			//	Member Functions
			uint8 read_register(REG address);
			feedback write_register(REG address, uint8 data);
			
			
			//	Friends
			
			
			
			
			
			
		public:
			
			MCP23016(I2C& i2c, bool a2 = false, bool a1 = false, bool a0 = false);
			MCP23016(const MCP23016& mcp23016) = delete;
			~MCP23016();
			
			feedback init_pin(uint8 pin, bool output);
			feedback set_pin(uint8 pin);
			feedback clear_pin(uint8 pin);
			feedback toggle_pin(uint8 pin);
			bool get_pinLevel(uint8 pin);
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