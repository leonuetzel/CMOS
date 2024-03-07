#pragma once

#include "defines.hpp"

#include "string.hpp"
#include "ringbufferDynamic.hpp"
#include "nvic.hpp"





class Thread
{
	public:
		
		enum class e_mailType:	uint8
		{
			NORMAL,
			EXIT
		};
		
		struct s_mail
		{
			uint32 data;
			e_mailType type;
			uint8 senderID;
			
			s_mail()
				:	data(0),
					type(e_mailType::NORMAL),
					senderID(0xFF)
			{
				
			}
			
			bool operator==(const Thread::s_mail& mail)
			{
				if(data != mail.data)
				{
					return(false);
				}
				if(type != mail.type)
				{
					return(false);
				}
				if(senderID != mail.senderID)
				{
					return(false);
				}
				return(true);
			}
		};
		
		
		
		
		
	private:
		
		//	This Threads Name
		String m_name;
		
		
		//	Highest possible Address of Stackpointer of this Thread
		uint32* m_stack_highestAddress;
		
		
		//	Current Stackpointer after a Preemption
		uint32* m_stack_pointer;
		
		
		//	Remaining Sleep Time in Periods of CMOS::c_clock_systick, 0 means no sleep is requested
		uint32 m_sleepTime;
		
		
		//	Stack Size
		uint16 m_stack_size;
		
		
		//	Interrupt ID that is allowed to wake up this Thread
		uint16 m_wakeUpInterruptID;
		
		
		//	Parent Thread ID
		uint8 m_parent_ID;
		
		
		//	This Threads Priority
		uint8 m_priority;
		
		
		//	A Mail has to origin from this specific Thread ID to wake up this Thread
		//	Putting the Threads own ID in here will wake up this Thread from any Interrupt oder Mail Origin
		uint8 m_wakeUpMailSender;
		
		
		//	Mailbox
		RingbufferDynamic<s_mail> m_mailBox;
		
		
		//	Bit-Array, where Interrupts can mark that they wanted to wake up this Thread
		uint8 m_wakeUpInterruptHistory[(NVIC::c_numberOfExceptions + NVIC::c_numberOfInterrupts) / 8 + (((NVIC::c_numberOfExceptions + NVIC::c_numberOfInterrupts) % 8 == 0) ? 0: 1)];
		
		
		//	Statistics
		#if defined(CORTEX_M3) || defined(CORTEX_M7)
			uint32 m_ticks;
			float m_cpuLoad;
			float m_stackLoad;
		#endif
		
		
		//	Constructor and Destructor
		inline Thread();
		inline Thread(const Thread& thread);
		inline ~Thread();
		
		inline Thread& operator=(const Thread& thread);
		
		
		//	Friends
		friend void EXCEPTION_PENDING_SV();
		friend void EXCEPTION_SYSTICK();
		friend class CMOS;
		friend class Array<Thread>;
		
		
		
		
		
	public:
		
		constexpr inline bool is_valid() const;
};



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

inline Thread::Thread()
	:	m_name(),
		m_stack_highestAddress(nullptr),
		m_stack_pointer(nullptr),
		m_sleepTime(0),
		m_stack_size(0),
		m_wakeUpInterruptID(0),
		m_parent_ID(0),
		m_priority(0),
		m_wakeUpMailSender(0),
		m_mailBox(0),
		m_wakeUpInterruptHistory()
		#if defined(CORTEX_M3) || defined(CORTEX_M7)
			,
			m_ticks(0),
			m_cpuLoad(0),
			m_stackLoad(0)
		#endif
{
	
}


inline Thread::Thread(const Thread& thread)
	:	m_mailBox(0)
{
	operator=(thread);
}


inline Thread::~Thread()
{
	
}







inline Thread& Thread::operator=(const Thread& thread)
{
	//	Simply copy the whole Memory
	//	Only Usage is for Array<Thread> to default Fill CMOS::m_thread in CMOS::run()
	
	uint8* source = (uint8*) &thread;
	uint8* destination = (uint8*) this;
	constexpr uint32 sizeInBytes = sizeof(Thread);
	
	for(uint32 i = 0; i < sizeInBytes; i++)
	{
		destination[i] = source[i];
	}
	
	return(*this);
}



/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

constexpr inline bool Thread::is_valid() const
{
	if(m_stack_pointer != nullptr && m_stack_highestAddress != nullptr)
	{
		return(true);
	}
	return(false);
}