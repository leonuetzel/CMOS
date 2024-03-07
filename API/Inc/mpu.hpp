#pragma once

#if defined(CORTEX_M0P) || defined(CORTEX_M7)
	#include "defines.hpp"
	
	
	
	
	
	class MPU
	{
		public:
			
			#if defined(CORTEX_M0P)
				static constexpr uint32 c_numberOfRegions = 8;
			#endif
			#if defined(CORTEX_M7)
				static constexpr uint32 c_numberOfRegions = 16;
			#endif
			
			enum class e_size
			{
				#if defined(CORTEX_M7)
					_32B			= 5,
					_64B			= 6,
					_128B			= 7,
				#endif
				_256B			= 8,
				_512B			= 9,
				_1k				= 10,
				_2k				= 11,
				_4k				= 12,
				_8k				= 13,
				_16k			= 14,
				_32k			= 15,
				_64k			= 16,
				_128k			= 17,
				_256k			= 18,
				_512k			= 19,
				_1M				= 20,
				_2M				= 21,
				_4M				= 22,
				_8M				= 23,
				_16M			= 24,
				_32M			= 25,
				_64M			= 26,
				_128M			= 27,
				_256M			= 28,
				_512M			= 29,
				_1G				= 30,
				_2G				= 31,
				_4G				= 32
			};
			
			enum class e_accessPermission
			{
				NONE_NONE	= 0x0,
				RO_NONE		= 0x5,
				RO_RO			= 0x6,
				RW_NONE		= 0x1,
				RW_RO			= 0x2,
				RW_RW			= 0x3
			};
			
			enum class e_attribute
			{
				STRONGLYORDERED_SHAREABLE		= 0x00,
				DEVICE_SHAREABLE						= 0x01,
				NORMAL_NOTSHAREABLE_WT			= 0x02,
				NORMAL_SHAREABLE_WT					= 0x06,
				NORMAL_NOTSHAREABLE_WB			= 0x03,
				NORMAL_SHAREABLE_WB					= 0x07
				#if defined(CORTEX_M7)
					,
					NORMAL_NOTSHAREABLE_NONE		= 0x08,
					NORMAL_SHAREABLE_NONE				= 0x0C,
					NORMAL_NOTSHAREABLE_WBWARA	= 0x0B,
					NORMAL_SHAREABLE_WBWARA			= 0x0F,
					DEVICE_NOTSHAREABLE_NONE		= 0x10
				#endif
			};
			
			
			
			
			
		private:
			
			constexpr inline MPU();
			MPU(const MPU& mpu) = delete;
			inline ~MPU();
			
			e_size get_size(uint32 sizeInBytes);
			
			
			friend class CMOS;
			
			
			
			
			
		public:
			
			feedback init();
			feedback set_region(uint8 regionNumber, uint32 baseAddress, e_size size, e_accessPermission accessPermission, e_attribute attribute, bool allowInstructionFetch, uint8 subRegionsEnabled, bool enable);
	};
	
	
	
	/*****************************************************************************/
	/*                    Globals and Static Initialization					 						 */
	/*****************************************************************************/
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Private	  			 						 						 */
	/*****************************************************************************/
	
	constexpr inline MPU::MPU()
	{
		
	}
	
	
	inline MPU::~MPU()
	{
		
	}
	
	
	
	/*****************************************************************************/
	/*                      					Protected	  			 						 						 */
	/*****************************************************************************/
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Public	  			 						 						 */
	/*****************************************************************************/
	
	
#endif