#pragma once

#include "defines.hpp"





class Stack
{
	public:
		
		static constexpr uint32 c_checkValue = 0xAAAA5555;
		
		
		
		
		
	private:
		
		static constexpr uint8 c_empty = 0xFF;
		
		const uint32 c_size;
		const uint32 c_blockSize;
		const uint32 c_blockNumber;
		uint8* m_blockOwner;
		uint32* m_data;
		
		
		inline Stack();
		Stack(const Stack& stack) = delete;
		~Stack();
		
		
		friend class CMOS;
		
		
		
		
		
	public:
		
		
		uint32* reserve(uint8 thread_ID, uint32 sizeInBytes);
		void free(uint8 thread_ID);
		uint32 get_sizeInBytes();
		uint32 alignToBlocksize(uint32 sizeInBytes);
};



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/

extern uint32 __cmos_stack_start__;
extern uint32 __cmos_stack_size__;
extern uint32 __cmos_stack_blockSize__;
extern uint32 __cmos_stack_blockNumber__;
extern uint32 __cmos_stack_blockOwner__;



/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

inline Stack::Stack()
	:	c_size				((uint32)		&__cmos_stack_size__				),
		c_blockSize		((uint32)		&__cmos_stack_blockSize__	),
		c_blockNumber	((uint32)		&__cmos_stack_blockNumber__),
		m_blockOwner	((uint8*)		&__cmos_stack_blockOwner__	),
		m_data				((uint32*)	&__cmos_stack_start__			)
{
	for(uint32 i = 0; i < c_blockNumber; i++)
	{
		m_blockOwner[i] = c_empty;
	}
}


inline Stack::~Stack()
{
	
}



/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

