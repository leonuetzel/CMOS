#pragma once

#include "defines.hpp"





class I_DMA
{
	public:
		
		typedef void (*f_callback)();
		
		enum class e_priority: uint8
		{
			LOW				= 0,
			MEDIUM		= 1,
			HIGH			= 2,
			VERY_HIGH	= 3	
		};
		
		enum class e_dataType: uint8
		{
			UINT8			= 0,
			UINT16		= 1,
			UINT32		= 2
		};
		
		enum class e_direction: uint8
		{
			PERIPHERAL_TO_MEMORY	= 0,
			MEMORY_TO_PERIPHERAL	= 1,
			MEMORY_TO_MEMORY			= 2
		};
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		virtual feedback reset() = 0;
		virtual feedback transfer(const void* source, const void* destination, e_dataType sourceType, e_dataType destinationType, bool source_inc, bool dest_inc, e_direction direction, bool circularMode, uint16 numberOfItems, e_priority priority = e_priority::LOW, bool waitForTransferEnd = true, f_callback callback = nullptr) = 0;
};