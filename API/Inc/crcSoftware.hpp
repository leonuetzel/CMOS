#pragma once

#include "i_crc.hpp"





class CRC_Software: public I_CRC
{
	public:
		
		
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		uint8 m_width;
		uint32 m_polynomial;
		bool m_reverseOutputData;
		bool m_reverseInputData;
		uint32 m_currentValue;
		
		
		//	Constructor and Destructor
		CRC_Software(const CRC_Software& crcSoftware) = delete;
		
		
		//	Member Functions
		uint32 reflect(uint32 data, uint8 bits) const;
		I_CRC& process(uint32 data);
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		constexpr inline CRC_Software();
		inline ~CRC_Software();
		
		feedback init(uint8 initialValue, uint8 polynomial, bool reverseOutputData = false, bool reverseInputData = false) override;
		feedback init(uint16 initialValue, uint16 polynomial, bool reverseOutputData = false, bool reverseInputData = false) override;
		feedback init(uint32 initialValue, uint32 polynomial, bool reverseOutputData = false, bool reverseInputData = false) override;
		
		I_CRC& operator<<(uint8 data) override;
		I_CRC& operator<<(uint16 data) override;
		I_CRC& operator<<(uint32 data) override;
		
		uint32 operator()() const override;
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

constexpr inline CRC_Software::CRC_Software()
	:	m_width(0),
		m_polynomial(0),
		m_reverseOutputData(false),
		m_reverseInputData(false),
		m_currentValue(0)
{
	
}


inline CRC_Software::~CRC_Software()
{
	
}