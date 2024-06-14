#include "../Inc/semaphoreExclusive.hpp"





#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
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
	
	feedback SemaphoreExclusive::lock()
	{
		uint8* semaphore_address = &m_ownerID;
		
		if(m_ownerID != 0xFF)
		{
			return(FAIL);
		}
		
		uint8 ownerID = 0xFF;
		asm volatile																																																									//  Load Exclusive (Read Semaphore State)
		(
			"ldrexb %[out_a], %[in_a]"
				: [out_a] "=r" (ownerID)
				:	[in_a] "Q" (*semaphore_address)
				:
		);
		
		if(ownerID != 0xFF)																																																						//	Semaphore not free
		{
			return(FAIL);
		}
		ownerID = m_runningThreadID;
		
		uint32 lock_taken = 1;																																																				//	1 -> Failed to lock, 0 -> Lock successful
		asm volatile																																																									//  Store Exclusive (Write new Owner)
		(
			"strexb %[out_a], %[in_a], %[out_b]"
				: [out_a] "=&r" (lock_taken), [out_b] "=Q" (*semaphore_address)
				:	[in_a] "r" (ownerID)
				:
		);
		asm volatile("dmb");
		if(lock_taken == 0)
		{
			return(OK);
		}
		return(FAIL);
	}
	
	
	feedback SemaphoreExclusive::unlock()
	{
		uint8* semaphore_address = &m_ownerID;
		uint8 ownerID;
		
		asm volatile("dmb");
		asm volatile																																																									//  Load Exclusive
		(
			"ldrexb %[out_a], %[in_a]"
				: [out_a] "=r" (ownerID)
				:	[in_a] "Q" (*semaphore_address)
				:
		);
		
		if(ownerID != m_runningThreadID)
		{
			return(FAIL);
		}
		
		ownerID = 0xFF;
		uint32 lock_taken = 1;																																																				//	1 -> Failed to lock, 0 -> Lock successful
		asm volatile																																																									//  Store Exclusive (Write new Owner)
		(
			"strexb %[out_a], %[in_a], %[out_b]"
				: [out_a] "=&r" (lock_taken), [out_b] "=Q" (*semaphore_address)
				: [in_a] "r" (ownerID)
				:
		);
		if(lock_taken == 0)
		{
			return(OK);
		}
		return(FAIL);
	}
	
	
	feedback SemaphoreExclusive::force_unlock()
	{
		asm volatile("clrex");
		m_ownerID = 0xFF;
		return(OK);
	}
	
	
	uint8 SemaphoreExclusive::get_owner() const
	{
		return(m_ownerID);
	}
#endif