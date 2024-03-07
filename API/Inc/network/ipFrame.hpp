#pragma once

#include "../cmos.hpp"





class IP_Frame
{
	public:
		
		enum class e_protocol: uint8
		{
			HOPOPT									= 0x00,
			ICMP										= 0x01,
			IGMP										= 0x02,
			GGP											= 0x03,
			IPinIP									= 0x04,
			ST 											= 0x05,
			TCP											= 0x06,
			CBT											= 0x07,
			EGP											= 0x08,
			IGP											= 0x09,
			BBN_RCC_MON							= 0x0A,
			NVP_II									= 0x0B,
			PUP											= 0x0C,
			ARGUS										= 0x0D,
			EMCON										= 0x0E,
			XNET										= 0x0F,
			CHAOS										= 0x10,
			UDP											= 0x11,
			MUX 										= 0x12,
			DCN_MEAS								= 0x13,
			HMP											= 0x14,
			PRM											= 0x15,
			XNS_IDP									= 0x16,
			TRUNK_1									= 0x17,
			TRUNK_2									= 0x18,
			LEAF_1									= 0x19,
			LEAF_2									= 0x1A,
			RDP											= 0x1B,
			IRTP										= 0x1C,
			ISO_TP4									= 0x1D,
			NETBLT									= 0x1E,
			MFE_NSP									= 0x1F,
			MERIT_INP								= 0x20,
			DCCP										= 0x21,
			_3PC										= 0x22,
			IDPR										= 0x23,
			XTP											= 0x24,
			DDP											= 0x25,
			IDPR_CMTP								= 0x26,
			TP_PLUSPLUS							= 0x27,
			IL											= 0x28,
			IPv6										= 0x29,
			SDRP										= 0x2A,
			IPv6_ROUTE							= 0x2B,
			IPv6_FRAG								= 0x2C,
			IDRP										= 0x2D,
			RSVP										= 0x2E,
			GRE											= 0x2F,
			DSR											= 0x30,
			BNA											= 0x31,
			ESP											= 0x32,
			AH											= 0x33,
			I_NLSP									= 0x34,
			SwIPe										= 0x35,
			NARP										= 0x36,
			MOBILE									= 0x37,
			TLSP										= 0x38,
			SKIP										= 0x39,
			IPv6_ICMP								= 0x3A,
			IPv6_NoNxt							= 0x3B,
			IPv6_Opts								= 0x3C,
			HOST_INTERNAL						= 0x3D,
			CFTP										= 0x3E,
			LOCAL_NETWORK						= 0x3F,
			SAT_EXPAK								= 0x40,
			KRYPTOLAN								= 0x41,
			RVD											= 0x42,
			IPPC										= 0x43,
			DISTRIBUTED_FILE_SYSTEM	= 0x44,
			SAT_MON									= 0x45,
			VISA										= 0x46,
			IPCU										= 0x47,
			CPNX										= 0x48,
			CPHB										= 0x49,
			WSN											= 0x4A,
			PVP											= 0x4B,
			BR_SAT_MON							= 0x4C,
			SUN_ND									= 0x4D,
			WB_MON									= 0x4E,
			WB_EXPAK								= 0x4F,
			ISO_IP									= 0x50,
			VMTP										= 0x51,
			SECURE_VMTP							= 0x52,
			VINES										= 0x53,
			TTP_IPTM								= 0x54,
			NSFNET_IGP							= 0x55,
			DGP											= 0x56,
			TCF											= 0x57,
			EIGRP										= 0x58,
			OSPF										= 0x59,
			Sprite_RPC							= 0x5A,
			LARP										= 0x5B,
			MTP											= 0x5C,
			AX25										= 0x5D,
			OS											= 0x5E,
			MICP										= 0x5F,
			SCC_SP									= 0x60,
			ETHERIP									= 0x61,
			ENCAP										= 0x62,
			PRIVATE_ENCRYPTION			= 0x63,
			GMTP										= 0x64,
			IFMP										= 0x65,
			PNNI										= 0x66,
			PIM											= 0x67,
			ARIS										= 0x68,
			SCPS										= 0x69,
			QNX											= 0x6A,
			A_N											= 0x6B,
			IPComp									= 0x6C,
			SNP											= 0x6D,
			Compaq_Peer							= 0x6E,
			IPXinIP									= 0x6F,
			VRRP										= 0x70,
			PGM											= 0x71,
			_0_HOP									= 0x72,
			L2TP										= 0x73,
			DDX											= 0x74,
			IATP										= 0x75,
			STP											= 0x76,
			SRP											= 0x77,
			UTI											= 0x78,
			SMP											= 0x79,
			SM											= 0x7A,
			PTP											= 0x7B,
			IS_ISoverIPv4						= 0x7C,
			FIRE										= 0x7D,
			CRTP										= 0x7E,
			CRUDP										= 0x7F,
			SSCOPMCE								= 0x80,
			IPLT										= 0x81,
			SPS											= 0x82,
			PIPE										= 0x83,
			SCTP										= 0x84,
			FC											= 0x85,
			RSVP_E2E_IGNORE					= 0x86,
			MobilityHeader					= 0x87,
			UDP_Lite								= 0x88,
			MPLSinIP								= 0x89,
			MANET										= 0x8A,
			HIP											= 0x8B,
			Shim6										= 0x8C,
			WESP										= 0x8D,
			ROHC										= 0x8E,
			ETHERNET								= 0x8F,
			AGGFRAG									= 0x90,
			NSH											= 0x91,
			TESTING									= 0xFD,
			TESTING_								= 0xFE,
			RESERVED								= 0xFF
		};
		
		enum class e_optionClass: uint8
		{
			CONTROL										= 0,
			DEBUGGING_AND_MEASUREMENT	= 2
		};
		
		enum class e_option: uint8
		{
			END_OF_OPTIONS			= 0x00,
			NOP									= 0x01,
			SECURITY_DEFUNCT		= 0x02,
			RECORD_ROUTE				= 0x07,
			ZSU									= 0x0A,
			MTU_PROBE						= 0x0B,
			MTU_REPLY						= 0x0C,
			ENCODE							= 0x0F,
			QUICK_START					= 0x19,
			EXP									= 0x1E,
			TIMESTAMP						= 0x44,
			TRACE_ROUTE					= 0x52,
			EXP_								= 0x5E,
			SEC_RIPSO						= 0x82,
			LOOSE_SOURCE_ROUTE	= 0x83,
			EXTENDED_SECURITY		= 0x85,
			CIPSO								= 0x86,
			STREAM_ID						= 0x88,
			STRICT_SOURCE_ROUTE	= 0x89,
			VISA								= 0x8E,
			IMITD								= 0x90,
			EXTENDED_IP					= 0x91,
			ADDRESS_EXTENSION		= 0x93,
			ROUTER_ALERT				= 0x94,
			SDB									= 0x95,
			DPS									= 0x97,
			UMP									= 0x98,
			EXP__								= 0x9E,
			FINN								= 0xCD,
			EXP___							= 0xDE
		};
		
		enum class e_DSCP: uint8
		{
			CS0							= 0x00,
			LE							= 0x01,
			CS1_AF11_TO_13	= 0x02,
			CS2_AF21_TO_23	= 0x03,
			CS3_AF31_TO_33	= 0x04,
			CS4_AF41_TO_43	= 0x05,
			CS6							= 0x06,
			CS7							= 0x07
		};
		
		enum class e_ECN: uint8
		{
			NOT_CAPABLE		= 0x00,
			ECT_1					= 0x01,
			ECT_2					= 0x02,
			CE						= 0x03
		};
		
		
		typedef struct
		{
			bool copied;
			e_optionClass optionClass;
			e_option option;
			Array<uint8> data;
		}s_option;
		
		uint8 version;
		uint8 internetHeaderLength;
		e_DSCP dscp;
		e_ECN ecn;
		uint16 totalLength;
		uint16 identification;
		bool dontFragment;
		bool moreFragments;
		uint16 fragmentOffset;
		uint8 timeToLive;
		e_protocol protocol;
		uint16 headerChecksum;
		uint8 sourceAddress[4];
		uint8 destinationAddress[4];
		Array<s_option> options;
		Array<uint8> payload;
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		uint16 calculate_checksum(const Array<uint8>& data);
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		IP_Frame();
		IP_Frame(const Array<uint8>& data);
		~IP_Frame();
		
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

