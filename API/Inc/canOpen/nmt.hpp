#pragma once

#include "../cmos.hpp"
#include "objectDictionary.hpp"
#include "sdoServer.hpp"





namespace canOpen
{
	class NMT
	{
		public:
			
			enum class e_command: uint8
			{
				OPERATIONAL					= 0x01,
				STOP_NODE						= 0x02,
				PRE_OPERATIONAL			= 0x80,
				RESET_NODE					= 0x81,
				RESET_COMMUNICATION	= 0x82
			};
			
			enum class e_state: uint8
			{
				BOOT_UP							= 0x00,
				OPERATIONAL					= 0x05,
				PRE_OPERATIONAL			= 0x7F
			};
			
			
			
			
			
		private:
			
			//	Static Member
			
			
			
			//	Non-static Member
			I_CAN& m_can;
			const uint8 m_nodeID;
			e_state m_state;
			UniquePairArray<uint8, e_state> m_nodes;
			
			
			//	Constructor and Destructor
			NMT(I_CAN& can, const uint8& ownNodeID);
			NMT(const NMT& nmt) = delete;
			~NMT();
			
			
			//	Member Functions
			
			
			
			//	Friends
			friend class CanOpen;
			
			
			
			
			
		public:
			
			void handlePacket(const CAN_Frame& canFrame);
			
			const UniquePairArray<uint8, e_state>& get_nodes() const;
			feedback send_networkState();
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