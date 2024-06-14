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
	
	ObjectDictionary_Entry::ObjectDictionary_Entry()
		:	type(e_objectType::NULL_NO_DATA_FIELDS),
			attribute(e_attribute::READ_ONLY_CONSTANT)
	{
		
	}
	
	ObjectDictionary_Entry::ObjectDictionary_Entry(e_objectType type, e_attribute attribute)
		:	type(type),
			attribute(attribute)
	{
		
	}
	
	
	ObjectDictionary_Entry::~ObjectDictionary_Entry()
	{
		
	}
};