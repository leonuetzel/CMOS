#pragma once

#include "defines.hpp"





class I_CRC
{
	public:
		
		
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		virtual void init(uint8 initialValue, uint8 polynomial, bool reverseOutputData = false, bool reverseInputData = false) = 0;
		virtual void init(uint16 initialValue, uint16 polynomial, bool reverseOutputData = false, bool reverseInputData = false) = 0;
		virtual void init(uint32 initialValue, uint32 polynomial, bool reverseOutputData = false, bool reverseInputData = false) = 0;
		
		virtual I_CRC& operator<<(uint8 data) = 0;
		virtual I_CRC& operator<<(uint16 data) = 0;
		virtual I_CRC& operator<<(uint32 data) = 0;
		
		virtual uint32 operator()() const = 0;
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

