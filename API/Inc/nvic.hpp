#pragma once

#include "defines.hpp"
#include "bit.hpp"
#include "peripherals.hpp"





class NVIC
{
	public:
		
		//	Maximum Interrupt Priority
		#if defined(CORTEX_M7)
			static constexpr uint32 c_priorityBits = 7;
			static constexpr uint32 c_priority_max = (1 << c_priorityBits) - 1;
		#endif
		
		#if defined(CORTEX_M3)
			static constexpr uint32 c_priorityBits = 4;
			static constexpr uint32 c_priority_max = (1 << c_priorityBits) - 1;
		#endif
		
		#if defined(CORTEX_M0) || defined(CORTEX_M0P)
			static constexpr uint32 c_priorityBits = 2;
			static constexpr uint32 c_priority_max = (1 << c_priorityBits) - 1;
		#endif
		
		
		//	Number of Exceptions and Interrupts
		static constexpr uint32 c_numberOfExceptions	= 16;
		
		#if defined(CORTEX_M7)
			static constexpr uint32 c_numberOfInterrupts	= 240;
		#endif
		
		#if defined(CORTEX_M3)
			static constexpr uint32 c_numberOfInterrupts	= 68;
		#endif
		
		#if defined(CORTEX_M0) || defined(CORTEX_M0P)
			static constexpr uint32 c_numberOfInterrupts	= 32;
		#endif
		
		
		uint8 m_basePriority;
		
		
		
		
		
	private:
		
		constexpr inline NVIC();
		NVIC(const NVIC& nvic) = delete;
		inline ~NVIC();
		
		friend class CMOS;
		
		
		
		
		
	public:
		
		inline feedback init();
		
		inline feedback enable(uint16 interrupt);
		inline feedback disable(uint16 interrupt);
		inline feedback setPending(uint16 interrupt);
		inline feedback setPriority(uint16 interrupt, uint8 priority);
		inline feedback clearPending(uint16 interrupt);
		inline bool isEnabled(uint16 interrupt);
		inline bool isPending(uint16 interrupt);
		
		#if defined(CORTEX_M3) || defined(CORTEX_M7)
			inline bool isActive(uint16 interrupt);
		#endif
		
		inline uint32 set_basePriority(uint8 basePriority);
		inline void enable_configurablePriorityExceptions();
		inline void disable_configurablePriorityExceptions();
		#if defined(CORTEX_M3) || defined(CORTEX_M7)
			inline void enable_allExceptions();
			inline void disable_allExceptions();
		#endif
};



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

constexpr inline NVIC::NVIC()
	: m_basePriority(0)
{
	
}


inline NVIC::~NVIC()
{
	
}



/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

inline feedback NVIC::init()
{
	return(OK);
}







inline feedback NVIC::enable(uint16 interrupt)
{
	if(interrupt < c_numberOfExceptions || interrupt >= c_numberOfExceptions + c_numberOfInterrupts)
	{
		return(FAIL);
	}
	interrupt -= c_numberOfExceptions;
	
	volatile uint32* address = CORE::NVIC::ISER + interrupt / 32;
	*address = 1 << (interrupt % 32);
	
	return(OK);
}


inline uint32 NVIC::disable(uint16 interrupt)
{
	if(interrupt < c_numberOfExceptions || interrupt >= c_numberOfExceptions + c_numberOfInterrupts)
	{
		return(FAIL);
	}
	interrupt -= c_numberOfExceptions;
	
	volatile uint32* address = CORE::NVIC::ICER + interrupt / 32;
	*address = 1 << (interrupt % 32);
	
	return(OK);
}


inline feedback NVIC::setPending(uint16 interrupt)
{
	if(interrupt < c_numberOfExceptions || interrupt >= c_numberOfExceptions + c_numberOfInterrupts)
	{
		return(FAIL);
	}
	interrupt -= c_numberOfExceptions;
	
	volatile uint32* address = CORE::NVIC::ISPR + interrupt / 32;
	*address = (1 << interrupt % 32);
	
	return(OK);
}


inline feedback NVIC::setPriority(uint16 interrupt, uint8 priority)
{
	if(interrupt < c_numberOfExceptions || interrupt >= c_numberOfExceptions + c_numberOfInterrupts || priority > c_priority_max)
	{
		return(FAIL);
	}
	interrupt -= c_numberOfExceptions;
	
	
	//	In Cortex M0 and Cortex M0+ (seems like an ARMv6-M Restriction), the NVIC_IPR Registers are word-accessible only
	uint32* const address = ((uint32*) CORE::NVIC::IPR) + (interrupt / 4);
	const uint32 bitFieldOffset = 8 * (interrupt % 4);
	const uint8 offset = 8 - c_priorityBits;
	
	
	//	Modify IPR Register
	const uint32 mask = ~(0xFF << bitFieldOffset);
	uint32 ipr = *address & mask;
	*address = ipr | (priority << (bitFieldOffset + offset));
	
	
	return(OK);
}


inline feedback NVIC::clearPending(uint16 interrupt)
{
	if(interrupt < c_numberOfExceptions || interrupt >= c_numberOfExceptions + c_numberOfInterrupts)
	{
		return(FAIL);
	}
	interrupt -= c_numberOfExceptions;
	
	volatile uint32* address = CORE::NVIC::ICPR + (interrupt / 32);
	*address = (1 << (interrupt % 32));
	
	return(OK);
}


inline bool NVIC::isEnabled(uint16 interrupt)
{
	if(interrupt < c_numberOfExceptions || interrupt >= c_numberOfExceptions + c_numberOfInterrupts)
	{
		return(false);
	}
	interrupt -= c_numberOfExceptions;
	
	volatile uint32* address = CORE::NVIC::ISER + (interrupt / 32);
	return(bit::isSet(*address, interrupt % 32));
}


inline bool NVIC::isPending(uint16 interrupt)
{
	if(interrupt < c_numberOfExceptions || interrupt >= c_numberOfExceptions + c_numberOfInterrupts)
	{
		return(false);
	}
	interrupt -= c_numberOfExceptions;
	
	volatile uint32* address = CORE::NVIC::ISPR + (interrupt / 32);
	return(bit::isSet(*address, interrupt % 32));
}


#if defined(CORTEX_M3) || defined(CORTEX_M7)
inline bool NVIC::isActive(uint16 interrupt)
{
	if(interrupt < c_numberOfExceptions || interrupt >= c_numberOfExceptions + c_numberOfInterrupts)
	{
		return(false);
	}
	interrupt -= c_numberOfExceptions;
	
	volatile uint32* address = CORE::NVIC::IABR + (interrupt / 32);
	return(bit::isSet(*address, interrupt % 32));
}
#endif







inline uint32 NVIC::set_basePriority(uint8 basePriority)
{
	uint32 temp = (basePriority << 4) & 0xF0;
	uint32 temp2 = 0xFF;

	asm volatile
	(
		"msr basepri, %[in_a]"
			:
			:	[in_a] "r" (temp)
			:
	);

	asm volatile
	(
		"mrs %[out_a], basepri"
			:	[out_a] "=r" (temp2)
			:
			:
	);

	if(temp != temp2)
	{
		return(0);
	}

	m_basePriority = basePriority;
	return(1);
}


inline void NVIC::enable_configurablePriorityExceptions()
{
	asm volatile("cpsie i");
}


inline void NVIC::disable_configurablePriorityExceptions()
{
	asm volatile("cpsid i");
}


#if defined(CORTEX_M3) || defined(CORTEX_M7)
	inline void NVIC::enable_allExceptions()
	{
		asm volatile("cpsie f");
	}
	
	
	inline void NVIC::disable_allExceptions()
	{
		asm volatile("cpsid f");
	}
#endif