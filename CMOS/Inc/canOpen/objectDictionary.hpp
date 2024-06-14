#pragma once

#include "../cmos.hpp"
#include "objectDictionaryEntry.hpp"





namespace canOpen
{
	class Object_Dictionary: private UniquePairArray<uint16, UniquePairArray<uint8, ObjectDictionary_Entry>>
	{
		public:
			
			enum class e_index: uint16
			{
				STATIC_DATA_TYPES														= 0x0001,
				COMPLEX_DATA_TYPES													= 0x0020,
				MANUFACTURER_SPECIFIC_COMPLEX_DATA_TYPES		= 0x0040,
				DEVICE_PROFILE_SPECIFIC_DATA_TYPES					= 0x0060,
				COMMUNICATION_PROFILE_AREA									= 0x1000,
				MANUFACTURER_SPECIFIC_PROFILE_AREA					= 0x2000,
				STANDARDIZED_PROFILE_AREA_LOGICAL_DEVICE_1	= 0x6000,
				STANDARDIZED_PROFILE_AREA_LOGICAL_DEVICE_2	= 0x6800,
				STANDARDIZED_PROFILE_AREA_LOGICAL_DEVICE_3	= 0x7000,
				STANDARDIZED_PROFILE_AREA_LOGICAL_DEVICE_4	= 0x7800,
				STANDARDIZED_PROFILE_AREA_LOGICAL_DEVICE_5	= 0x8000,
				STANDARDIZED_PROFILE_AREA_LOGICAL_DEVICE_6	= 0x8800,
				STANDARDIZED_PROFILE_AREA_LOGICAL_DEVICE_7	= 0x9000,
				STANDARDIZED_PROFILE_AREA_LOGICAL_DEVICE_8	= 0x9800,
				STANDARDIZED_NETWORK_VARIABLE_AREA					= 0xA000,
				STANDARDIZED_SYSTEM_VARIABLE_AREA						= 0xB000
			};
			
			
			
			
			
		private:
			
			//	Static Member
			
			
			
			//	Non-static Member
			
			
			
			//	Constructor and Destructor
			Object_Dictionary();
			~Object_Dictionary();
			
			
			//	Member Functions
			
			
			
			//	Friends
			friend class CanOpen;
			
			
			
			
			
		public:
			
			//	Getter
			uint32 get_size() const;
			bool contains(uint16 index, uint8 subIndex) const;
			
			
			//	Modify
			void erase();
			feedback erase(const uint16& first);
			
			
			//	Operators
			Object_Dictionary& operator+=(const Pair<uint16, UniquePairArray<uint8, ObjectDictionary_Entry>>& pair);
			
			UniquePairArray<uint8, ObjectDictionary_Entry> operator[](const uint16& first) const;
			UniquePairArray<uint8, ObjectDictionary_Entry>& operator[](const uint16& first);
			
			Pair<uint16, UniquePairArray<uint8, ObjectDictionary_Entry>>* begin() const;
			Pair<uint16, UniquePairArray<uint8, ObjectDictionary_Entry>>* end() const;
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
	
	
};