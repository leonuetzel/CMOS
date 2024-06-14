#include "../../Inc/canOpen/canOpen.hpp"





namespace canOpen
{
	/*****************************************************************************/
	/*                    Globals and Static Initialization					 						 */
	/*****************************************************************************/
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Private	  			 						 						 */
	/*****************************************************************************/
	
	NMT::NMT(I_CAN& can, const uint8& ownNodeID)
		:	m_can(can),
			m_nodeID(ownNodeID),
			m_state(e_state::BOOT_UP)
	{
		
	}
	
	
	NMT::~NMT()
	{
		
	}
	
	
	
	/*****************************************************************************/
	/*                      						Public	  			 						 						 */
	/*****************************************************************************/
	
	void NMT::handlePacket(const CAN_Frame& canFrame)
	{
		//	The Node ID is the lower 7 Bit of the 11-Bit CAN ID
		const uint8 nodeID = canFrame.ID & 0x0000007F;
		
		
		//	The Communication Object Type is the upper 4 Bit of the 11-Bit CAN ID
		const CanOpen::e_communicationObject communicationObject = (CanOpen::e_communicationObject) ((canFrame.ID & 0x00000780) >> 7);
		
		
		switch(communicationObject)
		{
			case CanOpen::e_communicationObject::NMT:
			{
				//	Data Structure
				//	ID[10:7]		0x0
				//	ID[ 6:0]		Node ID
				//	Data[0]			NMT Command
				//	Data[1]			Slave Node ID
				
				
				//	NMT Messages are always 2 Byte long
				if(canFrame.dataSize != 2)
				{
					return;
				}
				
				
				//	Check Node ID Consistency
				if(canFrame.data[1] != nodeID)
				{
					return;
				}
				
				
				const e_command nmtCommand = (e_command) canFrame.data[0];
				switch(nmtCommand)
				{
					case e_command::OPERATIONAL:
					{
						
					}
					break;
					
					case e_command::STOP_NODE:
					{
						
					}
					break;
					
					case e_command::PRE_OPERATIONAL:
					{
						
					}
					break;
					
					case e_command::RESET_NODE:
					{
						
					}
					break;
					
					case e_command::RESET_COMMUNICATION:
					{
						
					}
					break;
					
					default:
					{
						
					}
					break;
				}
			}
			break;
			
			case CanOpen::e_communicationObject::HEARTBEAT:
			{
				//	Data Structure
				//	ID[10:7]		0xE
				//	ID[ 6:0]		Node ID
				//	Data[0]			NMT State
				
				
				//	Heartbeat Messages are always 1 Byte long
				if(canFrame.dataSize != 1)
				{
					return;
				}
				
				
				//	Update Node State
				const e_state nodeState = (e_state) canFrame.data[0];
				if(m_nodes.contains(nodeID) == true)
				{
					m_nodes[nodeID] = nodeState;
				}
				else
				{
					const Pair<uint8, e_state> node(nodeID, nodeState);
					m_nodes += node;
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
	
	
	
	
	
	
	
	const UniquePairArray<uint8, NMT::e_state>& NMT::get_nodes() const
	{
		return(m_nodes);
	}
	
	
	feedback NMT::send_networkState()
	{
		//	Create CAN Frame with one Byte Length containing the Node State
		CAN_Frame canFrame;
		canFrame.ID = (((uint32) CanOpen::e_communicationObject::HEARTBEAT) << 7) | m_nodeID;
		canFrame.data[0] = (uint8) m_state;
		canFrame.dataSize = 1;
		
		
		//	Send Frame
		m_can << canFrame;
		
		
		return(OK);
	}
};