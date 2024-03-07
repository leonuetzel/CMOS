#pragma once

#include "../cmos.hpp"
#include "objectDictionary.hpp"





namespace canOpen
{
	class SDO_Server
	{
		public:
			
			
			
			
			
			
			
		private:
			
			//	Static Member
			enum class e_commandSpecifier: uint8
			{
				WRITE_REQUEST		= 0x1,
				READ						= 0x2,
				WRITE_RESPONSE	= 0x3,
				ABORT 					= 0x4
			};
			
			
			typedef struct
			{
				uint8 nodeID;
				uint16 index;
				uint8 subIndex;
				bool read;
				bool success;
				Array<uint8> data;
			}s_transfer;
			
			
			//	Non-static Member
			I_CAN& m_can;
			const uint8& m_nodeID;
			Object_Dictionary& m_objectDictionary;
			Array<s_transfer*> m_transfers;
			UniquePairArray<uint16, Array<uint8>> m_wakeUpListOnSDOwrite;
			UniquePairArray<uint16, Array<uint8>> m_wakeUpListOnSDOread;
			
			
			//	Constructor and Destructor
			SDO_Server(I_CAN& can, const uint8& nodeID, Object_Dictionary& objectDictionary);
			SDO_Server(const SDO_Server& sdoServer) = delete;
			~SDO_Server();
			
			
			
			//	Member Functions
			void process_readFromObjectDictionary(const CAN_Frame& canFrame);
			void process_writeToObjectDictionary(const CAN_Frame& canFrame);
			void process_abort(const CAN_Frame& canFrame);
			
			
			//	Friends
			friend class CanOpen;
			
			
			
			
			
		public:
			
			void handlePacket(const CAN_Frame& canFrame);
			
			Array<uint8> read(uint8 nodeID, uint16 index, uint8 subIndex, uint32 timeoutMilliseconds = 1000);
			feedback write(uint8 nodeID, uint16 index, uint8 subIndex, const Array<uint8>& data, uint32 timeoutMilliseconds = 1000);
			
			feedback wakeupOnSDOwrite(uint16 index);
			feedback wakeupOnSDOread(uint16 index);
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