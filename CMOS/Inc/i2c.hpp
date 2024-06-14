#pragma once

#include "defines.hpp"
#include "ringbufferDynamic.hpp"





class I2C: public I_Serial<uint8>
{
	public:
		
		enum class e_mode
		{
			STANDARD_100KHZ,
			FAST_400KHZ,
			FAST_MODE_PLUS_1000KHZ,
			HIGH_SPEED_MODE_3400KHZ,
			ULTRA_FAST_MODE_5000KHZ
		};
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		
		
		
		//	Friends
		
		
		
		
		
	protected:
		
		I2C(void* txDataRegister);
		I2C(const I2C& i2c) = delete;
		inline ~I2C();
		
		bool m_initialized;
		RingbufferExternal<uint8> m_txBuffer;
		const uint16 m_eventID_dataReceived;
		I_DMA* m_dma_tx;
		const void* m_txDataRegister;
		
		
		
		
		
	public:
		
		virtual feedback init(e_mode mode, uint8* rxBuffer, uint32 rxBufferSize, uint8* txBuffer, uint32 txBufferSize) = 0;
		
		virtual feedback start(uint8 slaveAddress, bool write) = 0;
		virtual void stop() = 0;
		
		uint8 rx() override;
		feedback tx() override;
		inline void clear() override;
		
		inline bool is_dataAvailable() const override;
		inline uint32 get_numberOfUnread() const override;
		inline Buffer::e_state get_state() const override;
		inline bool contains(const uint8& data) const override;
		
		inline I2C& operator<<(char data) override;
		inline I2C& operator<<(uint8 data) override;
		inline I2C& operator<<(uint16 data) override;
		inline I2C& operator<<(uint32 data) override;
		inline I2C& operator<<(int8 data) override;
		inline I2C& operator<<(int16 data) override;
		inline I2C& operator<<(int32 data) override;
		inline I2C& operator<<(float data) override;
		inline I2C& operator<<(double data) override;
		inline I2C& operator<<(const Array<uint8>& data) override;
		inline I2C& operator<<(const String& data) override;
		
		inline I2C& operator>>(char& data) override;
		inline I2C& operator>>(uint8& data) override;
		inline I2C& operator>>(uint16& data) override;
		inline I2C& operator>>(uint32& data) override;
		inline I2C& operator>>(int8& data) override;
		inline I2C& operator>>(int16& data) override;
		inline I2C& operator>>(int32& data) override;
		inline I2C& operator>>(float& data) override;
		inline I2C& operator>>(double& data) override;
		inline I2C& operator>>(String& data) override;
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

inline I2C::~I2C()
{
	
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

inline void I2C::clear()
{
	m_tail = m_head;
	m_state = Buffer::e_state::EMPTY;
}







inline bool I2C::is_dataAvailable() const
{
	return(RingbufferExternal<uint8>::is_dataAvailable());
}


inline uint32 I2C::get_numberOfUnread() const
{
	return(RingbufferExternal<uint8>::get_numberOfUnread());
}


inline Buffer::e_state I2C::get_state() const
{
	return(RingbufferExternal<uint8>::get_state());
}


inline bool I2C::contains(const uint8& data) const
{
	return(RingbufferExternal<uint8>::contains(data));
}







inline I2C& I2C::operator<<(char data)
{
	m_txBuffer.write(data);
	return(*this);
}


inline I2C& I2C::operator<<(uint8 data)
{
	m_txBuffer.write(data);
	return(*this);
}


inline I2C& I2C::operator<<(uint16 data)
{
	uint8 buffer[2];
	bit::pack::_1x16Bit_to_2x8Bit(data, buffer);
	
	for(auto& i: buffer)
	{
		if(m_txBuffer.write(i) != OK)
		{
			break;
		}
	}
	return(*this);
}


inline I2C& I2C::operator<<(uint32 data)
{
	uint8 buffer[4];
	bit::pack::_1x32Bit_to_4x8Bit(data, buffer);
	
	for(auto& i: buffer)
	{
		if(m_txBuffer.write(i) != OK)
		{
			break;
		}
	}
	return(*this);
}


inline I2C& I2C::operator<<(int8 data)
{
	return(operator<<((uint8) data));
}


inline I2C& I2C::operator<<(int16 data)
{
	return(operator<<((uint16) data));
}


inline I2C& I2C::operator<<(int32 data)
{
	return(operator<<((uint32) data));
}


inline I2C& I2C::operator<<(float data)
{
	uint8 buffer[4];
	bit::pack::float_to_4x8Bit(data, buffer);
	
	for(uint32 i = 0; i < 4; i++)
	{
		if(m_txBuffer.write(buffer[i]) != OK)
		{
			break;
		}
	}
	return(*this);
}


inline I2C& I2C::operator<<(double data)
{
	uint8 buffer[8];
	bit::pack::double_to_8x8Bit(data, buffer);
	
	for(uint32 i = 0; i < 8; i++)
	{
		if(m_txBuffer.write(buffer[i]) != OK)
		{
			break;
		}
	}
	return(*this);
}


inline I2C& I2C::operator<<(const Array<uint8>& data)
{
	for(auto& i: data)
	{
		operator<<(i);
	}
	return(*this);
}


inline I2C& I2C::operator<<(const String& data)
{
	for(auto& i: data)
	{
		operator<<(i);
	}
	return(*this);
}








inline I2C& I2C::operator>>(char& data)
{
	data = rx();
	return(*this);
}


inline I2C& I2C::operator>>(uint8& data)
{
	data = rx();
	return(*this);
}


inline I2C& I2C::operator>>(uint16& data)
{
	uint8 buffer[2];
	for(auto& i: buffer)
	{
		i = rx();
	}
	data = bit::pack::_2x8Bit_to_1x16Bit(buffer);
	return(*this);
}


inline I2C& I2C::operator>>(uint32& data)
{
	uint8 buffer[4];
	for(auto& i: buffer)
	{
		i = rx();
	}
	data = bit::pack::_4x8Bit_to_1x32Bit(buffer);
	return(*this);
}


inline I2C& I2C::operator>>(int8& data)
{
	return(operator>>((uint8&) data));
}


inline I2C& I2C::operator>>(int16& data)
{
	return(operator>>((uint16&) data));
}


inline I2C& I2C::operator>>(int32& data)
{
	return(operator>>((uint32&) data));
}


inline I2C& I2C::operator>>(float& data)
{
	uint8 buffer[4];
	for(auto& i: buffer)
	{
		i = rx();
	}
	data = bit::pack::_4x8Bit_to_float(buffer);
	return(*this);
}


inline I2C& I2C::operator>>(double& data)
{
	uint8 buffer[8];
	for(auto& i: buffer)
	{
		i = rx();
	}
	data = bit::pack::_8x8Bit_to_double(buffer);
	return(*this);
}


inline I2C& I2C::operator>>(String& data)
{
	while(1)
	{
		char character = rx();
		if(character == 0)
		{
			break;
		}
		data += character;
	}
	return(*this);
}