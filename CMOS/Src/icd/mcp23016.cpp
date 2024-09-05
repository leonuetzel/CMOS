#include "../../Inc/cmos.hpp"





namespace ICD
{
	/*****************************************************************************/
	/*                    Globals and Static Initialization					 						 */
	/*****************************************************************************/
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Private	  			 						 						 */
	/*****************************************************************************/
	
	uint8 MCP23016::read_register(REG address)
	{
		//	Send Start Condition
		if(m_i2c.start(slaveAddressCustomized, true, 1) != OK)
		{
			return(0);
		}
		
		
		//	Send Register Address to read from
		if(m_i2c.tx((uint8) address) != OK)
		{
			return(0);
		}
		
		
		//	Send another Start Condition with read Bit this Time
		if(m_i2c.start(slaveAddressCustomized, false, 1) != OK)
		{
			return(0);
		}
		
		
		//	Read Data
		return(m_i2c.rx());
	}
	
	
	feedback MCP23016::write_register(REG address, uint8 data)
	{
		//	Send Start Condition
		if(m_i2c.start(slaveAddressCustomized, true, 2) != OK)
		{
			return(FAIL);
		}
		
		
		//	Send Register Address to write to
		if(m_i2c.tx((uint8) address) != OK)
		{
			return(FAIL);
		}
		
		
		//	Transmit Data
		return(m_i2c.tx(data));
	}
	
	
	
	/*****************************************************************************/
	/*                      						Public	  			 						 						 */
	/*****************************************************************************/
	
	MCP23016::MCP23016(I_I2C& i2c, bool a2, bool a1, bool a0)
		:	m_i2c(i2c),
			slaveAddressCustomized(slaveAddress)
	{
		if(a2 == true)
		{
			bit::set(slaveAddressCustomized, 2);
		}
		if(a1 == true)
		{
			bit::set(slaveAddressCustomized, 1);
		}
		if(a0 == true)
		{
			bit::set(slaveAddressCustomized, 0);
		}
	}
	
	
	MCP23016::~MCP23016()
	{
		
	}
	
	
	
	
	
	
	
	feedback MCP23016::init_pin(uint8 pin, bool output)
	{
		//	Determine Register to modify
		REG address = REG::IODIR0;
		if(pin > 7)
		{
			address = REG::IODIR1;
		}
		
		
		//	Read Register
		const uint8 IODIR = read_register(address);
		
		
		//	Modify Register
		uint8 IODIR_modified = IODIR;
		if(output == true)
		{
			bit::clear(IODIR_modified, pin % 8);
		}
		else
		{
			bit::set(IODIR_modified, pin % 8);
		}
		
		
		//	Write Register only if needed
		if(IODIR != IODIR_modified)
		{
			return(write_register(address, IODIR_modified));
		}
		return(OK);
	}
	
	
	feedback MCP23016::set_pin(uint8 pin)
	{
		//	Boundary Check
		if(pin >= numberOfPins)
		{
			return(FAIL);
		}
		
		
		//	Determine Register to modify
		REG address = REG::GP0;
		if(pin > 7)
		{
			address = REG::GP1;
		}
		
		
		//	Read Register
		const uint8 GP = read_register(address);
		
		
		//	Modify Register
		uint8 GP_modified = GP;
		bit::set(GP_modified, pin % 8);
		
		
		//	Write Register
		if(GP != GP_modified)
		{
			return(write_register(address, GP_modified));
		}
		return(OK);
	}
	
	
	feedback MCP23016::clear_pin(uint8 pin)
	{
		//	Boundary Check
		if(pin >= numberOfPins)
		{
			return(FAIL);
		}
		
		
		//	Determine Register to modify
		REG address = REG::GP0;
		if(pin > 7)
		{
			address = REG::GP1;
		}
		
		
		//	Read Register
		const uint8 GP = read_register(address);
		
		
		//	Modify Register
		uint8 GP_modified = GP;
		bit::clear(GP_modified, pin % 8);
		
		
		//	Write Register
		if(GP != GP_modified)
		{
			return(write_register(address, GP_modified));
		}
		return(OK);
	}
	
	
	feedback MCP23016::toggle_pin(uint8 pin)
	{
		//	Boundary Check
		if(pin >= numberOfPins)
		{
			return(FAIL);
		}
		
		
		if(get_pinLevel(pin) == true)
		{
			return(clear_pin(pin));
		}
		return(set_pin(pin));
	}
	
	
	bool MCP23016::get_pinLevel(uint8 pin)
	{
		//	Boundary Check
		if(pin >= numberOfPins)
		{
			return(FAIL);
		}
		
		
		//	Determine Register to read
		REG address = REG::GP0;
		if(pin > 7)
		{
			address = REG::GP1;
		}
		
		
		//	Read Register
		const uint8 GP = read_register(address);
		return(bit::isSet(GP, pin % 8));
	}
};