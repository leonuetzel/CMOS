#include "../Inc/heap.hpp"





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

CODE_RAM void* Heap::reserve(uint8 thread_ID, uint32 sizeInBytes)
{
	//	Check requested Size
	if(sizeInBytes > c_size)
	{
		return(nullptr);
	}
	
	
	//	Align requested Size to Block Size
	uint32 numberOfBlocks = sizeInBytes / c_blockSize;
	if(sizeInBytes % c_blockSize != 0)
	{
		numberOfBlocks++;
	}
	
	
	//	Check requested Number of Blocks
	if(numberOfBlocks > c_blockNumber)
	{
		return(nullptr);
	}
	
	
	//	Search free Blockchain
	uint32 start_block = c_blockNumber;
	for(uint32 i = 0; i < c_blockNumber && start_block == c_blockNumber; i++)
	{
		uint32 length = 0;
		for(uint32 j = i; j < c_blockNumber; j++)
		{
			if(isBlockFree(j) == true)
			{
				length++;
				if(length >= numberOfBlocks)
				{
					start_block = i;
					break;
				}
			}
			else
			{
				break;
			}
		}
		i += length;
	}
	
	
	//	No matching Blockchain found
	if(start_block == c_blockNumber)
	{
		return(nullptr);
	}
	
	
	//	Reserve Blocks
	for(uint32 i = 0; i < numberOfBlocks - 1; i++)
	{
		reserve_block(start_block + i, false);
	}
	reserve_block(start_block + numberOfBlocks - 1, true);
	return(&m_data[(c_blockSize / 4) * start_block]);
}


CODE_RAM uint32 Heap::free(void* address)
{
	//	Check Constraints
	if(address == nullptr || address < m_data || address >= m_data + c_size / 4)
	{
		return(0);
	}
	
	
	//	Calculate Start Block and check if first Block is reserved (so probably there is a Blockchain)
	uint32 start_block = ((uint32) address - (uint32) m_data) / c_blockSize;
	if(isBlockFree(start_block) == true)
	{
		return(0);
	}
	
	
	//	Free Blocks
	uint32 numberOfBlocks = 0;
	bool lastBlock = false;
	while(lastBlock == false)
	{
		const uint32 blockNumber = start_block + numberOfBlocks;
		
		lastBlock = isEndOfBlockchain(blockNumber);
		
		free_block(blockNumber);
		numberOfBlocks++;
	}
	return(numberOfBlocks * c_blockSize);
}