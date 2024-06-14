#include "../../Inc/canOpen/canOpen.hpp"





namespace canOpen
{
	/*****************************************************************************/
	/*                    Globals and Static Initialization					 						 */
	/*****************************************************************************/
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Private	  			 						 						 */
	/*****************************************************************************/
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Public	  			 						 						 */
	/*****************************************************************************/
	
	Object_Dictionary::Object_Dictionary()
	{
		erase();
		CMOS::get().semaphore_create(this);
	}
	
	
	Object_Dictionary::~Object_Dictionary()
	{
		
	}
	
	
	
	
	
	
	
	uint32 Object_Dictionary::get_size() const
	{
		CMOS& cmos = CMOS::get();
		cmos.semaphore_lock(this);
		const uint32 size = UniquePairArray<uint16, UniquePairArray<uint8, ObjectDictionary_Entry>>::get_size();
		cmos.semaphore_unlock(this);
		return(size);
	}


	bool Object_Dictionary::contains(uint16 index, uint8 subIndex) const
	{
		CMOS& cmos = CMOS::get();
		cmos.semaphore_lock(this);
		if(UniquePairArray<uint16, UniquePairArray<uint8, ObjectDictionary_Entry>>::contains(index) == true)
		{
			const bool doesContain = (*this)[index].contains(subIndex);
			cmos.semaphore_unlock(this);
			return(doesContain);
		}
		cmos.semaphore_unlock(this);
		return(false);
	}
	
	
	
	
	
	
	
	void Object_Dictionary::erase()
	{
		CMOS& cmos = CMOS::get();
		cmos.semaphore_lock(this);
		UniquePairArray<uint16, UniquePairArray<uint8, ObjectDictionary_Entry>>::erase();
		cmos.semaphore_unlock(this);
	}
	
	
	feedback Object_Dictionary::erase(const uint16& first)
	{
		CMOS& cmos = CMOS::get();
		cmos.semaphore_lock(this);
		feedback success = UniquePairArray<uint16, UniquePairArray<uint8, ObjectDictionary_Entry>>::erase(first);
		cmos.semaphore_unlock(this);
		return(success);
	}
	
	
	
	
	
	
	
	Object_Dictionary& Object_Dictionary::operator+=(const Pair<uint16, UniquePairArray<uint8, ObjectDictionary_Entry>>& pair)
	{
		CMOS& cmos = CMOS::get();
		cmos.semaphore_lock(this);
		UniquePairArray<uint16, UniquePairArray<uint8, ObjectDictionary_Entry>>::operator+=(pair);
		cmos.semaphore_unlock(this);
		return(*this);
	}
	
	
	
	
	
	
	
	UniquePairArray<uint8, ObjectDictionary_Entry> Object_Dictionary::operator[](const uint16& first) const
	{
		CMOS& cmos = CMOS::get();
		cmos.semaphore_lock(this);
		UniquePairArray<uint8, ObjectDictionary_Entry> value = UniquePairArray<uint16, UniquePairArray<uint8, ObjectDictionary_Entry>>::operator[](first);
		cmos.semaphore_unlock(this);
		return(value);
	}
	
	
	UniquePairArray<uint8, ObjectDictionary_Entry>& Object_Dictionary::operator[](const uint16& first)
	{
		CMOS& cmos = CMOS::get();
		cmos.semaphore_lock(this);
		UniquePairArray<uint8, ObjectDictionary_Entry>& value = UniquePairArray<uint16, UniquePairArray<uint8, ObjectDictionary_Entry>>::operator[](first);
		cmos.semaphore_unlock(this);
		return(value);
	}
	
	
	
	
	
	
	
	Pair<uint16, UniquePairArray<uint8, ObjectDictionary_Entry>>* Object_Dictionary::begin() const
	{
		CMOS& cmos = CMOS::get();
		cmos.semaphore_lock(this);
		Pair<uint16, UniquePairArray<uint8, ObjectDictionary_Entry>>* value = UniquePairArray<uint16, UniquePairArray<uint8, ObjectDictionary_Entry>>::begin();
		cmos.semaphore_unlock(this);
		return(value);
	}
	
	
	Pair<uint16, UniquePairArray<uint8, ObjectDictionary_Entry>>* Object_Dictionary::end() const
	{
		CMOS& cmos = CMOS::get();
		cmos.semaphore_lock(this);
		Pair<uint16, UniquePairArray<uint8, ObjectDictionary_Entry>>* value = UniquePairArray<uint16, UniquePairArray<uint8, ObjectDictionary_Entry>>::end();
		cmos.semaphore_unlock(this);
		return(value);
	}
};