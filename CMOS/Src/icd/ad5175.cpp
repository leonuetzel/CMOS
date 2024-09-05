#include "../../Inc/cmos.hpp"





namespace ICD
{
	/*****************************************************************************/
	/*                    Globals and Static Initialization					 						 */
	/*****************************************************************************/
	
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Private	  			 						 						 */
	/*****************************************************************************/
	
	feedback AD5175::sendCommand(e_command command, uint16 data)
	{
		//	I2C Start
		if(m_i2c.start(m_address, true, 2) != OK)
		{
			m_i2c.stop();
			return(FAIL);
		}
		
		
		//	Calculate Bytes to be sent
		const uint16 dataToSend = (((uint16) command) << 10) | (data & 0x03FF);
		const uint8 byteLow = dataToSend & 0x00FF;
		const uint8 byteHigh = (dataToSend & 0xFF00) >> 8;
		
		
		//	Send Data MSB first
		if(m_i2c.tx(byteHigh) != OK)
		{
			m_i2c.stop();
			return(FAIL);
		}
		if(m_i2c.tx(byteLow) != OK)
		{
			m_i2c.stop();
			return(FAIL);
		}
		return(OK);
	}
	
	
	uint16 AD5175::read_rdac()
	{
		if(sendCommand(e_command::READ_RDAC) != OK)
		{
			return(0xFFFF);
		}
		
		
		m_i2c.start(m_address, false, 2);
		const uint8 byteHigh = m_i2c.rx();
		const uint8 byteLow = m_i2c.rx();
		
		const uint16 data = (byteHigh << 8) | byteLow;
		return(data & 0x03FF);
	}
	
	
	uint16 AD5175::read_controlRegister()
	{
		if(sendCommand(e_command::READ_CONTROL_REGISTER) != OK)
		{
			return(0xFFFF);
		}
		
		
		m_i2c.start(m_address, false, 2);
		const uint8 byteHigh = m_i2c.rx();
		const uint8 byteLow = m_i2c.rx();
		
		const uint16 data = (byteHigh << 8) | byteLow;
		return(data & 0x03FF);
	}
	
	
	
	/*****************************************************************************/
	/*                      						Public	  			 						 						 */
	/*****************************************************************************/
	
	AD5175::AD5175(I_I2C& i2c, bool a0, bool a1)
		:	m_i2c(i2c),
			m_address(0x2F)
	{
		//	I2C Address
		if(a0 == true)
		{
			bit::set(m_address, 0);
		}
		if(a1 == true)
		{
			bit::set(m_address, 1);
		}
		
		
		//	Try to read RDAC
		//	It should be mid-scale after Reset
		if(read_rdac() != maximumValue / 2)
		{
			CMOS::get().reset();
		}
		
		
		//	Unlock Write Access to RDAC Register
		uint8 controlRegister = read_controlRegister();
		bit::set(controlRegister, 1);
		sendCommand(e_command::WRITE_CONTROL_REGISTER, controlRegister);
		if(read_controlRegister() != controlRegister)
		{
			//	Fail
			return;
		}
	}
	
	
	AD5175::~AD5175()
	{
		
	}
	
	
	
	
	
	
	
	feedback AD5175::write_rdac(uint16 value)
	{
		if(value > maximumValue)
		{
			return(FAIL);
		}
		if(sendCommand(e_command::WRITE_RDAC, value) != OK)
		{
			return(FAIL);
		}
		if(read_rdac() != value)
		{
			return(FAIL);
		}
		return(OK);
	}
	
	
	feedback AD5175::shutdown()
	{
		return(sendCommand(e_command::SHUTDOWN, 1));
	}
	
	
	feedback AD5175::run()
	{
		return(sendCommand(e_command::SHUTDOWN, 0));
	}
};