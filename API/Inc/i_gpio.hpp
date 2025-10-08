#pragma once

#include "defines.hpp"


namespace MCU
{
	enum class PIN: uint8;
	enum class AF: uint16;
	enum class PORT: uint8;
};





class I_GPIO
{
	public:
		
		
		
		enum class e_mode
		{
			IN					= 0x0,
			OUT_GP_PP		= 0x2,
			OUT_GP_OD		= 0x3,
			AF_PP				= 0x4,
			AF_OD				= 0x5,
			ANALOG			= 0x6
		};
		
		enum class e_speed
		{
			LOW					= 0x0,
			MEDIUM			= 0x1,
			HIGH				= 0x2,
			VERY_HIGH		= 0x3
		};
		
		enum class e_pupd
		{
			NONE				= 0x0,
			PULL_UP			= 0x1,
			PULL_DOWN		= 0x2
		};
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		virtual feedback init_pin(MCU::PIN pin, e_mode mode, e_speed speed = e_speed::HIGH, e_pupd pupd = e_pupd::NONE) = 0;
		virtual void set_pin(MCU::PIN pin) = 0;
		virtual void clear_pin(MCU::PIN pin) = 0;
		virtual void toggle_pin(MCU::PIN pin) = 0;
		virtual bool get_pinLevel(MCU::PIN pin) = 0;
		virtual feedback set_AF(MCU::AF alternateFunction) = 0;
		
		virtual feedback init_portAsInput(MCU::PORT port) = 0;
		virtual feedback init_portAsOutput(MCU::PORT port) = 0;
		virtual void write_port(MCU::PORT port, uint32 data) = 0;
		virtual uint32 read_port(MCU::PORT port) = 0;
		
		virtual uint8 get_pinNumber(MCU::PIN pin) = 0;
		virtual uint8 get_portNumber(MCU::PIN pin) = 0;
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

