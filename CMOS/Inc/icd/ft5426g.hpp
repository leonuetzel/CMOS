#pragma once

#include "../i_i2c.hpp"
#include "../graphics/i_touchController.hpp"





namespace ICD
{
	class FT5426G: public I_TouchController
	{
		public:
			
			static constexpr uint8 maximumNumberOfTouchPoints = 10;
			
			
			
			
			
		private:
			
			//	Static Member
			static constexpr uint8 slaveAddress = 0x38;
			
			enum class REG: uint8
			{
				MODE_SWITCH				= 0x00,
				GUESTURE					= 0x01,
				CUR_POINT					= 0x02,
				TOUCH_0_X_HIGH		= 0x03,
				TOUCH_0_X_LOW			= 0x04,
				TOUCH_0_Y_HIGH		= 0x05,
				TOUCH_0_Y_LOW			= 0x06,
				TOUCH_0_WEIGHT		= 0x07,
				TOUCH_0_MISC			= 0x08,
				TOUCH_1_X_HIGH		= 0x09,
				TOUCH_1_X_LOW			= 0x0A,
				TOUCH_1_Y_HIGH		= 0x0B,
				TOUCH_1_Y_LOW			= 0x0C,
				TOUCH_1_WEIGHT		= 0x0D,
				TOUCH_1_MISC			= 0x0E,
				TOUCH_2_X_HIGH		= 0x0F,
				TOUCH_2_X_LOW			= 0x10,
				TOUCH_2_Y_HIGH		= 0x11,
				TOUCH_2_Y_LOW			= 0x12,
				TOUCH_2_WEIGHT		= 0x13,
				TOUCH_2_MISC			= 0x14,
				TOUCH_3_X_HIGH		= 0x15,
				TOUCH_3_X_LOW			= 0x16,
				TOUCH_3_Y_HIGH		= 0x17,
				TOUCH_3_Y_LOW			= 0x18,
				TOUCH_3_WEIGHT		= 0x19,
				TOUCH_3_MISC			= 0x1A,
				TOUCH_4_X_HIGH		= 0x1B,
				TOUCH_4_X_LOW			= 0x1C,
				TOUCH_4_Y_HIGH		= 0x1D,
				TOUCH_4_Y_LOW			= 0x1E,
				TOUCH_4_WEIGHT		= 0x1F,
				TOUCH_4_MISC			= 0x20,
				TOUCH_5_X_HIGH		= 0x21,
				TOUCH_5_X_LOW			= 0x22,
				TOUCH_5_Y_HIGH		= 0x23,
				TOUCH_5_Y_LOW			= 0x24,
				TOUCH_5_WEIGHT		= 0x25,
				TOUCH_5_MISC			= 0x26,
				TOUCH_6_X_HIGH		= 0x27,
				TOUCH_6_X_LOW			= 0x28,
				TOUCH_6_Y_HIGH		= 0x29,
				TOUCH_6_Y_LOW			= 0x2A,
				TOUCH_6_WEIGHT		= 0x2B,
				TOUCH_6_MISC			= 0x2C,
				TOUCH_7_X_HIGH		= 0x2D,
				TOUCH_7_X_LOW			= 0x2E,
				TOUCH_7_Y_HIGH		= 0x2F,
				TOUCH_7_Y_LOW			= 0x30,
				TOUCH_7_WEIGHT		= 0x31,
				TOUCH_7_MISC			= 0x32,
				TOUCH_8_X_HIGH		= 0x33,
				TOUCH_8_X_LOW			= 0x34,
				TOUCH_8_Y_HIGH		= 0x35,
				TOUCH_8_Y_LOW			= 0x36,
				TOUCH_8_WEIGHT		= 0x37,
				TOUCH_8_MISC			= 0x38,
				TOUCH_9_X_HIGH		= 0x39,
				TOUCH_9_X_LOW			= 0x3A,
				TOUCH_9_Y_HIGH		= 0x3B,
				TOUCH_9_Y_LOW			= 0x3C,
				TOUCH_9_WEIGHT		= 0x3D,
				TOUCH_9_MISC			= 0x3E
			};
			
			
			//	Non-static Member
			I_I2C& m_i2c;
			
			
			//	Constructor and Destructor
			
			
			
			//	Member Functions
			uint8 register_read(REG address) const;
			feedback register_write(REG address, uint8 data);
			
			
			//	Friends
			
			
			
			
			
			
		public:
			
			FT5426G(I_I2C& i2c);
			FT5426G(const FT5426G& ft5426g) = delete;
			~FT5426G();
			
			e_touchGuesture getTouchGuesture()															override;
			uint8 getNumberOfTouchPoints()																	override;
			Pair<Vec2, e_touchEvent> getTouchPointInformation(uint8 index)	override;
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