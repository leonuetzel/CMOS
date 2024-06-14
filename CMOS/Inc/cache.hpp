#pragma once

#if defined(CORTEX_M7)
	#include "defines.hpp"
	
	
	
	
	
	class Cache
	{
		public:
			
			
			
			
			
			
			
		private:
			
			bool m_iCache_enabled;
			bool m_iCache_writeThroughSupport;
			bool m_iCache_writeBackSupport;
			bool m_iCache_readAllocateSupport;
			bool m_iCache_writeAllocateSupport;
			uint32 m_iCache_setNumber;
			uint32 m_iCache_wayNumber;
			uint32 m_iCache_lineSize;
			uint32 m_iCache_size;
			
			bool m_dCache_enabled;
			bool m_dCache_writeThroughSupport;
			bool m_dCache_writeBackSupport;
			bool m_dCache_readAllocateSupport;
			bool m_dCache_writeAllocateSupport;
			uint32 m_dCache_setNumber;
			uint32 m_dCache_wayNumber;
			uint32 m_dCache_lineSize;
			uint32 m_dCache_size;
			
			constexpr inline Cache();
			Cache(const Cache& cache) = delete;
			inline ~Cache();
			
			
			friend class CMOS;
			
			
			
			
			
		public:
			
			feedback startup();
			
			feedback ICache_setState(bool enable);
			feedback DCache_setState(bool enable);
			
			feedback ICache_invalidate();
			feedback ICache_invalidate(uint32* address);
			feedback ICache_invalidate(uint32* address, uint32 sizeInBytes);
			
			feedback DCache_invalidate();
			feedback DCache_invalidate(uint32* address);
			feedback DCache_invalidate(uint32* address, uint32 sizeInBytes);
			feedback DCache_invalidate(uint32 set, uint32 way);
			
			feedback DCache_clean();
			feedback DCache_clean(uint32* address);
			feedback DCache_clean(uint32* address, uint32 sizeInBytes);
			feedback DCache_clean(uint32 set, uint32 way);
			
			feedback DCache_cleanAndInvalidate();
			feedback DCache_cleanAndInvalidate(uint32* address);
			feedback DCache_cleanAndInvalidate(uint32* address, uint32 sizeInBytes);
			feedback DCache_cleanAndInvalidate(uint32 set, uint32 way);
	};
	
	
	
	/*****************************************************************************/
	/*                    Globals and Static Initialization					 						 */
	/*****************************************************************************/
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Private	  			 						 						 */
	/*****************************************************************************/
	
	constexpr inline Cache::Cache()
		:	m_iCache_enabled(false),
			m_iCache_writeThroughSupport(false),
			m_iCache_writeBackSupport(false),
			m_iCache_readAllocateSupport(false),
			m_iCache_writeAllocateSupport(false),
			m_iCache_setNumber(0),
			m_iCache_wayNumber(0),
			m_iCache_lineSize(0),
			m_iCache_size(0),
			
			m_dCache_enabled(false),
			m_dCache_writeThroughSupport(false),
			m_dCache_writeBackSupport(false),
			m_dCache_readAllocateSupport(false),
			m_dCache_writeAllocateSupport(false),
			m_dCache_setNumber(0),
			m_dCache_wayNumber(0),
			m_dCache_lineSize(0),
			m_dCache_size(0)
	{
		
	}
	
	
	inline Cache::~Cache()
	{
		
	}
	
	
	
	/*****************************************************************************/
	/*                      					Protected	  			 						 						 */
	/*****************************************************************************/
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Public	  			 						 						 */
	/*****************************************************************************/
	
	
#endif