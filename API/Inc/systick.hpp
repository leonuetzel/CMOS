#pragma once

#include "defines.hpp"





class Systick
{
	public:
		
		enum class e_clockSource
		{
			CPU,
			HCLK_DIV8
		};
		
		
		
		
		
	private:
		
		uint32 m_clock;
		
		
		constexpr inline Systick();
		Systick(const Systick& systick) = delete;
		inline ~Systick();
		
		
		friend class CMOS;
		
		
		
		
		
	public:
		
		feedback init(e_clockSource clockSource, uint32 clock_in, uint32 frequency, bool enableInterrupt);
};



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

constexpr inline Systick::Systick()
	: m_clock(0)
{
	
}


inline Systick::~Systick()
{
	
}



/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

