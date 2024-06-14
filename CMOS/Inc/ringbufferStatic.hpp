#pragma once

#include "defines.hpp"
#include "i_ringbuffer.hpp"
#include "array.hpp"





template<typename dataType, uint32 N>
class RingbufferStatic: public I_Ringbuffer<dataType>
{
	public:
		
		
		
		
		
		
		
	private:
		
		dataType m_data[N];
		uint32 m_head;
		uint32 m_tail;
		Buffer::e_state m_state;
		
		
		
		
		
	public:
		
		constexpr inline RingbufferStatic();
		inline ~RingbufferStatic() override;
		
		feedback write(const dataType& data) override;
		dataType read() override;
		inline void clear() override;
		
		inline bool is_dataAvailable() const override;
		uint32 get_numberOfUnread() const override;
		inline Buffer::e_state get_state() const override;
		inline uint32 get_size() const override;
		inline bool is_valid() const override;
		
		bool contains(const dataType& data) const override;
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

template<typename dataType, uint32 N>
constexpr inline RingbufferStatic<dataType, N>::RingbufferStatic()
	:	m_data(),
		m_head(0),
		m_tail(0),
		m_state(Buffer::e_state::EMPTY)
{
	static_assert(N > 0, "N must be greater than 0");
}


template<typename dataType, uint32 N>
inline RingbufferStatic<dataType, N>::~RingbufferStatic()
{
	
}







template<typename dataType, uint32 N>
CODE_RAM feedback RingbufferStatic<dataType, N>::write(const dataType& data)
{
	if(m_state == Buffer::e_state::FULL)
	{
		return(FAIL);
	}
	
	m_data[m_head] = data;
	
	m_head = (m_head + 1) % N;
	if(m_head == m_tail)
	{
		m_state = Buffer::e_state::FULL;
	}
	else
	{
		m_state = Buffer::e_state::NORMAL;
	}
	return(OK);
}


template<typename dataType, uint32 N>
CODE_RAM dataType RingbufferStatic<dataType, N>::read()
{
	dataType data = dataType();
	if(get_numberOfUnread() > 0)
	{
		data = m_data[m_tail];
		
		m_tail = (m_tail + 1) % N;
		if(m_tail == m_head)
		{
			m_state = Buffer::e_state::EMPTY;
		}
		else
		{
			m_state = Buffer::e_state::NORMAL;
		}
	}
	return(data);
}


template<typename dataType, uint32 N>
inline void RingbufferStatic<dataType, N>::clear()
{
	m_head = 0;
	m_tail = 0;
	m_state = Buffer::e_state::EMPTY;
}







template<typename dataType, uint32 N>
inline bool RingbufferStatic<dataType, N>::is_dataAvailable() const
{
	if(m_state == Buffer::e_state::EMPTY)
	{
		return(false);
	}
	return(true);
}


template<typename dataType, uint32 N>
CODE_RAM uint32 RingbufferStatic<dataType, N>::get_numberOfUnread() const
{
	if(m_state == Buffer::e_state::FULL)
	{
		return(N);
	}
	if(m_state == Buffer::e_state::EMPTY)
	{
		return(0);
	}
	
	uint64 size = N - m_tail;
	size += m_head;
	if(size > N)
	{
		size -= N;
	}
	return(size);
}


template<typename dataType, uint32 N>
inline Buffer::e_state RingbufferStatic<dataType, N>::get_state() const
{
	return(m_state);
}


template<typename dataType, uint32 N>
inline uint32 RingbufferStatic<dataType, N>::get_size() const
{
	return(N);
}


template<typename dataType, uint32 N>
inline bool RingbufferStatic<dataType, N>::is_valid() const
{
	if(m_data != nullptr)
	{
		return(true);
	}
	return(false);
}







template<typename dataType, uint32 N>
bool RingbufferStatic<dataType, N>::contains(const dataType& data) const
{
	uint32 numberOfData = get_numberOfUnread();
	for(uint32 i = 0; i < numberOfData; i++)
	{
		uint32 index = (((uint64) m_tail) + i) % N;
		if(m_data[index] == data)
		{
			return(true);
		}
	}
	return(false);
}