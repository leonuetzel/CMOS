#pragma once

#include "defines.hpp"
#include "i_ringbuffer.hpp"
#include "array.hpp"





template<typename dataType>
class RingbufferExternal: public I_Ringbuffer<dataType>
{
	public:
		
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	protected:
		
		dataType* m_data;
		uint32 m_size;
		uint32 m_head;
		uint32 m_tail;
		Buffer::e_state m_state;
		
		
		
		
		
	public:
		
		constexpr inline RingbufferExternal(dataType* buffer, uint32 size);
		inline ~RingbufferExternal() override;
		
		feedback write(const dataType& data) override;
		dataType read() override;
		inline void clear() override;
		
		inline bool is_dataAvailable() const override;
		uint32 get_numberOfUnread() const override;
		inline Buffer::e_state get_state() const override;
		inline uint32 get_size() const override;
		inline bool is_valid() const override;
		
		bool contains(const dataType& data) const override;
		constexpr inline dataType* get_data() const;
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

template<typename dataType>
constexpr inline RingbufferExternal<dataType>::RingbufferExternal(dataType* buffer, uint32 size)
	:	m_data(buffer),
		m_size(size),
		m_head(0),
		m_tail(0),
		m_state(Buffer::e_state::EMPTY)
{
	if(size == 0 || m_data == nullptr)
	{
		m_data = nullptr;
		m_size = 0;
	}
}


template<typename dataType>
inline RingbufferExternal<dataType>::~RingbufferExternal()
{
	
}







template<typename dataType>
CODE_RAM feedback RingbufferExternal<dataType>::write(const dataType& data)
{
	if(m_state == Buffer::e_state::FULL || m_size == 0 || m_data == nullptr)
	{
		return(FAIL);
	}
	
	m_data[m_head] = data;
	
	m_head = (m_head + 1) % m_size;
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


template<typename dataType>
CODE_RAM dataType RingbufferExternal<dataType>::read()
{
	dataType data = dataType();
	if(m_size == 0 || m_data == nullptr)
	{
		return(data);
	}
	
	if(get_numberOfUnread() > 0)
	{
		data = m_data[m_tail];
		
		m_tail = (m_tail + 1) % m_size;
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


template<typename dataType>
inline void RingbufferExternal<dataType>::clear()
{
	m_head = 0;
	m_tail = 0;
	m_state = Buffer::e_state::EMPTY;
}







template<typename dataType>
inline bool RingbufferExternal<dataType>::is_dataAvailable() const
{
	if(m_state == Buffer::e_state::EMPTY || m_size == 0 || m_data == nullptr)
	{
		return(false);
	}
	return(true);
}


template<typename dataType>
CODE_RAM uint32 RingbufferExternal<dataType>::get_numberOfUnread() const
{
	if(m_size == 0 || m_data == nullptr)
	{
		return(0);
	}
	
	if(m_state == Buffer::e_state::FULL)
	{
		return(m_size);
	}
	if(m_state == Buffer::e_state::EMPTY)
	{
		return(0);
	}
	
	uint64 size = m_size - m_tail;
	size += m_head;
	if(size > m_size)
	{
		size -= m_size;
	}
	return(size);
}


template<typename dataType>
inline Buffer::e_state RingbufferExternal<dataType>::get_state() const
{
	return(m_state);
}

template<typename dataType>
inline uint32 RingbufferExternal<dataType>::get_size() const
{
	return(m_size);
}


template<typename dataType>
inline bool RingbufferExternal<dataType>::is_valid() const
{
	if(m_size > 0 && m_data != nullptr)
	{
		return(true);
	}
	return(false);
}







template<typename dataType>
bool RingbufferExternal<dataType>::contains(const dataType& data) const
{
	if(m_size == 0 || m_data == nullptr)
	{
		return(false);
	}
	
	uint32 numberOfData = get_numberOfUnread();
	for(uint32 i = 0; i < numberOfData; i++)
	{
		uint32 index = (((uint64) m_tail) + i) % m_size;
		if(m_data[index] == data)
		{
			return(true);
		}
	}
	return(false);
}



template<typename dataType>
constexpr inline dataType* RingbufferExternal<dataType>::get_data() const
{
	return(m_data);
}