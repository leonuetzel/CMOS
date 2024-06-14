#include "../Inc/stack.hpp"





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

uint32* Stack::reserve(uint8 thread_ID, uint32 sizeInBytes)
{
	if(sizeInBytes > c_size)
	{
		return(nullptr);
	}
	
	sizeInBytes = alignToBlocksize(sizeInBytes);
	
	uint32 numberOfBlocks = sizeInBytes / c_blockSize;
	uint32 start_block = c_blockNumber;
	for(uint32 i = 0; i < c_blockNumber && start_block == c_blockNumber; i++)																											//	Search free Blockchain
	{
		uint32 length = 0;
		for(uint32 j = i; m_blockOwner[j] == c_empty && j < c_blockNumber; j++)
		{
			length++;
			if(length >= numberOfBlocks)
			{
				start_block = i;
				break;
			}
		}
		i += length;
	}
	
	if(start_block == c_blockNumber)																																															//	No matching Blockchain found
	{
		return(nullptr);
	}
	
	for(uint32 i = 0; i < numberOfBlocks; i++)
	{
		m_blockOwner[start_block + i] = thread_ID;
	}
	for(uint32 i = 0; i < numberOfBlocks * c_blockSize / 4; i++)
	{
		m_data[start_block * c_blockSize / 4 + i] = 0;
	}
	m_data[(c_blockSize / 4) * start_block] = c_checkValue;
	
	return(&m_data[(c_blockSize / 4) * start_block + sizeInBytes / 4]);
}


void Stack::free(uint8 thread_ID)
{
	for(uint32 i = 0; i < c_blockNumber; i++)
	{
		if(m_blockOwner[i] == thread_ID)
		{
			m_blockOwner[i] = c_empty;
		}
	}
}


CODE_RAM uint32 Stack::get_sizeInBytes()
{
	return(c_size);
}


uint32 Stack::alignToBlocksize(uint32 sizeInBytes)
{
	if(sizeInBytes < c_blockSize)
	{
		return(c_blockSize);
	}
	
	uint32 mod = sizeInBytes % c_blockSize;
	if(mod != 0)
	{
		sizeInBytes += c_blockSize - mod;
	}
	return(sizeInBytes);
}