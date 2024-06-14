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
		uint8 data[8];
		uint8 dataSize;
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		constexpr inline CAN_Frame();
		inline ~CAN_Frame();
		
		constexpr inline bool operator==(const CAN_Frame& canFrame);
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
		data(),
		dataSize(0)
{
	
}


inline CAN_Frame::~CAN_Frame()
{
	
}







constexpr inline bool CAN_Frame::operator==(const CAN_Frame& canFrame)
{
	if(extendedID != canFrame.extendedID)
	{
		return(false);
	}
	if(ID != canFrame.ID)
	{
		return(false);
	}
	if(remoteTransmissionRequest != canFrame.remoteTransmissionRequest)
	{
		return(false);
	}
	if(timeStamp != canFrame.timeStamp)
	{
		return(false);
	}
	for(uint32 i = 0; i < sizeof(data) / sizeof(data[0]); i++)
	{
		if(data[i] != canFrame.data[i])
		{
			return(false);
		}
	}
	if(dataSize != canFrame.dataSize)
	{
		return(false);
	}
	return(true);
}