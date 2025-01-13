#include "../Inc/cmos.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/

extern uint32 __cmos_numberOfThreads__;

CMOS* CMOS::c_this = nullptr;



/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

CMOS::CMOS()
	:	
		#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
			m_semaphoreExclusive(m_runningThreadID),
		#endif
		m_heap							(),
		c_numberOfThreads		((uint32)		&__cmos_numberOfThreads__	),
		m_runningThreadID		(0),
		m_semaphore					(threadID_invalid),
		m_semaphoreCounter	(0),
		m_semaphoresUser		()
{
	if(c_this != nullptr)
	{
		return;
	}
	c_this = this;
	
	
	//	Init Thread Array
	m_thread.set_size(c_numberOfThreads);
	
	
	//	Init Parent IDs
	for(auto& i: m_thread)
	{
		i.m_parentID = threadID_invalid;
	}
	
	
	//	Create Semaphore for the Event ID Array
	semaphore_create(&m_events);
	
	
	//	Init Hardware Modules
	#if defined(CORTEX_M0) || defined(CORTEX_M0P) || defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
		m_scb.init();
		m_nvic.init();
	#endif
	#if defined(CORTEX_M0P) || defined(CORTEX_M4) || defined(CORTEX_M7)
		m_mpu.init();
	#endif
	#if defined(CORTEX_M7)
		//m_cache.startup();
	#endif
}


void CMOS::constructors() const
{
	typedef void (*f_void)();
	
	extern uint32 __cmos_constructor_start__;
	extern uint32 __cmos_constructor_size__;
	
	const uint32* start				= (uint32*)	&__cmos_constructor_start__;
	const uint32  sizeInBytes	= (uint32)	&__cmos_constructor_size__;
	
	for(uint32 i = 0; i < sizeInBytes / 4; i++)
	{
		const uint32 jumpAddress = *(start + i);
		if(jumpAddress != 0 && jumpAddress != 0xFFFFFFFF)
		{
			f_void constructor = (f_void) jumpAddress;
			constructor();
		}
	}
}


CODE_RAM void CMOS::lockSemaphore()
{
	//	If Semaphore is already locked by this Thread, increase Counter and return
	if(m_semaphore == m_runningThreadID)
	{
		m_semaphoreCounter++;
		return;
	}
	
	
	//	Try to lock Semaphore
	while(m_semaphore != m_runningThreadID)
	{
		if(m_semaphore == threadID_invalid)
		{
			//	Lock Semaphore if it is unlocked
			asm volatile("svc #0x0");
		}
		else
		{
			//	Sleep if Semaphore is locked by another Thread
			#if defined(CORTEX_M7)
				sleep_100us(1);
			#endif
			#if defined(CORTEX_M0) || defined(CORTEX_M0P) || defined(CORTEX_M3) || defined(CORTEX_M4)
				sleep_ms(1);
			#endif
		}
	}
}


CODE_RAM feedback CMOS::unlockSemaphore()
{
	//	Check if Semaphore is locked by this Thread - this is the only Thread that is allowed to unlock the Semaphore
	if(m_semaphore != m_runningThreadID)
	{
		return(FAIL);
	}
	
	
	if(m_semaphoreCounter != 0)
	{
		//	Decrease Counter if it is not 0 - that means the Semaphore is locked multiple times by this Thread
		m_semaphoreCounter--;
	}
	else
	{
		//	Unlock Semaphore
		m_semaphore = threadID_invalid;
	}
	return(OK);
}







CODE_RAM void CMOS::contextSwitch()
{
	//	Check if this Functions is already executing
	if(m_runningThreadID == threadID_invalid)
	{
		return;
	}
	
	
	//	Take local Copy
	uint8 runningThreadID = m_runningThreadID;
	
	
	//	Invalidate m_runningThreadID, so this Function is only entered once at a time and for Statistic Purposes in EXCEPTION_SYSTICK
	m_runningThreadID = threadID_invalid;
	
	
	//	Search until newThread is found
	uint8 newThreadID = threadID_invalid;
	while(newThreadID == threadID_invalid)
	{
		//	Search for the Thread with the maximium Priority that isnt sleeping or waiting for an Event or Mail
		uint8 maximumPriority = 0;
		for(uint8 i = 0; i < c_numberOfThreads; i++)
		{
			Thread& thread(m_thread[i]);
			if(thread.is_valid() == true)
			{
				//	Thread is qualified only if he is not sleeping
				if(thread.m_sleepTime == 0)
				{
					//	Thread is qualified only if its not waiting for an Event
					if(thread.m_wakeUpEventID == eventID_invalid)
					{
						//	Thread is qualified only if he is not waiting for a Mail
						if(thread.m_wakeUpMailSender == threadID_invalid)
						{
							const uint8 priority = thread.m_priority;
							if(priority > maximumPriority)
							{
								maximumPriority = priority;
								newThreadID = i;
							}
						}
					}
				}
			}
		}
		
		
		if(newThreadID == threadID_invalid)
		{
			//	No newThread has been found
			if(m_scb.get_activeInterruptVector() != 0)
			{
				//	If this Function is called from an Interrupt, avoid endless while-loop
				newThreadID = runningThreadID;
				break;
			}
			
			
			//	Go sleep if no Thread has been found and wait for an Interrupt (EXCEPTION_SYSTICK for sleep-Time Functions) to wake a Thread up
			asm volatile("wfe");
		}
	}
	m_runningThreadID = runningThreadID;
	m_nextThreadID = newThreadID;
	if(m_runningThreadID != m_nextThreadID)
	{
		m_scb.setPending(Exception::PENDING_SV);
	}
}


void CMOS::thread_terminateOnReturn()
{
	CMOS& cmos = CMOS::get();
	cmos.thread_terminate(cmos.m_runningThreadID, true);
	cmos.contextSwitch();
}


feedback CMOS::thread_terminate(uint8 thread_ID, bool hardShutdown)
{
	if(isValid_threadID(thread_ID) == false)
	{
		return(FAIL);
	}
	Thread& thread(m_thread[thread_ID]);
	if(thread.is_valid() == false)
	{
		return(FAIL);
	}
	
	
	//	Soft Shutdown
	if(thread_ID != m_runningThreadID && hardShutdown == false)
	{
		send_mail(m_runningThreadID, thread_ID, Thread::e_mailType::EXIT, 0);
		return(OK);
	}
	
	
	//	Terminate Child-Threads
	for(uint32 i = 0; i < c_numberOfThreads; i++)
	{
		if(m_thread[i].is_valid() == true)
		{
			if(m_thread[i].m_parentID == thread_ID)
			{
				if(thread_terminate(i, hardShutdown) != OK)
				{
					return(FAIL);
				}
			}
		}
	}
	
	
	
	
	
	//	Free Resources
	
	//	Free all Semaphores locked by this Thread
	for(auto& i: m_semaphoresUser)
	{
		if(i.second() == thread_ID)
		{
			i.second() = threadID_invalid;
		}
	}
	
	
	//	Delete Thread Entry
	lockSemaphore();
	{
		thread.m_name.~String();
		thread.m_sleepTime						= 0;
		thread.m_parentID							= 0;
		thread.m_mailBox.clear();
		
		m_stack.free(thread_ID);
		
		
		
		//	Delete Statistics
		#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
			thread.m_ticks			= 0;
			thread.m_cpuLoad		= 0;
			thread.m_stackLoad	= 0;
		#endif
	}
	unlockSemaphore();
	
	//	Reset remaining Informations that have been needed for correct Execution until now:
	//	Stack Highest Address and Stackpointer are needed for the thread.is_valid() to return true and continue Execution
	//	else contextswitch() doesnt schedule this Thread anymore
	//	Stack Size is needed for Systick else Systick resets because of Stack Corruption
	//	Priority and WakeUpSender are needed for contextswitch() scheduling
	m_nvic.disable_configurablePriorityExceptions();
	thread.m_stackHighestAddress		= nullptr;
	thread.m_stackPointer						= nullptr;
	thread.m_stackSize							= 0;
	thread.m_priority								= 0;
	thread.m_wakeUpEventID					= eventID_invalid;
	thread.m_triggeredWakeUpEventID	= eventID_invalid;
	thread.m_listeningEventID				= eventID_invalid;
	thread.m_listeningEventOccured	= false;
	thread.m_wakeUpMailSender 			= 0;
	m_nvic.enable_configurablePriorityExceptions();
	
	return(OK);
}


CODE_RAM feedback CMOS::send_mail(uint8 senderID, uint8 receiverID, Thread::e_mailType mailType, uint32 data)
{
	if(isValid_threadID(receiverID) == false)
	{
		return(FAIL);
	}
	Thread& thread = m_thread[receiverID];
	if(thread.is_valid() == false)
	{
		return(FAIL);
	}
	
	
	//	Create Message
	Thread::s_mail mail;
	mail.data = data;
	mail.type = mailType;
	mail.senderID = senderID;
	
	
	//	Write Message to Message Buffer of Thread
	lockSemaphore();
	feedback result = thread.m_mailBox.write(mail);
	unlockSemaphore();
	
	
	//	Check if Thread is waiting for sending Thread or waiting for anything to happen (= waiting for itself)
	if(thread.m_wakeUpMailSender == senderID || thread.m_wakeUpMailSender == receiverID)
	{
		thread.m_wakeUpMailSender = threadID_invalid;
		contextSwitch();
	}
	
	return(result);
}



/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

CMOS& CMOS::get()
{
	static CMOS cmos;
	return(cmos);
}







void CMOS::run()
{
	//	Create Semaphore to avoid multiple Calls to this Function
	if(semaphore_create(this) != OK)
	{
		return;
	}
	
	
	// Initialize Hardware
	m_scb.setPriority(Exception::SYSTICK, 0);
	m_scb.setPriority(Exception::SVC, 1);
	m_scb.setPriority(Exception::PENDING_SV, NVIC::c_priority_max);
	
	
	//	Clocks
	extern uint32 __startup__();
	uint32 clockCore = __startup__();
	if(clockCore == 0)
	{
		reset();
	}
	if(set_systemClock(clockCore) != OK)
	{
		reset();
	}
	
	
	//	Main Thread
	extern uint32 __cmos_mainStackSize__;
	const uint32 mainStackSize = (uint32) &__cmos_mainStackSize__;
	
	union
	{
		int (*entryFunction)();
		f_thread entryFunctionPure;
	}pointerConversion;
	
	extern int main();
	pointerConversion.entryFunction = main;
	uint8 thread_ID = thread_create(pointerConversion.entryFunctionPure, "Main", 255, mainStackSize);
	
	if(isValid_threadID(thread_ID) == false)
	{
		reset();
	}
	constructors();
	
	
	//	Switch to main
	asm volatile
	(
		"msr msp, %[in_a]"
			:
			:	[in_a] "r" (m_thread[thread_ID].m_stackHighestAddress)
			:
	);
	main();
	reset();
}







void CMOS::reset()
{
	m_scb.reset();
}







uint8 CMOS::thread_create(f_thread entry, String name, uint8 priority, uint16 stack_size, uint8 mailBoxSize, void* object)
{
	if(entry == nullptr || name.get_size() == 0 || mailBoxSize == 0)
	{
		return(threadID_invalid);
	}
	
	lockSemaphore();
	uint8 thread_ID = threadID_invalid;
	for(uint32 i = 0; i < c_numberOfThreads; i++)
	{
		if(m_thread[i].is_valid() == false)
		{
			thread_ID = i;
			break;
		}
	}
	if(thread_ID == threadID_invalid)
	{
		unlockSemaphore();
		return(threadID_invalid);
	}
	
	
	
	
	
	stack_size = m_stack.alignToBlocksize(stack_size);
	uint32* stackHighestAddress = m_stack.reserve(thread_ID, stack_size);
	if(stackHighestAddress == nullptr)
	{
		unlockSemaphore();
		return(threadID_invalid);
	}
	
	
	
	Thread& thread(m_thread[thread_ID]);
	
	thread.m_name										= name;
	thread.m_stackSize							= stack_size;
	thread.m_stackHighestAddress		= stackHighestAddress;
	
	#if defined(CORTEX_M4) || defined(CORTEX_M7)
		thread.m_stackPointer					= stackHighestAddress - 0x32;
	#endif
	
	#if defined(CORTEX_M0) || defined(CORTEX_M0P) || defined(CORTEX_M3)
		thread.m_stackPointer					= stackHighestAddress - 0x10;
	#endif
	
	thread.m_priority								=	priority;
	thread.m_sleepTime							= 0;
	thread.m_wakeUpEventID					= eventID_invalid;
	thread.m_triggeredWakeUpEventID	= eventID_invalid;
	thread.m_listeningEventID				= eventID_invalid;
	thread.m_listeningEventOccured	= false;
	thread.m_wakeUpMailSender				= threadID_invalid;
	thread.m_parentID								= m_runningThreadID;
	
	#if defined(CORTEX_M4) || defined(CORTEX_M7)
		*(thread.m_stackHighestAddress - 0x13) = 0x01000000;
		*(thread.m_stackHighestAddress - 0x14) = (uint32) entry;
		*(thread.m_stackHighestAddress - 0x15) = (uint32) thread_terminateOnReturn;
		*(thread.m_stackHighestAddress - 0x1A) = (uint32) object;
	#endif
	
	#if defined(CORTEX_M0) || defined(CORTEX_M0P) || defined(CORTEX_M3)
		*(thread.m_stackHighestAddress - 0x01) = 0x01000000;
		*(thread.m_stackHighestAddress - 0x02) = (uint32) entry;
		*(thread.m_stackHighestAddress - 0x03) = (uint32) thread_terminateOnReturn;
		*(thread.m_stackHighestAddress - 0x08) = (uint32) object;
	#endif
	
	unlockSemaphore();
	
	
	if(thread.m_mailBox.set_size(mailBoxSize) != OK)
	{
		return(threadID_invalid);
	}
	
	
	contextSwitch();
	return(thread_ID);
}


void CMOS::thread_setPriority(uint8 newPriority)
{
	m_thread[m_runningThreadID].m_priority = newPriority;
	contextSwitch();
}


feedback CMOS::thread_joinChildThread(uint8 threadID, uint32 sleepTime_ms)
{
	if(isValid_threadID(threadID) == false)
	{
		return(FAIL);
	}
	if(isChildThread(threadID) == false)
	{
		return(FAIL);
	}
	Thread& childThread(m_thread[threadID]);
	while(childThread.is_valid() == true && isChildThread(threadID) == true)
	{
		sleep_ms(sleepTime_ms);
	}
	return(OK);
}


feedback CMOS::thread_detachChildThread(uint8 threadID)
{
	if(isValid_threadID(threadID) == false)
	{
		return(FAIL);
	}
	if(isChildThread(threadID) == false)
	{
		return(FAIL);
	}
	m_thread[threadID].m_parentID = threadID_invalid;
	return(OK);
}


feedback CMOS::thread_shutdown(uint8 threadID, bool hardShutdown)
{
	//	Check if Thread is valid
	if(isValid_threadID(threadID) == false)
	{
		return(FAIL);
	}
	
	
	//	Check if we are allowed to shutdown this Thread
	//	- Only if we are the Parent Thread
	//	- Or if the targeted Threads is detached = has no Parent Thread anymore = threadID_invalid
	if(isChildThread(threadID) == false && m_thread[threadID].m_parentID != threadID_invalid)
	{
		return(FAIL);
	}
	return(thread_terminate(threadID, hardShutdown));
}







uint16 CMOS::sleep_untilEvent(uint32 timeout_ms)
{
	//	Sleep until any subscripted Event occurs
	Thread& thread = m_thread[m_runningThreadID];
	if(thread.m_listeningEventOccured == true)
	{
		//	Listened Event already occured -> dont go sleep
		if(thread.m_triggeredWakeUpEventID >= m_events.get_size())
		{
			thread.m_triggeredWakeUpEventID = thread.m_listeningEventID;
		}
	}
	else
	{
		//	Reset Triggered Event ID, so that eventID_invalid is returned if no Event occurs and the Thread gets woken up by the Systick because of the Timeout
		thread.m_triggeredWakeUpEventID = eventID_invalid;
		
		
		//	Set Timeout for Sleep - Systick Interrupt will wake-up Thread if Event does not occur
		if(timeout_ms > c_sleepTime_ms_max)
		{
			timeout_ms = c_sleepTime_ms_max;
		}
		m_thread[m_runningThreadID].m_sleepTime = (c_clock_systick / 1000) * timeout_ms;
		
		
		//	Go sleep until any Event occurs
		thread.m_wakeUpEventID = eventID_any;
		contextSwitch();
	}
	
	
	//	Reset Event Wakeup Parameters
	thread.m_listeningEventOccured = false;
	thread.m_wakeUpEventID = eventID_invalid;
	
	
	//	Return the Event ID that woke us up
	return(thread.m_triggeredWakeUpEventID);
}


uint16 CMOS::sleep_untilEvent(uint16 eventID, uint32 timeout_ms)
{
	if(eventID < m_events.get_size())
	{
		Thread& thread = m_thread[m_runningThreadID];
		
		
		//	Reset Triggered Event ID, so that eventID_invalid is returned if no Event occurs and the Thread gets woken up by the Systick because of the Timeout
		thread.m_triggeredWakeUpEventID = eventID_invalid;
		
		
		//	Check if Event already occured by checking the Listening Event ID
		if(thread.m_listeningEventID == eventID && thread.m_listeningEventOccured == true)
		{
			//	Listened Event already occured -> dont go sleep
			thread.m_triggeredWakeUpEventID = eventID;
			thread.m_listeningEventOccured = false;
			return(eventID);
		}
		
		
		//	Set Event ID to be woke up by
		thread.m_wakeUpEventID = eventID;
		
		
		//	Set Timeout for Sleep - Systick Interrupt will wake-up Thread if Event does not occur
		if(timeout_ms > c_sleepTime_ms_max)
		{
			timeout_ms = c_sleepTime_ms_max;
		}
		m_thread[m_runningThreadID].m_sleepTime = (c_clock_systick / 1000) * timeout_ms;
		
		
		//	Go sleep until Event occurs
		contextSwitch();
		
		
		//	Reset Event Wakeup Parameters
		thread.m_listeningEventOccured = false;
		thread.m_wakeUpEventID = eventID_invalid;
		return(thread.m_triggeredWakeUpEventID);
	}
	return(eventID_invalid);
}


void CMOS::sleep_untilMail(uint8 senderID)
{
	if(senderID == threadID_invalid)
	{
		return;
	}
	m_thread[m_runningThreadID].m_wakeUpMailSender = senderID;
	contextSwitch();
}







uint16 CMOS::event_create()
{
	//	Lock Semaphore
	if(semaphore_lock(&m_events) != OK)
	{
		return(eventID_invalid);
	}
	
	
	//	Create a new Event
	m_events += UniqueArray<uint8>();
	const uint16 eventID = m_events.get_size() - 1;
	
	
	//	Unlock Semaphore
	semaphore_unlock(&m_events);
	
	
	return(eventID);
}


feedback CMOS::event_emit(uint16 eventID)
{
	if(eventID < m_events.get_size())
	{
		bool contextSwitchNecessary = false;
		
		const UniqueArray<uint8>& subscriber = m_events[eventID];
		for(auto& i: subscriber)
		{
			if(isValid_threadID(i) == true)
			{
				Thread& thread = m_thread[i];
				if(thread.is_valid() == true)
				{
					//	Check if Thread waits for any subscripted Event or for the occured Event
					if(thread.m_wakeUpEventID == eventID_any || thread.m_wakeUpEventID == eventID)
					{
						//	Reset Timeout Sleep Value of this Thread
						thread.m_sleepTime = 0;
						
						
						//	Reset Event Wakeup Parameters
						thread.m_wakeUpEventID = eventID_invalid;
						thread.m_triggeredWakeUpEventID = eventID;
						
						
						//	Wake up Thread
						contextSwitchNecessary = true;
					}
					
					
					//	Check if Thread listens to this Event
					if(thread.m_listeningEventID == eventID)
					{
						//	Reset Timeout Sleep Value of this Thread
						thread.m_sleepTime = 0;
						
						
						//	Reset Event Listening Parameters
						thread.m_listeningEventOccured = true;
						
						
						//	Wake up Thread
						contextSwitchNecessary = true;
					}
					
					
					//	Check if Thread is waiting for anything to happen (= waiting for itself)
					if(thread.m_wakeUpMailSender == i)
					{
						thread.m_wakeUpMailSender = threadID_invalid;
						contextSwitchNecessary = true;
					}
				}
			}
		}
		
		
		if(contextSwitchNecessary == true)
		{
			contextSwitch();
		}
		return(OK);
	}
	return(FAIL);
}


feedback CMOS::event_listen(uint16 eventID)
{
	if(eventID < m_events.get_size())
	{
		Thread& thread = m_thread[m_runningThreadID];
		
		thread.m_triggeredWakeUpEventID = eventID_invalid;
		thread.m_listeningEventID = eventID;
		thread.m_listeningEventOccured = false;
		return(OK);
	}
	return(FAIL);
}


feedback CMOS::event_unlisten()
{
	Thread& thread = m_thread[m_runningThreadID];
	thread.m_listeningEventID = eventID_invalid;
	thread.m_listeningEventOccured = false;
	thread.m_triggeredWakeUpEventID = eventID_invalid;
	
	return(OK);
}


feedback CMOS::event_subscribe(uint16 eventID)
{
	//	Lock Semaphore
	if(semaphore_lock(&m_events) != OK)
	{
		return(FAIL);
	}
	
	
	if(eventID >= m_events.get_size())
	{
		semaphore_unlock(&m_events);
		return(FAIL);
	}
	
	
	//	Subscribe
	m_events[eventID] += m_runningThreadID;
	
	
	//	Unlock Semaphore
	semaphore_unlock(&m_events);
	
	
	return(OK);
}


feedback CMOS::event_unsubscribe()
{
	//	Lock Semaphore
	if(semaphore_lock(&m_events) != OK)
	{
		return(FAIL);
	}
	
	
	//	Unsubscribe from all Events
	for(auto& i: m_events)
	{
		i.erase(m_runningThreadID);
	}
	
	
	//	Unlock Semaphore
	semaphore_unlock(&m_events);
	
	
	return(OK);
}


feedback CMOS::event_unsubscribe(uint16 eventID)
{
	//	Lock Semaphore
	if(semaphore_lock(&m_events) != OK)
	{
		return(FAIL);
	}
	
	
	if(eventID >= m_events.get_size())
	{
		semaphore_unlock(&m_events);
		return(FAIL);
	}
	
	
	//	Unsubscribe
	m_events[eventID].erase(m_runningThreadID);
	
	
	//	Unlock Semaphore
	semaphore_unlock(&m_events);
	
	
	return(OK);
}







CODE_RAM bool CMOS::semaphore_doesExist(const void* semaphore) const
{
	return(m_semaphoresUser.contains(semaphore));
}


CODE_RAM uint8 CMOS::semaphore_getOwner(const void* semaphore) const
{
	//	Check if Semaphore even exists
	if(semaphore_doesExist(semaphore) == false)
	{
		return(threadID_invalid);
	}
	
	
	//	If it exists, check if it is occupied
	return(m_semaphoresUser[semaphore]);
}


CODE_RAM feedback CMOS::semaphore_create(const void* semaphore)
{
	//	Check if Semaphore already exists
	if(m_semaphoresUser.contains(semaphore) == true)
	{
		return(FAIL);
	}
	
	
	//	Create new Semaphore
	Pair newSemaphore(semaphore, threadID_invalid);
	m_semaphoresUser += newSemaphore;
	return(OK);
}


CODE_RAM feedback CMOS::semaphore_erase(const void* semaphore)
{
	//	Check if Semaphore is owned by the Thread that is trying to erase it
	if(semaphore_isOwnedByRunningThread(semaphore) == false)
	{
		return(FAIL);
	}
	
	
	//	Erase Semaphore
	return(m_semaphoresUser.erase(semaphore));
}


CODE_RAM feedback CMOS::semaphore_lock(const void* semaphore)
{
	//	Check if Semaphore exists
	if(semaphore_doesExist(semaphore) == false)
	{
		return(FAIL);
	}
	
	
	//	Check if Semaphore is already locked by this Thread
	if(semaphore_isOwnedByRunningThread(semaphore) == true)
	{
		return(OK);
	}
	
	
	//	Lock User Semaphore
	while(1)
	{
		//	Wait until User Semaphore is unlocked
		if(m_semaphoresUser[semaphore] == threadID_invalid)
		{
			//	Lock global Semaphore
			lockSemaphore();
			
			
			//	Check again, just in Case some Thread slipped in before the global Semaphore Lock
			bool locked = false;
			uint8& semaphoreValue = m_semaphoresUser[semaphore];
			if(semaphoreValue == threadID_invalid)
			{
				//	Lock User Semaphore
				semaphoreValue = m_runningThreadID;
				locked = true;
			}
			
			
			//	Unlock global Semaphore
			unlockSemaphore();
			
			
			//	Return OK, if User Semaphore has been locked successfully
			if(locked == true)
			{
				return(OK);
			}
		}
		
		
		//	Sleep
		#if defined(CORTEX_M7)
			sleep_100us(1);
		#endif
		
		#if defined(CORTEX_M0) || defined(CORTEX_M0P) || defined(CORTEX_M3) || defined(CORTEX_M4)
			sleep_ms(1);
		#endif
	}
	return(FAIL);
}


CODE_RAM feedback CMOS::semaphore_unlock(const void* semaphore)
{
	//	Check if Semaphore exists
	if(semaphore_doesExist(semaphore) == false)
	{
		return(FAIL);
	}
	
	
	//	Check if User Semaphore is occupied by actual Thread
	if(semaphore_isOwnedByRunningThread(semaphore) == false)
	{
		return(FAIL);
	}
	
	
	//	Unlock User Semaphore
	m_semaphoresUser[semaphore] = threadID_invalid;
	return(OK);
}


CODE_RAM void CMOS::semaphore_transferAllOwnershipsToParentThread()
{
	const uint8 parentThreadID = m_thread[m_runningThreadID].m_parentID;
	const uint8 runningThreadID = m_runningThreadID;
	for(auto& i: m_semaphoresUser)
	{
		uint8& threadID = i.second();
		if(threadID == runningThreadID)
		{
			threadID = parentThreadID;
		}
	}
}









#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
	void EXCEPTION_MEMORY_MANAGEMENT_FAULT()
	{
		CMOS::c_this->reset();
	}
	
	
	void EXCEPTION_BUS_FAULT()
	{
		CMOS::c_this->reset();
	}
	
	
	void EXCEPTION_USAGE_FAULT()
	{
		CMOS::c_this->reset();
	}
#endif


void EXCEPTION_HARD_FAULT()
{
	CMOS::c_this->reset();
}


CODE_RAM void EXCEPTION_SYSTICK()
{
	#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
		static uint32 idleTicks;
	#endif
	static uint32 systemTimeCounter;
	CMOS& cmos = *CMOS::c_this;
	
	//	CMOS Time, CPU Load, Stack Load, Thread Sleep Wake up and Heap Load
	cmos.m_ticks = (cmos.m_ticks + 1) & 0x7FFFFFFF;
	systemTimeCounter++;
	if(systemTimeCounter >= CMOS::c_clock_systick)
	{
		//	CMOS Time
		cmos.m_time++;
		systemTimeCounter = 0;
		
		
		#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
			//	CPU Load
			float sum = 0;
			for(uint8 i = 0; i < cmos.c_numberOfThreads; i++)
			{
				Thread& thread(cmos.m_thread[i]);
				if(thread.is_valid() == true)
				{
					sum += cmos.m_thread[i].m_ticks;
				}
			}
			sum += idleTicks;
			for(uint8 i = 0; i < cmos.c_numberOfThreads; i++)
			{
				Thread& thread(cmos.m_thread[i]);
				if(thread.is_valid() == true)
				{
					thread.m_cpuLoad = 100 * ((float) thread.m_ticks) / sum;
					thread.m_ticks = 0;
				}
			}
			cmos.m_load_cpu = 100 * (sum - idleTicks) / sum;
			idleTicks = 0;
			
			
			
			//	Stack Load
			uint32 reserved = 0;
			uint32 reservedAndUsed = 0;
			uint32 reservedAndUnused = 0;
			for(uint32 i = 0; i < cmos.c_numberOfThreads; i++)
			{
				Thread& thread(cmos.m_thread[i]);
				if(thread.is_valid() == true)
				{
					uint32 stackUsed = 4 * (thread.m_stackHighestAddress - thread.m_stackPointer);
					uint32 stackUnused = thread.m_stackSize - stackUsed;
					
					thread.m_stackLoad = 100 * (((float) stackUsed) / thread.m_stackSize);
					
					reserved += thread.m_stackSize;
					reservedAndUsed += stackUsed;
					reservedAndUnused += stackUnused;
				}
			}
			
			cmos.m_load_stack = 100 * ((float) reserved) / cmos.m_stack.get_sizeInBytes();
			cmos.m_load_stack_reservedAndUsed = 100 * ((float) reservedAndUsed) / reserved;
			cmos.m_load_stack_reservedAndUnused = 100 * ((float) reservedAndUnused) / reserved;
			
			
			
			cmos.m_load_heap = 100 * ((float) cmos.m_heap.get_sizeReservedInBytes()) / cmos.m_heap.get_sizeTotalInBytes();
		#endif
	}
	
	
	
	
	//	Check actual Stackpointer
	uint32 stackPointer = 0;
	asm volatile
	(
		"mrs %[out_a], msp"
			: [out_a] "=r" (stackPointer)
			:
			:
	);
	
	
	for(uint8 i = 0; i < cmos.c_numberOfThreads; i++)
	{
		Thread& thread(cmos.m_thread[i]);
		
		
		//	Check Stack Integrity
		if(thread.is_valid() == true)
		{
			//	Check Value at Stack Lower End
			uint32* start = thread.m_stackHighestAddress - (thread.m_stackSize / 4);
			if(*start != Stack::c_checkValue)
			{
				cmos.reset();
			}
		}
		
		
		//	Thread Sleep Management
		uint32 temp = thread.m_sleepTime;
		if(temp > 0)
		{
			temp--;
			thread.m_sleepTime = temp;
			if(temp == 0)
			{
				thread.m_wakeUpEventID = cmos.eventID_invalid;
				cmos.contextSwitch();
			}
		}
		
		
		//	Wakeup Thread if listened Event occured and this Thread waits for it
		if(thread.m_listeningEventOccured == true)
		{
			//	Only wakeup Thread if its sleeping already and waiting for this Event
			if(thread.m_wakeUpEventID == thread.m_listeningEventID || thread.m_wakeUpEventID == CMOS::eventID_any)
			{
				thread.m_wakeUpEventID = CMOS::eventID_invalid;
				cmos.contextSwitch();
			}
		}
	}
	
	
	//	Thread Tick Counter for CPU Load Measurement
	#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
		if(cmos.m_runningThreadID == CMOS::threadID_invalid)
		{
			idleTicks++;
		}
		else
		{
			cmos.m_thread[cmos.m_runningThreadID].m_ticks++;
		}
	#endif
}


CODE_RAM void EXCEPTION_SVC()
{
	CMOS& cmos = *CMOS::c_this;
	
	if(cmos.m_semaphore == CMOS::threadID_invalid)
	{
		cmos.m_semaphore = cmos.m_runningThreadID;
	}
}


CODE_RAM NAKED void EXCEPTION_PENDING_SV()
{
	asm volatile("push {r4}");
	asm volatile("push {r5}");
	asm volatile("push {r6}");
	asm volatile("push {r7}");
	
	#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
		asm volatile("push {r8}");
		asm volatile("push {r9}");
		asm volatile("push {r10}");
		asm volatile("push {r11}");
	#endif
	
	#if defined(CORTEX_M0) || defined(CORTEX_M0P)
		asm volatile("mov r7, r8");
		asm volatile("push {r7}");
		asm volatile("mov r7, r9");
		asm volatile("push {r7}");
		asm volatile("mov r7, r10");
		asm volatile("push {r7}");
		asm volatile("mov r7, r11");
		asm volatile("push {r7}");
	#endif
	
	#if	defined(CORTEX_M4) || defined(CORTEX_M7)
		asm volatile("vpush {s16}");
		asm volatile("vpush {s17}");
		asm volatile("vpush {s18}");
		asm volatile("vpush {s19}");
		asm volatile("vpush {s20}");
		asm volatile("vpush {s21}");
		asm volatile("vpush {s22}");
		asm volatile("vpush {s23}");
		asm volatile("vpush {s24}");
		asm volatile("vpush {s25}");
		asm volatile("vpush {s26}");
		asm volatile("vpush {s27}");
		asm volatile("vpush {s28}");
		asm volatile("vpush {s29}");
		asm volatile("vpush {s30}");
		asm volatile("vpush {s31}");
	#endif
	
	
	
	//  Store old Stackpointer
	CMOS& cmos = *CMOS::c_this;
	asm volatile
	(
		"mrs %[out_a], msp"
			: [out_a] "=r" (cmos.m_thread[cmos.m_runningThreadID].m_stackPointer)
			:
			:
	);
	cmos.m_runningThreadID = cmos.m_nextThreadID;
	
	
	//	Load Stack Pointer
	asm volatile
	(
		"msr msp, %[in_a]"
			:
			:	[in_a] "r" (cmos.m_thread[cmos.m_runningThreadID].m_stackPointer)
			:
	);
	
	#if defined(CORTEX_M4) || defined(CORTEX_M7)
		asm volatile("vpop {s31}");
		asm volatile("vpop {s30}");
		asm volatile("vpop {s29}");
		asm volatile("vpop {s28}");
		asm volatile("vpop {s27}");
		asm volatile("vpop {s26}");
		asm volatile("vpop {s25}");
		asm volatile("vpop {s24}");
		asm volatile("vpop {s23}");
		asm volatile("vpop {s22}");
		asm volatile("vpop {s21}");
		asm volatile("vpop {s20}");
		asm volatile("vpop {s19}");
		asm volatile("vpop {s18}");
		asm volatile("vpop {s17}");
		asm volatile("vpop {s16}");
	#endif
	
	#if defined(CORTEX_M0) || defined(CORTEX_M0P)
		asm volatile("pop {r7}");
		asm volatile("mov r11, r7");
		asm volatile("pop {r7}");
		asm volatile("mov r10, r7");
		asm volatile("pop {r7}");
		asm volatile("mov r9, r7");
		asm volatile("pop {r7}");
		asm volatile("mov r8, r7");
	#endif
	
	#if defined(CORTEX_M3) || defined(CORTEX_M4) || defined(CORTEX_M7)
		asm volatile("pop {r11}");
		asm volatile("pop {r10}");
		asm volatile("pop {r9}");
		asm volatile("pop {r8}");
	#endif
	
	asm volatile("pop {r7}");
	asm volatile("pop {r6}");
	asm volatile("pop {r5}");
	asm volatile("pop {r4}");
	
	asm volatile("bx lr");
}









CODE_RAM void* operator new(unsigned int sizeInBytes)
{
	CMOS& cmos = *CMOS::c_this;
	
	uint32 sizeInBytes_aligned = cmos.m_heap.alignToBlocksize(sizeInBytes);
	if(sizeInBytes_aligned > cmos.m_heap.get_sizeTotalInBytes())
	{
		void* returnValue = nullptr;
		return(returnValue);
	}
	
	cmos.lockSemaphore();
	void* data = cmos.m_heap.reserve(cmos.m_runningThreadID, sizeInBytes_aligned);
	cmos.unlockSemaphore();
	
	if(data != nullptr)
	{
		uint32* dataTemp = (uint32*) data;
		for(uint32 i = 0; i < sizeInBytes_aligned / 4; i++)
		{
			dataTemp[i] = 0;
		}
	}
	
	return(data);
}


CODE_RAM void operator delete(void* address)
{
	CMOS& cmos = *CMOS::c_this;
	
	void* addressAligned = cmos.m_heap.alignToBlock(address);
	if(addressAligned == nullptr)
	{
		return;
	}
	
	
	
	cmos.lockSemaphore();
	uint32 sizeInBytes = cmos.m_heap.free(addressAligned);
	cmos.unlockSemaphore();
	
	
	
	if(addressAligned != nullptr)
	{
		uint32* dataTemp = (uint32*) addressAligned;
		for(uint32 i = 0; i < sizeInBytes / 4; i++)
		{
			dataTemp[i] = 0;
		}
	}
}



extern "C"
{
	int _kill(int, int)
	{
		return(-1);
	}
	
	
	#include <reent.h>
	int _kill_r(struct _reent*, int, int)
	{
		return(-1);
	}
	
	
	
	
	
	int _getpid()
	{
		return(1);
	}
	
	
	int _getpid_r(struct _reent*)
	{
		return(1);
	}
	
	
	
	
	
	void _exit(int)
	{
		while(1)
		{
			
		}
	}
	
	
	int atexit(void (*)())
	{
		return(0);
	}
	
	
	
	
	
	void abort()
	{
		while(1)
		{
			
		}
	}
	
	
	void __cxa_pure_virtual()
	{
		while(1)
		{
			
		}
	}
}
