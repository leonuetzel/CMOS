#pragma once

#include "../cmos.hpp"





class EthernetFrame
{
	public:
		
		enum class e_etherType: uint16
		{
			//	Non-standard Values
			SIZE																		= 0x0000,
			UNKNOWN																	= 0x0001,
			
			
			//	Standard Values
			IPv4																		= 0x0800,
			ARP 																		= 0x0806,
			WAKE_ON_LAN															= 0x0842,
			AVTP																		= 0x22F0,
			IETF_TRILL															= 0x22F3,
			STREAM_RESERVATION_PROTOCOL							= 0x22EA,
			DEC_MOP_RC															= 0x6002,
			DECnet_PHASE_IV_DNA_ROUTING							= 0x6003,
			DEC_LAT 																= 0x6003,
			RARP																		= 0x8035,
			ETHERTALK																= 0x809B,
			AARP																		= 0x80F3,
			VLAN_IEEE802_1Q_IEEE802_1AQ_NNI					= 0x8100,
			SLPP																		= 0x8102,
			VLACP																		= 0x8103,
			IPX																			= 0x8137,
			QNX_Qnet																= 0x8204,
			IPv6																		= 0x86DD,
			ETHERNET_FLOW_CONTROL										= 0x8808,
			ETHERNET_SLOW_PROTOCOLS									= 0x8809,
			COBRA_NET																= 0x8819,
			MPLS_UNICAST														= 0x8847,
			MPLS_MULTICAST													= 0x8848,
			PPPoE_DISCOVERY													= 0x8863,
			PPPoE_SESSION														= 0x8864,
			HOME_PLUG_1v0_MME												= 0x887B,
			EAP_OVER_LAN_IEEE802_1X									= 0x888E,
			PROFINET																= 0x8892,
			HYPER_SCSI															= 0x889A,
			ATA_OVER_ETHERNET												= 0x88A2,
			ETHERCAT																= 0x88A4,
			VLAN_STAG_Q_IN_Q_TUNNEL									= 0x88A8,
			ETHERNET_POWERLINK											= 0x88AB,
			GOOSE																		= 0x88B8,
			GSE_MANAGEMENT_SERVICES									= 0x88B9,
			SV																			= 0x88BA,
			MIKRO_TIK_RoMON													= 0x88BF,
			LLDP																		= 0x88CC,
			SERCOS_III															= 0x88CD,
			HOME_PLUG_GREEN_PHY											= 0x88E1,
			MEDIA_REDUNDANCY_PROTOCOL_IEC62439_2		= 0x88E3,
			MACsec_IEEE802_1AE											= 0x88E5,
			PBB_IEEE802_1AH													= 0x88E7,
			PTP																			= 0x88F7,
			NC_SI																		= 0x88F8,
			PRP																			= 0x88FB,
			CFM_IEEE802_1AG													= 0x8902,
			FCoE																		= 0x8906,
			FCoE_INITIALIZATION											= 0x8914,
			RoCE																		= 0x8915,
			TTE																			= 0x891D,
			HSR																			= 0x892F,
			IEEE_1905_1															= 0x893A,
			ETHERNET_CONFIGURATION_TESTING_PROTOCOL	= 0x9000,
			REDUNDANCY_TAG_IEEE802_1CB							= 0xF1C1
		};
		
		
		uint8 MAC_destination[6];
		uint8 MAC_source[6];
		e_etherType etherType;
		uint8 VLAN_Tag[4];
		Array<uint8> payload;
		uint32 crc;
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		uint32 calculate_checksum2(const Array<uint8>& data);
		uint32 calculate_checksum(const Array<uint8>& data);
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		EthernetFrame();
		EthernetFrame(const Array<uint8>& data);
		~EthernetFrame();
		
		Array<uint8> convertToByteStream();
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

