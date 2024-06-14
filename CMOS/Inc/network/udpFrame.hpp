#pragma once

#include "../cmos.hpp"





class UDP_Frame
{
	public:
		
		uint16 sourcePort;
		uint16 destinationPort;
		uint16 checkSum;
		Array<uint8> payload;
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		UDP_Frame();
		UDP_Frame(const Array<uint8>& data);
		~UDP_Frame();
		
		Array<uint8> convertToByteStream() const;
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

