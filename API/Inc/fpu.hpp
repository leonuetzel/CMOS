#pragma once

#if defined(CORTEX_M4) || defined(CORTEX_M7)
	#include "defines.hpp"
	#include "peripherals.hpp"
	
	
	
	
	
	class FPU
	{
		public:
		
		
		
			
			
			
		private:
			
			inline FPU();
			FPU(const FPU& fpu) = delete;
			inline ~FPU();
			
			
			friend class CMOS;
			
			
			
			
			
		public:
			
			
	};
	
	
	
	/*****************************************************************************/
	/*                    Globals and Static Initialization					 						 */
	/*****************************************************************************/
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Private	  			 						 						 */
	/*****************************************************************************/
	
	inline FPU::FPU()
	{
		*CORE::COPROCESSOR::ACR &= 0xFF0FFFFF;
		*CORE::COPROCESSOR::ACR |= 0x00F00000;
		
		bit::clear(*CORE::FPU::CCR, 30);
		
		asm volatile("dsb");
		asm volatile("isb");
	}
	
	
	inline FPU::~FPU()
	{
		
	}
	
	
	
	/*****************************************************************************/
	/*                      					Protected	  			 						 						 */
	/*****************************************************************************/
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Public	  			 						 						 */
	/*****************************************************************************/
	
	
#endif