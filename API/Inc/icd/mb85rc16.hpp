#pragma once

#include "../defines.hpp"
#include "../i_i2c.hpp"
#include "../i_nvm.hpp"





namespace ICD
{
	class MB85RC16: public I_NVM
	{
		public:
			
			static constexpr uint32 sizeInBytes = 2048;
			
			
			
			
			
		private:
			
			//	Static Member
			static constexpr uint8 slaveAddress = 0x50;
			
			
			//	Non-static Member
			I_I2C& m_i2c;
			
			
			//	Constructor and Destructor
			
			
			
			//	Member Functions
			uint8 operator[](uint32 address) const;
			feedback operator()(uint32 address, uint8 data);
			
			
			//	Friends
			
			
			
			
			
			
		public:
			
			MB85RC16(I_I2C& i2c);
			MB85RC16(const MB85RC16& mb85rc16) = delete;
			~MB85RC16();
			
			feedback write(uint32 address, int8 data) override;
			feedback write(uint32 address, uint8 data) override;
			feedback write(uint32 address, int16 data) override;
			feedback write(uint32 address, uint16 data) override;
			feedback write(uint32 address, int32 data) override;
			feedback write(uint32 address, uint32 data) override;
			feedback write(uint32 address, int64 data) override;
			feedback write(uint32 address, uint64 data) override;
			feedback write(uint32 address, float data) override;
			feedback write(uint32 address, double data) override;
			
			int8 read_int8(uint32 address) override;
			uint8 read_uint8(uint32 address) override;
			int16 read_int16(uint32 address) override;
			uint16 read_uint16(uint32 address) override;
			int32 read_int32(uint32 address) override;
			uint32 read_uint32(uint32 address) override;
			int64 read_int64(uint32 address) override;
			uint64 read_uint64(uint32 address) override;
			float read_float(uint32 address) override;
			double read_double(uint32 address) override;
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