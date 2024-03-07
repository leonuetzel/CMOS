#pragma once

#include "../cmos.hpp"
#include "objectDictionary.hpp"
#include "nmt.hpp"
#include "sdoServer.hpp"





namespace canOpen
{
	class CanOpen
	{
		public:
			
			enum class e_communicationObject: uint8
			{
				NMT					= 0x0,
				SYNC_EMCY		= 0x1,
				TIME				= 0x2,
				TX_PDO1			= 0x3,
				RX_PDO1			= 0x4,
				TX_PDO2			= 0x5,
				RX_PDO2			= 0x6,
				TX_PDO3			= 0x7,
				RX_PDO3			= 0x8,
				TX_PDO4			= 0x9,
				RX_PDO4			= 0xA,
				TX_SDO			= 0xB,
				RX_SDO			= 0xC,
				DYNAMIC_SDO	= 0xD,
				HEARTBEAT		= 0xE,
				LSS					= 0xF
			};
			
			
			
			
			
		private:
			
			//	Static Member
			
			
			
			//	Non-static Member
			I_CAN& m_can;
			const uint8 m_nodeID;
			uint8 m_threadID;
			NMT m_nmt;
			Object_Dictionary m_objectDictionary;
			SDO_Server m_sdoServer;
			
			
			//	Constructor and Destructor
			
			
			
			//	Member Functions
			void manager();
			
			void handlePacket(const CAN_Frame& canFrame);
			
			
			//	Friends
			
			
			
			
			
			
		public:
			
			CanOpen(I_CAN& can, uint8 nodeID, uint8 threadPriority = 50);
			CanOpen(const CanOpen& canOpen) = delete;
			~CanOpen();
			
			
			//	NMT
			const UniquePairArray<uint8, NMT::e_state>& get_nodes() const;
			feedback send_networkState();
			
			
			//	SDO
			Array<uint8> sdo_read(uint8 nodeID, uint16 index, uint8 subIndex, uint32 timeoutMilliseconds = 1000);
			feedback sdo_write(uint8 nodeID, uint16 index, uint8 subIndex, const Array<uint8>& data, uint32 timeoutMilliseconds = 1000);
			feedback wakeupOnSDOwrite(uint16 index);
			feedback wakeupOnSDOread(uint16 index);
			
			
			//	Object Dictionary
			Object_Dictionary& get_objectDictionary();
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
	
	
};