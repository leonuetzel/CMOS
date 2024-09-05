#pragma once

#include "defines.hpp"
#include "i_ringbuffer.hpp"





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
		
		
		
		
		
	public:
		
		constexpr inline RingbufferExternal(dataType* buffer, uint32 size);
		inline ~RingbufferExternal() override;
		
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
		m_tail(0)
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
	//	Sanity Check
	if(is_full() == true || m_size == 0 || m_data == nullptr)
	{
		return(FAIL);
	}
	
	
	//	Write Data
	m_data[m_head] = data;
	
	
	//	Increment Head with Wrap-Around
	m_head = (m_head + 1) % m_size;
	
	
	return(OK);
}


template<typename dataType>
CODE_RAM dataType RingbufferExternal<dataType>::read()
{
	dataType data = dataType();
	if(m_size == 0 || m_data == nullptr || is_empty() == true)
	{
		return(data);
	}
	
	
	//	Read Data
	data = m_data[m_tail];
	
	
	//	Increment Tail with Wrap-Around
	m_tail = (m_tail + 1) % m_size;
	
	
	return(data);
}


template<typename dataType>
inline void RingbufferExternal<dataType>::clear()
{
	m_tail = m_head;
}


template<typename dataType>
inline void RingbufferExternal<dataType>::reset()
{
	m_head = 0;
	m_tail = 0;
}







template<typename dataType>
CODE_RAM uint32 RingbufferExternal<dataType>::get_numberOfUnread() const
{
	if(m_size == 0 || m_data == nullptr || is_empty() == true)
	{
		return(0);
	}
	
	if(is_full() == true)
	{
		return(m_size - 1);
	}
	
	
	//	Calculate Number of Unread Data
	uint32 size = m_size - m_tail;
	size += m_head;
	if(size > m_size)
	{
		size -= m_size;
	}
	return(size);
}


template<typename dataType>
inline bool RingbufferExternal<dataType>::is_empty() const
{
	if(m_head == m_tail)
	{
		return(true);
	}
	return(false);
}


template<typename dataType>
inline bool RingbufferExternal<dataType>::is_full() const
{
	//	Add 1 to Head with Wrap-Around
	uint32 head = m_head + 1;
	if(head == m_size)
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
inline uint32 RingbufferExternal<dataType>::get_size() const
{
	return(m_size - 1);
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
	if(m_size < 2 || m_data == nullptr)
	{
		return(false);
	}
	
	const uint32 numberOfData = get_numberOfUnread();
	for(uint32 i = 0; i < numberOfData; i++)
	{
		const uint32 index = (((uint64) m_tail) + i) % m_size;
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