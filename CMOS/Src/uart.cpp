#include "../Inc/cmos.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/

UART::UART(void* txDataRegister)
	:	I_Serial<uint8>(),
		
		m_initialized(false),
		m_dma_tx(nullptr),
		m_txBuffer(nullptr, 0),
		m_eventID_dataReceived(CMOS::get().event_create()),
		m_txDataRegister(txDataRegister)
{
	
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

CODE_RAM uint8 UART::rx()
{
	//	Protect Rx Buffer from unauthorized Access
	if(CMOS::get().semaphore_isOwnedByRunningThread(this) == false)
	{
		return(0);
	}
	
	
	//	If not initialized, no Data will arrive
	if(m_initialized == false)
	{
		return(0);
	}
	
	
	//	Sleep until Data arrives
	CMOS& cmos = CMOS::get();
	cmos.event_listen(m_eventID_dataReceived);
	while(is_empty() == true)
	{
		cmos.sleep_untilEvent(m_eventID_dataReceived);
	}
	cmos.event_unlisten();
	
	
	//	Return Data
	return(read());
}


CODE_RAM feedback UART::tx()
{
	//	Protect UART from unauthorized Access
	if(m_initialized == false || CMOS::get().semaphore_isOwnedByRunningThread(this) == false)
	{
		return(FAIL);
	}
	
	
	//	Check Buffer Size and Tx Data Registers
	if(m_txBuffer.get_size() == 0 || m_txDataRegister == nullptr)
	{
		return(OK);
	}
	
	
	//	Transfer by DMA and wait for it to finish
	if(m_dma_tx->transfer(m_txBuffer.get_data(), m_txDataRegister, I_DMA::e_dataType::UINT8, I_DMA::e_dataType::UINT8, true, false, I_DMA::e_direction::MEMORY_TO_PERIPHERAL, false, m_txBuffer.get_numberOfUnread(), I_DMA::e_priority::VERY_HIGH) != OK)
	{
		return(FAIL);
	}
	
	
	//	Reset Ringbuffer after Transfer finished
	//	Its important to use the reset() Function here, because it resets the Buffer Pointers to 0 where the DMA starts to read from
	//	The clear() Function would just set tail = head, which would result in the DMA reading the right amount of Data, but from the wrong Position
	m_txBuffer.reset();
	return(OK);
}