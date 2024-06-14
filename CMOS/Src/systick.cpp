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





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

feedback Systick::init(e_clockSource clockSource, uint32 clock_in, uint32 frequency, bool enableInterrupt)
{
	//	Set Clocksource
	switch(clockSource)
	{
		case e_clockSource::CPU:
		{
			bit::set(*CORE::SYSTICK::CSR, 2);
			break;
		}
		
		case e_clockSource::HCLK_DIV8:
		{
			bit::clear(*CORE::SYSTICK::CSR, 2);
			clock_in /= 8;
			break;
		}
		
		default:
		{
			return(FAIL);
		}
	}
	
	
	
	if(frequency > clock_in || frequency < clock_in / 0x00FFFFFF)
	{
		return(FAIL);
	}
	
	
	//	Reload Value
	*CORE::SYSTICK::RVR = (clock_in / frequency) - 1;
	
	
	//	Enable / Disable Interrupt
	if(enableInterrupt == true)
	{
		bit::set(*CORE::SYSTICK::CSR, 1);
	}
	else
	{
		bit::clear(*CORE::SYSTICK::CSR, 1);
	}
	
	
	//	Reset current Timer Value
	*CORE::SYSTICK::CVR = 0;
	
	
	//  Enable System Tick Timer
	bit::set(*CORE::SYSTICK::CSR, 0);

	m_clock = frequency;

	return(OK);
}