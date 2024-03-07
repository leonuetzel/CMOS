#pragma once

#include "defines.hpp"
#include "array.hpp"





template<typename dataType>
class UniqueArray
{
	public:
		
		
		
		
		
		
		
	private:
		
		Array<dataType> m_data;
		
		
		
		
		
	public:
		
		constexpr inline UniqueArray();
		inline UniqueArray(const UniqueArray& uniqueArray);
		inline ~UniqueArray();
		
		
		//	Getter
		constexpr inline uint32 get_size() const;
		inline bool contains(const dataType& data) const;
		
		
		//	Modify
		inline void erase();
		inline feedback erase(const dataType& data);
		
		
		//	Operators
		inline UniqueArray& operator=(const UniqueArray& uniqueArray);
		
		UniqueArray operator+(const dataType& data) const;
		UniqueArray& operator+=(const dataType& data);
		UniqueArray operator+(const UniqueArray& uniqueArray) const;
		UniqueArray& operator+=(const UniqueArray& uniqueArray);
		
		constexpr inline dataType* begin() const;
		constexpr inline dataType* end() const;
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
constexpr inline UniqueArray<dataType>::UniqueArray()
	:	m_data()
{
	
}


template<typename dataType>
inline UniqueArray<dataType>::UniqueArray(const UniqueArray<dataType>& uniqueArray)
	:	UniqueArray()
{
	operator=(uniqueArray);
}


template<typename dataType>
inline UniqueArray<dataType>::~UniqueArray()
{
	
}







template<typename dataType>
constexpr inline uint32 UniqueArray<dataType>::get_size() const
{
	return(m_data.get_size());
}


template<typename dataType>
inline bool UniqueArray<dataType>::contains(const dataType& data) const
{
	return(m_data.contains(data));
}







template<typename dataType>
inline void UniqueArray<dataType>::erase()
{
	m_data.erase();
}


template<typename dataType>
inline feedback UniqueArray<dataType>::erase(const dataType& data)
{
	return(m_data.erase(m_data.find(data)));
}







template<typename dataType>
inline UniqueArray<dataType>& UniqueArray<dataType>::operator=(const UniqueArray& uniqueArray)
{
	m_data.operator=(uniqueArray.m_data);
	return(*this);
}







template<typename dataType>
CODE_RAM UniqueArray<dataType> UniqueArray<dataType>::operator+(const dataType& data) const
{
	UniqueArray uniqueArray(*this);
	uniqueArray += data;
	return(uniqueArray);
}


template<typename dataType>
CODE_RAM UniqueArray<dataType>& UniqueArray<dataType>::operator+=(const dataType& data)
{
	//	Check if this Value already exists
	//	If it exists, cancel because it wouldnt be a unique Array anymore
	if(m_data.contains(data) == false)
	{
		m_data += data;
	}
	return(*this);
}


template<typename dataType>
CODE_RAM UniqueArray<dataType> UniqueArray<dataType>::operator+(const UniqueArray& uniqueArray) const
{
	UniqueArray returnArray(*this);
	returnArray += uniqueArray;
	return(returnArray);
}


template<typename dataType>
CODE_RAM UniqueArray<dataType>& UniqueArray<dataType>::operator+=(const UniqueArray& uniqueArray)
{
	for(auto& i: uniqueArray)
	{
		operator+=(i);
	}
	return(*this);
}







template<typename dataType>
constexpr inline dataType* UniqueArray<dataType>::begin() const
{
	return(m_data.begin());
}


template<typename dataType>
constexpr inline dataType* UniqueArray<dataType>::end() const
{
	return(m_data.end());
}