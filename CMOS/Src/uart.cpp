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
		m_txBuffer(nullptr, 0),
		m_eventID_dataReceived(CMOS::get().event_create()),
		m_dma_tx(nullptr),
		m_txDataRegister(txDataRegister)
{
	CMOS::get().event_subscribe(m_eventID_dataReceived);
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
	while(is_dataAvailable() == false)
	{
		cmos.sleep_untilEvent(m_eventID_dataReceived);
	}
	
	
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
	
	
	//	Clear Ringbuffer after Transfer finished
	m_txBuffer.clear();
	return(OK);
}