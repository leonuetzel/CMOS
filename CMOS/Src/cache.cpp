#include "../Inc/cmos.hpp"





#if defined(CORTEX_M7)
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
	
	feedback Cache::startup()
	{
		*CORE::CPU_FEATURES::CSSELR = 1;																																																//	Select I-Cache
		asm volatile("dsb");
		uint32 iCacheData = *CORE::CPU_FEATURES::CCSIDR;
		{																																																															//	Readout Cache Properties
		if(bit::isSet(iCacheData, 31))
		{
			m_iCache_writeThroughSupport = true;
		}
		else
		{
			m_iCache_writeThroughSupport = false;
		}
		if(bit::isSet(iCacheData, 30))
		{
			m_iCache_writeBackSupport = true;
		}
		else
		{
			m_iCache_writeBackSupport = false;
		}
		if(bit::isSet(iCacheData, 29))
		{
			m_iCache_readAllocateSupport = true;
		}
		else
		{
			m_iCache_readAllocateSupport = false;
		}
		if(bit::isSet(iCacheData, 28))
		{
			m_iCache_writeAllocateSupport = true;
		}
		else
		{
			m_iCache_writeAllocateSupport = false;
		}
		
		m_iCache_setNumber	= ((iCacheData & 0x0FFFE000) >> 13) + 1;
		m_iCache_wayNumber	= ((iCacheData & 0x00001FF8) >> 3 ) + 1;
		m_iCache_lineSize		= ((iCacheData & 0x00000007)			) + 2;
		m_iCache_lineSize = 4 * Math::power<uint32, uint32>(2, m_iCache_lineSize);
		
		m_iCache_size = m_iCache_setNumber * m_iCache_wayNumber * m_iCache_lineSize;
		}
		
		
		
		*CORE::CPU_FEATURES::CSSELR = 0;																																																//	Select D-Cache																																																					//	Select I-Cache
		asm volatile("dsb");
		uint32 dCacheData = *CORE::CPU_FEATURES::CCSIDR;
		{																																																															//	Readout Cache Properties
		if(bit::isSet(dCacheData, 31))
		{
			m_dCache_writeThroughSupport = true;
		}
		else
		{
			m_dCache_writeThroughSupport = false;
		}
		if(bit::isSet(dCacheData, 30))
		{
			m_dCache_writeBackSupport = true;
		}
		else
		{
			m_dCache_writeBackSupport = false;
		}
		if(bit::isSet(dCacheData, 29))
		{
			m_dCache_readAllocateSupport = true;
		}
		else
		{
			m_dCache_readAllocateSupport = false;
		}
		if(bit::isSet(dCacheData, 28))
		{
			m_dCache_writeAllocateSupport = true;
		}
		else
		{
			m_dCache_writeAllocateSupport = false;
		}
		
		m_dCache_setNumber	= ((dCacheData & 0x0FFFE000) >> 13) + 1;
		m_dCache_wayNumber	= ((dCacheData & 0x00001FF8) >> 3 ) + 1;
		m_dCache_lineSize		= ((dCacheData & 0x00000007)			) + 2;
		m_dCache_lineSize = 4 * Math::power<uint32, uint32>(2, m_dCache_lineSize);
		
		m_dCache_size = m_dCache_setNumber * m_dCache_wayNumber * m_dCache_lineSize;
		}
		
		ICache_setState(true);
		DCache_setState(true);
		
		return(OK);
	}
	
	
	
	
	
	
	
	feedback Cache::ICache_setState(bool enable)
	{
		if(enable == true && m_iCache_enabled == false)
		{
		ICache_invalidate();
		bit::set(*CORE::SCB::CCR, 17);
		asm volatile("dsb");
		asm volatile("isb");
		m_iCache_enabled = true;
		}
		if(enable == false && m_iCache_enabled == true)
		{
		asm volatile("dsb");
		asm volatile("isb");
		bit::clear(*CORE::SCB::CCR, 17);
		ICache_invalidate();
		m_iCache_enabled = false;
		}
		return(OK);
	}
	
	
	feedback Cache::DCache_setState(bool enable)
	{
		if(enable == true && m_dCache_enabled == false)
		{
		DCache_invalidate();
		bit::set(*CORE::SCB::CCR, 16);
		asm volatile("dsb");
		asm volatile("isb");
		m_dCache_enabled = true;
		}
		if(enable == false && m_dCache_enabled == true)
		{
		bit::clear(*CORE::SCB::CCR, 16);
		DCache_cleanAndInvalidate();
		asm volatile("dsb");
		asm volatile("isb");
		m_dCache_enabled = false;
		}
		return(OK);
	}
	
	
	
	
	
	
	
	feedback Cache::ICache_invalidate()
	{
		asm volatile("dsb");
		asm volatile("isb");
		*CORE::CACHE::ICACHE_I_ALL_U = 0;
		asm volatile("dsb");
		asm volatile("isb");
		
		return(OK);
	}
	
	
	feedback Cache::ICache_invalidate(uint32* address)
	{
		uint32 alignDiff = ((uint32) address) % m_iCache_lineSize;
		address -= (alignDiff / 4);
		
		asm volatile("dsb");
		asm volatile("isb");
		*CORE::CACHE::ICACHE_I_MVA_U = (uint32) address;
		asm volatile("dsb");
		asm volatile("isb");
		
		return(OK);
	}
	
	
	feedback Cache::ICache_invalidate(uint32* address, uint32 sizeInBytes)
	{
		uint32* end_old = address + (sizeInBytes / 4);
		uint32 alignDiff = ((uint32) address) % m_iCache_lineSize;
		address -= (alignDiff / 4);
		sizeInBytes += m_iCache_lineSize - (sizeInBytes % m_iCache_lineSize);
		uint32* end_new = address + (sizeInBytes / 4);
		if(end_old > end_new)
		{
		sizeInBytes += m_iCache_lineSize;
		}
		
		asm volatile("dsb");
		asm volatile("isb");
		for(uint32 i = 0; i < sizeInBytes / m_iCache_lineSize; i++)
		{
		*CORE::CACHE::ICACHE_I_MVA_U = (uint32) (address + i * (m_iCache_lineSize / 4));
		}
		asm volatile("dsb");
		asm volatile("isb");
		
		return(OK);
	}
	
	
	
	
	
	
	
	feedback Cache::DCache_invalidate()
	{
		asm volatile("dsb");
		asm volatile("isb");
		for(uint32 set = 0; set < m_dCache_setNumber; set++)
		{
		for(uint32 way = 0; way < m_dCache_wayNumber; way++)
		{
			*CORE::CACHE::DCACHE_I_SW = (way<< 30) | (set << 5);
		}
		}
		asm volatile("dsb");
		asm volatile("isb");
		
		return(OK);
	}
	
	
	feedback Cache::DCache_invalidate(uint32* address)
	{
		uint32 alignDiff = ((uint32) address) % m_dCache_lineSize;
		address -= (alignDiff / 4);
		
		asm volatile("dsb");
		asm volatile("isb");
		*CORE::CACHE::DCACHE_I_MVA_C = (uint32) address;
		asm volatile("dsb");
		asm volatile("isb");
		
		return(OK);
	}
	
	
	feedback Cache::DCache_invalidate(uint32* address, uint32 sizeInBytes)
	{
		uint32* end_old = address + (sizeInBytes / 4);
		uint32 alignDiff = ((uint32) address) % m_dCache_lineSize;
		address -= (alignDiff / 4);
		sizeInBytes += m_dCache_lineSize - (sizeInBytes % m_dCache_lineSize);
		uint32* end_new = address + (sizeInBytes / 4);
		if(end_old > end_new)
		{
		sizeInBytes += m_dCache_lineSize;
		}
		
		asm volatile("dsb");
		asm volatile("isb");
		for(uint32 i = 0; i < sizeInBytes / m_dCache_lineSize; i++)
		{
		*CORE::CACHE::DCACHE_I_MVA_C = (uint32) (address + i * (m_dCache_lineSize / 4));
		}
		asm volatile("dsb");
		asm volatile("isb");
		
		return(OK);
	}
	
	
	feedback Cache::DCache_invalidate(uint32 set, uint32 way)
	{
		asm volatile("dsb");
		asm volatile("isb");
		if(set >= m_dCache_setNumber || way >= m_dCache_wayNumber)
		{
		return(FAIL);
		}
		
		*CORE::CACHE::DCACHE_I_SW = (way<< 30) | (set << 5);
		asm volatile("dsb");
		asm volatile("isb");
		
		return(OK);
	}
	
	
	
	
	
	
	
	feedback Cache::DCache_clean()
	{
		asm volatile("dsb");
		asm volatile("isb");
		for(uint32 set = 0; set < m_dCache_setNumber; set++)
		{
		for(uint32 way = 0; way < m_dCache_wayNumber; way++)
		{
			*CORE::CACHE::DCACHE_C_SW = (way<< 30) | (set << 5);
		}
		}
		asm volatile("dsb");
		asm volatile("isb");
		
		return(OK);
	}
	
	
	feedback Cache::DCache_clean(uint32* address)
	{
		uint32 alignDiff = ((uint32) address) % m_dCache_lineSize;
		address -= (alignDiff / 4);
		
		asm volatile("dsb");
		asm volatile("isb");
		*CORE::CACHE::DCACHE_C_MVA_C = (uint32) address;
		asm volatile("dsb");
		asm volatile("isb");
		
		return(OK);
	}
	
	
	feedback Cache::DCache_clean(uint32* address, uint32 sizeInBytes)
	{
		uint32* end_old = address + (sizeInBytes / 4);
		uint32 alignDiff = ((uint32) address) % m_dCache_lineSize;
		address -= (alignDiff / 4);
		sizeInBytes += m_dCache_lineSize - (sizeInBytes % m_dCache_lineSize);
		uint32* end_new = address + (sizeInBytes / 4);
		if(end_old > end_new)
		{
		sizeInBytes += m_dCache_lineSize;
		}
		
		asm volatile("dsb");
		asm volatile("isb");
		for(uint32 i = 0; i < sizeInBytes / m_dCache_lineSize; i++)
		{
		*CORE::CACHE::DCACHE_C_MVA_C = (uint32) (address + i * (m_dCache_lineSize / 4));
		}
		asm volatile("dsb");
		asm volatile("isb");
		
		return(OK);
	}
	
	
	feedback Cache::DCache_clean(uint32 set, uint32 way)
	{
		if(set >= m_dCache_setNumber || way >= m_dCache_wayNumber)
		{
		return(FAIL);
		}
		
		asm volatile("dsb");
		asm volatile("isb");
		*CORE::CACHE::DCACHE_C_SW = (way<< 30) | (set << 5);
		asm volatile("dsb");
		asm volatile("isb");
		
		return(OK);
	}
	
	
	
	
	
	
	
	feedback Cache::DCache_cleanAndInvalidate()
	{
		asm volatile("dsb");
		asm volatile("isb");
		for(uint32 set = 0; set < m_dCache_setNumber; set++)
		{
		for(uint32 way = 0; way < m_dCache_wayNumber; way++)
		{
			*CORE::CACHE::DCACHE_CI_SW = (way<< 30) | (set << 5);
		}
		}
		asm volatile("dsb");
		asm volatile("isb");
		
		return(OK);
	}
	
	
	feedback Cache::DCache_cleanAndInvalidate(uint32* address)
	{
		uint32 alignDiff = ((uint32) address) % m_dCache_lineSize;
		address -= (alignDiff / 4);
		
		asm volatile("dsb");
		asm volatile("isb");
		*CORE::CACHE::DCACHE_CI_MVA_C = (uint32) address;
		asm volatile("dsb");
		asm volatile("isb");
		
		return(OK);
	}
	
	
	feedback Cache::DCache_cleanAndInvalidate(uint32* address, uint32 sizeInBytes)
	{
		uint32* end_old = address + (sizeInBytes / 4);
		uint32 alignDiff = ((uint32) address) % m_dCache_lineSize;
		address -= (alignDiff / 4);
		sizeInBytes += m_dCache_lineSize - (sizeInBytes % m_dCache_lineSize);
		uint32* end_new = address + (sizeInBytes / 4);
		if(end_old > end_new)
		{
		sizeInBytes += m_dCache_lineSize;
		}
		
		asm volatile("dsb");
		asm volatile("isb");
		for(uint32 i = 0; i < sizeInBytes / m_dCache_lineSize; i++)
		{
		*CORE::CACHE::DCACHE_CI_MVA_C = (uint32) (address + i * (m_dCache_lineSize / 4));
		}
		asm volatile("dsb");
		asm volatile("isb");
		
		return(OK);
	}
	
	
	feedback Cache::DCache_cleanAndInvalidate(uint32 set, uint32 way)
	{
		if(set >= m_dCache_setNumber || way >= m_dCache_wayNumber)
		{
		return(FAIL);
		}
		
		asm volatile("dsb");
		asm volatile("isb");
		*CORE::CACHE::DCACHE_CI_SW = (way<< 30) | (set << 5);
		asm volatile("dsb");
		asm volatile("isb");
		
		return(OK);
	}
#endif