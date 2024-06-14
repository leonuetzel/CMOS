#include "../Inc/cmos.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

CODE_RAM feedback DMA_Software::transfer(const void* source, const void* destination, e_dataType sourceType, e_dataType destinationType, bool source_inc, bool dest_inc, e_direction direction, bool circularMode, uint16 numberOfItems, e_priority priority, bool waitForTransferEnd, f_callback callback)
{
	if(source == nullptr || destination == nullptr)
	{
		return(FAIL);
	}
	
	
	CMOS& cmos = CMOS::get();
	uint32 sourceCounter = 0;
	uint32 destinationCounter = 0;
	switch(sourceType)
	{
		case e_dataType::UINT8:
		{
			const uint8* sourcePointer = (const uint8*) source;
			switch(destinationType)
			{
				case e_dataType::UINT8:
				{
					uint8* destinationPointer = (uint8*) destination;
					for(uint32 i = 0; i < numberOfItems; i++)
					{
						while(m_readyForNextTransfer() == false)
						{
							cmos.sleep_ms(1);
						}
						destinationPointer[destinationCounter] = sourcePointer[sourceCounter];
						if(source_inc == true)
						{
							sourceCounter++;
						}
						if(dest_inc == true)
						{
							destinationCounter++;
						}
					}
				}
				break;
				
				case e_dataType::UINT16:
				{
					uint16* destinationPointer = (uint16*) destination;
					for(uint32 i = 0; i < numberOfItems; i++)
					{
						while(m_readyForNextTransfer() == false)
						{
							cmos.sleep_ms(1);
						}
						destinationPointer[destinationCounter] = sourcePointer[sourceCounter];
						if(source_inc == true)
						{
							sourceCounter++;
						}
						if(dest_inc == true)
						{
							destinationCounter++;
						}
					}
				}
				break;
				
				case e_dataType::UINT32:
				{
					uint32* destinationPointer = (uint32*) destination;
					for(uint32 i = 0; i < numberOfItems; i++)
					{
						while(m_readyForNextTransfer() == false)
						{
							cmos.sleep_ms(1);
						}
						destinationPointer[destinationCounter] = sourcePointer[sourceCounter];
						if(source_inc == true)
						{
							sourceCounter++;
						}
						if(dest_inc == true)
						{
							destinationCounter++;
						}
					}
				}
				break;
				
				default:
				{
					return(FAIL);
				}
				break;
			}
		}
		break;
		
		case e_dataType::UINT16:
		{
			const uint16* sourcePointer = (const uint16*) source;
			switch(destinationType)
			{
				case e_dataType::UINT8:
				{
					uint8* destinationPointer = (uint8*) destination;
					for(uint32 i = 0; i < numberOfItems; i++)
					{
						while(m_readyForNextTransfer() == false)
						{
							cmos.sleep_ms(1);
						}
						destinationPointer[destinationCounter] = sourcePointer[sourceCounter];
						if(source_inc == true)
						{
							sourceCounter++;
						}
						if(dest_inc == true)
						{
							destinationCounter++;
						}
					}
				}
				break;
				
				case e_dataType::UINT16:
				{
					uint16* destinationPointer = (uint16*) destination;
					for(uint32 i = 0; i < numberOfItems; i++)
					{
						while(m_readyForNextTransfer() == false)
						{
							cmos.sleep_ms(1);
						}
						destinationPointer[destinationCounter] = sourcePointer[sourceCounter];
						if(source_inc == true)
						{
							sourceCounter++;
						}
						if(dest_inc == true)
						{
							destinationCounter++;
						}
					}
				}
				break;
				
				case e_dataType::UINT32:
				{
					uint32* destinationPointer = (uint32*) destination;
					for(uint32 i = 0; i < numberOfItems; i++)
					{
						while(m_readyForNextTransfer() == false)
						{
							cmos.sleep_ms(1);
						}
						destinationPointer[destinationCounter] = sourcePointer[sourceCounter];
						if(source_inc == true)
						{
							sourceCounter++;
						}
						if(dest_inc == true)
						{
							destinationCounter++;
						}
					}
				}
				break;
				
				default:
				{
					return(FAIL);
				}
				break;
			}
		}
		break;
		
		case e_dataType::UINT32:
		{
			const uint32* sourcePointer = (const uint32*) source;
			switch(destinationType)
			{
				case e_dataType::UINT8:
				{
					uint8* destinationPointer = (uint8*) destination;
					for(uint32 i = 0; i < numberOfItems; i++)
					{
						while(m_readyForNextTransfer() == false)
						{
							cmos.sleep_ms(1);
						}
						destinationPointer[destinationCounter] = sourcePointer[sourceCounter];
						if(source_inc == true)
						{
							sourceCounter++;
						}
						if(dest_inc == true)
						{
							destinationCounter++;
						}
					}
				}
				break;
				
				case e_dataType::UINT16:
				{
					uint16* destinationPointer = (uint16*) destination;
					for(uint32 i = 0; i < numberOfItems; i++)
					{
						while(m_readyForNextTransfer() == false)
						{
							cmos.sleep_ms(1);
						}
						destinationPointer[destinationCounter] = sourcePointer[sourceCounter];
						if(source_inc == true)
						{
							sourceCounter++;
						}
						if(dest_inc == true)
						{
							destinationCounter++;
						}
					}
				}
				break;
				
				case e_dataType::UINT32:
				{
					uint32* destinationPointer = (uint32*) destination;
					for(uint32 i = 0; i < numberOfItems; i++)
					{
						while(m_readyForNextTransfer() == false)
						{
							cmos.sleep_ms(1);
						}
						destinationPointer[destinationCounter] = sourcePointer[sourceCounter];
						if(source_inc == true)
						{
							sourceCounter++;
						}
						if(dest_inc == true)
						{
							destinationCounter++;
						}
					}
				}
				break;
				
				default:
				{
					return(FAIL);
				}
				break;
			}
		}
		break;
		
		default:
		{
			return(FAIL);
		}
		break;
	}
	return(OK);
}