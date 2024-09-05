#include "../../Inc/canOpen/canOpen.hpp"





namespace canOpen
{
	/*****************************************************************************/
	/*                    Globals and Static Initialization					 						 */
	/*****************************************************************************/
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Private	  			 						 						 */
	/*****************************************************************************/
	
	void CanOpen::manager()
	{
		//	Subscribe to CAN Events
		CMOS& cmos = CMOS::get();
		cmos.event_subscribe(m_can.get_eventID());
		
		
		//	Handle Events
		while(1)
		{
			//	Wait for anything to happen
			//	Expected Events:
			//	-	CAN Rx
			//	-	Message from another Thread to initiate Actions on the CAN Bus
			cmos.sleep();
			
			
			//	Handle Bus Off State
			while(m_can.get_state() == I_CAN::e_state::BUS_OFF)
			{
				m_can.recoverFromBusOffState();
				cmos.sleep_ms(20);
			}
			
			
			//	Handle CAN Rx
			while(m_can.is_dataAvailable() == true)
			{
				CAN_Frame canFrame;
				if(m_can.rx(canFrame) == OK)
				{
					handlePacket(canFrame);
				}
			}
		}
	}
	
	
	
	
	
	
	
	void CanOpen::handlePacket(const CAN_Frame& canFrame)
	{
		//	CANopen doesnt use extended IDs
		if(canFrame.extendedID == true)
		{
			return;
		}
		
		
		//	The Communication Object Type is the upper 4 Bit of the 11-Bit CAN ID
		const e_communicationObject communicationObject = (e_communicationObject) ((canFrame.ID & 0x00000780) >> 7);
		switch(communicationObject)
		{
			case e_communicationObject::NMT:
			{
				m_nmt.handlePacket(canFrame);
			}
			break;
			
			case e_communicationObject::SYNC_EMCY:
			{
				
			}
			break;
			
			case e_communicationObject::TIME:
			{
				
			}
			break;
			
			case e_communicationObject::TX_PDO1:
			{
				
			}
			break;
			
			case e_communicationObject::RX_PDO1:
			{
				
			}
			break;
			
			case e_communicationObject::TX_PDO2:
			{
				
			}
			break;
			
			case e_communicationObject::RX_PDO2:
			{
				
			}
			break;
			
			case e_communicationObject::TX_PDO3:
			{
				
			}
			break;
			
			case e_communicationObject::RX_PDO3:
			{
				
			}
			break;
			
			case e_communicationObject::TX_PDO4:
			{
				
			}
			break;
			
			case e_communicationObject::RX_PDO4:
			{
				
			}
			break;
			
			case e_communicationObject::TX_SDO:
			{
				m_sdoServer.handlePacket(canFrame);
			}
			break;
			
			case e_communicationObject::RX_SDO:
			{
				m_sdoServer.handlePacket(canFrame);
			}
			break;
			
			case e_communicationObject::HEARTBEAT:
			{
				m_nmt.handlePacket(canFrame);
			}
			break;
			
			default:
			{
				return;
			}
		}
	}
	
	
	
	/*****************************************************************************/
	/*                      						Public	  			 						 						 */
	/*****************************************************************************/
	
	CanOpen::CanOpen(I_CAN& can, uint8 nodeID, uint8 threadPriority)
		:	m_can(can),
			m_nodeID(nodeID),
			m_nmt(can, m_nodeID),
			m_objectDictionary(),
			m_sdoServer(m_can, m_nodeID, m_objectDictionary)
	{
		CMOS& cmos = CMOS::get();
		if(cmos.semaphore_create(this) != OK)
		{
			return;
		}
		m_threadID = cmos.thread_create(&CanOpen::manager, this, "CanOpen", threadPriority);
	}
	
	
	CanOpen::~CanOpen()
	{
		
	}
	
	
	
	
	
	
	
	const UniquePairArray<uint8, NMT::e_state>& CanOpen::get_nodes() const
	{
		return(m_nmt.get_nodes());
	}
	
	
	feedback CanOpen::send_networkState()
	{
		return(m_nmt.send_networkState());
	}
	
	
	
	
	
	
	
	Array<uint8> CanOpen::sdo_read(uint8 nodeID, uint16 index, uint8 subIndex, uint32 timeoutMilliseconds)
	{
		return(m_sdoServer.read(nodeID, index, subIndex, timeoutMilliseconds));
	}
	
	
	feedback CanOpen::sdo_write(uint8 nodeID, uint16 index, uint8 subIndex, const Array<uint8>& data, uint32 timeoutMilliseconds)
	{
		return(m_sdoServer.write(nodeID, index, subIndex, data, timeoutMilliseconds));
	}
	
	
	feedback CanOpen::wakeupOnSDOwrite(uint16 index)
	{
		return(m_sdoServer.wakeupOnSDOwrite(index));
	}
	
	
	feedback CanOpen::wakeupOnSDOread(uint16 index)
	{
		return(m_sdoServer.wakeupOnSDOread(index));
	}
	
	
	
	
	
	
	
	Object_Dictionary& CanOpen::get_objectDictionary()
	{
		return(m_objectDictionary);
	}
};