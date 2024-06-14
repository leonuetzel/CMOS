#pragma once

#include "../cmos.hpp"




namespace canOpen
{
	class ObjectDictionary_Entry: public Array<uint8>
	{
		public:
			
			enum class e_objectType: uint8
			{
				NULL_NO_DATA_FIELDS	= 0x0,
				DOMAIN							= 0x2,
				TYPE_DEFINITION			= 0x3,
				STRUCT_DEFINITION		= 0x4,
				VARIABLE						= 0x7,
				ARRAY								= 0x8,
				RECORD							= 0x9
			};
			
			enum class e_attribute: uint8
			{
				READ_WRITE,
				WRITE_ONLY,
				READ_ONLY,
				READ_ONLY_CONSTANT
			};
			
			
			e_objectType type;
			e_attribute attribute;
			
			
			
			
			
		private:
			
			//	Static Member
			
			
			
			//	Non-static Member
			
			
			
			//	Constructor and Destructor
			
			
			
			//	Member Functions
			
			
			
			//	Friends
			
			
			
			
			
			
		public:
			
			ObjectDictionary_Entry();
			ObjectDictionary_Entry(e_objectType type, e_attribute attribute);
			~ObjectDictionary_Entry();
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