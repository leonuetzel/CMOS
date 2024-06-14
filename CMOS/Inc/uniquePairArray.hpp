#pragma once

#include "defines.hpp"
#include "pair.hpp"
#include "array.hpp"





template<typename dataType_1, typename dataType_2>
class UniquePairArray
{
	public:
		
		
		
		
		
		
		
	private:
		
		Array<Pair<dataType_1, dataType_2>> m_data;
		
		
		
		
		
	public:
		
		constexpr inline UniquePairArray();
		inline UniquePairArray(const UniquePairArray& uniquePairArray);
		inline ~UniquePairArray();
		
		
		//	Getter
		constexpr inline uint32 get_size() const;
		bool contains(const dataType_1& first) const;
		
		
		//	Modify
		inline void erase();
		feedback erase(const dataType_1& first);
		
		
		//	Operators
		inline UniquePairArray& operator=(const UniquePairArray& uniquePairArray);
		
		UniquePairArray operator+(const Pair<dataType_1, dataType_2>& pair) const;
		UniquePairArray& operator+=(const Pair<dataType_1, dataType_2>& pair);
		UniquePairArray operator+(const UniquePairArray& uniquePairArray) const;
		UniquePairArray& operator+=(const UniquePairArray& uniquePairArray);
		
		dataType_2 operator[](const dataType_1& first) const;
		dataType_2& operator[](const dataType_1& first);
		
		constexpr inline Pair<dataType_1, dataType_2>* begin() const;
		constexpr inline Pair<dataType_1, dataType_2>* end() const;
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
constexpr inline UniquePairArray<dataType_1, dataType_2>::UniquePairArray()
	:	m_data()
{
	
}


template<typename dataType_1, typename dataType_2>
inline UniquePairArray<dataType_1, dataType_2>::UniquePairArray(const UniquePairArray<dataType_1, dataType_2>& uniquePairArray)
	:	UniquePairArray()
{
	operator=(uniquePairArray);
}


template<typename dataType_1, typename dataType_2>
inline UniquePairArray<dataType_1, dataType_2>::~UniquePairArray()
{
	
}







template<typename dataType_1, typename dataType_2>
constexpr inline uint32 UniquePairArray<dataType_1, dataType_2>::get_size() const
{
	return(m_data.get_size());
}


template<typename dataType_1, typename dataType_2>
CODE_RAM bool UniquePairArray<dataType_1, dataType_2>::contains(const dataType_1& first) const
{
	for(auto& i: m_data)
	{
		if(i.first() == first)
		{
			return(true);
		}
	}
	return(false);
}







template<typename dataType_1, typename dataType_2>
inline void UniquePairArray<dataType_1, dataType_2>::erase()
{
	m_data.erase();
}


template<typename dataType_1, typename dataType_2>
CODE_RAM feedback UniquePairArray<dataType_1, dataType_2>::erase(const dataType_1& first)
{
	//	If Array doesnt have this first Value, return Fail
	if(contains(first) == false)
	{
		return(FAIL);
	}
	
	
	//	Get Index of Element
	uint32 index = m_data.c_invalid;
	for(uint32 i = 0; i < m_data.get_size(); i++)
	{
		if(m_data[i].first() == first)
		{
			index = i;
			break;
		}
	}
	if(index == m_data.c_invalid)
	{
		return(FAIL);
	}
	
	
	//	Erase Pair
	return(m_data.erase(index));
}







template<typename dataType_1, typename dataType_2>
inline UniquePairArray<dataType_1, dataType_2>& UniquePairArray<dataType_1, dataType_2>::operator=(const UniquePairArray& uniquePairArray)
{
	m_data.operator=(uniquePairArray.m_data);
	return(*this);
}







template<typename dataType_1, typename dataType_2>
CODE_RAM UniquePairArray<dataType_1, dataType_2> UniquePairArray<dataType_1, dataType_2>::operator+(const Pair<dataType_1, dataType_2>& pair) const
{
	UniquePairArray uniquePairArray(*this);
	uniquePairArray += pair;
	return(uniquePairArray);
}


template<typename dataType_1, typename dataType_2>
CODE_RAM UniquePairArray<dataType_1, dataType_2>& UniquePairArray<dataType_1, dataType_2>::operator+=(const Pair<dataType_1, dataType_2>& pair)
{
	//	Check if a Pair with this first Value already exists
	//	If it exists, cancel because it wouldnt be a unique Pair Array anymore
	for(auto& i: m_data)
	{
		if(i.first() == pair.first())
		{
			return(*this);
		}
	}
	
	
	//	If its unique, append it
	m_data += pair;
	return(*this);
}


template<typename dataType_1, typename dataType_2>
CODE_RAM UniquePairArray<dataType_1, dataType_2> UniquePairArray<dataType_1, dataType_2>::operator+(const UniquePairArray& uniquePairArray) const
{
	UniquePairArray returnArray(*this);
	returnArray += uniquePairArray;
	return(returnArray);
}


template<typename dataType_1, typename dataType_2>
CODE_RAM UniquePairArray<dataType_1, dataType_2>& UniquePairArray<dataType_1, dataType_2>::operator+=(const UniquePairArray& uniquePairArray)
{
	for(auto& i: uniquePairArray)
	{
		operator+=(i);
	}
	return(*this);
}







template<typename dataType_1, typename dataType_2>
CODE_RAM dataType_2 UniquePairArray<dataType_1, dataType_2>::operator[](const dataType_1& first) const
{
	//	Search for Pair with first Value and return second Value if found
	for(auto& i: m_data)
	{
		if(i.first() == first)
		{
			return(i.second());
		}
	}
	
	
	//	If not found, return default constructed second Value
	return(dataType_2());
}


template<typename dataType_1, typename dataType_2>
CODE_RAM dataType_2& UniquePairArray<dataType_1, dataType_2>::operator[](const dataType_1& first)
{
	//	Search for Pair with first Value and return second Value if found
	for(auto& i: m_data)
	{
		if(i.first() == first)
		{
			return(i.second());
		}
	}
	
	
	//	If not found, create new default constructed Pair, append it and return second Value
	Pair<dataType_1, dataType_2> newPair(first, dataType_2());
	operator+=(newPair);
	return(m_data[m_data.get_size() - 1].second());
}







template<typename dataType_1, typename dataType_2>
constexpr inline Pair<dataType_1, dataType_2>* UniquePairArray<dataType_1, dataType_2>::begin() const
{
	return(m_data.begin());
}


template<typename dataType_1, typename dataType_2>
constexpr inline Pair<dataType_1, dataType_2>* UniquePairArray<dataType_1, dataType_2>::end() const
{
	return(m_data.end());
}