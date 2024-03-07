#pragma once

#include "../cmos.hpp"





class TCP_Frame
{
	public:
		
		enum class e_optionKind: uint8
		{
			END_OF_OPTIONS																	= 0x00,
			NO_OPERATION																		= 0x01,
			MAXIMUM_SEGMENT_SIZE														= 0x02,
			WINDOW_SCALE																		= 0x03,
			SACK_PERMITTED																	= 0x04,
			SACK																						= 0x05,
			ECHO_OBSOLETED_BY_OPTION_0x08										= 0x06,
			ECHO_REPLY_OBSOLETED_BY_OPTION_0x08							= 0x07,
			TIMESTAMPS																			= 0x08,
			PARTIAL_ORDER_CONNECTION_PERMITTED_OBSOLETE			= 0x09,
			PARTIAL_ORDER_SERVICE_PROFILE_OBSOLETE					= 0x0A,
			CC_OBSOLETE																			= 0x0B,
			CC_NEW_OBSOLETE																	= 0x0C,
			CC_ECHO_OBSOLETE																= 0x0D,
			TCP_ALTERNATE_CHECKSUM_REQUEST_OBSOLETE					= 0x0E,
			TCP_ALTERNATE_CHECKSUM_DATA_OBSOLETE						= 0x0F,
			SKEETER																					= 0x10,
			BUBBA																						= 0x11,
			TRAILER_CHECKSUM_OPTION													= 0x12,
			MD5_SIGNATURE_OPTION_OBSOLETED_BY_OPTION_0x1D		= 0x13,
			SCPS_CAPABILITIES																= 0x14,
			SELECTIVE_NEGATIVE_ACKNOWLEDGEMENTS							= 0x15,
			RECORD_BOUNDARIES																= 0x16,
			CORRUPTION_EXPERIENCED													= 0x17,
			SNAP																						= 0x18,
			UNASSIGNED_RELEASED_2000_12_18									= 0x19,
			TCP_COMPRESSION_FILTER													= 0x1A,
			QUICK_START_RESPONSE														= 0x1B,
			USER_TIMEOUT_OPTION															= 0x1C,
			TCP_AUTHENTICATION_OPTION_TCP_AO								= 0x1D,
			MULTIPATH_TCP_MPTCP															= 0x1E,
			TCP_FAST_OPEN_COOKIE														= 0x22,
			ENCRYPTION_NEGOTATION_TCP_ENO										= 0x45,
			ACCURATE_ECN_ORDER_0_AccECN0_EXPIRES_2024_08_03	= 0xAC,
			ACCURATE_ECN_ORDER_1_AccECN1_EXPIRES_2024_08_03	= 0xAE,
			RFC3692_STYLE_EXPERIMENT_1											= 0xFD,
			RFC3692_STYLE_EXPERIMENT_2											= 0xFE
		};
		
		typedef struct
		{
			e_optionKind kind;
			Array<uint8> data;
		}s_option;
		
		uint16 sourcePort;
		uint16 destinationPort;
		uint32 sequenceNumber;
		uint32 ackNumber;
		uint8 dataOffset;
		bool congestionWindowReduced;
		bool explicitCongestionNotification;
		bool urgent;
		bool ack;
		bool push;
		bool reset;
		bool synchronize;
		bool finish;
		uint16 receiveWindow;
		uint16 checksum;
		uint16 urgentPointer;
		Array<s_option> options;
		Array<uint8> payload;
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		uint16 calculate_checksum(const Array<uint8>& data, const Array<uint8>& sourceIpAddress, const Array<uint8>& destinationIpAddress);
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		TCP_Frame();
		TCP_Frame(const Array<uint8>& data);
		~TCP_Frame();
		
		Array<uint8> convertToByteStream(const Array<uint8>& sourceIpAddress, const Array<uint8>& destinationIpAddress);
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

