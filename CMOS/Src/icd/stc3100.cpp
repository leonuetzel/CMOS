#include "../../Inc/cmos.hpp"




namespace ICD
{
	/*****************************************************************************/
	/*                    Globals and Static Initialization					 						 */
	/*****************************************************************************/
	
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Private	  			 						 						 */
	/*****************************************************************************/
	
	Array<uint8> STC3100::register_read(REG address, uint32 numberOfRegisters)
	{
		Array<uint8> data;
		
		
		//	Boundary Check
		if(numberOfRegisters == 0)
		{
			return(data);
		}
		
		
		//	Transmit Register Address
		if(m_i2c.start(slaveAddress, true) != OK)
		{
			return(data);
		}
		m_i2c << (uint8) address;
		if(m_i2c.tx() != OK)
		{
			return(data);
		}
		
		
		//	Readout Registers
		if(m_i2c.start(slaveAddress, false) != OK)
		{
			return(data);
		}
		for(uint32 i = 0; i < numberOfRegisters; i++)
		{
			data += m_i2c.rx();
		}
		return(data);
	}
	
	
	feedback STC3100::register_write(REG address, const Array<uint8>& data)
	{
		const uint32 numberOfRegisters = data.get_size();
		if(numberOfRegisters == 0)
		{
			return(OK);
		}
		
		
		//	Transmit Register Address
		if(m_i2c.start(slaveAddress, true) != OK)
		{
			return(FAIL);
		}
		m_i2c << (uint8) address;
		if(m_i2c.tx() != OK)
		{
			return(FAIL);
		}
		
		
		//	Write Register Data
		m_i2c << data;
		return(m_i2c.tx());
	}
	
	
	
	/*****************************************************************************/
	/*                      						Public	  			 						 						 */
	/*****************************************************************************/
	
	STC3100::STC3100(I2C& i2c, I_CRC& crc, float shuntResistance)
		:	m_i2c(i2c),
			m_crc(crc),
			m_shuntResistance(shuntResistance)
	{
		//	Read 8 Byte Unique Device ID Registers
		const Array<uint8> ID = register_read(REG::ID0, 8);
		if(ID.get_size() != 8)
		{
			return;
		}
		
		
		//	Check CRC of Unique Device ID
		m_crc.init((uint8) 0x00, 0x07);
		for(uint32 i = 0; i < 7; i++)
		{
			m_crc << ID[i];
		}
		if(m_crc() != ID[7])
		{
			return;
		}
		
		
		//	Save Unique Device ID for later
		for(uint32 i = 0; i < 8; i++)
		{
			m_ID[i] = ID[i];
		}
		
		
		//	Start Measurements
		if(run() != OK)
		{
			CMOS::get().reset();
		}
	}
	
	
	STC3100::~STC3100()
	{
		
	}
	
	
	
	
	
	
	
	float STC3100::read_temperature()
	{
		const Array<uint8> rawData = register_read(REG::TEMPERATURE_LOW, 2);
		if(rawData.get_size() != 2)
		{
			return(FAIL);
		}
		
		
		//	Assemble Temperature
		const int16 temperatureRaw = (rawData[1] << 8) | rawData[0];
		const float temperature = temperatureRaw * 0.125f;
		
		
		return(temperature);
	}
	
	
	float STC3100::read_voltage()
	{
		const Array<uint8> rawData = register_read(REG::VOLTAGE_LOW, 2);
		if(rawData.get_size() != 2)
		{
			return(FAIL);
		}
		
		
		//	Assemble Voltage
		const int16 voltageRaw = (rawData[1] << 8) | rawData[0];
		const float voltage = voltageRaw * 0.00244f;
		
		
		return(voltage);
	}
	
	
	float STC3100::read_current()
	{
		const Array<uint8> rawData = register_read(REG::CURRENT_LOW, 2);
		if(rawData.get_size() != 2)
		{
			return(FAIL);
		}
		
		
		//	Assemble Current
		int16 currentRaw = (rawData[1] << 8) | rawData[0];
		
		
		//	Convert from 14 Bit to 16 Bit 2's Complement
		if(bit::isSet(currentRaw, 13) == true)
		{
			currentRaw |= 0xC000;
		}
		
		
		const float current = currentRaw * 0.00001177f / m_shuntResistance;
		return(current);
	}
	
	
	float STC3100::read_charge()
	{
		//	Raw Value's Unit is 11.77µV
		//	The Accumulator inside the STC3100 is 28 Bit wide, reading the CHARGE_LOW and CHARGE_HIGH Registers delivers the upper 16 Bit
		//	Therefore the Resolution of the raw Values is 11.77µV x 2^12 = 48.20992 mV
		
		//	Conversion Time t_conv is 125ms (12 Bit), 250ms(13 Bit) or 500ms(14 Bit) so that one Measurement is integrated (= multiplicated) with (t_conv / 3600) x 1h
		//	We choose 14 Bit Resolution (default), so t_conv = 500ms
		//	This makes one LSB equal to 48.20992mV x (500ms / 3600) x 1h = 6.695822222 µVh
		//	Divided by the Shunt Resistance delivers the Value in mAh
		//	The maximum measureable Amount of Charge calculates to: ± 2^15 (not 16 Bit because of Sign Bit) x 6.695822222 µVh / R_shunt [Ohm]
		
		constexpr float lsbResolution = 11.77e-6 * (1 << 12);
		constexpr float t_conv = 0.5;
		constexpr float lsbIntegrated = lsbResolution * t_conv / 3600;
		
		
		const float lsbCharge = lsbIntegrated / m_shuntResistance;
		//const float chargeMaximum = (1 << 15) * lsbCharge;
		
		
		
		
		
		//	Read Data
		const Array<uint8> rawData = register_read(REG::CHARGE_LOW, 2);
		if(rawData.get_size() != 2)
		{
			return(FAIL);
		}
		
		
		//	Assemble Charge
		const int16 chargeRaw = (rawData[1] << 8) | rawData[0];
		const float charge = chargeRaw * lsbCharge;
		
		
		return(charge);
	}
	
	
	uint16 STC3100::read_conversionCounter()
	{
		const Array<uint8> rawData = register_read(REG::COUNTER_LOW, 2);
		if(rawData.get_size() != 2)
		{
			return(FAIL);
		}
		
		
		//	Assemble Counter
		const uint16 counter = (rawData[1] << 8) | rawData[0];
		
		
		return(counter);
	}
	
	
	
	
	
	
	
	feedback STC3100::resetAccumulatorAndConversionCounter()
	{
		Array<uint8> CTRL = register_read(REG::CTRL);
		if(CTRL.get_size() != 1)
		{
			return(FAIL);
		}
		bit::clear(CTRL[0], 4);
		bit::set(CTRL[0], 1);
		return(register_write(REG::CTRL, CTRL));
	}
	
	
	
	
	
	
	
	feedback STC3100::run()
	{
		Array<uint8> MODE = register_read(REG::MODE);
		if(MODE.get_size() != 1)
		{
			return(FAIL);
		}
		
		bit::set(MODE[0], 4);
		if(register_write(REG::MODE, MODE) != OK)
		{
			return(FAIL);
		}
		
		if(bit::isCleared(register_read(REG::MODE)[0], 4) == true)
		{
			return(FAIL);
		}
		return(OK);
	}
	
	
	feedback STC3100::standby()
	{
		Array<uint8> MODE = register_read(REG::MODE);
		if(MODE.get_size() != 1)
		{
			return(FAIL);
		}
		
		bit::clear(MODE[0], 4);
		if(register_write(REG::MODE, MODE) != OK)
		{
			return(FAIL);
		}
		
		if(bit::isSet(register_read(REG::MODE)[0], 4) == true)
		{
			return(FAIL);
		}
		return(OK);
	}
};