#pragma once

#include "defines.hpp"
#include "i_ringbuffer.hpp"
#include "array.hpp"





template<typename dataType>
class RingbufferDynamic: public I_Ringbuffer<dataType>
{
	public:
		
		
		
		
		
		
		
	private:
		
		Array<dataType> m_data;
		uint32 m_head;
		uint32 m_tail;
		Buffer::e_state m_state;
		
		
		
		
		
	public:
		
		inline RingbufferDynamic(uint32 size, dataType defaultValue = dataType());
		inline ~RingbufferDynamic() override;
		
		feedback set_size(uint32 size);
		
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

template<typename dataType>
inline RingbufferDynamic<dataType>::RingbufferDynamic(uint32 size, dataType defaultValue)
	:	m_data(defaultValue, size),
		m_head(0),
		m_tail(0),
		m_state(Buffer::e_state::EMPTY)
{
	
}


template<typename dataType>
inline RingbufferDynamic<dataType>::~RingbufferDynamic()
{
	
}







template<typename dataType>
feedback RingbufferDynamic<dataType>::set_size(uint32 size)
{
	//	Check special Cases
	uint32 numberOfData = get_numberOfUnread();
	if(size < numberOfData)
	{
		return(FAIL);
	}
	
	const uint32 c_size = m_data.get_size();
	if(size == c_size)
	{
		return(OK);
	}
	
	
	//	Create new Array and copy Contents manually
	Array array(dataType(), size);
	
	
	//	Copy old Data to new Array
	for(uint32 i = 0; i < numberOfData; i++)
	{
		uint32 index = (((uint64) m_tail) + i) % c_size;
		array[i] = m_data[index];
	}
	m_data = array;
	m_tail = 0;
	m_head = numberOfData;
	
	
	//	Update State
	if(numberOfData == 0)
	{
		m_state = Buffer::e_state::EMPTY;
	}
	if(numberOfData == size)
	{
		m_state = Buffer::e_state::FULL;
	}
	if(numberOfData > 0 && numberOfData < size)
	{
		m_state = Buffer::e_state::NORMAL;
	}
	return(OK);
}







template<typename dataType>
CODE_RAM feedback RingbufferDynamic<dataType>::write(const dataType& data)
{
	const uint32 c_size = m_data.get_size();
	if(m_state == Buffer::e_state::FULL || c_size == 0)
	{
		return(FAIL);
	}
	
	m_data[m_head] = data;
	
	m_head = (m_head + 1) % c_size;
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
CODE_RAM dataType RingbufferDynamic<dataType>::read()
{
	dataType data = dataType();
	if(get_numberOfUnread() > 0)
	{
		data = m_data[m_tail];
		
		m_tail = (m_tail + 1) % m_data.get_size();
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
inline void RingbufferDynamic<dataType>::clear()
{
	m_head = 0;
	m_tail = 0;
	m_state = Buffer::e_state::EMPTY;
}







template<typename dataType>
inline bool RingbufferDynamic<dataType>::is_dataAvailable() const
{
	if(m_state == Buffer::e_state::EMPTY)
	{
		return(false);
	}
	return(true);
}


template<typename dataType>
CODE_RAM uint32 RingbufferDynamic<dataType>::get_numberOfUnread() const
{
	const uint32 c_size = m_data.get_size();
	if(c_size == 0)
	{
		return(0);
	}
	if(m_state == Buffer::e_state::FULL)
	{
		return(c_size);
	}
	if(m_state == Buffer::e_state::EMPTY)
	{
		return(0);
	}
	
	uint64 size = c_size - m_tail;
	size += m_head;
	if(size > c_size)
	{
		size -= c_size;
	}
	return(size);
}


template<typename dataType>
inline Buffer::e_state RingbufferDynamic<dataType>::get_state() const
{
	return(m_state);
}


template<typename dataType>
inline uint32 RingbufferDynamic<dataType>::get_size() const
{
	return(m_data.get_size());
}


template<typename dataType>
inline bool RingbufferDynamic<dataType>::is_valid() const
{
	if(m_data.get_size() > 0 && m_data.get_data() != nullptr)
	{
		return(true);
	}
	return(false);
}







template<typename dataType>
bool RingbufferDynamic<dataType>::contains(const dataType& data) const
{
	const uint32 c_size = m_data.get_size();
	if(c_size == 0)
	{
		return(false);
	}
	uint32 numberOfData = get_numberOfUnread();
	for(uint32 i = 0; i < numberOfData; i++)
	{
		uint32 index = (((uint64) m_tail) + i) % c_size;
		if(m_data[index] == data)
		{
			return(true);
		}
	}
	return(false);
}