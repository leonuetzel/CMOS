#pragma once

#include "defines.hpp"
#include "bit.hpp"





class Heap
{
	public:
		
		
		
		
		
		
	private:
		
		const uint32 c_size;
		const uint32 c_blockSize;
		const uint32 c_blockNumber;
		uint8* m_blockOwner;
		uint32* m_data;
		
		uint32 m_sizeReservedInBytes;
		
		
		inline Heap();
		Heap(const Heap& heap) = delete;
		inline ~Heap();
		
		inline feedback reserve_block(uint32 blockNumber, bool isEndOfBlockchain);
		inline feedback free_block(uint32 blockNumber);
		inline bool isBlockFree(uint32 blockNumber) const;
		inline bool isEndOfBlockchain(uint32 blockNumber) const;
		
		
		friend class CMOS;
		
		
		
		
		
	public:
		
		void* reserve(uint8 thread_ID, uint32 sizeInBytes);
		uint32 free(void* address);
		
		inline uint32 alignToBlocksize(uint32 sizeInBytes);
		inline void* alignToBlock(void* address);
		
		constexpr inline uint32 get_sizeTotalInBytes() const;
		inline static uint32 get_blockSize();
		constexpr inline uint32 get_sizeReservedInBytes() const;
};



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/

extern uint32 __cmos_heap_start__;
extern uint32 __cmos_heap_size__;
extern uint32 __cmos_heap_blockSize__;
extern uint32 __cmos_heap_blockNumber__;
extern uint32 __cmos_heap_blockOwner__;



/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

inline Heap::Heap()
	:	c_size				((uint32)		&__cmos_heap_size__				),
		c_blockSize		((uint32)		&__cmos_heap_blockSize__	),
		c_blockNumber	((uint32)		&__cmos_heap_blockNumber__),
		m_blockOwner	((uint8*)		&__cmos_heap_blockOwner__	),
		m_data				((uint32*)	&__cmos_heap_start__			),
		
		m_sizeReservedInBytes(0)
{
	for(uint32 i = 0; i < c_blockNumber / 4; i++)
	{
		m_blockOwner[i] = 0;
	}
}


inline Heap::~Heap()
{
	
}







inline feedback Heap::reserve_block(uint32 blockNumber, bool isEndOfBlockchain)
{
	if(blockNumber >= c_blockNumber)
	{
		return(FAIL);
	}
	
	
	//	Get Bit Positions of Block
	const uint8 bit_isReserved = 2 * (blockNumber % 4);
	const uint8 bit_isBlockEnd = bit_isReserved + 1;
	
	
	//	Get local Copy of Block Owner Information and check if already reserved
	uint8 blockOwner = m_blockOwner[blockNumber / 4];
	if(bit::isSet(blockOwner, bit_isReserved) == true)
	{
		return(FAIL);
	}
	
	
	//	Set new Owner Bits
	bit::set(blockOwner, bit_isReserved);
	if(isEndOfBlockchain == true)
	{
		bit::set(blockOwner, bit_isBlockEnd);
	}
	else
	{
		bit::clear(blockOwner, bit_isBlockEnd);
	}
	
	
	//	Write local modified Copy back to Memory
	m_blockOwner[blockNumber / 4] = blockOwner;
	m_sizeReservedInBytes += c_blockSize;
	return(OK);
}


inline feedback Heap::free_block(uint32 blockNumber)
{
	if(blockNumber >= c_blockNumber)
	{
		return(FAIL);
	}
	
	
	//	Get Bit Positions of Block
	const uint8 bit_isReserved = 2 * (blockNumber % 4);
	const uint8 bit_isBlockEnd = bit_isReserved + 1;
	
	
	//	Get local Copy of Block Owner Information and check if Block is reserved
	uint8 blockOwner = m_blockOwner[blockNumber / 4];
	if(bit::isCleared(blockOwner, bit_isReserved) == true)
	{
		return(FAIL);
	}
	
	
	//	Free Block
	bit::clear(blockOwner, bit_isReserved);
	bit::clear(blockOwner, bit_isBlockEnd);
	
	
	//	Write local modified Copy back to Memory
	m_blockOwner[blockNumber / 4] = blockOwner;
	m_sizeReservedInBytes -= c_blockSize;
	return(OK);
}


inline bool Heap::isBlockFree(uint32 blockNumber) const
{
	if(blockNumber >= c_blockNumber)
	{
		return(true);
	}
	
	
	//	Get Bit Positions of Block
	const uint8 bit_isReserved = 2 * (blockNumber % 4);
	
	
	//	Check if Block is reserved
	uint8& blockOwner = m_blockOwner[blockNumber / 4];
	return(bit::isCleared(blockOwner, bit_isReserved));
}


inline bool Heap::isEndOfBlockchain(uint32 blockNumber) const
{
	if(blockNumber >= c_blockNumber)
	{
		return(true);
	}
	
	
	//	Get Bit Positions of Block
	const uint8 bit_isReserved = 2 * (blockNumber % 4);
	const uint8 bit_isBlockEnd = bit_isReserved + 1;
	
	
	//	Check if Block is reserved
	uint8& blockOwner = m_blockOwner[blockNumber / 4];
	if(bit::isCleared(blockOwner, bit_isReserved) == true)
	{
		return(true);
	}
	
	
	//	Get End-of-Blockchain Information
	return(bit::isSet(blockOwner, bit_isBlockEnd));
}



/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

inline uint32 Heap::alignToBlocksize(uint32 sizeInBytes)
{
	if(sizeInBytes < c_blockSize)
	{
		return(c_blockSize);
	}
	
	uint32 mod = sizeInBytes % c_blockSize;
	if(mod != 0)
	{
		sizeInBytes += c_blockSize - mod;
	}
	return(sizeInBytes);
}


inline void* Heap::alignToBlock(void* address)
{
	if(address == nullptr || address < m_data || address >= m_data + c_size / 4)
	{
		return(nullptr);
	}
	
	uint32 start_block = ((uint32) address - (uint32) m_data) / c_blockSize;
	return(&m_data[start_block * c_blockSize / 4]);
}







constexpr inline uint32 Heap::get_sizeTotalInBytes() const
{
	return(c_size);
}


inline uint32 Heap::get_blockSize()
{
	extern uint32 __cmos_heap_blockSize__;
	return((uint32) &__cmos_heap_blockSize__);
}


constexpr inline uint32 Heap::get_sizeReservedInBytes() const
{
	return(m_sizeReservedInBytes);
}