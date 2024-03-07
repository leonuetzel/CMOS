#pragma once

#include "i_dma.hpp"





class DMA_Software : public I_DMA
{
	public:
		
		typedef bool (*f_readyForNextTransfer)();
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		f_readyForNextTransfer m_readyForNextTransfer;
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		constexpr inline DMA_Software(f_readyForNextTransfer readyForNextTransferFunction);
		
		inline feedback reset() override;
		feedback transfer(const void* source, const void* destination, e_dataType sourceType, e_dataType destinationType, bool source_inc, bool dest_inc, e_direction direction, bool circularMode, uint16 numberOfItems, e_priority priority = e_priority::LOW, bool waitForTransferEnd = true, f_callback callback = nullptr) override;
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

constexpr inline DMA_Software::DMA_Software(f_readyForNextTransfer readyForNextTransferFunction)
	:	m_readyForNextTransfer(readyForNextTransferFunction)
{
	
}







inline feedback DMA_Software::reset()
{
	return(OK);
}