#include "../Inc/cmos.hpp"





#if defined(CORTEX_M0P) || defined(CORTEX_M4) || defined(CORTEX_M7)
	/*****************************************************************************/
	/*                    Globals and Static Initialization					 						 */
	/*****************************************************************************/
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Private	  			 						 						 */
	/*****************************************************************************/
	
	MPU::e_size MPU::get_size(uint32 sizeInBytes)
	{
		return((e_size) Math::log2(sizeInBytes));
	}
	
	
	
	/*****************************************************************************/
	/*                      					Protected	  			 						 						 */
	/*****************************************************************************/
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Public	  			 						 						 */
	/*****************************************************************************/
	
	feedback MPU::init()
	{
		//	NULL Pointer Catching
		#if defined(CORTEX_M0P)
			set_region(0, 0x00000000, e_size::_512M,	e_accessPermission::RW_NONE,		e_attribute::NORMAL_NOTSHAREABLE_WT, true,	0xFF, true);
			set_region(1, 0x00000000, e_size::_256B,	e_accessPermission::NONE_NONE,	e_attribute::NORMAL_NOTSHAREABLE_WT, false,	0xFF, true);
		#endif
		#if defined(CORTEX_M4) || defined(CORTEX_M7)
			set_region(0, 0x00000000, e_size::_512M,	e_accessPermission::RW_NONE,		e_attribute::NORMAL_NOTSHAREABLE_NONE, true,	0xFF, true);
			set_region(1, 0x00000000, e_size::_32B,		e_accessPermission::NONE_NONE,	e_attribute::NORMAL_NOTSHAREABLE_NONE, false,	0xFF, true);
		#endif
		
		
		//	Any Accesses to mpu-undefined Regions is allowed for priviledged Software
		bit::set(*CORE::MPU::CTRL, 2);
		
		//	MPU disabled during Hard Fault, NMI oder FAULTMASK Handlers
		bit::clear(*CORE::MPU::CTRL, 1);
		
		//	Enable MPU
		bit::set(*CORE::MPU::CTRL, 0);
		
		return(OK);
	}
	
	
	feedback MPU::set_region(uint8 regionNumber, uint32 baseAddress, e_size size, e_accessPermission accessPermission, e_attribute attribute, bool allowInstructionFetch, uint8 subRegionsEnabled, bool enable)
	{
		if(regionNumber >= c_numberOfRegions)
		{
			return(FAIL);
		}
		
		uint32 mask = (1 << ((uint32) size));
		if(baseAddress % mask != 0)
		{
			return(FAIL);
		}
		
		mask = ((((uint32) size) - 1) << 1);
		mask |= (((uint32) accessPermission	) << 24);
		mask |= (((uint32) attribute				) << 16);
		
		
		//	Instruction Fetching
		if(allowInstructionFetch == false)
		{
			bit::set(mask, 28);
		}
		
		
		//	Subregions Enable
		for(uint32 i = 0; i < 8; i++)
		{
			if(bit::isCleared(subRegionsEnabled, i))
			{
				bit::set(mask, i + 8);
			}
		}
		
		
		//	Select Region
		*CORE::MPU::RNR = regionNumber & 0x0000000F;
		
		
		//	Disable Region
		bit::clear(*CORE::MPU::RASR, 0);
		
		
		//	Region Base Address
		*CORE::MPU::RBAR = baseAddress;
		*CORE::MPU::RASR = mask;
		
		if(enable == true)
		{
			//	Enabled Region
			bit::set(*CORE::MPU::RASR, 0);
		}
		
		return(OK);
	}
#endif