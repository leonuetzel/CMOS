#pragma once

#include "../i_i2c.hpp"
#include "../graphics/i_touchController.hpp"





namespace ICD
{
	class FT5536U: public I_TouchController
	{
		public:
			
			static constexpr uint8 maximumNumberOfTouchPoints = 5;
			
			
			
			
			
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
				
				
				TOUCH_1_X_HIGH		= 0x09,
				TOUCH_1_X_LOW			= 0x0A,
				TOUCH_1_Y_HIGH		= 0x0B,
				TOUCH_1_Y_LOW			= 0x0C,
				
				
				TOUCH_2_X_HIGH		= 0x0F,
				TOUCH_2_X_LOW			= 0x10,
				TOUCH_2_Y_HIGH		= 0x11,
				TOUCH_2_Y_LOW			= 0x12,
				
				
				TOUCH_3_X_HIGH		= 0x15,
				TOUCH_3_X_LOW			= 0x16,
				TOUCH_3_Y_HIGH		= 0x17,
				TOUCH_3_Y_LOW			= 0x18,
				
				
				TOUCH_4_X_HIGH		= 0x1B,
				TOUCH_4_X_LOW			= 0x1C,
				TOUCH_4_Y_HIGH		= 0x1D,
				TOUCH_4_Y_LOW			= 0x1E
			};
			
			
			//	Non-static Member
			I_I2C& m_i2c;
			
			
			//	Constructor and Destructor
			
			
			
			//	Member Functions
			uint8 register_read(REG address) const;
			feedback register_write(REG address, uint8 data);
			
			
			//	Friends
			
			
			
			
			
			
		public:
			
			FT5536U(I_I2C& i2c);
			FT5536U(const FT5536U& ft55336u) = delete;
			~FT5536U();
			
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