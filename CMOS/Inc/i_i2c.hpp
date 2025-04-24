#pragma once

#include "defines.hpp"





class I_I2C
{
	public:
		
		enum class e_mode: uint8
		{
			STANDARD_100KHZ						= 0,
			FAST_400KHZ								= 1,
			FAST_MODE_PLUS_1000KHZ		= 2,
			HIGH_SPEED_MODE_3400KHZ		= 3,
			ULTRA_FAST_MODE_5000KHZ		= 4
		};
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		virtual feedback start(uint8 slaveAddress, bool write, uint8 numberOfBytes = 0, uint32 timeout_ms = 100) = 0;
		virtual void stop() = 0;
		
		virtual feedback tx(uint8 data, uint32 timeout_ms = 100) = 0;
		virtual uint8 rx(uint32 timeout_ms = 100) = 0;
};



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

