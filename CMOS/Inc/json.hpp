#pragma once

#include "string.hpp"
#include "array.hpp"





class JSON
{
	public:
		
		
		
		
		
		
		
	private:
		
		bool m_isArray;
		String m_name;
		String m_value;
		Array<JSON*> m_children;
		bool m_valid;
		
		feedback transformToString(String& string, bool prettyPrinting = true, bool deleteChildrenAfterTransformation = false, uint32 treeDepth = 0);
		
		
		
		
		
	public:
		
		JSON();
		JSON(String name, String data = "{}");
		~JSON();
		
		constexpr inline bool is_valid() const;
		
		constexpr inline bool is_array() const;
		inline String get_name() const;
		inline String get_value() const;
		String get_valueWithoutQuotes() const;
		constexpr inline uint32 get_numberOfChildren() const;
		inline Array<JSON*> get_children() const;
		inline Array<JSON*>& get_children();
		bool has_child(String childName) const;
		JSON* get_child(String childName) const;
		JSON* get_child(uint32 childNumber) const;
		
		void erase();
		inline feedback set_name(String name);
		inline feedback set_value(String value);
		
		JSON& operator+=(JSON* child);
		JSON& operator+=(Array<String> array);
		JSON& operator+=(Array<JSON*> array);
		JSON& operator-=(JSON* child);
		
		JSON& operator=(const JSON& json);
		
		inline String transformToString(bool prettyPrinting = true, bool deleteChildrenAfterTransformation = false);
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

constexpr inline bool JSON::is_valid() const
{
	return(m_valid);
}







constexpr inline bool JSON::is_array() const
{
	if(m_valid == false)
	{
		return(false);
	}
	return(m_isArray);
}


inline String JSON::get_name() const
{
	if(m_valid == false)
	{
		return("");
	}
	return(m_name);
}


inline String JSON::get_value() const
{
	if(m_valid == false)
	{
		return("");
	}
	return(m_value);
}







inline Array<JSON*> JSON::get_children() const
{
	if(m_valid == false)
	{
		return(Array<JSON*>());
	}
	return(m_children);
}


inline Array<JSON*>& JSON::get_children()
{
	if(m_valid == false)
	{
		static Array<JSON*> empty;
		return(empty);
	}
	return(m_children);
}







constexpr inline uint32 JSON::get_numberOfChildren() const
{
	if(m_valid == false)
	{
		return(0);
	}
	return(m_children.get_size());
}







inline feedback JSON::set_name(String name)
{
	if(m_valid == false)
	{
		return(FAIL);
	}
	m_name = name;
	return(OK);
}


inline feedback JSON::set_value(String value)
{
	if(m_valid == false)
	{
		return(FAIL);
	}
	m_value = value;
	return(OK);
}







inline String JSON::transformToString(bool prettyPrinting, bool deleteChildrenAfterTransformation)
{
	String data;
	if(transformToString(data, prettyPrinting, deleteChildrenAfterTransformation) != OK)
	{
		data.erase();
	}
	return(data);
}