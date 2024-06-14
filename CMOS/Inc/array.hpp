#pragma once

#include "defines.hpp"





template<typename dataType>
class Array
{
	public:
		
		static constexpr uint32 c_invalid = 0xFFFFFFFF;
		
		
		
		
		
	private:
		
		dataType* m_data;
		uint32 m_size;
		uint32 m_sizeMax;
		
		
		feedback expandIfNecessary(uint32 newSize);
		void freeMemory();
		
		
		
		
		
	public:
		
		constexpr inline Array();
		inline Array(const Array& array);
		Array(dataType defaultValue, uint32 numberOfElements = 1);
		inline ~Array();
		
		
		//	Informational
		static constexpr inline bool is_valid(uint32 index);
		constexpr inline uint32 get_size() const;
		
		
		//	Getter
		constexpr inline const dataType* get_data() const;
		
		
		//	Algorithms
		dataType mediumValue() const;
		
		
		//	Searching
		uint32 count(const dataType& data) const;
		uint32 find(const dataType& data, uint32 start = 0, uint32 occurence = 1) const;
		uint32 find(const Array<dataType>& data, uint32 start = 0, uint32 occurence = 1) const;
		bool contains(const dataType& data) const;
		
		
		//	Cut-Out
		Array sub(uint32 index, uint32 size = c_invalid) const;
		Array subWithDelimiter(dataType delimiter, uint32 subArrayNumber) const;
		Array<Array> explode(dataType delimiter) const;
		
		
		//	Editing
		feedback set_size(uint32 newSize, bool fillWithDefaultValue = true, dataType defaultValue = dataType());
		inline void fill(const dataType& data);
		feedback insert(uint32 index, dataType data);
		feedback insert(uint32 index, const Array& array);
		feedback erase(uint32 index, uint32 numberOfElementsToErase = 1);
		void erase();
		inline feedback eraseToEnd(uint32 index);
		inline feedback eraseFromEnd(uint32 numberOfElementsToErase);
		void eraseAllOccurences(dataType elementToErase, uint32 start = 0);
		
		
		//	Operators
		Array& operator=(const Array& array);
		
		Array operator+(const dataType& data) const;
		inline Array& operator+=(const dataType& data);
		Array operator+(const Array& array) const;
		inline Array& operator+=(const Array& array);
		
		constexpr inline dataType operator[](uint32 index) const;
		constexpr inline dataType& operator[](uint32 index);
		constexpr inline const dataType& operator()(uint32 index) const;
		
		bool operator==(const Array<dataType> array) const;
		inline bool operator!=(const Array<dataType> array) const;
		
		constexpr inline dataType* begin() const;
		constexpr inline dataType* end() const;
};



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

template<typename dataType>
CODE_RAM feedback Array<dataType>::expandIfNecessary(uint32 newSize)
{
	if(newSize <= m_sizeMax || newSize == 0)
	{
		return(OK);
	}
	
	
	//	Reserve new Memory and copy old Content bytewise
	extern uint32 __cmos_array_expand__(uint8** pointerToData, uint32 actualSizeInBytes, uint32 newSizeInBytes);
	
	const uint32 actualSizeInBytes	= m_size * sizeof(dataType);
	const uint32 newSizeInBytes			= newSize * sizeof(dataType);
	const uint32 sizeInBytesAllocated = __cmos_array_expand__((uint8**) &m_data, actualSizeInBytes, newSizeInBytes);
	if(sizeInBytesAllocated == 0)
	{
		return(FAIL);
	}
	
	
	//	Calculate new Max Size
	m_sizeMax = sizeInBytesAllocated / sizeof(dataType);
	
	
	return(OK);
}


template<typename dataType>
void Array<dataType>::freeMemory()
{
	//	Call Destructors of Elements
	if(m_data != nullptr)
	{
		for(uint32 i = 1; i < m_size; i++)
		{
			m_data[i].~dataType();
		}
		delete m_data;
		m_data = nullptr;
	}
	m_size = 0;
	m_sizeMax = 0;
}



/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

template<typename dataType>
constexpr inline Array<dataType>::Array()
	:	m_data(nullptr),
		m_size(0),
		m_sizeMax(0)
{
	
}


template<typename dataType>
inline Array<dataType>::Array(const Array& array)
	:	Array()
{
	operator=(array);
}


template<typename dataType>
CODE_RAM Array<dataType>::Array(dataType defaultValue, uint32 numberOfElements)
	:	Array()
{
	if(expandIfNecessary(numberOfElements) == OK)
	{
		m_size = numberOfElements;
		for(uint32 i = 0; i < m_size; i++)
		{
			m_data[i] = defaultValue;
		}
	}
}


template<typename dataType>
inline Array<dataType>::~Array()
{
	freeMemory();
}







template<typename dataType>
constexpr inline bool Array<dataType>::is_valid(uint32 index)
{
	if(index == c_invalid)
	{
		return(false);
	}
	return(true);
}


template<typename dataType>
constexpr inline uint32 Array<dataType>::get_size() const
{
	return(m_size);
}







template<typename dataType>
constexpr inline const dataType* Array<dataType>::get_data() const
{
	return(m_data);
}







template<typename dataType>
dataType Array<dataType>::mediumValue() const
{
	dataType sum = dataType();
	for(auto& i: *this)
	{
		sum += i;
	}
	return(sum / m_size);
}







template<typename dataType>
CODE_RAM uint32 Array<dataType>::count(const dataType& data) const
{
	uint32 occurences = 0;
	
	for(uint32 i = 0; i < m_size; i++)
	{
		if(m_data[i] == data)
		{
			occurences++;
		}
	}
	return(occurences);
}


template<typename dataType>
CODE_RAM uint32 Array<dataType>::find(const dataType& data, uint32 start, uint32 occurence) const
{
	if(start >= m_size)
	{
		return(c_invalid);
	}
	
	uint32 counter = 0;
	for(uint32 i = start; i < m_size; i++)
	{
		if(m_data[i] == data)
		{
			counter++;
			if(counter == occurence)
			{
				return(i);
			}
		}
	}
	return(c_invalid);
}


template<typename dataType>
CODE_RAM uint32 Array<dataType>::find(const Array<dataType>& data, uint32 start, uint32 occurence) const
{
	if(start + data.m_size - 1 >= m_size)
	{
		return(c_invalid);
	}
	
	uint32 counter = 0;
	for(uint32 i = start; i < m_size - data.m_size + 1; i++)
	{
		bool sequenceFound = true;
		for(uint32 j = 0; j < data.m_size; j++)
		{
			if(m_data[i + j] != data.m_data[j])
			{
				sequenceFound = false;
				break;
			}
		}
		
		if(sequenceFound == true)
		{
			counter++;
			if(counter == occurence)
			{
				return(i);
			}
		}
	}
	return(c_invalid);
}


template<typename dataType>
CODE_RAM bool Array<dataType>::contains(const dataType& data) const
{
	return(is_valid(find(data)));
}







template<typename dataType>
Array<dataType> Array<dataType>::sub(uint32 index, uint32 size) const
{
	if(size == 0xFFFFFFFF)
	{
		size = m_size - index;
	}
	
	Array subArray;
	if(m_data != nullptr && index < m_size && index + size <= m_size)
	{
		for(uint32 i = 0; i < size; i++)
		{
			subArray += m_data[index + i];
		}
	}
	return(subArray);
}


template<typename dataType>
Array<dataType> Array<dataType>::subWithDelimiter(dataType delimiter, uint32 subArrayNumber) const
{
	//	Get Start Position
	uint32 start = 0;
	if(subArrayNumber != 0)
	{
		start = find(delimiter, 0, subArrayNumber);
		if(start >= m_size)
		{
			return(Array());
		}
		start++;
	}
	
	
	//	Get End Position
	if(start < m_size)
	{
		uint32 end = find(delimiter, start);
		uint32 size = c_invalid;
		if(end != c_invalid)
		{
			size = end - start;
		}
		return(sub(start, size));
	}
	
	
	//	Return empty Array
	return(Array());
}


template<typename dataType>
Array<Array<dataType>> Array<dataType>::explode(dataType delimiter) const
{
	Array<Array> array;
	if(m_data == nullptr)
	{
		return(array);
	}
	
	
	uint32 start = 0;
	for(uint32 i = 0; i < m_size; i++)
	{
		if(m_data[i] == delimiter)
		{
			array += sub(start, i - start);
			start = i + 1;
		}
	}
	array += sub(start);
	return(array);
}







template<typename dataType>
feedback Array<dataType>::set_size(uint32 newSize, bool fillWithDefaultValue, dataType defaultValue)
{
	if(expandIfNecessary(newSize) != OK)
	{
		return(FAIL);
	}
	m_size = newSize;
	
	if(fillWithDefaultValue == true)
	{
		fill(defaultValue);
	}
	return(OK);
}


template<typename dataType>
inline void Array<dataType>::fill(const dataType& data)
{
	for(uint32 i = 0; i < m_size; i++)
	{
		m_data[i] = data;
	}
}


template<typename dataType>
CODE_RAM feedback Array<dataType>::insert(uint32 index, dataType data)
{
	//	Check Index Validity
	if(index > m_size)
	{
		return(FAIL);
	}
	
	
	//	Expand Memory
	if(expandIfNecessary(m_size + 1) != OK)
	{
		return(FAIL);
	}
	
	
	//	Shift Data above Insert-Position to make Space for new Data
	extern void __cmos_array_move__(uint8* source, uint8* destination, uint32 sizeInBytes);
	
	uint8* destination	= (uint8*) &m_data[index + 1];
	uint8* source				= (uint8*) &m_data[index];
	const uint32 numberOfElementsToShift = m_size - index;
	const uint32 sizeInBytes	= numberOfElementsToShift * sizeof(dataType);
	
	__cmos_array_move__(source, destination, sizeInBytes);
	
	
	//	Insert new Data
	m_data[index] = data;
	m_size++;
	return(OK);
}


template<typename dataType>
CODE_RAM feedback Array<dataType>::insert(uint32 index, const Array& array)
{
	//	Check Index Validity
	if(index > m_size)
	{
		return(FAIL);
	}
	
	
	//	Expand Memory
	if(expandIfNecessary(m_size + array.m_size) != OK)
	{
		return(FAIL);
	}
	
	
	//	Shift Data above Insert-Position to make Space for new Data
	extern void __cmos_array_move__(uint8* source, uint8* destination, uint32 sizeInBytes);
	
	uint8* destination	= (uint8*) &m_data[index + array.m_size];
	uint8* source				= (uint8*) &m_data[index];
	const uint32 numberOfElementsToShift = m_size - index;
	const uint32 sizeInBytes	= numberOfElementsToShift * sizeof(dataType);
	
	__cmos_array_move__(source, destination, sizeInBytes);
	
	
	//	Insert new Data
	for(uint32 i = 0; i < array.m_size; i++)
	{
		m_data[index + i] = array.m_data[i];
	}
	m_size += array.m_size;
	return(OK);
}


template<typename dataType>
CODE_RAM feedback Array<dataType>::erase(uint32 index, uint32 numberOfElementsToErase)
{
	//	Check if Index and Datapointer is valid
	if(is_valid(index) == false || m_data == nullptr)
	{
		return(FAIL);
	}
	
	
	//	Check Validity of Index
	if(index >= m_size || index + numberOfElementsToErase > m_size)
	{
		return(FAIL);
	}
	
	
	//	No need to to something if nothing should be erased
	if(numberOfElementsToErase == 0)
	{
		return(OK);
	}
	
	
	//	Shift Elements down to Index
	const uint32 c_numberOfPositionsToShift = m_size - index - numberOfElementsToErase;
	for(uint32 i = 0; i < c_numberOfPositionsToShift; i++)
	{
		m_data[index + i] = m_data[index + i + numberOfElementsToErase];
	}
	
	
	
	//	Set new Size
	m_size -= numberOfElementsToErase;
	
	
	//	Destroy Elements that have gotten free
	for(uint32 i = 0; i < numberOfElementsToErase; i++)
	{
		m_data[m_size + i].~dataType();
	}
	
	
	return(OK);
}


template<typename dataType>
void Array<dataType>::erase()
{
	if(m_data != nullptr)
	{
		for(auto& i: *this)
		{
			i.~dataType();
		}
	}
	m_size = 0;
}


template<typename dataType>
inline feedback Array<dataType>::eraseToEnd(uint32 index)
{
	return(erase(index, m_size - index));
}


template<typename dataType>
inline feedback Array<dataType>::eraseFromEnd(uint32 numberOfElementsToErase)
{
	if(m_size == 0)
	{
		return(FAIL);
	}
	return(erase(m_size - numberOfElementsToErase, numberOfElementsToErase));
}


template<typename dataType>
void Array<dataType>::eraseAllOccurences(dataType elementToErase, uint32 start)
{
	for(uint32 i = start; i < m_size; i++)
	{
		if(m_data[i] == elementToErase)
		{
			erase(i);
			i--;
		}
	}
}







template<typename dataType>
Array<dataType>& Array<dataType>::operator=(const Array& array)
{
	erase();
	insert(0, array);
	return(*this);
}







template<typename dataType>
CODE_RAM Array<dataType> Array<dataType>::operator+(const dataType& data) const
{
	Array array(*this);
	array.insert(array.m_size, data);
	return(array);
}


template<typename dataType>
inline Array<dataType>& Array<dataType>::operator+=(const dataType& data)
{
	insert(m_size, data);
	return(*this);
}


template<typename dataType>
CODE_RAM Array<dataType> Array<dataType>::operator+(const Array& array) const
{
	Array returnArray(*this);
	returnArray.insert(returnArray.m_size, array);
	return(returnArray);
}


template<typename dataType>
inline Array<dataType>& Array<dataType>::operator+=(const Array& array)
{
	insert(m_size, array);
	return(*this);
}







template<typename dataType>
constexpr inline dataType Array<dataType>::operator[](uint32 index) const
{
	return(m_data[index]);
}


template<typename dataType>
constexpr inline dataType& Array<dataType>::operator[](uint32 index)
{
	return(m_data[index]);
}


template<typename dataType>
constexpr inline const dataType& Array<dataType>::operator()(uint32 index) const
{
	return(m_data[index]);
}







template<typename dataType>
inline bool Array<dataType>::operator!=(const Array<dataType> array) const
{
	return(!operator==(array));
}







template<typename dataType>
bool Array<dataType>::operator==(const Array<dataType> array) const
{
	//	Simple Size Comparison
	if(array.m_size != m_size)
	{
		return(false);
	}
	
	
	//	Null Check
	if(m_data == nullptr || array.m_data == nullptr)
	{
		if(m_data != array.m_data)
		{
			return(false);
		}
		return(true);
	}
	
	
	//	Comparison
	for(uint32 i = 0; i < m_size; i++)
	{
		if(m_data[i] != array.m_data[i])
		{
			return(false);
		}
	}
	return(true);
}







template<typename dataType>
constexpr inline dataType* Array<dataType>::begin() const
{
	return(m_data);
}


template<typename dataType>
constexpr inline dataType* Array<dataType>::end() const
{
	return(m_data + m_size);
}