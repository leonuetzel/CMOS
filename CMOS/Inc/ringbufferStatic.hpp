#pragma once

#include "defines.hpp"
#include "i_ringbuffer.hpp"





template<typename dataType, uint32 N>
class RingbufferStatic: public I_Ringbuffer<dataType>
{
	public:
		
		
		
		
		
		
		
	private:
		
		dataType m_data[N];
		uint32 m_head;
		uint32 m_tail;
		
		
		
		
		
	public:
		
		constexpr inline RingbufferStatic();
		inline ~RingbufferStatic() override;
		
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

template<typename dataType, uint32 N>
constexpr inline RingbufferStatic<dataType, N>::RingbufferStatic()
	:	m_data(),
		m_head(0),
		m_tail(0)
{
	static_assert(N > 0, "N must be greater than 0");
	static_assert(N > 1, "N is recommended to be greater than 1");
}


template<typename dataType, uint32 N>
inline RingbufferStatic<dataType, N>::~RingbufferStatic()
{
	
}







template<typename dataType, uint32 N>
CODE_RAM feedback RingbufferStatic<dataType, N>::write(const dataType& data)
{
	//	Full Buffer - cant write
	if(is_full() == true)
	{
		return(FAIL);
	}
	
	
	//	Write Data
	m_data[m_head] = data;
	
	
	//	Increment Head with Wrap-Around
	m_head = (m_head + 1) % N;
	
	
	return(OK);
}


template<typename dataType, uint32 N>
CODE_RAM dataType RingbufferStatic<dataType, N>::read()
{
	dataType data = dataType();
	if(is_empty() == true)
	{
		return(data);
	}
	
	
	//	Read Data
	if(get_numberOfUnread() > 0)
	data = m_data[m_tail];
	
	
	//	Increment Tail with Wrap-Around	
	m_tail = (m_tail + 1) % N;
	
	
	return(data);
}


template<typename dataType, uint32 N>
inline void RingbufferStatic<dataType, N>::clear()
{
	m_tail = m_head;
}


template<typename dataType, uint32 N>
inline void RingbufferStatic<dataType, N>::reset()
{
	m_head = 0;
	m_tail = 0;
}







template<typename dataType, uint32 N>
CODE_RAM uint32 RingbufferStatic<dataType, N>::get_numberOfUnread() const
{
	if(is_full() == true)
	{
		return(N - 1);
	}
	if(is_empty() == true)
	{
		return(0);
	}
	
	
	uint32 size = N - m_tail;
	size += m_head;
	if(size > N)
	{
		size -= N;
	}
	return(size);
}


template<typename dataType, uint32 N>
inline bool RingbufferStatic<dataType, N>::is_empty() const
{
	if(m_head == m_tail)
	{
		return(true);
	}
	return(false);
}


template<typename dataType, uint32 N>
inline bool RingbufferStatic<dataType, N>::is_full() const
{
	//	Add 1 to Head with Wrap-Around
	uint32 head = m_head + 1;
	if(head == N)
	{
		head = 0;
	}
	
	
	if(head == m_tail)
	{
		return(true);
	}
	return(false);
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
		uint32 index = (m_tail + i) % N;
		if(m_data[index] == data)
		{
			return(true);
		}
	}
	return(false);
}