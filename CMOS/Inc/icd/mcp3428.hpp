#pragma once

#include "../defines.hpp"
#include "../i_i2c.hpp"





namespace ICD
{
	class MCP3428
	{
		public:
			
			static constexpr uint8 c_numberOfChannels = 4;
			
			enum class e_conversionMode
			{
				ONE_SHOT		= 0x0,
				CONTINUOUS	= 0x1
			};
			
			enum class e_sampleRate
			{
				HIGH_240SPS_12BIT		= 0x0,
				MEDIUM_60SPS_14BIT	= 0x1,
				LOW_15SPS_16BIT			= 0x2
			};
			
			enum class e_gain
			{
				x1	= 0x0,
				x2	= 0x1,
				x4	= 0x2,
				x8	= 0x3,
			};
			
			
			
			
			
		private:
			
			//	Static Member
			
			
			
			//	Non-static Member
			I_I2C& m_i2c;
			uint8 m_address;
			
			e_conversionMode m_conversionMode;
			uint8 m_channel;
			e_sampleRate m_sampleRate;
			e_gain m_gain;
			
			
			//	Constructor and Destructor
			
			
			
			//	Member Functions
			uint8 read_configurationRegister();
			
			
			//	Friends
			
			
			
			
			
			
		public:
			
			MCP3428(I_I2C& i2c, bool a0 = false, bool a1 = false, bool a2 = false);
			MCP3428(const MCP3428& mcp3428) = delete;
			~MCP3428();
			
			feedback set_conversionMode(e_conversionMode mode);
			feedback set_channel(uint8 channel);
			feedback set_sampleRate(e_sampleRate sampleRate);
			feedback set_gain(e_gain gain);
			
			int16 oneshot();
			int16 operator()();
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