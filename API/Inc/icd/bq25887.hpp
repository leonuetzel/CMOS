#pragma once

#include "../defines.hpp"
#include "../i_i2c.hpp"





namespace ICD
{
	class BQ25887
	{
		public:
			
			enum class e_watchdogI2C
			{
				DISABLED						= 0x00,
				ENABLED_40_SECONDS	= 0x01,
				ENABLED_80_SECONDS	= 0x02,
				ENABLED_160_SECONDS	= 0x03
			};
			
			enum class e_chargerState
			{
				NOT_CHARGING						= 0x0,
				TRICKLE_CHARGE					= 0x1,
				PRE_CHARGE							= 0x2,
				FAST_CHARGE							= 0x3,
				TAPER_CHARGE						= 0x4,
				TOP_OFF_TIMER_CHARGING	= 0x5,
				CHARGE_TERMINATION_DONE	= 0x6
			};
			
			enum class e_ICOState
			{
				DISABLED												= 0x0,
				OPTIMIZATION_IN_PROGRESS				= 0x1,
				MAXIMUM_INPUT_CURRENT_DETECTED	= 0x2
			};
			
			enum class e_vbusDetectionState
			{
				NO_INPUT														= 0x0,
				USB_HOST_SDP												= 0x1,
				USB_CDP_1A5													= 0x2,
				ADAPTER_3A													= 0x3,
				POORSRC_DETECTED_7_TIMES						= 0x4,
				UNKNOWN_ADAPTER_0A5									= 0x5,
				NON_STANDARD_ADAPTER_1A_2A_2A1_2A4	= 0x6
			};
			
			
			typedef struct
			{
				e_chargerState chargerState;
				bool i2c_watchdogExpired;
				bool thermalRegulationActive;
				bool inputVoltageLimitRegulationActive;
				bool inputCurrentLimitRegulationActive;
				e_ICOState icoState;
				e_vbusDetectionState vBusDetectionState;
				bool powerGood;
			}s_state;
			
			
			
			
			
		private:
			
			//	Static Member
			static constexpr uint8 slaveAddress = 0x6A;
			
			enum class REG: uint8
			{
				CELL_VOLTAGE_LIMIT								= 0x00,
				CHARGE_CURRENT_LIMIT							= 0x01,
				INPUT_VOLTAGE_LIMIT								= 0x02,
				INPUT_CURRENT_LIMIT								= 0x03,
				PRECHARGE_AND_TERMINATION_CONTROL	= 0x04,
				CHARGER_CONTROL_1									= 0x05,
				CHARGER_CONTROL_2									= 0x06,
				CHARGER_CONTROL_3									= 0x07,
				CHARGER_CONTROL_4									= 0x08,
				
				ICO_CURRENT_LIMIT									= 0x0A,
				CHARGER_STATUS_1									= 0x0B,
				CHARGER_STATUS_2									= 0x0C,
				NTC_STATUS												= 0x0D,
				FAULT_STATUS											= 0x0E,
				CHARGER_FLAG_1										= 0x0F,
				CHARGER_FLAG_2										= 0x10,
				FAULT_FLAG												= 0x11,
				CHARGER_MASK_1										= 0x12,
				CHARGER_MASK_2										= 0x13,
				FAULT_MASK												= 0x14,
				ADC_CONTROL												= 0x15,
				ADC_FUNCTION_DISABLE							= 0x16,
				IBUS_ADC1													= 0x17,
				IBUS_ADC0													= 0x18,
				ICHG_ADC1													= 0x19,
				ICHG_ADC0													= 0x1A,
				VBUS_ADC1													= 0x1B,
				VBUS_ADC0													= 0x1C,
				VBAT_ADC1													= 0x1D,
				VBAT_ADC0													= 0x1E,
				VCELLTOP_ADC1											= 0x1F,
				VCELLTOP_ADC0											= 0x20,
				TS_ADC1														= 0x21,
				TS_ADC0														= 0x22,
				TDIE_ADC1													= 0x23,
				TDIE_ADC0													= 0x24,
				PART_INFORMATION									= 0x25,
				VCELLBOT_ADC1											= 0x26,
				VCELLBOT_ADC0											= 0x27,
				CELL_BALANCING_CONTROL_1					= 0x28,
				CELL_BALANCING_CONTROL_2					= 0x29,
				CELL_BALANCING_STATUS_AND_CONTROL	= 0x2A,
				CELL_BALANCING_FLAG								= 0x2B,
				CELL_BALANCING_MASK								= 0x2C
			};
			
			
			//	Non-static Member
			I_I2C& m_i2c;
			
			
			//	Constructor and Destructor
			
			
			
			//	Member Functions
			uint8 register_read(REG address) const;
			feedback register_write(REG address, uint8 data);
			
			feedback register_bitSet(REG address, uint8 bit);
			feedback register_bitClear(REG address, uint8 bit);
			bool register_isbitSet(REG address, uint8 bit);
			bool register_isbitCleared(REG address, uint8 bit);
			
			
			//	Friends
			
			
			
			
			
			
		public:
			
			BQ25887(I_I2C& i2c);
			BQ25887(const BQ25887& bq25887) = delete;
			~BQ25887();
			
			feedback enableCharging(bool enable);
			
			feedback set_chargeCurrent(uint16 mA);
			feedback enablePinFunction_ILIM(bool enable);
			
			feedback setWatchdogI2C(e_watchdogI2C setting);
			feedback resetWatchdogI2C();
			
			s_state readState();
			float read_chargeCurrent();
			float read_batteryVoltage();
			float read_cellVoltagesTop();
			float read_cellVoltagesBottom();
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
	
	
};