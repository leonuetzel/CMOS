#pragma once

#include "../defines.hpp"
#include "../i_i2c.hpp"





namespace ICD
{
	class STC3100
	{
		public:
			
			
			
			
			
			
			
		private:
			
			//	Static Member
			enum class REG: uint8
			{
				MODE							= 0x00,
				CTRL							= 0x01,
				CHARGE_LOW				= 0x02,
				CHRAGE_HIGH				= 0x03,
				COUNTER_LOW				= 0x04,
				COUNTER_HIGH			= 0x05,
				CURRENT_LOW				= 0x06,
				CURRENT_HIGH			= 0x07,
				VOLTAGE_LOW				= 0x08,
				VOLTAGE_HIGH			= 0x09,
				TEMPERATURE_LOW		= 0x0A,
				TEMPERATURE_HIGH	= 0x0B,
				
				ID0								= 0x18,
				ID1								= 0x19,
				ID2								= 0x1A,
				ID3								= 0x1B,
				ID4								= 0x1C,
				ID5								= 0x1D,
				ID6								= 0x1E,
				ID7								= 0x1F,
				
				RAM0							= 0x20,
				RAM1							= 0x21,
				RAM2							= 0x22,
				RAM3							= 0x23,
				RAM4							= 0x24,
				RAM5							= 0x25,
				RAM6							= 0x26,
				RAM7							= 0x27,
				RAM8							= 0x28,
				RAM9							= 0x29,
				RAM10							= 0x2A,
				RAM11							= 0x2B,
				RAM12							= 0x2C,
				RAM13							= 0x2D,
				RAM14							= 0x2E,
				RAM15							= 0x2F,
				RAM16							= 0x30,
				RAM17							= 0x31,
				RAM18							= 0x32,
				RAM19							= 0x33,
				RAM20							= 0x34,
				RAM21							= 0x35,
				RAM22							= 0x36,
				RAM23							= 0x37,
				RAM24							= 0x38,
				RAM25							= 0x39,
				RAM26							= 0x3A,
				RAM27							= 0x3B,
				RAM28							= 0x3C,
				RAM29							= 0x3D,
				RAM30							= 0x3E,
				RAM31							= 0x3F
			};
			
			static constexpr uint8 slaveAddress = 0x70;
			
			
			
			//	Non-static Member
			I_I2C& m_i2c;
			I_CRC& m_crc;
			const float m_shuntResistance;
			uint8 m_ID[8];
			
			
			//	Constructor and Destructor
			
			
			
			//	Member Functions
			Array<uint8> register_read(REG address, uint32 numberOfRegisters = 1);
			feedback register_write(REG address, const Array<uint8>& data);
			
			
			//	Friends
			
			
			
			
			
		public:
			
			STC3100(I_I2C& i2c, I_CRC& crc, float shuntResistance);
			STC3100(const STC3100& stc3100) = delete;
			~STC3100();
			
			float read_temperature();
			float read_voltage();
			float read_current();
			float read_charge();
			uint16 read_conversionCounter();
			
			feedback resetAccumulatorAndConversionCounter();
			
			feedback run();
			feedback standby();
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