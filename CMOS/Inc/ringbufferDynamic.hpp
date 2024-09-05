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
		
		
		
		
		
	public:
		
		inline RingbufferDynamic(uint32 size = 0, dataType defaultValue = dataType());
		inline ~RingbufferDynamic() override;
		
		feedback set_size(uint32 size);
		
		feedback write(const dataType& data) override;
		dataType read() override;
		inline void clear() override;
		inline void reset() override;
		
		uint32 get_numberOfUnread() const override;
		inline bool is_empty() const override;
		inline bool is_full() const override;
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
		m_tail(0)
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
	const uint32 numberOfData = get_numberOfUnread();
	if(size < numberOfData + 1)
	{
		return(FAIL);
	}
	
	const uint32 bufferSize = m_data.get_size();
	if(bufferSize == size)
	{
		return(OK);
	}
	
	
	//	Create new Array and copy Contents manually
	Array array(dataType(), size);
	
	
	//	Copy old Data to new Array
	for(uint32 i = 0; i < numberOfData; i++)
	{
		const uint32 index = (m_tail + i) % bufferSize;
		array[i] = m_data[index];
	}
	m_data = array;
	m_tail = 0;
	m_head = numberOfData;
	
	
	return(OK);
}







template<typename dataType>
CODE_RAM feedback RingbufferDynamic<dataType>::write(const dataType& data)
{
	//	Check if Buffer is full or Buffer Size is 0
	const uint32 bufferSize = m_data.get_size();
	if(is_full() == true || bufferSize == 0)
	{
		return(FAIL);
	}
	
	
	//	Write Data
	m_data[m_head] = data;
	
	
	//	Increment Head with Wrap-Around
	m_head = (m_head + 1) % bufferSize;
	
	return(OK);
}


template<typename dataType>
CODE_RAM dataType RingbufferDynamic<dataType>::read()
{
	dataType data = dataType();
	if(is_empty() == true)
	{
		return(data);
	}
	
	
	//	Read Data
	data = m_data[m_tail];
	
	
	//	Increment Tail with Wrap-Around
	const uint32 bufferSize = m_data.get_size();
	m_tail = (m_tail + 1) % bufferSize;


	return(data);
}


template<typename dataType>
inline void RingbufferDynamic<dataType>::clear()
{
	m_tail = m_head;
}


template<typename dataType>
inline void RingbufferDynamic<dataType>::reset()
{
	m_tail = 0;
	m_head = 0;
}







template<typename dataType>
CODE_RAM uint32 RingbufferDynamic<dataType>::get_numberOfUnread() const
{
	const uint32 bufferSize = m_data.get_size();
	if(bufferSize == 0 || is_empty() == true)
	{
		return(0);
	}
	if(is_full() == true)
	{
		return(bufferSize - 1);
	}
	
	
	//	Calculate Number of Unread Data
	uint32 size = bufferSize - m_tail;
	size += m_head;
	if(size > bufferSize)
	{
		size -= bufferSize;
	}
	return(size);
}


template<typename dataType>
inline bool RingbufferDynamic<dataType>::is_empty() const
{
	if(m_head == m_tail)
	{
		return(true);
	}
	return(false);
}


template<typename dataType>
inline bool RingbufferDynamic<dataType>::is_full() const
{
	//	Add 1 to Head with Wrap-Around
	const uint32 bufferSize = m_data.get_size();
	uint32 head = m_head + 1;
	if(head == bufferSize)
	{
		head = 0;
	}
	
	
	if(head == m_tail)
	{
		return(true);
	}
	return(false);
}


template<typename dataType>
inline uint32 RingbufferDynamic<dataType>::get_size() const
{
	const uint32 bufferSize = m_data.get_size();
	if(bufferSize == 0)
	{
		return(0);
	}
	return(bufferSize - 1);
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
	const uint32 bufferSize = m_data.get_size();
	if(bufferSize < 2)
	{
		return(false);
	}
	const uint32 numberOfData = get_numberOfUnread();
	for(uint32 i = 0; i < numberOfData; i++)
	{
		const uint32 index = (m_tail + i) % bufferSize;
		if(m_data[index] == data)
		{
			return(true);
		}
	}
	return(false);
}