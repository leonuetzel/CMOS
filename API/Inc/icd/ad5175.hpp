#pragma once

#include "../defines.hpp"
#include "../i_i2c.hpp"





namespace ICD
{
	class AD5175
	{
		public:
			
			static constexpr uint16 maximumValue = 0x3FF;
			
			
			
			
			
		private:
			
			//	Static Member
			enum class e_command: uint8
			{
				NOP																		= 0x00,
				WRITE_RDAC														= 0x01,
				READ_RDAC															= 0x02,
				STORE_RDAC_TO_50TP										= 0x03,
				RESET																	= 0x04,
				READ_50TP															= 0x05,
				READ_ADDRESS_OF_LAST_PROGRAMMED_50TP	= 0x06,
				WRITE_CONTROL_REGISTER								= 0x07,
				READ_CONTROL_REGISTER									= 0x08,
				SHUTDOWN															= 0x09
			};
			
			
			//	Non-static Member
			I_I2C& m_i2c;
			uint8 m_address;
			
			
			//	Constructor and Destructor
			
			
			
			//	Member Functions
			feedback sendCommand(e_command command, uint16 data = 0);
			uint16 read_rdac();
			uint16 read_controlRegister();
			
			
			//	Friends
			
			
			
			
			
			
		public:
			
			AD5175(I_I2C& i2c, bool a0 = false, bool a1 = false);
			AD5175(const AD5175& ad5175) = delete;
			~AD5175();
			
			feedback write_rdac(uint16 value);
			feedback shutdown();
			feedback run();
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