#pragma once

#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
	#include "defines.hpp"
	#include "i_semaphore.hpp"
	
	
	
	
	
	class SemaphoreExclusive : public I_Semaphore
	{
		public:
			
			
			
			
			
			
			
		private:
			
			const uint8& m_runningThreadID;
			uint8 m_ownerID;
			
			constexpr inline SemaphoreExclusive(const uint8& runningThreadID);
			SemaphoreExclusive(const SemaphoreExclusive& semaphoreExclusive) = delete;
			inline ~SemaphoreExclusive();
			
			
			friend class CMOS;
			
			
			
			
			
		public:
			
			feedback lock() override;
			feedback unlock() override;
			feedback force_unlock() override;
			uint8 get_owner() const override;
	};
	
	
	
	/*****************************************************************************/
	/*                    Globals and Static Initialization					 						 */
	/*****************************************************************************/
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Private	  			 						 						 */
	/*****************************************************************************/
	
	constexpr inline SemaphoreExclusive::SemaphoreExclusive(const uint8& runningThreadID)
		:	m_runningThreadID(runningThreadID),
			m_ownerID(0xFF)
	{
		
	}
	
	
	inline SemaphoreExclusive::~SemaphoreExclusive()
	{
		
	}
	
	
	
	/*****************************************************************************/
	/*                      					Protected	  			 						 						 */
	/*****************************************************************************/
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Public	  			 						 						 */
	/*****************************************************************************/
	
	
#endif