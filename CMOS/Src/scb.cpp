#include "../Inc/scb.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/

extern uint32 __cmos_initial_stackpointer__;

#if defined(CORTEX_M0) || defined(CORTEX_M0P)
	EXCEPTION_VECTOR const SCB::s_exceptionTable SCB::c_exceptionTable =
	{
		(uint32) &__cmos_initial_stackpointer__,	                                      																					// 0x000 Main Stack Pointer
		(uint32) EXCEPTION_RESET,                                                                                                 // 0x004 Reset
		(uint32) EXCEPTION_NMI,                                                                                                   // 0x008 Non Maskable Interrupt
		(uint32) EXCEPTION_HARD_FAULT,                                                                                            // 0x00C Hard Fault
		(uint32) 0,                                                                              																	// 0x010 Reserved
		(uint32) 0,                                                                              																	// 0x014 Reserved
		(uint32) 0,                                                                              																	// 0x018 Reserved
		(uint32) 0,                                                                                                               // 0x01C Reserved
		(uint32) 0,                                                                                                               // 0x020 Reserved
		(uint32) 0,                                                                                                               // 0x024 Reserved
		(uint32) 0,                                                                                                               // 0x028 Reserved
		(uint32) EXCEPTION_SVC,                                                                                                   // 0x02C Supervisor Call
		(uint32) 0,                                                                                         											// 0x030 Reserved
		(uint32) 0,                                                                                                               // 0x034 Reserved
		(uint32) EXCEPTION_PENDING_SV,                                                                                            // 0x038 Pending Supervisor
		(uint32) EXCEPTION_SYSTICK,                                                                                               // 0x03C CMOS Tick Timer
	};
#endif

#if defined(CORTEX_M3)
	EXCEPTION_VECTOR const SCB::s_exceptionTable SCB::c_exceptionTable =
	{
		(uint32) &__cmos_initial_stackpointer__,	                                      																					// 0x000 Main Stack Pointer
		(uint32) EXCEPTION_RESET,                                                                                                 // 0x004 Reset
		(uint32) EXCEPTION_NMI,                                                                                                   // 0x008 Non Maskable Interrupt
		(uint32) EXCEPTION_HARD_FAULT,                                                                                            // 0x00C Hard Fault
		(uint32) EXCEPTION_MEMORY_MANAGEMENT_FAULT,                                                                               // 0x010 Memory Management
		(uint32) EXCEPTION_BUS_FAULT,                                                                                             // 0x014 Bus Fault
		(uint32) EXCEPTION_USAGE_FAULT,                                                                                           // 0x018 Usage Fault
		(uint32) 0,                                                                                                               // 0x01C Reserved
		(uint32) 0,                                                                                                               // 0x020 Reserved
		(uint32) 0,                                                                                                               // 0x024 Reserved
		(uint32) 0,                                                                                                               // 0x028 Reserved
		(uint32) EXCEPTION_SVC,                                                                                                   // 0x02C Supervisor Call
		(uint32) 0,                                                                                         											// 0x030 Reserved
		(uint32) 0,                                                                                                               // 0x034 Reserved
		(uint32) EXCEPTION_PENDING_SV,                                                                                            // 0x038 Pending Supervisor
		(uint32) EXCEPTION_SYSTICK,                                                                                               // 0x03C CMOS Tick Timer
	};
#endif

#if defined(CORTEX_M7)
	EXCEPTION_VECTOR const SCB::s_exceptionTable SCB::c_exceptionTable =
	{
		(uint32) &__cmos_initial_stackpointer__,	                                      																					// 0x000 Main Stack Pointer
		(uint32) EXCEPTION_RESET,                                                                                                 // 0x004 Reset
		(uint32) EXCEPTION_NMI,                                                                                                   // 0x008 Non Maskable Interrupt
		(uint32) EXCEPTION_HARD_FAULT,                                                                                            // 0x00C Hard Fault
		(uint32) EXCEPTION_MEMORY_MANAGEMENT_FAULT,                                                                               // 0x010 Memory Management
		(uint32) EXCEPTION_BUS_FAULT,                                                                                             // 0x014 Bus Fault
		(uint32) EXCEPTION_USAGE_FAULT,                                                                                           // 0x018 Usage Fault
		(uint32) 0,                                                                                                               // 0x01C Reserved
		(uint32) 0,                                                                                                               // 0x020 Reserved
		(uint32) 0,                                                                                                               // 0x024 Reserved
		(uint32) 0,                                                                                                               // 0x028 Reserved
		(uint32) EXCEPTION_SVC,                                                                                                   // 0x02C Supervisor Call
		(uint32) EXCEPTION_DEBUG_MONITOR,                                                                                         // 0x030 Debug Monitor
		(uint32) 0,                                                                                                               // 0x034 Reserved
		(uint32) EXCEPTION_PENDING_SV,                                                                                            // 0x038 Pending Supervisor
		(uint32) EXCEPTION_SYSTICK,                                                                                               // 0x03C CMOS Tick Timer
	};
#endif





extern "C"
{
	void __dummyExceptionHandler__()
	{
		while(1)
		{
			
		}
	}
}

WEAK void EXCEPTION_RESET()
{
	SCB::reset();
}



WEAK ALIAS(__dummyExceptionHandler__) NOTHROW void EXCEPTION_NMI();
WEAK ALIAS(__dummyExceptionHandler__) NOTHROW void EXCEPTION_HARD_FAULT();
#if defined(CORTEX_M3) || defined(CORTEX_M7)
	WEAK ALIAS(__dummyExceptionHandler__) NOTHROW void EXCEPTION_MEMORY_MANAGEMENT_FAULT();
	WEAK ALIAS(__dummyExceptionHandler__) NOTHROW void EXCEPTION_BUS_FAULT();
	WEAK ALIAS(__dummyExceptionHandler__) NOTHROW void EXCEPTION_USAGE_FAULT();
#endif
WEAK ALIAS(__dummyExceptionHandler__) NOTHROW void EXCEPTION_SVC();
#if defined(CORTEX_M7)
	WEAK ALIAS(__dummyExceptionHandler__) NOTHROW void EXCEPTION_DEBUG_MONITOR();
#endif
WEAK ALIAS(__dummyExceptionHandler__) NOTHROW void EXCEPTION_PENDING_SV();
WEAK ALIAS(__dummyExceptionHandler__) NOTHROW void EXCEPTION_SYSTICK();



/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

feedback SCB::init()
{
	#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
		set_priorityGrouping(e_priorityGrouping::GROUP_4_SUBGROUP_0);
		
		enable(Exception::MEMORY_MANAGEMENT_FAULT);
		enable(Exception::BUS_FAULT);
		enable(Exception::USAGE_FAULT);
	#endif
	return(OK);
}







#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
	feedback SCB::enable(uint16 exception)
	{
		switch(exception)
		{
			case Exception::MEMORY_MANAGEMENT_FAULT:
				bit::set(*CORE::SCB::SHCSR, 16);
				break;
				
			case Exception::BUS_FAULT:
				bit::set(*CORE::SCB::SHCSR, 17);
				break;
				
			case Exception::USAGE_FAULT:
				bit::set(*CORE::SCB::SHCSR, 18);
				break;
				
			default:
				return(FAIL);
		}
		return(OK);
	}
	
	
	feedback SCB::disable(uint16 exception)
	{
		switch(exception)
		{
			case Exception::MEMORY_MANAGEMENT_FAULT:
				bit::clear(*CORE::SCB::SHCSR, 16);
				break;
				
			case Exception::BUS_FAULT:
				bit::clear(*CORE::SCB::SHCSR, 17);
				break;
				
			case Exception::USAGE_FAULT:
				bit::clear(*CORE::SCB::SHCSR, 18);
				break;
				
			default:
				return(FAIL);
		}
		return(OK);
	}
#endif
	
	

#if defined(CORTEX_M0) || defined(CORTEX_M0P) || defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
	feedback SCB::setPending(uint16 exception)
	{
		switch(exception)
		{
			#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
				case Exception::MEMORY_MANAGEMENT_FAULT:
				{
					bit::set(*CORE::SCB::SHCSR, 13);
				}
				break;
				
				case Exception::BUS_FAULT:
				{
					bit::set(*CORE::SCB::SHCSR, 14);
				}
				break;
				
				case Exception::USAGE_FAULT:
				{
					bit::set(*CORE::SCB::SHCSR, 12);
				}
				break;
				
				case Exception::SVC:
				{
					bit::set(*CORE::SCB::SHCSR, 15);
				}
				break;
			#endif
			
			#if defined(CORTEX_M0) || defined(CORTEX_M0P) || defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
				case Exception::NMI:
				{
					bit::set(*CORE::SCB::ICSR, 31);
				}
				break;
				
				case Exception::PENDING_SV:
				{
					bit::set(*CORE::SCB::ICSR, 28);
				}
				break;
				
				case Exception::SYSTICK:
				{
					bit::set(*CORE::SCB::ICSR, 26);
				}
				break;
			#endif
			
			default:
			{
				return(FAIL);
			}
		}
		return(OK);
	}
	
	
	feedback SCB::setPriority(uint16 exception, uint8 priority)
	{
		if(priority > NVIC::c_priority_max)
		{
			return(FAIL);
		}
		
		
		#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
			switch (exception)
			{
				case Exception::MEMORY_MANAGEMENT_FAULT:
				{
					uint32 temp = *CORE::SCB::SHPR1 & 0xFFFFFF0F;
					*CORE::SCB::SHPR1 = temp | (priority << 4);
				}
				break;
				
				case Exception::BUS_FAULT:
				{
					uint32 temp = *CORE::SCB::SHPR1 & 0xFFFF0FFF;
					*CORE::SCB::SHPR1 = temp | (priority << 12);
				}
				break;
				
				case Exception::USAGE_FAULT:
				{
					uint32 temp = *CORE::SCB::SHPR1 & 0xFF0FFFFF;
					*CORE::SCB::SHPR1 = temp | (priority << 20);
				}
				break;
				
				case Exception::SVC:
				{
					*CORE::SCB::SHPR2 = (priority << 28);
				}
				break;
					
				case Exception::PENDING_SV:
				{
					uint32 temp = *CORE::SCB::SHPR3 & 0xFF0FFFFF;
					*CORE::SCB::SHPR3 = temp | (priority << 20);
				}
				break;
					
				case Exception::SYSTICK:
				{
					uint32 temp = *CORE::SCB::SHPR3 & 0x0FFFFFFF;
					*CORE::SCB::SHPR3 = temp | (priority << 28);
				}
				break;
				
				default:
				{
					return(FAIL);
				}
			}
		#endif
		
		#if defined(CORTEX_M0) || defined(CORTEX_M0P)
			switch (exception)
			{
				case Exception::SVC:
				{
					*CORE::SCB::SHPR2 = (priority << 30);
				}
				break;
					
				case Exception::PENDING_SV:
				{
					uint32 temp = *CORE::SCB::SHPR3 & 0xFF00FFFF;
					*CORE::SCB::SHPR3 = temp | (priority << 22);
				}
				break;
					
				case Exception::SYSTICK:
				{
					uint32 temp = *CORE::SCB::SHPR3 & 0x00FFFFFF;
					*CORE::SCB::SHPR3 = temp | (priority << 30);
				}
				break;
				
				default:
				{
					return(FAIL);
				}
			}
		#endif
		return(OK);
	}
	
	
	feedback SCB::clearPending(uint16 exception)
	{
		switch(exception)
		{
			#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
				case Exception::MEMORY_MANAGEMENT_FAULT:
				{
					bit::clear(*CORE::SCB::SHCSR, 13);
				}
				break;
				
				case Exception::BUS_FAULT:
				{
					bit::clear(*CORE::SCB::SHCSR, 14);
				}
				break;
				
				case Exception::USAGE_FAULT:
				{
					bit::clear(*CORE::SCB::SHCSR, 12);
				}
				break;
				
				case Exception::SVC:
				{
					bit::clear(*CORE::SCB::SHCSR, 15);
				}
				break;
			#endif
			
			#if defined(CORTEX_M0) || defined(CORTEX_M0P) || defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
				case Exception::PENDING_SV:
				{
					bit::set(*CORE::SCB::ICSR, 27);
				}
				break;
				
				case Exception::SYSTICK:
				{
					bit::set(*CORE::SCB::ICSR, 25);
				}
				break;
				
				case Exception::NMI:
				{
					bit::set(*CORE::SCB::ICSR, 31);
				}
				break;
			#endif
				
			default:
			{
				return(FAIL);
			}
		}
		return(OK);
	}
#endif
	

#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
	bool SCB::isEnabled(uint16 exception) const
	{
		switch(exception)
		{
			case Exception::MEMORY_MANAGEMENT_FAULT:
				return(bit::isSet(*CORE::SCB::SHCSR, 16));
				
			case Exception::BUS_FAULT:
				return(bit::isSet(*CORE::SCB::SHCSR, 17));
				
			case Exception::USAGE_FAULT:
				return(bit::isSet(*CORE::SCB::SHCSR, 18));
				
			default:
				return(false);
		}
	}
#endif


#if defined(CORTEX_M0) || defined(CORTEX_M0P) || defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
	bool SCB::isPending(uint16 exception) const
	{
		switch(exception)
		{
			#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
				case Exception::MEMORY_MANAGEMENT_FAULT:
				{
					return(bit::isSet(*CORE::SCB::SHCSR, 13));
				}
				
				case Exception::BUS_FAULT:
				{
					return(bit::isSet(*CORE::SCB::SHCSR, 14));
				}
				
				case Exception::USAGE_FAULT:
				{
					return(bit::isSet(*CORE::SCB::SHCSR, 12));
				}
				
				case Exception::SVC:
				{
					return(bit::isSet(*CORE::SCB::SHCSR, 15));
				}
			#endif
			
			#if defined(CORTEX_M0) || defined(CORTEX_M0P) || defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
				case Exception::NMI:
				{
					return(bit::isSet(*CORE::SCB::ICSR, 31));
				}
			#endif
			
			default:
			{
				return(false);
			}
		}
	}
#endif
	

#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
	bool SCB::isActive(uint16 exception) const
	{
		switch(exception)
		{
			case Exception::MEMORY_MANAGEMENT_FAULT:
			{
				return(bit::isSet(*CORE::SCB::SHCSR, 0));
			}
			
			case Exception::BUS_FAULT:
			{
				return(bit::isSet(*CORE::SCB::SHCSR, 1));
			}
			
			case Exception::USAGE_FAULT:
			{
				return(bit::isSet(*CORE::SCB::SHCSR, 2));
			}
			
			case Exception::SVC:
			{
				return(bit::isSet(*CORE::SCB::SHCSR, 7));
			}
			
			#if defined(CORTEX_M4) || defined(CORTEX_M7)
				case Exception::DEBUG_MONITOR:
				{
					return(bit::isSet(*CORE::SCB::SHCSR, 8));
				}
			#endif
			
			case Exception::PENDING_SV:
			{
				return(bit::isSet(*CORE::SCB::SHCSR, 10));
			}
			
			case Exception::SYSTICK:
			{
				return(bit::isSet(*CORE::SCB::SHCSR, 11));
			}
			
			default:
			{
				return(false);
			}
		}
	}
#endif