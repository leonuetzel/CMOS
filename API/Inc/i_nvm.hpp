#pragma once

#include "cmos.hpp"





class I_NVM
{
	public:
		
		
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		virtual feedback write(uint32 address, int8 data) = 0;
		virtual feedback write(uint32 address, uint8 data) = 0;
		virtual feedback write(uint32 address, int16 data) = 0;
		virtual feedback write(uint32 address, uint16 data) = 0;
		virtual feedback write(uint32 address, int32 data) = 0;
		virtual feedback write(uint32 address, uint32 data) = 0;
		virtual feedback write(uint32 address, int64 data) = 0;
		virtual feedback write(uint32 address, uint64 data) = 0;
		virtual feedback write(uint32 address, float data) = 0;
		virtual feedback write(uint32 address, double data) = 0;
		
		virtual int8 read_int8(uint32 address) = 0;
		virtual uint8 read_uint8(uint32 address) = 0;
		virtual int16 read_int16(uint32 address) = 0;
		virtual uint16 read_uint16(uint32 address) = 0;
		virtual int32 read_int32(uint32 address) = 0;
		virtual uint32 read_uint32(uint32 address) = 0;
		virtual int64 read_int64(uint32 address) = 0;
		virtual uint64 read_uint64(uint32 address) = 0;
		virtual float read_float(uint32 address) = 0;
		virtual double read_double(uint32 address) = 0;
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

