#include "../../Inc/cmos.hpp"





namespace ICD
{
	/*****************************************************************************/
	/*                    Globals and Static Initialization					 						 */
	/*****************************************************************************/
	
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Private	  			 						 						 */
	/*****************************************************************************/
	
	uint8 MCP3428::read_configurationRegister()
	{
		if(m_i2c.start(m_address, false, 3) != OK)
		{
			m_i2c.stop();
			return(0);
		}
		uint8 data = 0;
		for(uint32 i = 0; i < 3; i++)
		{
			data = m_i2c.rx();
		}
		return(data);
	}
	
	
	
	/*****************************************************************************/
	/*                      						Public	  			 						 						 */
	/*****************************************************************************/
	
	MCP3428::MCP3428(I_I2C& i2c, bool a0, bool a1, bool a2)
		:	m_i2c(i2c),
			m_address(0x68),
			
			m_conversionMode(e_conversionMode::CONTINUOUS),
			m_channel(0),
			m_sampleRate(e_sampleRate::HIGH_240SPS_12BIT),
			m_gain(e_gain::x1)
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
		if(a2 == true)
		{
			bit::set(m_address, 2);
		}
		
		
		//	Reset Chip Settings to default
		if(m_i2c.start(m_address, true, 1) != OK)
		{
			m_i2c.stop();
			return;
		}
		if(m_i2c.tx(0x10) != OK)
		{
			m_i2c.stop();
			return;
		}
	}
	
	
	MCP3428::~MCP3428()
	{
		
	}
	
	
	
	
	
	
	
	
	feedback MCP3428::set_conversionMode(e_conversionMode mode)
	{
		if(m_conversionMode == mode)
		{
			return(OK);
		}
		
		
		//	Assemble Configuration Register
		const uint8 configurationRegister = (m_channel << 5) | (((uint8) mode) << 4) | (((uint8) m_sampleRate) << 2) | ((uint8) m_gain);
		
		
		//	Write Configuration Register
		if(m_i2c.start(m_address, true, 1) != OK)
		{
			m_i2c.stop();
			return(FAIL);
		}
		if(m_i2c.tx(configurationRegister) != OK)
		{
			m_i2c.stop();
			return(FAIL);
		}
		m_conversionMode = mode;
		return(OK);
	}
	
	
	feedback MCP3428::set_channel(uint8 channel)
	{
		if(m_channel == channel)
		{
			return(OK);
		}
		if(channel >= c_numberOfChannels)
		{
			return(FAIL);
		}
		
		
		//	Assemble Configuration Register
		const uint8 configurationRegister = (channel << 5) | (((uint8) m_conversionMode) << 4) | (((uint8) m_sampleRate) << 2) | ((uint8) m_gain);
		
		
		//	Write Configuration Register
		if(m_i2c.start(m_address, true, 1) != OK)
		{
			m_i2c.stop();
			return(FAIL);
		}
		if(m_i2c.tx(configurationRegister) != OK)
		{
			m_i2c.stop();
			return(FAIL);
		}
		m_channel = channel;
		return(OK);
	}
	
	
	feedback MCP3428::set_sampleRate(e_sampleRate sampleRate)
	{
		if(m_sampleRate == sampleRate)
		{
			return(OK);
		}
		
		
		//	Assemble Configuration Register
		const uint8 configurationRegister = (m_channel << 5) | (((uint8) m_conversionMode) << 4) | (((uint8) sampleRate) << 2) | ((uint8) m_gain);
		
		
		//	Write Configuration Register
		if(m_i2c.start(m_address, true, 1) != OK)
		{
			m_i2c.stop();
			return(FAIL);
		}
		if(m_i2c.tx(configurationRegister) != OK)
		{
			m_i2c.stop();
			return(FAIL);
		}
		m_sampleRate = sampleRate;
		return(OK);
	}
	
	
	feedback MCP3428::set_gain(e_gain gain)
	{
		if(m_gain == gain)
		{
			return(OK);
		}
		
		
		//	Assemble Configuration Register
		const uint8 configurationRegister = (m_channel << 5) | (((uint8) m_conversionMode) << 4) | (((uint8) m_sampleRate) << 2) | ((uint8) gain);
		
		
		//	Write Configuration Register
		if(m_i2c.start(m_address, true, 1) != OK)
		{
			m_i2c.stop();
			return(FAIL);
		}
		if(m_i2c.tx(configurationRegister) != OK)
		{
			m_i2c.stop();
			return(FAIL);
		}
		m_gain = gain;
		return(OK);
	}
	
	
	
	
	
	
	
	int16 MCP3428::oneshot()
	{
		if(m_conversionMode == e_conversionMode::CONTINUOUS)
		{
			return(operator()());
		}
		
		
		//	Assemble Configuration Register
		const uint8 configurationRegister = 0x80 | (m_channel << 5) | (((uint8) m_conversionMode) << 4) | (((uint8) m_sampleRate) << 2) | ((uint8) m_gain);
		
		
		//	Write Configuration Register
		if(m_i2c.start(m_address, true, 1) != OK)
		{
			m_i2c.stop();
			return(0);
		}
		if(m_i2c.tx(configurationRegister) != OK)
		{
			m_i2c.stop();
			return(0);
		}
		
		
		CMOS& cmos = CMOS::get();
		while(bit::isSet(read_configurationRegister(), 7) == true)
		{
			cmos.sleep_ms(5);
		}
		return(operator()());
	}
	
	
	int16 MCP3428::operator()()
	{
		if(m_i2c.start(m_address, false, 3) != OK)
		{
			m_i2c.stop();
			return(0);
		}
		uint8 data[3];
		for(uint32 i = 0; i < 3; i++)
		{
			data[i] = m_i2c.rx();
		}
		
		
		const int16 value = (data[0] << 8) | data[1];
		return(value);
	}
};