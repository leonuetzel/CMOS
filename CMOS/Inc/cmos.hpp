#pragma once


//	General Defines
#include "defines.hpp"
#include "bit.hpp"


//	Hardware
#include "peripherals.hpp"


//	Interfaces
#include "filesystem/i_directory.hpp"
#include "filesystem/i_drive.hpp"
#include "filesystem/i_entry.hpp"
#include "filesystem/i_file.hpp"
#include "filesystem/i_volume.hpp"

#include "graphics/i_displayDriver.hpp"
#include "graphics/i_graphicAccelerator.hpp"

#include "i_can.hpp"
#include "i_crc.hpp"
#include "i_dma.hpp"
#include "i_i2c.hpp"
#include "i_mdio.hpp"
#include "i_nvm.hpp"
#include "i_semaphore.hpp"
#include "i_serial.hpp"
#include "i_spi.hpp"


//	Hardware Modules Design
#include "canFrame.hpp"
#include "uart.hpp"


//	Software DMA
#include "dmaSoftware.hpp"


//	Threading
#include "thread.hpp"
#include "stack.hpp"
#include "heap.hpp"


//	Container Classes
#include "array.hpp"
#include "matrix.hpp"
#include "i_ringbuffer.hpp"
#include "ringbufferStatic.hpp"
#include "ringbufferExternal.hpp"
#include "ringbufferDynamic.hpp"
#include "string.hpp"


//	Structures
#include "pair.hpp"
#include "triplet.hpp"
#include "uniqueArray.hpp"
#include "uniquePairArray.hpp"


//	Graphic
#include "graphics/rectGraphic.hpp"
#include "graphics/color.hpp"
#include "graphics/colors.hpp"


//	Math
#include "math.hpp"


//	Miscellaneous
#include "language.hpp"
#include "time.hpp"


//	File Formats
#include "csv.hpp"
#include "json.hpp"


//	IC Driver (ICD)
#include "icd/24lc02b.hpp"
#include "icd/ad5175.hpp"
#include "icd/bq25887.hpp"
#include "icd/dp83825i.hpp"
#include "icd/mb85rc16.hpp"
#include "icd/mcp23016.hpp"
#include "icd/mcp3427.hpp"
#include "icd/mcp3428.hpp"
#include "icd/stc3100.hpp"





#if defined(CORTEX_M0)
	#include "scb.hpp"
	#include "nvic.hpp"
	#include "systick.hpp"
#endif

#if defined(CORTEX_M0P)
	#include "scb.hpp"
	#include "nvic.hpp"
	#include "systick.hpp"
	#include "mpu.hpp"
#endif

#if defined(CORTEX_M3)
	#include "scb.hpp"
	#include "nvic.hpp"
	#include "systick.hpp"
	#include "semaphoreExclusive.hpp"
#endif

#if defined(CORTEX_M4)
	#include "scb.hpp"
	#include "nvic.hpp"
	#include "fpu.hpp"
	#include "systick.hpp"
	#include "mpu.hpp"
	#include "semaphoreExclusive.hpp"
#endif

#if defined(CORTEX_M7)
	#include "scb.hpp"
	#include "nvic.hpp"
	#include "fpu.hpp"
	#include "systick.hpp"
	#include "mpu.hpp"
	#include "cache.hpp"
	#include "semaphoreExclusive.hpp"
#endif





class CMOS
{
	public:
		
		//	Function Prototype for a Thread
		typedef void (*f_thread)(void);
		
		
		//	ID for invalid Thread Handle
		static constexpr uint8 threadID_invalid					= 0xFF;
		static constexpr uint16 eventID_invalid					= 0xFFFF;
		static constexpr uint16 eventID_any							= 0xFFFE;
		
		
		//	Systick Clock for Sleep Functions
		#if defined(CORTEX_M0) || defined(CORTEX_M0P) || defined(CORTEX_M3) || defined(CORTEX_M4)
			static constexpr uint32 c_clock_systick				= 10000;
		#endif
		
		#if defined(CORTEX_M7)
			static constexpr uint32 c_clock_systick				= 100000;
		#endif
		
		
		//	Maximum Sleep Durations
		static constexpr uint32 c_sleepTime_us_max			= (0xFFFFFFFF / c_clock_systick) * 1000000;
		static constexpr uint32 c_sleepTime_ms_max			= (0xFFFFFFFF / c_clock_systick) * 1000;
		static constexpr uint32 c_sleepTime_sec_max			=  0xFFFFFFFF / c_clock_systick;
		
		
		
		
		
	private:
		
		//	Logging
		#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
			float m_loadCpuPercent;
			float m_loadStackReservedAndUsedPercent;
			float m_loadStackReservedAndUnusedPercent;
			float m_loadStackPercent;
			float m_loadHeapPercent;
		#endif
		
		
		//	Hardware
		#if defined(CORTEX_M0)
			SCB m_scb;
			NVIC m_nvic;
			Systick m_systick;
		#endif
		
		#if defined(CORTEX_M0P)
			SCB m_scb;
			NVIC m_nvic;
			Systick m_systick;
			MPU m_mpu;
		#endif
		
		#if defined(CORTEX_M3)
			SCB m_scb;
			NVIC m_nvic;
			Systick m_systick;
			SemaphoreExclusive m_semaphoreExclusive;
		#endif
		
		#if defined(CORTEX_M4)
			SCB m_scb;
			NVIC m_nvic;
			FPU m_fpu;
			Systick m_systick;
			MPU m_mpu;
			SemaphoreExclusive m_semaphoreExclusive;
		#endif
		
		#if defined(CORTEX_M7)
			SCB m_scb;
			NVIC m_nvic;
			FPU m_fpu;
			Systick m_systick;
			MPU m_mpu;
			//Cache m_cache;
			SemaphoreExclusive m_semaphoreExclusive;
		#endif
		
		
		//	OS
		static CMOS* c_this;
		Stack m_stack;
		Heap m_heap;
		Array<Thread> m_thread;
		const uint8 c_numberOfThreads;
		uint8 m_runningThreadID;
		uint8 m_nextThreadID;
		Time m_time;
		uint64 m_ticks;
		uint8 m_semaphore;
		uint32 m_semaphoreCounter;
		UniquePairArray<const void*, uint8> m_semaphoresUser;
		Array<UniqueArray<uint8>> m_events;
		
		
		//	Constructors - Destructors
		CMOS();
		CMOS(const CMOS& cmos) = delete;
		void constructors() const;
		
		
		//	Semaphore realised with SVC Interrupt, used for Access to Heap, Stack and Threads
		void lockSemaphore();
		feedback unlockSemaphore();
		
		void contextSwitch();
		static void thread_terminateOnReturn();
		feedback thread_terminate(uint8 thread_ID, bool hardShutdown);
		feedback send_mail(uint8 senderID, uint8 receiverID, Thread::e_mailType mailType, uint32 data);
		
		#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
			friend void EXCEPTION_MEMORY_MANAGEMENT_FAULT();
			friend void EXCEPTION_BUS_FAULT();
			friend void EXCEPTION_USAGE_FAULT();
		#endif
		friend void EXCEPTION_HARD_FAULT();
		friend void EXCEPTION_SYSTICK();
		friend void EXCEPTION_SVC();
		friend void EXCEPTION_PENDING_SV();
		
		friend uint32 __cmos_array_expand__(uint8** pointerToData, uint32 actualSizeInBytes, uint32 newSizeInBytes);
		friend void* operator new(unsigned int sizeInBytes);
		friend void operator delete(void* address);
		
		
		
		
		
	public:
		
		//	Getter for OS
		static CMOS& get();
		
		
		//	Startup Functions
		void run();
		
		
		//	System Time
		constexpr inline Time get_time() const;
		
		
		//	Reset
		void reset();
		
		
		//	Threading Functions
		uint8 thread_create(f_thread entry, String name, uint8 priority, uint32 stack_size = 0, uint8 mailBoxSize = Heap::get_blockSize() / sizeof(Thread::s_mail), void* object = nullptr);
		
		template<typename Class>
		inline uint8 thread_create(void (Class::*entryFunction)(), Class* object, String name, uint8 priority, uint16 stack_size = 0, uint8 mailBoxSize = Heap::get_blockSize() / sizeof(Thread::s_mail));
		
		void thread_setPriority(uint8 newPriority);
		feedback thread_joinChildThread(uint8 threadID, uint32 sleepTime_ms = 1);
		feedback thread_detachChildThread(uint8 threadID);
		feedback thread_shutdown(uint8 threadID, bool hardShutdown = false);
		constexpr inline bool thread_doesExist(const String& name) const;
		constexpr inline bool thread_doesExist(uint8 threadID) const;
		constexpr inline bool isChildThread(uint8 threadID) const;
		constexpr inline uint8 get_runningThreadID() const;
		constexpr inline uint8 get_parentThreadID() const;
		inline String thread_getName(uint8 threadID) const;
		inline uint8 thread_getID(const String& name) const;
		constexpr inline uint8 get_numberOfThreadsMaximum() const;
		constexpr inline bool isValid_threadID(uint8 threadID) const;
		static constexpr inline void invalidate(uint8& threadID);
		
		
		//	Sleep Functions
		inline void sleep();
		uint16 sleep_untilEvent(uint32 timeout_ms = 0);
		uint16 sleep_untilEvent(uint16 eventID, uint32 timeout_ms = 0);
		void sleep_untilMail(uint8 senderID);
		#if defined(CORTEX_M7)
			inline void sleep_100us(uint32 units_of_100us);
		#endif
		inline void sleep_ms(uint32 ms);
		inline void sleep_sec(uint32 seconds);
		
		
		//	Events
		uint16 event_create();
		feedback event_emit(uint16 eventID);
		feedback event_listen(uint16 eventID);
		feedback event_unlisten();
		feedback event_subscribe(uint16 eventID);
		feedback event_unsubscribe();
		feedback event_unsubscribe(uint16 eventID);
		
		
		//	Mailbox, only for use from outside an Interrupt or Exception
		inline feedback send_mail(uint8 thread_ID, uint32 data);
		inline bool is_mailAvailable() const;
		inline Thread::s_mail read_mail();
		
		
		//	User Semaphores
		bool semaphore_doesExist(const void* semaphore) const;
		uint8 semaphore_getOwner(const void* semaphore) const;
		inline bool semaphore_isOwnedByRunningThread(const void* semaphore) const;
		inline bool semaphore_isLocked(const void* semaphore) const;
		feedback semaphore_create(const void* semaphore);
		feedback semaphore_erase(const void* semaphore);
		feedback semaphore_lock(const void* semaphore);
		feedback semaphore_unlock(const void* semaphore);
		void semaphore_transferAllOwnershipsToParentThread();
		
		
		
		//	Load Measurements
		#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
			constexpr inline float thread_getLoadCpuPercent(uint8 thread_ID) const;
			constexpr inline float thread_getLoadStackPercent(uint8 thread_ID) const;
			
			constexpr inline float getLoadCpuPercent() const;
			constexpr inline float getLoadStackReservedAndUsedPercent() const;
			constexpr inline float getLoadStackReservedAndUnusedPercent() const;
			constexpr inline float getLoadStackPercent() const;
			constexpr inline float getLoadHeapPercent() const;
		#endif
		
		
		//	Inform System about Changes in the Clock System
		inline feedback set_systemClock(uint32 clock);
		
		
		// Tick Counter with one Tick equal to a Period of c_clock_systick
		constexpr inline volatile uint64& get_ticks();
		
		
		//	Getter for Hardware Modules
		#if defined(CORTEX_M0)
			constexpr inline NVIC& get_nvic();
		#endif
		
		#if defined(CORTEX_M0P)
			constexpr inline NVIC& get_nvic();
		#endif
		
		#if defined(CORTEX_M3)
			constexpr inline NVIC& get_nvic();
		#endif
		
		#if defined(CORTEX_M4)
			constexpr inline NVIC& get_nvic();
		#endif
		
		#if defined(CORTEX_M7)
			constexpr inline NVIC& get_nvic();
			//Cache& get_cache();
		#endif
		
		
		//	Get Processor ID
		inline uint32 get_id_core() const;
};



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

constexpr inline Time CMOS::get_time() const
{
	return(m_time);
}







template<typename Class>
inline uint8 CMOS::thread_create(void (Class::*entryFunction)(), Class* object, String name, uint8 priority, uint16 stack_size, uint8 mailBoxSize)
{
	union
	{
		void (Class::*entryFunction)();
		f_thread entryFunctionPure;
	}pointerConversion;
	
	pointerConversion.entryFunction = entryFunction;
	return(thread_create(pointerConversion.entryFunctionPure, name, priority, stack_size, mailBoxSize, object));
}


constexpr inline bool CMOS::thread_doesExist(const String& name) const
{
	for(uint8 i = 0; i < c_numberOfThreads; i++)
	{
		const Thread& thread(m_thread(i));
		if(thread.is_valid() == true)
		{
			if(thread.m_name == name)
			{
				return(true);
			}
		}
	}
	return(false);
}


constexpr inline bool CMOS::thread_doesExist(uint8 threadID) const
{
	if(isValid_threadID(threadID) == false)
	{
		return(false);
	}
	if(m_thread(threadID).is_valid() == false)
	{
		return(false);
	}
	return(true);
}


constexpr inline bool CMOS::isChildThread(uint8 threadID) const
{
	if(isValid_threadID(threadID) == false)
	{
		return(false);
	}
	const Thread& thread(m_thread(threadID));
	if(thread.is_valid() == false)
	{
		return(false);
	}
	if(thread.m_parentID != m_runningThreadID)
	{
		return(false);
	}
	return(true);
}


constexpr inline uint8 CMOS::get_runningThreadID() const
{
	return(m_runningThreadID);
}


constexpr inline uint8 CMOS::get_parentThreadID() const
{
	return(m_thread(m_runningThreadID).m_parentID);
}


inline String CMOS::thread_getName(uint8 threadID) const
{
	String name;
	if(isValid_threadID(threadID) == true)
	{
		name = m_thread(threadID).m_name;
	}
	return(name);
}


inline uint8 CMOS::thread_getID(const String& name) const
{
	for(uint8 i = 0; i < c_numberOfThreads; i++)
	{
		const Thread& thread(m_thread[i]);
		if(thread.is_valid() == true)
		{
			if(thread.m_name == name)
			{
				return(i);
			}
		}
	}
	return(threadID_invalid);
}


constexpr inline uint8 CMOS::get_numberOfThreadsMaximum() const
{
	return(c_numberOfThreads);
}


constexpr inline bool CMOS::isValid_threadID(uint8 threadID) const
{
	if(threadID < c_numberOfThreads)
	{
		return(true);
	}
	return(false);
}


constexpr inline void CMOS::invalidate(uint8& threadID)
{
	threadID = threadID_invalid;
}







inline void CMOS::sleep()
{
	//	Sleep until anything happens
	sleep_untilMail(m_runningThreadID);
}


#if defined(CORTEX_M7)
	inline void CMOS::sleep_100us(uint32 units_of_100us)
	{
		if(units_of_100us > 0)
		{
			if(units_of_100us > 100 * c_sleepTime_us_max)
			{
				units_of_100us = 100 * c_sleepTime_us_max;
			}
			m_thread[m_runningThreadID].m_sleepTime = (c_clock_systick / 10000) * units_of_100us;
			contextSwitch();
		}
	}
#endif


inline void CMOS::sleep_ms(uint32 ms)
{
	if(ms > c_sleepTime_ms_max)
	{
		ms = c_sleepTime_ms_max;
	}
	#if defined(CORTEX_M7)
		sleep_100us(10 * ms);
	#endif
	#if defined(CORTEX_M0) || defined(CORTEX_M0P) || defined(CORTEX_M3) || defined(CORTEX_M4)
		m_thread[m_runningThreadID].m_sleepTime = (c_clock_systick / 1000) * ms;
		contextSwitch();
	#endif
}


inline void CMOS::sleep_sec(uint32 seconds)
{
	if(seconds > c_sleepTime_sec_max)
	{
		seconds = c_sleepTime_sec_max;
	}
	#if defined(CORTEX_M7)
		sleep_100us(10000 * seconds);
	#endif
	#if defined(CORTEX_M0) || defined(CORTEX_M0P) || defined(CORTEX_M3) || defined(CORTEX_M4)
		sleep_ms(1000 * seconds);
	#endif
}







inline feedback CMOS::send_mail(uint8 thread_ID, uint32 data)
{
	return(send_mail(m_runningThreadID, thread_ID, Thread::e_mailType::NORMAL, data));
}


inline bool CMOS::is_mailAvailable() const
{
	if(m_thread(m_runningThreadID).m_mailBox.is_empty() == true)
	{
		return(false);
	}
	return(true);
}


inline Thread::s_mail CMOS::read_mail()
{
	return(m_thread[m_runningThreadID].m_mailBox.read());
}







inline bool CMOS::semaphore_isOwnedByRunningThread(const void* semaphore) const
{
	if(semaphore_getOwner(semaphore) == m_runningThreadID)
	{
		return(true);
	}
	return(false);
}


inline bool CMOS::semaphore_isLocked(const void* semaphore) const
{
	if(semaphore_getOwner(semaphore) == threadID_invalid)
	{
		return(false);
	}
	return(true);
}







#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
	constexpr inline float CMOS::thread_getLoadCpuPercent(uint8 thread_ID) const
	{
		if(isValid_threadID(thread_ID))
		{
			return(m_thread[thread_ID].m_loadCpu);
		}
		return(0);
	}
	
	
	constexpr inline float CMOS::thread_getLoadStackPercent(uint8 thread_ID) const
	{
		if(isValid_threadID(thread_ID))
		{
			return(m_thread[thread_ID].m_loadStack);
		}
		return(0);
	}
#endif







inline feedback CMOS::set_systemClock(uint32 clock)
{
	return(m_systick.init(Systick::e_clockSource::HCLK_DIV8, clock, c_clock_systick, true));
}


constexpr inline volatile uint64& CMOS::get_ticks()
{
	return(m_ticks);
}







constexpr inline NVIC& CMOS::get_nvic()
{
	return(m_nvic);
}

/*
constexpr inline Cache& CMOS::get_cache()
{
	return(m_cache);
}*/







inline uint32 CMOS::get_id_core() const
{
	return(m_scb.get_ID_core());
}


#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
	constexpr inline float CMOS::getLoadCpuPercent() const
	{
		return(m_loadCpuPercent);
	}
	
	
	constexpr inline float CMOS::getLoadStackReservedAndUsedPercent() const
	{
		return(m_loadStackReservedAndUsedPercent);
	}
	
	
	constexpr inline float CMOS::getLoadStackReservedAndUnusedPercent() const
	{
		return(m_loadStackReservedAndUnusedPercent);
	}
	
	
	constexpr inline float CMOS::getLoadStackPercent() const
	{
		return(m_loadStackPercent);
	}
	
	
	constexpr inline float CMOS::getLoadHeapPercent() const
	{
		return(m_loadHeapPercent);
	}
#endif