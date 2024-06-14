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
		uint32* m_stackHighestAddress;
		
		
		//	Current Stackpointer after a Preemption
		uint32* m_stackPointer;
		
		
		//	Remaining Sleep Time in Periods of CMOS::c_clock_systick
		//	Set to 0 if Thread is ready to be executed
		uint32 m_sleepTime;
		
		
		//	Stack Size
		uint16 m_stackSize;
		
		
		//	Event ID that is allowed to wake up this Thread
		//	Set to 0xFFFF if Thread is ready to be executed
		//	Set to 0xFFFE if Thread is waiting for any subscripted Event to wake it up
		uint16 m_wakeUpEventID;
		
		
		//	Event ID that woke the Thread up
		uint16 m_triggeredWakeUpEventID;
		
		
		//	Event ID that is monitored
		//	Use Case:	A Mechanism is triggered that leads to an Event to be emitted
		//						but the Event is emitted before the Thread that triggered the Mechanism
		//						has gone to sleep. This leads to the Thread sleeping forever just because
		//						the Event occured "too fast".
		uint16 m_listeningEventID;
		bool m_listeningEventOccured;
		
		
		//	Parent Thread ID
		uint8 m_parentID;
		
		
		//	This Threads Priority
		uint8 m_priority;
		
		
		//	A Mail has to origin from this specific Thread ID to wake up this Thread
		//	Putting the Threads own ID in here will wake up this Thread from any Interrupt oder Mail Origin
		uint8 m_wakeUpMailSender;
		
		
		//	Mailbox
		RingbufferDynamic<s_mail> m_mailBox;
		
		
		//	Statistics
		#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
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
		m_stackHighestAddress(nullptr),
		m_stackPointer(nullptr),
		m_sleepTime(0),
		m_stackSize(0),
		m_wakeUpEventID(0xFFFF),
		m_triggeredWakeUpEventID(0xFFFF),
		m_listeningEventID(0xFFFF),
		m_listeningEventOccured(false),
		m_parentID(0),
		m_priority(0),
		m_wakeUpMailSender(0),
		m_mailBox(0)
		#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
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
	if(m_stackPointer != nullptr && m_stackHighestAddress != nullptr)
	{
		return(true);
	}
	return(false);
}