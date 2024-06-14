#pragma once

#include "../cmos.hpp"





class ARP_Frame
{
	public:
		
		enum class e_hardwareType: uint16
		{
			ETHERNET_10MB								= 0x0001,
			EXPERIMENTAL_ETHERNET_3MB		= 0x0002,
			AMATEUR_RADIO_AX25					= 0x0003,
			PROTEON_ProNET_TOKEN_RING		= 0x0004,
			CHAOS												= 0x0005,
			IEEE802											= 0x0006,
			ARCNET											= 0x0007,
			HYPERCHANNEL								= 0x0008,
			LAN_STAR										= 0x0009,
			AUTONET_SHORT_ADDRESS				= 0x000A,
			LOCAL_TALK									= 0x000B,
			LOCAL_NET										= 0x000C,
			ULTRA_LINK									= 0x000D,
			SMDS												= 0x000E,
			FRAME_RELAY									= 0x000F,
			ATM													= 0x0010,
			HDLC												= 0x0011,
			FIBRE_CHANNEL								= 0x0012,
			ATM_												= 0x0013,
			SERIAL_LINE									= 0x0014,
			ATM__												= 0x0015,
			MIL_STD_188_220							= 0x0016,
			METRICOM										= 0x0017,
			IEEE1394_1995								= 0x0018,
			MAPOS												= 0x0019,
			TWIN_AXIAL									= 0x001A,
			EUI64												= 0x001B,
			HIPARP											= 0x001C,
			IP_AND_ARC_OVER_ISO7816_3		= 0x001D,
			ARPSec											= 0x001E,
			IPsec_TUNNEL								= 0x001F,
			INIFINI_BAND								= 0x0020,
			TIA102_CAI									= 0x0021,
			WIEGAND											= 0x0022,
			PURE_IP											= 0x0023,
			HW_EXP1											= 0x0024,
			HFI													= 0x0025,
			UNIFIED_BUS									= 0x0026,
			HW_EXP2											= 0x0100,
			A_ETHERNET									= 0x0101
		};
		
		enum class e_protocolType: uint16
		{
			UNKNOWN											= 0x0000,
			IPv4												= 0x0800
		};
		
		enum class e_operationCode: uint16
		{
			REQUEST											= 0x0001,
			REPLY												= 0x0002,
			REQUEST_REVERSE							= 0x0003,
			REPLY_REVERSE								= 0x0004,
			DRARP_REQUEST								= 0x0005,
			DRARP_REPLY									= 0x0006,
			DRARP_ERROR									= 0x0007,
			InARP_REQUEST								= 0x0008,
			InARP_REPLY									= 0x0009,
			ARP_NAK											= 0x000A,
			MARS_REQUEST								= 0x000B,
			MARS_MULTI									= 0x000C,
			MARS_MSERV									= 0x000D,
			MARS_JOIN										= 0x000E,
			MARS_LEAVE									= 0x000F,
			MARS_NAK										= 0x0010,
			MARS_UNSERV									= 0x0011,
			MARS_SJOIN									= 0x0012,
			MARS_SLEAVE									= 0x0013,
			MARS_GROUPLIST_REQUEST			= 0x0014,
			MARS_GROUPLIST_REPLY				= 0x0015,
			MARS_REDIRECT_MAP						= 0x0016,
			MAPOS_UNARP									= 0x0017,
			OP_EXP1											= 0x0018,
			OP_EXP2											= 0x0019
		};
		
		
		e_hardwareType hardwareType;
		e_protocolType protocolType;
		uint8 hardwareAddressLength;
		uint8 protocolAddressLength;
		e_operationCode operationCode;
		uint8 senderHardwareAddress[6];
		uint8 senderProtocolAddress[4];
		uint8 targetHardwareAddress[6];
		uint8 targetProtocolAddress[4];
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		ARP_Frame();
		ARP_Frame(const Array<uint8>& data);
		ARP_Frame(const ARP_Frame& arpFrame) = delete;
		inline ~ARP_Frame();
		
		Array<uint8> convertToByteStream();
};



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

inline ARP_Frame::~ARP_Frame()
{
	
}



/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

