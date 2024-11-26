#pragma once

#include "defines.hpp"
#include "peripherals.hpp"
#include "nvic.hpp"





class SCB
{
	public:
		
		#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
			enum class e_priorityGrouping
			{
				GROUP_4_SUBGROUP_0	= 0,
				GROUP_3_SUBGROUP_1	= 1,
				GROUP_2_SUBGROUP_2	= 2,
				GROUP_1_SUBGROUP_3	= 3,
				GROUP_0_SUBGROUP_4	= 4
			};
		#endif
		
		
		
		
		
	private:
		
		typedef struct
		{
			uint32 main_stackpointer;
			uint32 exception[NVIC::c_numberOfExceptions - 1];
		}s_exceptionTable;
		
		static const s_exceptionTable c_exceptionTable;
		
		#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
			e_priorityGrouping m_priorityGrouping;
		#endif
		
		constexpr inline SCB();
		SCB(const SCB& scb) = delete;
		inline ~SCB();
		
		#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
			inline void set_priorityGrouping(e_priorityGrouping priorityGrouping);
		#endif
		
		friend class CMOS;
		
		
		
		
		
	public:
		
		feedback init();
		
		inline uint32 get_ID_core() const;
		static void reset();
		inline uint16 get_activeInterruptVector();
		
		#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
			feedback enable(uint16 exception);
			feedback disable(uint16 exception);
		#endif
		#if defined(CORTEX_M0) || defined(CORTEX_M0P) || defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
			feedback setPending(uint16 exception);
			feedback setPriority(uint16 exception, uint8 priority);
			feedback clearPending(uint16 exception);
		#endif
		#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
			bool isEnabled(uint16 exception) const;
		#endif
		#if defined(CORTEX_M0) || defined(CORTEX_M0P) || defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
			bool isPending(uint16 exception) const;
		#endif
		#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
			bool isActive(uint16 exception) const;
		#endif
};



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/

namespace Exception
{
	constexpr uint16 RESET											= 1;
	constexpr uint16 NMI												= 2;
	constexpr uint16 HARD_FAULT									= 3;
	#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
		constexpr uint16 MEMORY_MANAGEMENT_FAULT	= 4;
		constexpr uint16 BUS_FAULT								= 5;
		constexpr uint16 USAGE_FAULT							= 6;
	#endif
	constexpr uint16 SVC												= 11;
	#if defined(CORTEX_M4) || defined(CORTEX_M7)
	constexpr uint16 DEBUG_MONITOR							= 12;
	#endif
	constexpr uint16 PENDING_SV									= 14;
	constexpr uint16 SYSTICK										= 15;
};



extern "C"
{
	void EXCEPTION_RESET();
}

extern void EXCEPTION_NMI();
extern void EXCEPTION_HARD_FAULT();
#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
	extern void EXCEPTION_MEMORY_MANAGEMENT_FAULT();
	extern void EXCEPTION_BUS_FAULT();
	extern void EXCEPTION_USAGE_FAULT();
#endif
extern void EXCEPTION_SVC();
#if defined(CORTEX_M7)
extern void EXCEPTION_DEBUG_MONITOR();
#endif
extern void EXCEPTION_PENDING_SV();
extern void EXCEPTION_SYSTICK();



/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

constexpr inline SCB::SCB()
#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
	:	m_priorityGrouping(e_priorityGrouping::GROUP_4_SUBGROUP_0)
#endif
{
	
}


inline SCB::~SCB()
{
	
}







#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
inline void SCB::set_priorityGrouping(e_priorityGrouping priorityGrouping)
{
	uint32 offset = (uint32) priorityGrouping * 0x100;
	*CORE::SCB::AIR_CR = 0x05FA0300 + offset;
	
	m_priorityGrouping = priorityGrouping;
}
#endif



/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

inline uint32 SCB::get_ID_core() const
{
	return(*CORE::SCB::CPU_ID);
}


inline uint16 SCB::get_activeInterruptVector()
{
	#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
		return(*CORE::SCB::ICSR & 0x000001FF);
	#endif
	
	#if defined(CORTEX_M0) || defined(CORTEX_M4) || defined(CORTEX_M0P)
		return(*CORE::SCB::ICSR & 0x0000003F);
	#endif
}