#pragma once

#include "defines.hpp"





namespace CORE
{
	#if defined(CORTEX_M0)
		namespace BASE
		{
			constexpr uint32 CORE										= 0xE0000000;
			
			constexpr uint32 SYSTICK     						= CORE + 0x0000E010;
			constexpr uint32 NVIC        						= CORE + 0x0000E100;
			constexpr uint32 SCB         						= CORE + 0x0000ED00;
		};
		
		
		
		namespace SCB
		{
			volatile uint32* const CPU_ID						= (volatile uint32* const) (BASE::SCB + 0x00);
			volatile uint32* const ICSR							= (volatile uint32* const) (BASE::SCB + 0x04);
			volatile uint32* const AIR_CR						= (volatile uint32* const) (BASE::SCB + 0x0C);
			volatile uint32* const SCR							= (volatile uint32* const) (BASE::SCB + 0x10);
			volatile uint32* const CCR							= (volatile uint32* const) (BASE::SCB + 0x14);
			volatile uint32* const SHPR2						= (volatile uint32* const) (BASE::SCB + 0x1C);
			volatile uint32* const SHPR3						= (volatile uint32* const) (BASE::SCB + 0x20);
		};
		
		namespace SYSTICK
		{
			volatile uint32* const CSR							= (volatile uint32* const) (BASE::SYSTICK + 0x00);
			volatile uint32* const RVR							= (volatile uint32* const) (BASE::SYSTICK + 0x04);
			volatile uint32* const CVR							= (volatile uint32* const) (BASE::SYSTICK + 0x08);
			volatile uint32* const CALIB						= (volatile uint32* const) (BASE::SYSTICK + 0x0C);
		};
		
		namespace NVIC
		{
			volatile uint32* const ISER							= (volatile uint32* const) (BASE::NVIC + 0x000);
			volatile uint32* const ICER							= (volatile uint32* const) (BASE::NVIC + 0x080);
			volatile uint32* const ISPR							= (volatile uint32* const) (BASE::NVIC + 0x100);
			volatile uint32* const ICPR							= (volatile uint32* const) (BASE::NVIC + 0x180);
			volatile uint32* const IPR							= (volatile uint32* const) (BASE::NVIC + 0x300);
		};
	#endif
	
	#if defined(CORTEX_M0P)
		namespace BASE
		{
			constexpr uint32 CORE										= 0xE0000000;
			
			constexpr uint32 SYSTICK     						= CORE + 0x0000E010;
			constexpr uint32 NVIC        						= CORE + 0x0000E100;
			constexpr uint32 SCB         						= CORE + 0x0000ED00;
			constexpr uint32 MPU         						= CORE + 0x0000ED90;
		};
		
		
		
		namespace SCB
		{
			volatile uint32* const CPU_ID						= (volatile uint32* const) (BASE::SCB + 0x00);
			volatile uint32* const ICSR							= (volatile uint32* const) (BASE::SCB + 0x04);
			volatile uint32* const AIR_CR						= (volatile uint32* const) (BASE::SCB + 0x0C);
			volatile uint32* const SCR							= (volatile uint32* const) (BASE::SCB + 0x10);
			volatile uint32* const CCR							= (volatile uint32* const) (BASE::SCB + 0x14);
			volatile uint32* const SHPR2						= (volatile uint32* const) (BASE::SCB + 0x1C);
			volatile uint32* const SHPR3						= (volatile uint32* const) (BASE::SCB + 0x20);
		};
		
		namespace SYSTICK
		{
			volatile uint32* const CSR							= (volatile uint32* const) (BASE::SYSTICK + 0x00);
			volatile uint32* const RVR							= (volatile uint32* const) (BASE::SYSTICK + 0x04);
			volatile uint32* const CVR							= (volatile uint32* const) (BASE::SYSTICK + 0x08);
			volatile uint32* const CALIB						= (volatile uint32* const) (BASE::SYSTICK + 0x0C);
		};
		
		namespace NVIC
		{
			volatile uint32* const ISER							= (volatile uint32* const) (BASE::NVIC + 0x000);
			volatile uint32* const ICER							= (volatile uint32* const) (BASE::NVIC + 0x080);
			volatile uint32* const ISPR							= (volatile uint32* const) (BASE::NVIC + 0x100);
			volatile uint32* const ICPR							= (volatile uint32* const) (BASE::NVIC + 0x180);
			volatile uint32* const IPR							= (volatile uint32* const) (BASE::NVIC + 0x300);
		};
		
		namespace MPU
		{
			volatile uint32* const TYPE							= (volatile uint32* const) (BASE::MPU + 0x00);
			volatile uint32* const CTRL							= (volatile uint32* const) (BASE::MPU + 0x04);
			volatile uint32* const RNR							= (volatile uint32* const) (BASE::MPU + 0x08);
			volatile uint32* const RBAR							= (volatile uint32* const) (BASE::MPU + 0x0C);
			volatile uint32* const RASR							= (volatile uint32* const) (BASE::MPU + 0x10);
		};
	#endif
	
	#if defined(CORTEX_M3)
		namespace BASE
		{
			constexpr uint32 CORE										= 0xE0000000;
			
			constexpr uint32 SCB_ACTLR							= CORE + 0x0000E008;
			constexpr uint32 SYSTICK     						= CORE + 0x0000E010;
			constexpr uint32 NVIC        						= CORE + 0x0000E100;
			constexpr uint32 SCB         						= CORE + 0x0000ED00;
		};
		
		
		
		namespace SCB
		{
			volatile uint32* const ACTLR						= (volatile uint32* const) (BASE::SCB_ACTLR + 0x00);
			
			volatile uint32* const CPU_ID						= (volatile uint32* const) (BASE::SCB + 0x00);
			volatile uint32* const ICSR							= (volatile uint32* const) (BASE::SCB + 0x04);
			volatile uint32* const VTOR							= (volatile uint32* const) (BASE::SCB + 0x08);
			volatile uint32* const AIR_CR						= (volatile uint32* const) (BASE::SCB + 0x0C);
			volatile uint32* const SCR							= (volatile uint32* const) (BASE::SCB + 0x10);
			volatile uint32* const CCR							= (volatile uint32* const) (BASE::SCB + 0x14);
			volatile uint32* const SHPR1						= (volatile uint32* const) (BASE::SCB + 0x18);
			volatile uint32* const SHPR2						= (volatile uint32* const) (BASE::SCB + 0x1C);
			volatile uint32* const SHPR3						= (volatile uint32* const) (BASE::SCB + 0x20);
			volatile uint32* const SHCSR						= (volatile uint32* const) (BASE::SCB + 0x24);
			volatile uint32* const CFSR							= (volatile uint32* const) (BASE::SCB + 0x28);
			volatile uint32* const HFSR							= (volatile uint32* const) (BASE::SCB + 0x2C);
			volatile uint32* const MMAR							= (volatile uint32* const) (BASE::SCB + 0x34);
			volatile uint32* const BFAR							= (volatile uint32* const) (BASE::SCB + 0x38);
		};
		
		namespace SYSTICK
		{
			volatile uint32* const CSR							= (volatile uint32* const) (BASE::SYSTICK + 0x00);
			volatile uint32* const RVR							= (volatile uint32* const) (BASE::SYSTICK + 0x04);
			volatile uint32* const CVR							= (volatile uint32* const) (BASE::SYSTICK + 0x08);
			volatile uint32* const CALIB						= (volatile uint32* const) (BASE::SYSTICK + 0x0C);
		};
		
		namespace NVIC
		{
			volatile uint32* const ISER							= (volatile uint32* const) (BASE::NVIC + 0x000);
			volatile uint32* const ICER							= (volatile uint32* const) (BASE::NVIC + 0x080);
			volatile uint32* const ISPR							= (volatile uint32* const) (BASE::NVIC + 0x100);
			volatile uint32* const ICPR							= (volatile uint32* const) (BASE::NVIC + 0x180);
			volatile uint32* const IABR							= (volatile uint32* const) (BASE::NVIC + 0x200);
			volatile uint32* const IPR							= (volatile uint32* const) (BASE::NVIC + 0x300);
			volatile uint32* const STIR							= (volatile uint32* const) (BASE::NVIC + 0xE00);
		};
	#endif
	
	#if defined(CORTEX_M7)
		namespace BASE
		{
			constexpr uint32 CORE										= 0xE0000000;
			
			constexpr uint32 SCB_ACTLR							= CORE + 0x0000E008;
			constexpr uint32 SYSTICK     						= CORE + 0x0000E010;
			constexpr uint32 NVIC        						= CORE + 0x0000E100;
			constexpr uint32 SCB         						= CORE + 0x0000ED00;
			constexpr uint32 CPU_FEATURES						= CORE + 0x0000ED78;
			constexpr uint32 COPROCESSOR						= CORE + 0x0000ED88;
			constexpr uint32 MPU         						= CORE + 0x0000ED90;
			constexpr uint32 FPU         						= CORE + 0x0000EF30;
			constexpr uint32 CACHE       						= CORE + 0x0000EF50;
			constexpr uint32 ACCESS_CONTROL				  = CORE + 0x0000EF90;
		};
		
		
		
		namespace SCB
		{
			volatile uint32* const ACTLR						= (volatile uint32* const) (BASE::SCB_ACTLR + 0x00);
			
			volatile uint32* const CPU_ID						= (volatile uint32* const) (BASE::SCB + 0x00);
			volatile uint32* const ICSR							= (volatile uint32* const) (BASE::SCB + 0x04);
			volatile uint32* const VTOR							= (volatile uint32* const) (BASE::SCB + 0x08);
			volatile uint32* const AIR_CR						= (volatile uint32* const) (BASE::SCB + 0x0C);
			volatile uint32* const SCR							= (volatile uint32* const) (BASE::SCB + 0x10);
			volatile uint32* const CCR							= (volatile uint32* const) (BASE::SCB + 0x14);
			volatile uint32* const SHPR1						= (volatile uint32* const) (BASE::SCB + 0x18);
			volatile uint32* const SHPR2						= (volatile uint32* const) (BASE::SCB + 0x1C);
			volatile uint32* const SHPR3						= (volatile uint32* const) (BASE::SCB + 0x20);
			volatile uint32* const SHCSR						= (volatile uint32* const) (BASE::SCB + 0x24);
			volatile uint32* const CFSR							= (volatile uint32* const) (BASE::SCB + 0x28);
			volatile uint32* const HFSR							= (volatile uint32* const) (BASE::SCB + 0x2C);
			volatile uint32* const MMAR							= (volatile uint32* const) (BASE::SCB + 0x34);
			volatile uint32* const BFAR							= (volatile uint32* const) (BASE::SCB + 0x38);
			volatile uint32* const AFSR							= (volatile uint32* const) (BASE::SCB + 0x3C);
		};
		
		namespace SYSTICK
		{
			volatile uint32* const CSR							= (volatile uint32* const) (BASE::SYSTICK + 0x00);
			volatile uint32* const RVR							= (volatile uint32* const) (BASE::SYSTICK + 0x04);
			volatile uint32* const CVR							= (volatile uint32* const) (BASE::SYSTICK + 0x08);
			volatile uint32* const CALIB						= (volatile uint32* const) (BASE::SYSTICK + 0x0C);
		};
		
		namespace NVIC
		{
			volatile uint32* const ISER							= (volatile uint32* const) (BASE::NVIC + 0x000);
			volatile uint32* const ICER							= (volatile uint32* const) (BASE::NVIC + 0x080);
			volatile uint32* const ISPR							= (volatile uint32* const) (BASE::NVIC + 0x100);
			volatile uint32* const ICPR							= (volatile uint32* const) (BASE::NVIC + 0x180);
			volatile uint32* const IABR							= (volatile uint32* const) (BASE::NVIC + 0x200);
			volatile uint32* const IPR							= (volatile uint32* const) (BASE::NVIC + 0x300);
			volatile uint32* const STIR							= (volatile uint32* const) (BASE::NVIC + 0xE00);
		};
		
		namespace CPU_FEATURES
		{
			volatile uint32* const CLIDR						= (volatile uint32* const) (BASE::CPU_FEATURES + 0x00);
			volatile uint32* const CTR							= (volatile uint32* const) (BASE::CPU_FEATURES + 0x04);
			volatile uint32* const CCSIDR						= (volatile uint32* const) (BASE::CPU_FEATURES + 0x08);
			volatile uint32* const CSSELR						= (volatile uint32* const) (BASE::CPU_FEATURES + 0x0C);
		};
		
		namespace MPU
		{
			volatile uint32* const TYPE							= (volatile uint32* const) (BASE::MPU + 0x00);
			volatile uint32* const CTRL							= (volatile uint32* const) (BASE::MPU + 0x04);
			volatile uint32* const RNR							= (volatile uint32* const) (BASE::MPU + 0x08);
			volatile uint32* const RBAR							= (volatile uint32* const) (BASE::MPU + 0x0C);
			volatile uint32* const RASR							= (volatile uint32* const) (BASE::MPU + 0x10);
			volatile uint32* const RBAR_A1					= (volatile uint32* const) (BASE::MPU + 0x14);
			volatile uint32* const RASR_A1					= (volatile uint32* const) (BASE::MPU + 0x18);
			volatile uint32* const RBAR_A2					= (volatile uint32* const) (BASE::MPU + 0x1C);
			volatile uint32* const RASR_A2					= (volatile uint32* const) (BASE::MPU + 0x20);
			volatile uint32* const RBAR_A3					= (volatile uint32* const) (BASE::MPU + 0x24);
			volatile uint32* const RASR_A3					= (volatile uint32* const) (BASE::MPU + 0x28);
		};
		
		namespace COPROCESSOR
		{
			volatile uint32* const ACR							= (volatile uint32* const) (BASE::COPROCESSOR + 0x00);
		};
		
		namespace FPU
		{
			volatile uint32* const CCR							= (volatile uint32* const) (BASE::FPU + 0x04);
			volatile uint32* const CAR							= (volatile uint32* const) (BASE::FPU + 0x08);
			volatile uint32* const DSCR							= (volatile uint32* const) (BASE::FPU + 0x0C);
		};
		
		namespace CACHE
		{
			volatile uint32* const ICACHE_I_ALL_U		= (volatile uint32* const) (BASE::CACHE + 0x00);
			volatile uint32* const ICACHE_I_MVA_U		= (volatile uint32* const) (BASE::CACHE + 0x08);
			volatile uint32* const DCACHE_I_MVA_C		= (volatile uint32* const) (BASE::CACHE + 0x0C);
			volatile uint32* const DCACHE_I_SW			= (volatile uint32* const) (BASE::CACHE + 0x10);
			volatile uint32* const DCACHE_C_MVA_U		= (volatile uint32* const) (BASE::CACHE + 0x14);
			volatile uint32* const DCACHE_C_MVA_C		= (volatile uint32* const) (BASE::CACHE + 0x18);
			volatile uint32* const DCACHE_C_SW			= (volatile uint32* const) (BASE::CACHE + 0x1C);
			volatile uint32* const DCACHE_CI_MVA_C	= (volatile uint32* const) (BASE::CACHE + 0x20);
			volatile uint32* const DCACHE_CI_SW			= (volatile uint32* const) (BASE::CACHE + 0x24);
		};
		
		namespace ACCESS_CONTROL
		{
			volatile uint32* const ITCM_CR					= (volatile uint32* const) (BASE::ACCESS_CONTROL + 0x00);
			volatile uint32* const DTCM_CR					= (volatile uint32* const) (BASE::ACCESS_CONTROL + 0x04);
			volatile uint32* const AHBP_CR					= (volatile uint32* const) (BASE::ACCESS_CONTROL + 0x08);
			volatile uint32* const CA_CR					  = (volatile uint32* const) (BASE::ACCESS_CONTROL + 0x0C);
			volatile uint32* const AHBS_CR					= (volatile uint32* const) (BASE::ACCESS_CONTROL + 0x10);
			volatile uint32* const ABF_SR					  = (volatile uint32* const) (BASE::ACCESS_CONTROL + 0x18);
		};
	#endif
};