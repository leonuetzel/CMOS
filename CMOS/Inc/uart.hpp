#pragma once

#include "i_serial.hpp"
#include "i_dma.hpp"





class UART: public I_Serial<uint8>
{
	public:
		
		enum class e_databits
		{
			_7,
			_8,
			_9
		};
		
		enum class e_stopbits
		{
			_1      = 0x0,
			_0_5		= 0x1,
			_2      = 0x2,
			_1_5		= 0x3
		};
		
		enum class e_parity
		{
			NONE    = 0x0,
			EVEN    = 0x2,
			ODD	    = 0x3
		};
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		
		
		
		//	Friends
		
		
		
		
		
		
	protected:
		
		UART(void* txDataRegister);
		UART(const UART& uart) = delete;
		inline ~UART();
		
		bool m_initialized;
		RingbufferExternal<uint8> m_txBuffer;
		const uint16 m_eventID_dataReceived;
		I_DMA* m_dma_tx;
		const void* m_txDataRegister;
		
		
		
		
		
	public:
		
		virtual feedback init(uint32 baud, e_databits databits, e_stopbits stopbits, e_parity parity, uint8* rxBuffer, uint32 rxBufferSize, uint8* txBuffer, uint32 txBufferSize) = 0;
		
		uint8 rx() override;
		feedback tx() override;
		inline void clear() override;
		
		inline uint32 get_numberOfUnread() const override;
		inline bool is_empty() const override;
		inline bool is_full() const override;
		inline bool contains(const uint8& data) const override;
		
		inline UART& operator<<(char data) override;
		inline UART& operator<<(uint8 data) override;
		inline UART& operator<<(uint16 data) override;
		inline UART& operator<<(uint32 data) override;
		inline UART& operator<<(int8 data) override;
		inline UART& operator<<(int16 data) override;
		inline UART& operator<<(int32 data) override;
		inline UART& operator<<(float data) override;
		inline UART& operator<<(double data) override;
		inline UART& operator<<(const Array<uint8>& data) override;
		inline UART& operator<<(const String& data) override;
		
		inline UART& operator>>(char& data) override;
		inline UART& operator>>(uint8& data) override;
		inline UART& operator>>(uint16& data) override;
		inline UART& operator>>(uint32& data) override;
		inline UART& operator>>(int8& data) override;
		inline UART& operator>>(int16& data) override;
		inline UART& operator>>(int32& data) override;
		inline UART& operator>>(float& data) override;
		inline UART& operator>>(double& data) override;
		inline UART& operator>>(String& data) override;
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

inline UART::~UART()
{
	
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

inline void UART::clear()
{
	return(RingbufferExternal<uint8>::clear());
}







inline uint32 UART::get_numberOfUnread() const
{
	return(RingbufferExternal<uint8>::get_numberOfUnread());
}


inline bool UART::is_empty() const
{
	return(RingbufferExternal<uint8>::is_empty());
}


inline bool UART::is_full() const
{
	return(RingbufferExternal<uint8>::is_full());
}


inline bool UART::contains(const uint8& data) const
{
	return(RingbufferExternal<uint8>::contains(data));
}







inline UART& UART::operator<<(char data)
{
	m_txBuffer.write(data);
	return(*this);
}


inline UART& UART::operator<<(uint8 data)
{
	m_txBuffer.write(data);
	return(*this);
}


inline UART& UART::operator<<(uint16 data)
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


inline UART& UART::operator<<(uint32 data)
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


inline UART& UART::operator<<(int8 data)
{
	return(operator<<((uint8) data));
}


inline UART& UART::operator<<(int16 data)
{
	return(operator<<((uint16) data));
}


inline UART& UART::operator<<(int32 data)
{
	return(operator<<((uint32) data));
}


inline UART& UART::operator<<(float data)
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


inline UART& UART::operator<<(double data)
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


inline UART& UART::operator<<(const Array<uint8>& data)
{
	for(auto& i: data)
	{
		operator<<(i);
	}
	return(*this);
}


inline UART& UART::operator<<(const String& data)
{
	for(auto& i: data)
	{
		operator<<(i);
	}
	return(*this);
}








inline UART& UART::operator>>(char& data)
{
	data = rx();
	return(*this);
}


inline UART& UART::operator>>(uint8& data)
{
	data = rx();
	return(*this);
}


inline UART& UART::operator>>(uint16& data)
{
	uint8 buffer[2];
	for(auto& i: buffer)
	{
		i = rx();
	}
	data = bit::pack::_2x8Bit_to_1x16Bit(buffer);
	return(*this);
}


inline UART& UART::operator>>(uint32& data)
{
	uint8 buffer[4];
	for(auto& i: buffer)
	{
		i = rx();
	}
	data = bit::pack::_4x8Bit_to_1x32Bit(buffer);
	return(*this);
}


inline UART& UART::operator>>(int8& data)
{
	return(operator>>((uint8&) data));
}


inline UART& UART::operator>>(int16& data)
{
	return(operator>>((uint16&) data));
}


inline UART& UART::operator>>(int32& data)
{
	return(operator>>((uint32&) data));
}


inline UART& UART::operator>>(float& data)
{
	uint8 buffer[4];
	for(auto& i: buffer)
	{
		i = rx();
	}
	data = bit::pack::_4x8Bit_to_float(buffer);
	return(*this);
}


inline UART& UART::operator>>(double& data)
{
	uint8 buffer[8];
	for(auto& i: buffer)
	{
		i = rx();
	}
	data = bit::pack::_8x8Bit_to_double(buffer);
	return(*this);
}


inline UART& UART::operator>>(String& data)
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