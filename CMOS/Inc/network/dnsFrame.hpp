#pragma once

#include "../cmos.hpp"





class DNS_Frame
{
	public:
		
		enum class e_opCode: uint8
		{
			STANDARD_QUERY		= 0x00,
			INVERSE_QUERY			= 0x01,
			STATUS						= 0x02,
			NOTIFY						= 0x04,
			UPDATE						= 0x05
		};
		
		enum class e_responseCode: uint8
		{
			NO_ERROR					= 0x00,
			FORMAT_ERROR			= 0x01,
			SERVER_FAILURE		= 0x02,
			NAME_ERROR				= 0x03,
			NOT_IMPLEMENTED		= 0x04,
			REFUSED						= 0x05,
			YX_DOMAIN					= 0x06,
			YX_RR_SET					= 0x07,
			NX_RR_SET					= 0x08,
			NOT_AUTHORITATIVE	= 0x09,
			NOT_ZONE					= 0x0A
		};
		
		enum class e_qType: uint16
		{
			A									= 0x0001,
			NS								= 0x0002,
			MD								= 0x0003,
			MF								= 0x0004,
			CNAME							= 0x0005,
			SOA								= 0x0006,
			MB								= 0x0007,
			MG								= 0x0008,
			MR								= 0x0009,
			NULL_							= 0x000A,
			WKS								= 0x000B,
			PTR								= 0x000C,
			HINFO							= 0x000D,
			MINFO							= 0x000E,
			MX								= 0x000F,
			TXT								= 0x0010,
			RP								= 0x0011,
			AFSDB							= 0x0012,
			X25								= 0x0013,
			ISDN							= 0x0014,
			RT								= 0x0015,
			NSAP							= 0x0016,
			NSAP_PTR					= 0x0017,
			SIG								= 0x0018,
			KEY								= 0x0019,
			PX								= 0x001A,
			GPOS							= 0x001B,
			AAAA							= 0x001C,
			LOC								= 0x001D,
			NXT								= 0x001E,
			EID								= 0x001F,
			NB_NIMLOC					= 0x0020,
			SRV_NBSTAT				= 0x0021,
			ATMA							= 0x0022,
			NAPTR							= 0x0023,
			KX								= 0x0024,
			CERT							= 0x0025,
			A6								= 0x0026,
			DNAME							= 0x0027,
			SINK							= 0x0028,
			OPT								= 0x0029,
			APL								= 0x002A,
			DS								= 0x002B,
			SSHFP							= 0x002C,
			IPSECKEY					= 0x002D,
			RRSIG							= 0x002E,
			NSEC							= 0x002F,
			DNSKEY						= 0x0030,
			DHCID							= 0x0031,
			NSEC3							= 0x0032,
			NSEC3PARAM				= 0x0033,
			TLSA							= 0x0034,
			SMIMEA						= 0x0035,
			HIP								= 0x0037,
			NINFO							= 0x0038,
			RKEY							= 0x0039,
			TALINK						= 0x003A,
			CDS								= 0x003B,
			CDNSKEY						= 0x003C,
			OPENPGKEY					= 0x003D,
			CSYNC							= 0x003E,
			ZONEMD						= 0x003F,
			SVCB							= 0x0040,
			HTTPS							= 0x0041,
			SPF								= 0x0063,
			UINFO							= 0x0064,
			UID								= 0x0065,
			GID								= 0x0066,
			UNSPEC						= 0x0067,
			NID								= 0x0068,
			L32								= 0x0069,
			L64								= 0x006A,
			LP								= 0x006B,
			EUI48							= 0x006C,
			EUI64							= 0x006D,
			TKEY							= 0x00F9,
			TSIG							= 0x00FA,
			IXFR							= 0x00FB,
			AXFR							= 0x00FC,
			MAILB							= 0x00FD,
			MAILA							= 0x00FE,
			ASTERISK					= 0x00FF,
			URI								= 0x0100,
			CAA								= 0x0101,
			DOA								= 0x0103,
			TA_								= 0x8000,
			DLV								= 0x8001
		};
		
		
		typedef struct
		{
			String name;
			e_qType type;
			uint16 questionClass;
		}s_question;
		
		typedef struct
		{
			String name;
			e_qType type;
			uint16 resourceClass;
			uint32 timeToLive;
			uint16 length;
			Array<uint8> data;
		}s_commonResourceRecord;
		
		
		
		uint16 ID;
		bool query;
		e_opCode opCode;
		bool authoritativeAnswer;
		bool truncation;
		bool recursionDesired;
		bool recursionAvailable;
		e_responseCode responseCode;
		Array<s_question> questions;
		Array<s_commonResourceRecord> answerRecords;
		Array<s_commonResourceRecord> authorityRecords;
		Array<s_commonResourceRecord> additionalRecords;
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		String decodeDomainName(const Array<uint8>& data, uint32& index) const;
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		DNS_Frame();
		DNS_Frame(const Array<uint8>& data);
		~DNS_Frame();
		
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

