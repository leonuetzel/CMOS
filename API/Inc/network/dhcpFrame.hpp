#pragma once

#include "../cmos.hpp"





class DHCP_Frame
{
	public:
		
		enum class e_hardwareType: uint8
		{
			ETHERNET_10MB						= 0x01,
			IEEE802									= 0x06,
			ARCNET									= 0x07,
			LOCAL_TALK							= 0x0B,
			LOCAL_NET								= 0x0C,
			SMDS										= 0x0E,
			FRAME_RELAY							= 0x0F,
			ATM											= 0x10,
			HDLC										= 0x11,
			FIBRE_CHANNEL						= 0x12,
			ATM_										= 0x13,
			SERIAL_LINE							= 0x14
		};
		
		enum class e_option: uint8
		{
			PADDING																						= 0x00,
			SUBNET_MASK																				= 0x01,
			TIME_OFFSET																				= 0x02,
			ROUTER																						= 0x03,
			TIME_SERVER																				= 0x04,
			IEN116_NAME_SERVER																= 0x05,
			DNS_NAME_SERVER																		= 0x06,
			LOG_SERVER																				= 0x07,
			COOKIE_SERVER																			= 0x08,
			LPR_SERVER																				= 0x09,
			IMPRESS_SERVER																		= 0x0A,
			RESOURCE_LOCATION_SERVER													= 0x0B,
			HOST_NAME																					= 0x0C,
			BOOT_FILE_SIZE																		= 0x0D,
			MERIT_DUMP_FILE																		= 0x0E,
			DOMAIN_NAME																				= 0x0F,
			SWAP_SERVER																				= 0x10,
			ROOT_PATH																					= 0x11,
			EXTENSIONS_PATH																		= 0x12,
			IP_FORWARDING_ENABLE_DISABLE											= 0x13,
			NON_LOCAL_SOURCE_ROUTINGE_ENABLE_DISABLE					= 0x14,
			POLICY_FILTER																			= 0x15,
			MAXIMUM_DATAGRAM_REASSEMBLY_SIZE									= 0x16,
			DEFAULT_IP_TIME_TO_LIVE														= 0x17,
			PATH_MTU_AGING_TIMEOUT														= 0x18,
			PATH_MTU_PLATEAU_TABLE														= 0x19,
			INTERFACE_MTU																			= 0x1A,
			ALL_SUBNETS_ARE_LOCAL															= 0x1B,
			BROADCAST_ADDRESS																	= 0x1C,
			PERFORM_MASK_DISCOVERY														= 0x1D,
			MASK_SUPPLIER																			= 0x1E,
			PERFORM_ROUTER_DISCOVERY													= 0x1F,
			ROUTER_SOLICITATION_ADDRESS												= 0x20,
			STATIC_ROUTE																			= 0x21,
			TRAILER_ENCAPSULATION															= 0x22,
			ARP_CACHE_TIMEOUT																	= 0x23,
			ETHERNET_ENCAPSULATION														= 0x24,
			DEFAULT_TTL																				= 0x25,
			TCP_KEEPALIVE_INTERVAL														= 0x26,
			TCP_KEEPALIVE_GARBAGE															= 0x27,
			NETWORK_INFORMATION_SERVICE_DOMAIN								= 0x28,
			NETWORK_INFORMATION_SERVERS												= 0x29,
			NETWORK_TIME_PROTOCOL_SERVERS											= 0x2A,
			VENDOR_SPECIFIC_INFORMATION												= 0x2B,
			NetBIOS_OVER_TCP_IP_NAME_SERVERS									= 0x2C,
			NetBIOS_OVER_TCP_IP_DATAGRAM_DISTRIBUTION_SERVERS	= 0x2D,
			NetBIOS_OVER_TCP_IP_NODE_TYPE											= 0x2E,
			NetBIOS_OVER_TCP_IP_SCOPE													= 0x2F,
			X_WINDOW_SYSTEM_FONT_SERVERS											= 0x30,
			X_WINDOW_SYSTEM_DISPLAY_MANAGER										= 0x31,
			REQUESTED_IP_ADDRESS															= 0x32,
			IP_ADDRESS_LEASE_TIME															= 0x33,
			OPTION_OVERLOAD																		= 0x34,
			DHCP_MESSAGE_TYPE																	= 0x35,
			SERVER_IDENTIFIER																	= 0x36,
			PARAMETER_REQUEST_LIST														= 0x37,
			MESSAGE																						= 0x38,
			MAXIMUM_DHCP_MESSAGE_SIZE													= 0x39,
			RENEWAL_TIME_VALUE																= 0x3A,
			REBINDING_TIME_VALUE															= 0x3B,
			VENDOR_CLASS_IDENTIFIER														= 0x3C,
			CLIENT_IDENTIFIER																	= 0x3D,
			NETWORK_INFORMATION_SERVICE_PLUS_DOMAIN						= 0x40,
			NETWORK_INFORMATION_SERVICE_PLUS_SERVERS					= 0x41,
			TFTP_SERVER_NAME																	= 0x42,
			BOOT_FILE_NAME																		= 0x43,
			MOBILE_IP_HOME_AGENT															= 0x44,
			SMTP_SERVERS																			= 0x45,
			POP3_SERVERS																			= 0x46,
			NNTP_SERVERS																			= 0x47,
			WWW_SERVERS																				= 0x48,
			DEFAULT_FINGER_SERVERS														= 0x49,
			DEFAULT_IRC_SERVERS																= 0x4A,
			STREET_TALK_SERVERS																= 0x4B,
			STREET_TALK_DIRECTORY_ASSISTANCE_SERVERS					= 0x4C,
			END_OF_OPTIONS																		= 0xFF
		};
		
		enum class e_dhcpMessageType: uint8
		{
			DISCOVER	= 1,
			OFFER			= 2,
			REQUEST		= 3,
			DECLINE		= 4,
			ACK				= 5,
			NAK				= 6,
			RELEASE		= 7,
			INFORM		= 8
		};
		
		typedef struct
		{
			e_option code;
			Array<uint8> data;
		}s_option;
		
		uint8 operationCode;
		e_hardwareType hardwareType;
		uint8 hardwareAddressLength;
		uint8 hops;
		uint32 xID;
		uint16 seconds;
		bool broadcast;
		uint8 clientIPaddress[4];
		uint8 yourIPaddress[4];
		uint8 serverIPaddress[4];
		uint8 gatewayIPaddress[4];
		uint8 clientHardwareAddress[16];
		uint8 serverName[64];
		uint8 bootFileName[128];
		uint8 magicCookie[4];
		Array<s_option*> options;
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		DHCP_Frame();
		DHCP_Frame(const Array<uint8>& data);
		~DHCP_Frame();
		
		Array<uint8> convertToByteStream();
		void sortOptionsForBetterAlignment();
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

