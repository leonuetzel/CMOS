#include "../../Inc/cmos.hpp"





namespace ICD
{
	/*****************************************************************************/
	/*                    Globals and Static Initialization					 						 */
	/*****************************************************************************/
	
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Private	  			 						 						 */
	/*****************************************************************************/
	
	uint8 BQ25887::register_read(REG address) const
	{
		//	Send Start Condition
		if(m_i2c.start(slaveAddress, true, 1) != OK)
		{
			return(0);
		}
		
		
		//	Transmit Register Address
		if(m_i2c.tx((uint8) address) != OK)
		{
			return(0);
		}
		
		
		//	Readout Register
		if(m_i2c.start(slaveAddress, false, 1) != OK)
		{
			return(0);
		}
		return(m_i2c.rx());
	}
	
	
	feedback BQ25887::register_write(REG address, uint8 data)
	{
		//	Send Start Condition
		if(m_i2c.start(slaveAddress, true, 2) != OK)
		{
			return(FAIL);
		}
		
		
		//	Transmit Register Address
		if(m_i2c.tx((uint8) address) != OK)
		{
			return(FAIL);
		}
		
		
		//	Transmit Data
		return(m_i2c.tx(data));
	}
	
	
	
	
	
	
	
	feedback BQ25887::register_bitSet(REG address, uint8 bit)
	{
		uint8 registerValue = register_read(address);
		bit::set(registerValue, bit);
		return(register_write(address, registerValue));
	}
	
	
	feedback BQ25887::register_bitClear(REG address, uint8 bit)
	{
		uint8 registerValue = register_read(address);
		bit::clear(registerValue, bit);
		return(register_write(address, registerValue));
	}
	
	
	bool BQ25887::register_isbitSet(REG address, uint8 bit)
	{
		const uint8 registerValue = register_read(address);
		return(bit::isSet(registerValue, bit));
	}
	
	
	bool BQ25887::register_isbitCleared(REG address, uint8 bit)
	{
		const uint8 registerValue = register_read(address);
		return(bit::isCleared(registerValue, bit));
	}
	
	
	
	/*****************************************************************************/
	/*                      						Public	  			 						 						 */
	/*****************************************************************************/
	
	BQ25887::BQ25887(I_I2C& i2c)
		:	m_i2c(i2c)
	{
		//	Check if we are talking with the correct Device
		const uint8 PART_INFORMATION = register_read(REG::PART_INFORMATION);
		const uint8 PART_NUMBER = (PART_INFORMATION & 0x78) >> 3;
		if(PART_NUMBER != 0x05)
		{
			return;
		}
		
		
		//	Disable I2C Watchdog
		register_bitClear(REG::CHARGER_CONTROL_1, 4);
		
		
		//	Enable ADC
		register_write(REG::ADC_CONTROL, 0x80);
		CMOS& cmos = CMOS::get();
		cmos.sleep_ms(100);
		if(register_isbitSet(REG::ADC_CONTROL, 7) == false)
		{
			return;
		}
		cmos.sleep_ms(100);
		
		
		//	Read Battery Voltage
		const int16 voltage_battery = (register_read(REG::VBAT_ADC1) << 8) | register_read(REG::VBAT_ADC0);
		if(voltage_battery > 5500)
		{
			const int16 voltage_cellTop = (register_read(REG::VCELLTOP_ADC1) << 8) | register_read(REG::VCELLTOP_ADC0);
			const int16 voltage_cellBot = (register_read(REG::VCELLBOT_ADC1) << 8) | register_read(REG::VCELLBOT_ADC0);
			
			if(voltage_cellTop < 2750 || voltage_cellBot < 2750)
			{
				cmos.sleep_ms(100);
			}
		}
	}
	
	
	BQ25887::~BQ25887()
	{
		
	}
	
	
	
	
	
	
	
	feedback BQ25887::enableCharging(bool enable)
	{
		if(enable == true)
		{
			if(register_bitSet(REG::CHARGER_CONTROL_2, 3) != OK)
			{
				return(FAIL);
			}
			if(register_isbitSet(REG::CHARGER_CONTROL_2, 3) != true)
			{
				return(FAIL);
			}
		}
		else
		{
			if(register_bitClear(REG::CHARGER_CONTROL_2, 3) != OK)
			{
				return(FAIL);
			}
			if(register_isbitCleared(REG::CHARGER_CONTROL_2, 3) != true)
			{
				return(FAIL);
			}
		}
		return(OK);
	}
	
	
	feedback BQ25887::set_chargeCurrent(uint16 mA)
	{
		if(mA > 2200)
		{
			return(FAIL);
		}
		
		
		const uint8 value = mA / 50;
		const uint8 CHARGE_CURRENT_LIMIT = (register_read(REG::CHARGE_CURRENT_LIMIT) & 0xC0) | value;
		register_write(REG::CHARGE_CURRENT_LIMIT, CHARGE_CURRENT_LIMIT);
		
		
		//	Check if Values have been written correctly
		if(register_read(REG::CHARGE_CURRENT_LIMIT) != CHARGE_CURRENT_LIMIT)
		{
			return(FAIL);
		}
		return(OK);
	}
	
	
	feedback BQ25887::enablePinFunction_ILIM(bool enable)
	{
		uint8 CHARGE_CURRENT_LIMIT = register_read(REG::CHARGE_CURRENT_LIMIT);
		if(enable == true)
		{
			bit::set(CHARGE_CURRENT_LIMIT, 6);
		}
		else
		{
			bit::clear(CHARGE_CURRENT_LIMIT, 6);
		}
		register_write(REG::CHARGE_CURRENT_LIMIT, CHARGE_CURRENT_LIMIT);
		
		
		//	Check if Values have been written correctly
		if(register_read(REG::CHARGE_CURRENT_LIMIT) != CHARGE_CURRENT_LIMIT)
		{
			return(FAIL);
		}
		return(OK);
	}
	
	
	
	
	
	
	
	feedback BQ25887::setWatchdogI2C(e_watchdogI2C setting)
	{
		uint8 CHARGER_CONTROL_1 = register_read(REG::CHARGER_CONTROL_1);
		CHARGER_CONTROL_1 &= 0xCF;
		CHARGER_CONTROL_1 |= ((uint8) setting) << 4;
		if(register_write(REG::CHARGER_CONTROL_1, CHARGER_CONTROL_1) != OK)
		{
			return(FAIL);
		}
		if(register_read(REG::CHARGER_CONTROL_1) != CHARGER_CONTROL_1)
		{
			return(FAIL);
		}
		return(OK);
	}
	
	
	feedback BQ25887::resetWatchdogI2C()
	{
		return(register_bitSet(REG::CHARGER_CONTROL_3, 6));
	}
	
	
	
	
	
	
	
	BQ25887::s_state BQ25887::readState()
	{
		const uint8 CHARGER_STATUS_1 = register_read(REG::CHARGER_STATUS_1);
		const uint8 CHARGER_STATUS_2 = register_read(REG::CHARGER_STATUS_2);
		
		s_state state;
		
		state.chargerState = (e_chargerState) (CHARGER_STATUS_1 & 0x07);
		state.i2c_watchdogExpired = bit::isSet(CHARGER_STATUS_1, 3);
		state.thermalRegulationActive = bit::isSet(CHARGER_STATUS_1, 4);
		state.inputVoltageLimitRegulationActive = bit::isSet(CHARGER_STATUS_1, 5);
		state.inputCurrentLimitRegulationActive = bit::isSet(CHARGER_STATUS_1, 6);
		
		state.icoState = (e_ICOState) ((CHARGER_STATUS_2 & 0x06) >> 1);
		state.vBusDetectionState = (e_vbusDetectionState) ((CHARGER_STATUS_2 & 0x70) >> 4);
		state.powerGood = bit::isSet(CHARGER_STATUS_2, 7);
		
		return(state);
	}
	
	
	float BQ25887::read_chargeCurrent()
	{
		const int16 chargeCurrent_mA = (register_read(REG::ICHG_ADC1) << 8) | register_read(REG::ICHG_ADC0);
		const float chargeCurrent = chargeCurrent_mA / 1000.0f;
		return(chargeCurrent);
	}
	
	
	float BQ25887::read_batteryVoltage()
	{
		const int16 voltage_battery_mV = (register_read(REG::VBAT_ADC1) << 8) | register_read(REG::VBAT_ADC0);
		const float voltage_battery = voltage_battery_mV / 1000.0f;
		return(voltage_battery);
	}
	
	
	float BQ25887::read_cellVoltagesTop()
	{
		const int16 voltage_mV = (register_read(REG::VCELLTOP_ADC1) << 8) | register_read(REG::VCELLTOP_ADC0);
		const float voltage = voltage_mV / 1000.0f;
		return(voltage);
	}
	
	
	float BQ25887::read_cellVoltagesBottom()
	{
		const int16 voltage_mV = (register_read(REG::VCELLBOT_ADC1) << 8) | register_read(REG::VCELLBOT_ADC0);
		const float voltage = voltage_mV / 1000.0f;
		return(voltage);
	}
};