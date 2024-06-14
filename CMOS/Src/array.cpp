#include "../Inc/cmos.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/

uint32 __cmos_array_expand__(uint8** pointerToData, uint32 actualSizeInBytes, uint32 newSizeInBytes)
{
	if(newSizeInBytes == 0 || pointerToData == nullptr)
	{
		return(0);
	}
	
	
	//	Reserve Memory
	uint8* newData = new uint8[newSizeInBytes];
	if(newData == nullptr)
	{
		return(0);
	}
	
	
	//	Copy Data if necessary and free old Memory
	uint8* data = *pointerToData;
	if(data != nullptr)
	{
		for(uint32 i = 0; i < actualSizeInBytes; i++)
		{
			newData[i] = data[i];
		}
		delete[] data;
	}
	*pointerToData = newData;
	
	
	//	Return allocated Size in Bytes
	return(CMOS::c_this->m_heap.alignToBlocksize(newSizeInBytes));
}


void __cmos_array_move__(uint8* source, uint8* destination, uint32 sizeInBytes)
{
	//	Check if Pointer are identical
	if(source == destination)
	{
		return;
	}
	
	
	//	Copy bytewise
	for(uint32 i = 0; i < sizeInBytes; i++)
	{
		//	Copy from highest Address downwards so Data doesnt get overwritten
		const uint32 index = sizeInBytes - i - 1;
		destination[index] = source[index];
	}
}



/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

