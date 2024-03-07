#pragma once

#include "defines.hpp"
#include "array.hpp"





class CAN_Frame
{
	public:
		
		bool extendedID;
		uint32 ID;
		bool remoteTransmissionRequest;
		uint16 timeStamp;
		Array<uint8> data;
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		constexpr inline CAN_Frame();
		inline ~CAN_Frame();
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

constexpr inline CAN_Frame::CAN_Frame()
	:	extendedID(false),
		ID(0),
		remoteTransmissionRequest(false),
		timeStamp(0),
		data()
{
	
}


inline CAN_Frame::~CAN_Frame()
{
	
}