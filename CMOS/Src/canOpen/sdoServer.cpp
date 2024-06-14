#include "../../Inc/canOpen/canOpen.hpp"





namespace canOpen
{
	/*****************************************************************************/
	/*                    Globals and Static Initialization					 						 */
	/*****************************************************************************/
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Private	  			 						 						 */
	/*****************************************************************************/
	
	SDO_Server::SDO_Server(I_CAN& can, const uint8& nodeID, Object_Dictionary& objectDictionary)
		:	m_can(can),
			m_nodeID(nodeID),
			m_objectDictionary(objectDictionary),
			m_transfers(),
			m_wakeUpListOnSDOwrite(),
			m_wakeUpListOnSDOread()
	{
		CMOS::get().semaphore_create(this);
	}
	
	
	SDO_Server::~SDO_Server()
	{
		
	}
	
	
	
	
	
	
	
	void SDO_Server::process_readFromObjectDictionary(const CAN_Frame& canFrame)
	{
		
	}
	
	
	void SDO_Server::process_writeToObjectDictionary(const CAN_Frame& canFrame)
	{
		
	}
	
	
	void SDO_Server::process_abort(const CAN_Frame& canFrame)
	{
		
	}
	
	
	
	/*****************************************************************************/
	/*                      						Public	  			 						 						 */
	/*****************************************************************************/
	
	void SDO_Server::handlePacket(const CAN_Frame& canFrame)
	{
		//	SDO Frames are always 8 Byte long
		if(canFrame.dataSize != 8)
		{
			return;
		}
		
		
		//	Read Communication Object and Node ID
		const CanOpen::e_communicationObject communicationObject = (CanOpen::e_communicationObject) ((canFrame.ID & 0x00000780) >> 7);
		const uint8 nodeID = canFrame.ID & 0x0000007F;
		
		const uint16 index = (canFrame.data[2] << 8) | canFrame.data[1];
		const uint8 subIndex = canFrame.data[3];
		
		//	Read Command Specifier
		const e_commandSpecifier commandSpecifier = (e_commandSpecifier) ((canFrame.data[0] & 0xE0) >> 5);
		
		
		//	Handle Packet
		CMOS& cmos = CMOS::get();
		switch(communicationObject)
		{
			case CanOpen::e_communicationObject::TX_SDO:
			{
				//	TX SDOs are used as Responses
				switch(commandSpecifier)
				{
					case e_commandSpecifier::READ:
					{
						//	This is the Response for our own SDO Read Request
						const bool expedited = bit::isSet(canFrame.data[0], 1);
						if(expedited == true)
						{
							//	Check for Data Set Size Indication
							const bool dataSetSizeIndicated = bit::isSet(canFrame.data[0], 0);
							if(dataSetSizeIndicated == false)
							{
								return;
							}
							//	Search for corresponding SDO Read in the Transfer Queue
							cmos.semaphore_lock(this);
							for(auto& i: m_transfers)
							{
								if(i->nodeID == nodeID && i->index == index && i->subIndex == subIndex && i->read == true)
								{
									//	Read Payload and copy it to the 's_transfer' Struct in the Transfer Queue
									const uint8 numberOfDataBytesSent = 4 - ((canFrame.data[0] & 0x0C) >> 2);
									i->data.set_size(numberOfDataBytesSent);
									for(uint32 j = 0; j < numberOfDataBytesSent; j++)
									{
										i->data[j] = canFrame.data[j + 4];
									}
									
									
									//	Remove the 's_transfer' Struct from the Transfer Queue to indicate that the Transfer is finished
									m_transfers.erase(m_transfers.find(i));
									
									
									//	Stop searching the Transfer Queue because we already found the corresponding SDO Read
									break;
								}
							}
							cmos.semaphore_unlock(this);
						}
						else
						{
							//	To be done
						}
					}
					break;
					
					case e_commandSpecifier::WRITE_RESPONSE:
					{
						//	This is the Response for our own SDO Write Request
						//	Search for corresponding SDO Write in the Transfer Queue
						cmos.semaphore_lock(this);
						for(auto& i: m_transfers)
						{
							if(i->nodeID == nodeID && i->index == index && i->subIndex == subIndex && i->read == false)
							{
								i->success = true;
								
								
								//	Check for all 0's in received Payload
								for(uint32 j = 4; j < 8; j++)
								{
									if(canFrame.data[j] != 0)
									{
										i->success = false;
										break;
									}
								}
								
								
								//	Remove the 's_transfer' Struct from the Transfer Queue to indicate that the Transfer is finished
								m_transfers.erase(m_transfers.find(i));
								
								
								//	Stop searching the Transfer Queue because we already found the corresponding SDO Write
								break;
							}
						}
						cmos.semaphore_unlock(this);
					}
					break;
					
					default:
					{
						return;
					}
					break;
				}
			}
			break;
			
			case CanOpen::e_communicationObject::RX_SDO:
			{
				//	RX SDOs are used as Requests
				
				
				//	Check if Node ID matches
				if(nodeID != m_nodeID)
				{
					return;
				}
				
				
				//	Process the Request
				switch(commandSpecifier)
				{
					case e_commandSpecifier::READ:
					{
						//	This is an SDO Read Request from another Master
						
						
						//	All other Data Bytes should be 0
						for(uint32 i = 4; i < 8; i++)
						{
							if(canFrame.data[i] != 0)
							{
								return;
							}
						}
						
						
						//	Check if requested Index and Subindex exists
						if(m_objectDictionary.contains(index, subIndex) == true)
						{
							//	Wakeup all subscripted Threads
							if(m_wakeUpListOnSDOread.contains(index) == true)
							{
								const Array<uint8>& threadIDs = m_wakeUpListOnSDOread[index];
								for(auto& i: threadIDs)
								{
									cmos.send_mail(i, (subIndex << 16) | index);
								}
							}
							
							//	Read from Object Dictionary
							//	Dont read as Reference because the Object Dictionary wont be semaphore-locked the whole Time
							const Array<uint8> data = m_objectDictionary[index][subIndex];
							
							
							//	Get Number of Bytes to be transmitted
							const uint32 numberOfBytes = data.get_size();
							if(numberOfBytes == 0)
							{
								return;
							}
							
							
							//	Check if the requested Data can be transmitted via expedited SDO
							if(numberOfBytes <= 4)
							{
								//	Expedited SDO
								CAN_Frame canFrameResponse;
								canFrameResponse.ID = (((uint32) CanOpen::e_communicationObject::TX_SDO) << 7) | m_nodeID;
								canFrameResponse.dataSize = 8;
								canFrameResponse.data[0] = (((uint32) e_commandSpecifier::READ) << 5) | ((4 - numberOfBytes) << 2) | 0x03;
								canFrameResponse.data[1] = canFrame.data[1];
								canFrameResponse.data[2] = canFrame.data[2];
								canFrameResponse.data[3] = canFrame.data[3];
								for(uint32 i = 0; i < numberOfBytes; i++)
								{
									canFrameResponse.data[i + 4] = data[i];
								}
								
								
								//	Send CAN Frame
								m_can << canFrameResponse;
							}
							else
							{
								//	To be done
							}
						}
						else
						{
							//	Error Handling
							//	To be done
						}
					}
					break;
					
					case e_commandSpecifier::WRITE_REQUEST:
					{
						//	This is an SDO Write Request from another Master
						const bool expedited = bit::isSet(canFrame.data[0], 1);
						if(expedited == true)
						{
							//	Check for Data Set Size Indication
							const bool dataSetSizeIndicated = bit::isSet(canFrame.data[0], 0);
							if(dataSetSizeIndicated == false)
							{
								return;
							}
							
							
							//	Read Number of Data Bytes transmitted
							const uint32 numberOfBytes = 4 - ((canFrame.data[0] & 0x0C) >> 2);
							
							
							//	Check if Entry in Object Dictionary exists
							if(m_objectDictionary.contains(index, subIndex) == true)
							{
								//	Check for correct Length
								Array<uint8>& subEntry = m_objectDictionary[index][subIndex];
								if(subEntry.get_size() != numberOfBytes)
								{
									return;
								}
								
								
								//	Write to Entry in Object Dictionary
								for(uint32 i = 0; i < numberOfBytes; i++)
								{
									subEntry[i] = canFrame.data[i + 4];
								}
								
								
								//	Wakeup all subscripted Threads
								if(m_wakeUpListOnSDOwrite.contains(index) == true)
								{
									const Array<uint8>& threadIDs = m_wakeUpListOnSDOwrite[index];
									for(auto& i: threadIDs)
									{
										cmos.send_mail(i, (subIndex << 16) | index);
									}
								}
								
								
								//	Send Response
								CAN_Frame canFrameResponse;
								canFrameResponse.ID = (((uint32) CanOpen::e_communicationObject::TX_SDO) << 7) | m_nodeID;
								canFrameResponse.dataSize = 8;
								canFrameResponse.data[0] = ((uint32) e_commandSpecifier::WRITE_RESPONSE) << 5;
								canFrameResponse.data[1] = canFrame.data[1];
								canFrameResponse.data[2] = canFrame.data[2];
								canFrameResponse.data[3] = canFrame.data[3];
								
								
								//	Send CAN Frame
								m_can << canFrameResponse;
							}
						}
						else
						{
							//	To be done
							//	Non-expedited Transfer
						}
					}
					break;
					
					default:
					{
						return;
					}	
					break;
				}
			}
			break;
			
			case CanOpen::e_communicationObject::DYNAMIC_SDO:
			{
				//	To be done
			}
			break;
			
			default:
			{
				return;
			}
		}
	}
	
	
	
	
	
	
	
	Array<uint8> SDO_Server::read(uint8 nodeID, uint16 index, uint8 subIndex, uint32 timeoutMilliseconds)
	{
		s_transfer transfer;
		transfer.nodeID = nodeID;
		transfer.index = index;
		transfer.subIndex = subIndex;
		transfer.read = true;
		transfer.success = false;
		transfer.data.erase();
		
		
		//	Put Transfer Request into the Queue
		CMOS& cmos = CMOS::get();
		if(cmos.semaphore_lock(this) != OK)
		{
			return(Array<uint8>());
		}
		m_transfers += &transfer;
		cmos.semaphore_unlock(this);
		
		
		//	Build CAN Frame to send
		CAN_Frame canFrame;
		canFrame.ID = (((uint32) CanOpen::e_communicationObject::RX_SDO) << 7) | nodeID;
		canFrame.dataSize = 8;
		canFrame.data[0] = ((uint32) e_commandSpecifier::READ) << 5;
		canFrame.data[1] = (index & 0x00FF);
		canFrame.data[2] = (index & 0xFF00) >> 8;
		canFrame.data[3] = subIndex;
		
		
		//	Send CAN Frame
		m_can << canFrame;
		
		
		//	Wait for Transfer to be finished
		//	'SDO_Server::handlePacket' will remove the Pointer to the 's_transfer' Struct from the Transfer Queue on finishing the Transfer
		uint32 timeoutCounter = 0;
		while(m_transfers.contains(&transfer) == true)
		{
			if(timeoutCounter > timeoutMilliseconds)
			{
				cmos.semaphore_lock(this);
				m_transfers.erase(m_transfers.find(&transfer));
				cmos.semaphore_unlock(this);
				return(Array<uint8>());
			}
			timeoutCounter += 5;
			cmos.sleep_ms(5);
		}
		
		
		//	Return received Data
		return(transfer.data);
	}
	
	
	feedback SDO_Server::write(uint8 nodeID, uint16 index, uint8 subIndex, const Array<uint8>& data, uint32 timeoutMilliseconds)
	{
		//	Minimum 1 Byte has to be written
		const uint32 numberOfBytes = data.get_size();
		if(numberOfBytes == 0)
		{
			return(FAIL);
		}
		
		
		//	If more than 4 Byte should be written, no expedited Write can be used
		if(numberOfBytes > 4)
		{
			//	To be done: Not-expedited Write
		}
		else
		{
			//	Expedited Write
			s_transfer transfer;
			transfer.nodeID = nodeID;
			transfer.index = index;
			transfer.subIndex = subIndex;
			transfer.read = false;
			transfer.data = data;
			
			
			//	Put Transfer Request into the Queue
			CMOS& cmos = CMOS::get();
			if(cmos.semaphore_lock(this) != OK)
			{
				return(FAIL);
			}
			m_transfers += &transfer;
			cmos.semaphore_unlock(this);
			
			
			//	Build CAN Frame to send
			CAN_Frame canFrame;
			canFrame.ID = (((uint32) CanOpen::e_communicationObject::RX_SDO) << 7) | nodeID;
			canFrame.dataSize = 8;
			canFrame.data[0] = (((uint32) e_commandSpecifier::WRITE_REQUEST) << 5) | ((4 - numberOfBytes) << 2) | 0x03;
			canFrame.data[1] = (index & 0x00FF);
			canFrame.data[2] = (index & 0xFF00) >> 8;
			canFrame.data[3] = subIndex;
			for(uint32 i = 0; i < numberOfBytes; i++)
			{
				canFrame.data[i + 4] = data[i];
			}
			
			
			//	Send CAN Frame
			m_can << canFrame;
			
			
			//	Wait for Transfer to be finished
			//	'SDO_Server::handlePacket' will remove the Pointer to the 's_transfer' Struct from the Transfer Queue on finishing the Transfer
			uint32 timeoutCounter = 0;
			while(m_transfers.contains(&transfer) == true)
			{
				if(timeoutCounter > timeoutMilliseconds)
				{
					cmos.semaphore_lock(this);
					m_transfers.erase(m_transfers.find(&transfer));
					cmos.semaphore_unlock(this);
					return(FAIL);
				}
				timeoutCounter += 5;
				cmos.sleep_ms(5);
			}
			
			
			if(transfer.success == true)
			{
				return(OK);
			}
			return(FAIL);
		}
		return(FAIL);
	}
	
	
	
	
	
	
	
	feedback SDO_Server::wakeupOnSDOwrite(uint16 index)
	{
		//	Entry not available in Object Dictionary, therefore a Subscription is not possible
		if(m_objectDictionary.contains(index, 0) == false)
		{
			return(FAIL);
		}
		
		
		//	No Subscription yet on that Index, create a new one
		const uint8 threadID = CMOS::get().get_runningThreadID();
		if(m_wakeUpListOnSDOwrite.contains(index) == false)
		{
			m_wakeUpListOnSDOwrite += Pair<uint16, Array<uint8>>(index, Array<uint8>(threadID));
		}
		else
		{
			m_wakeUpListOnSDOwrite[index] += threadID;
		}
		return(OK);
	}
	
	
	feedback SDO_Server::wakeupOnSDOread(uint16 index)
	{
		//	Entry not available in Object Dictionary, therefore a Subscription is not possible
		if(m_objectDictionary.contains(index, 0) == false)
		{
			return(FAIL);
		}
		
		
		//	No Subscription yet on that Index, create a new one
		const uint8 threadID = CMOS::get().get_runningThreadID();
		if(m_wakeUpListOnSDOread.contains(index) == false)
		{
			m_wakeUpListOnSDOread += Pair<uint16, Array<uint8>>(index, Array<uint8>(threadID));
		}
		else
		{
			m_wakeUpListOnSDOread[index] += threadID;
		}
		return(OK);
	}
};