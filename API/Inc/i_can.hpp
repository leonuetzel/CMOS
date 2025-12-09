#pragma once

#include "uniquePairArray.hpp"
#include "canFrame.hpp"





class I_CAN
{
	public:
		
		enum class e_error: uint8
		{
			STUFFING															= 0x1,
			FORM																	= 0x2,
			ACK																		= 0x3,
			BIT_RECESSIVE													= 0x4,
			BIT_DOMINANT													= 0x5,
			CRC																		= 0x6,
			SET_BY_SOFTWARE												= 0x7,
			
			ACCESS_TO_RESERVED_AREA								= 0xF0,
			PROTOCOL_ERROR_IN_DATA_PHASE					= 0xF1,
			PROTOCOL_ERROR_IN_ARBITRATION_PHASE		= 0xF2,
			WATCHDOG_INTERRUPT										= 0xF3,
			ERROR_LOGGING_OVERFLOW								= 0xF4,
			TIMEOUT																= 0xF5,
			MESSAGE_RAM_ACCESS_FAILURE						= 0xF6,
			TX_EVENT_FIFO_ELEMENT_LOST						= 0xF7,
			
			
			RX_FIFO_OVERFLOW											= 0xFD,
			TX_RINGBUFFER_OVERFLOW								= 0xFE,
			RX_RINGBUFFER_OVERFLOW								= 0xFF
		};
		
		enum class e_state: uint8
		{
			ERROR_ACTIVE,
			ERROR_PASSIVE,
			BUS_OFF
		};
		
		enum class e_filterType: uint8
		{
			RANGE,
			DUAL_ID,
			MASK
		};
		
		enum class e_filterBehavior: uint8
		{
			DISABLED,
			STORE_IN_RX_FIFO_0,
			STORE_IN_RX_FIFO_1,
			REJECT_ID,
			SET_PRIORITY,
			SET_PRIORITY_AND_STORE_IN_RX_FIFO_0,
			SET_PRIORITY_AND_STORE_IN_RX_FIFO_1
		};
		
		typedef struct
		{
			e_filterType type;
			e_filterBehavior behavior;
			uint32 ID1_or_filter;
			uint32 ID2_or_mask;
		}s_filterElement;
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	protected:
		
		constexpr inline I_CAN();
		inline ~I_CAN();
		
		
		
		
		
	public:
		
		virtual feedback stop() = 0;
		virtual feedback tx(const CAN_Frame& canFrame) = 0;
		virtual feedback rx(CAN_Frame& canFrame, uint32 fifoID = 0) = 0;
		
		virtual uint32 get_numberOfUnread(uint32 fifoID = 0) const = 0;
		virtual bool is_dataAvailable() const = 0;
		
		virtual uint16 get_eventID() = 0;
		virtual e_state get_state() = 0;
		virtual const UniquePairArray<e_error, bool>& get_errors() const = 0;
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

