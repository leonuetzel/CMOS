#pragma once

#include "uniqueArray.hpp"
#include "canFrame.hpp"





class I_CAN
{
	public:
		
		enum class e_error: uint8
		{
			STUFFING									= 0x1,
			FORM											= 0x2,
			ACK												= 0x3,
			BIT_RECESSIVE							= 0x4,
			BIT_DOMINANT							= 0x5,
			CRC												= 0x6,
			SET_BY_SOFTWARE						= 0x7
		};
		
		enum class e_state: uint8
		{
			ERROR_ACTIVE,
			ERROR_PASSIVE,
			BUS_OFF
		};
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	protected:
		
		constexpr inline I_CAN();
		inline ~I_CAN();
		
		
		
		
		
	public:
		
		virtual I_CAN& operator<<(const CAN_Frame& packet) = 0;
		virtual I_CAN& operator>>(CAN_Frame*& packet) = 0;
		
		virtual uint32 get_numberOfUnread() const = 0;
		virtual bool is_dataAvailable() const = 0;
		
		virtual e_state get_state() = 0;
		virtual const UniqueArray<e_error>& get_errors() const = 0;
		virtual void clearErrors() = 0;
		
		virtual uint32 get_baudRate() const = 0;
		
		virtual feedback recoverFromBusOffState() = 0;
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

constexpr inline I_CAN::I_CAN()
{
	
}


inline I_CAN::~I_CAN()
{
	
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

