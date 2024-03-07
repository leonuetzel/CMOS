#pragma once

#include "../cmos.hpp"
#include "ipFrame.hpp"





class IP
{
	public:
		
		
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		const uint8& m_networkID;
		Array<uint8> m_ipAddress;
		uint16 m_datagramIdentificationCounter;
		Array<IP_Frame*> m_packetBuffer;
		
		
		//	Constructor and Destructor
		IP(const uint8& networkID);
		IP(const IP& ip) = delete;
		~IP();
		
		
		//	Member Functions
		const IP_Frame* handleFragmentedPacket(uint16 datagramIdentification);
		void packetHandler(const IP_Frame& ipFrame);
		
		
		//	Friends
		friend class Network;
		
		
		
		
		
	public:
		
		void handlePacket(const IP_Frame& ipFrame);
		
		feedback set_ipAddress(const Array<uint8>& ipAddress);
		inline const Array<uint8>& get_ipAddress() const;
		
		feedback tx(const Array<uint8>& data, IP_Frame::e_protocol protocol, const Array<uint8>& destinationIpAddress);
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

inline const Array<uint8>& IP::get_ipAddress() const
{
	return(m_ipAddress);
}