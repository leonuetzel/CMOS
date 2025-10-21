#pragma once

#include "defines.hpp"





template<typename dataType_1, typename dataType_2>
class Pair
{
	public:
		
		
		
		
		
		
		
	private:
		
		dataType_1 m_first;
		dataType_2 m_second;
		
		
		
		
		
	public:
		
		constexpr inline Pair();
		constexpr inline Pair(const dataType_1& first, const dataType_2& second);
		constexpr inline Pair(const Pair& pair);
		inline ~Pair();
		
		
		//	Getter
		constexpr inline dataType_1& first();
		constexpr inline dataType_2& second();
		constexpr inline dataType_1 first() const;
		constexpr inline dataType_2 second() const;
		
		
		//	Operators
		inline Pair& operator=(const Pair<dataType_1, dataType_2>& pair);
		inline bool operator==(const Pair<dataType_1, dataType_2>& pair) const;
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

template<typename dataType_1, typename dataType_2>
constexpr inline Pair<dataType_1, dataType_2>::Pair()
	:	m_first(dataType_1()),
		m_second(dataType_2())
{
	
}


template<typename dataType_1, typename dataType_2>
constexpr inline Pair<dataType_1, dataType_2>::Pair(const dataType_1& first, const dataType_2& second)
	:	m_first(first),
		m_second(second)
{
	
}


template<typename dataType_1, typename dataType_2>
constexpr inline Pair<dataType_1, dataType_2>::Pair(const Pair<dataType_1, dataType_2>& pair)
	:	Pair()
{
	operator=(pair);
}


template<typename dataType_1, typename dataType_2>
inline Pair<dataType_1, dataType_2>::~Pair()
{
	
}







template<typename dataType_1, typename dataType_2>
constexpr inline dataType_1& Pair<dataType_1, dataType_2>::first()
{
	return(m_first);
}


template<typename dataType_1, typename dataType_2>
constexpr inline dataType_2& Pair<dataType_1, dataType_2>::second()
{
	return(m_second);
}


template<typename dataType_1, typename dataType_2>
constexpr inline dataType_1 Pair<dataType_1, dataType_2>::first() const
{
	return(m_first);
}


template<typename dataType_1, typename dataType_2>
constexpr inline dataType_2 Pair<dataType_1, dataType_2>::second() const
{
	return(m_second);
}







template<typename dataType_1, typename dataType_2>
inline Pair<dataType_1, dataType_2>& Pair<dataType_1, dataType_2>::operator=(const Pair& pair)
{
	m_first = pair.m_first;
	m_second = pair.m_second;
	return(*this);
}


template<typename dataType_1, typename dataType_2>
inline bool Pair<dataType_1, dataType_2>::operator==(const Pair& pair) const
{
	if(m_first != pair.m_first)
	{
		return(false);
	}
	if(m_second != pair.m_second)
	{
		return(false);
	}
	return(true);
}