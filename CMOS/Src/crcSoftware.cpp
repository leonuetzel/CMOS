#include "../Inc/cmos.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

uint32 CRC_Software::reflect(uint32 data, uint8 bits) const
{
	uint32 reflection = 0x00000000;
	
	for(uint8 i = 0; i < bits; i++)
	{
		if((data & 0x01) == 0x01)
		{
			reflection |= (1U << ((bits - 1) - i));
		}
		data = (data >> 1);
	}
	return(reflection);
}


I_CRC& CRC_Software::process(uint32 data)
{
	//	Check if Semaphore is locked
	CMOS& cmos = CMOS::get();
	if(cmos.semaphore_isOwnedByRunningThread(this) == false)
	{
		return(*this);
	}
	
	
	uint32 processedData = data;
	
	
	//	1. Input Data Reflection
	if(m_reverseInputData == true)
	{
		processedData = reflect(processedData, m_width);
	}
	
	
	//	2. CRC Calculation
	m_currentValue ^= processedData;
	
	
	//	3. Polynomial Division
	for(uint8 i = 0; i < m_width; i++)
	{
		if(bit::isSet(m_currentValue, m_width - 1) == true)
		{
			m_currentValue = (m_currentValue << 1) ^ m_polynomial;
		}
		else
		{
			m_currentValue <<= 1;
		}
	}
	
	
	//	4. Cutting CRC Value to Width
	m_currentValue &= ((1U << m_width) - 1);
	return(*this);
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

feedback CRC_Software::init(uint8 initialValue, uint8 polynomial, bool reverseOutputData, bool reverseInputData)
{
	//	Check if Semaphore is locked
	CMOS& cmos = CMOS::get();
	if(cmos.semaphore_isOwnedByRunningThread(this) == false)
	{
		return(FAIL);
	}
	
	
	//	CRC Settings
	m_width = 8;
	m_polynomial = polynomial;
	m_currentValue = initialValue;
	m_reverseOutputData = reverseOutputData;
	m_reverseInputData = reverseInputData;
	
	return(OK);
}


feedback CRC_Software::init(uint16 initialValue, uint16 polynomial, bool reverseOutputData, bool reverseInputData)
{
	//	Check if Semaphore is locked
	CMOS& cmos = CMOS::get();
	if(cmos.semaphore_isOwnedByRunningThread(this) == false)
	{
		return(FAIL);
	}
	
	
	//	CRC Settings
	m_width = 16;
	m_polynomial = polynomial;
	m_currentValue = initialValue;
	m_reverseOutputData = reverseOutputData;
	m_reverseInputData = reverseInputData;
	
	return(OK);
}


feedback CRC_Software::init(uint32 initialValue, uint32 polynomial, bool reverseOutputData, bool reverseInputData)
{
	//	Check if Semaphore is locked
	CMOS& cmos = CMOS::get();
	if(cmos.semaphore_isOwnedByRunningThread(this) == false)
	{
		return(FAIL);
	}
	
	
	//	CRC Settings
	m_width = 32;
	m_polynomial = polynomial;
	m_currentValue = initialValue;
	m_reverseOutputData = reverseOutputData;
	m_reverseInputData = reverseInputData;
	
	return(OK);
}







I_CRC& CRC_Software::operator<<(uint8 data)
{
	return(process(data));
}


I_CRC& CRC_Software::operator<<(uint16 data)
{
	return(process(data));
}


I_CRC& CRC_Software::operator<<(uint32 data)
{
	return(process(data));
}







uint32 CRC_Software::operator()() const
{
	//	Check if Semaphore is locked
	CMOS& cmos = CMOS::get();
	if(cmos.semaphore_isOwnedByRunningThread(this) == false)
	{
		return(0);
	}
	
	
	if(m_reverseOutputData == true)
	{
		return(reflect(m_currentValue, m_width));
	}
	
	
	return(m_currentValue);
}