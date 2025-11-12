#include "../Inc/json.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

feedback JSON::transformToString(String& string, bool prettyPrinting, bool deleteChildrenAfterTransformation, uint32 treeDepth)
{
	if(m_valid == false)
	{
		return(FAIL);
	}
	
	
	String fill;
	if(prettyPrinting == true)
	{
		fill = String(treeDepth, '\t');
	}
	
	
	char closingBracket = '}';
	if(prettyPrinting == true)
	{
		string += fill;
	}
	
	if(m_isArray == true)
	{
		if(m_name.get_size() > 0)
		{
			string += "\"" + m_name + "\":";
			if(prettyPrinting == true)
			{
				string += '\n';
				string += fill;
			}
		}
		string += '[';
		if(prettyPrinting == true)
		{
			string += '\n';
		}
		closingBracket = ']';
	}
	else
	{
		if(m_children.get_size() == 0)
		{
			if(m_name.get_size() > 0)
			{
				if(treeDepth == 0)
				{
					string += "{";
				}
				string += "\"" + m_name + "\":";
				if(prettyPrinting == true)
				{
					string += ' ';
				}
			}
			if(m_value.get_size() <= 0 || m_value == "")
			{
				string += "null";
			}
			else
			{
				string += m_value;
			}
			if(treeDepth == 0)
			{
				string += "}";
			}
			return(OK);
		}
		else
		{
			if(m_name.get_size() > 0 && treeDepth > 0)
			{
				string += "\"" + m_name + "\":";
				if(prettyPrinting == true)
				{
					string += '\n';
					string += fill;
				}
			}
			string += '{';
			if(prettyPrinting == true)
			{
				string += '\n';
			}
		}
	}
	
	
	
	const uint32 numberOfChildren = m_children.get_size();
	for(uint32 i= 0; i < numberOfChildren; i++)
	{
		if(m_children[i]->transformToString(string, prettyPrinting, deleteChildrenAfterTransformation, treeDepth + 1) != OK)
		{
			string.erase();
			return(FAIL);
		}
		if(deleteChildrenAfterTransformation == true)
		{
			delete m_children[i];
			m_children[i] = nullptr;
		}
		
		if(i < numberOfChildren - 1)
		{
			string += ',';
		}
		if(prettyPrinting == true)
		{
			string += '\n';
		}
	}
	if(numberOfChildren == 0)
	{
		if(prettyPrinting == true)
		{
			string += Array<char>('\t', treeDepth + 1) + '\n';
		}
	}
	
	if(prettyPrinting == true)
	{
		string += fill;
	}
	string += closingBracket;
	
	
	return(OK);
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

JSON::JSON()
	:	m_isArray(false),
		m_name(""),
		m_value(""),
		m_valid(false)
{
	m_children.erase();
	m_valid = true;
}


JSON::JSON(String name, String data)
	:	m_isArray(false),
		m_name(name),
		m_value(""),
		m_valid(false)
{
	if(data.get_size() == 0)
	{
		return;
	}
	
	
	//	Delete all non printable Characters and all Spaces that arent contained in String Quotes
	{
		bool isString = false;
		for(uint32 i = 0; i < data.get_size(); i++)
		{
			const char& character(data[i]);
			
			if(String::isPrintable(character) == false)
			{
				data.erase(i, 1); 
				i--;
			}
			else
			{
				if(character == '\"')
				{
					if(isString == false)
					{
						isString = true;
					}
					else
					{
						isString = false;
					}
				}
				else
				{
					if(character == ' ' && isString == false)
					{
						data.erase(i, 1);
						i--;
					}
				}
			}
		}
	}
	
	
	
	//	Check for simple Value
	{
		bool isSimpleValue = true;
		bool isString = false;
		const uint32 numberOfCharacters = data.get_size();
		for(uint32 i = 0; i < numberOfCharacters; i++)
		{
			const char& character(data[i]);
			if(character == '\"')
			{
				if(isString == false)
				{
					isString = true;
				}
				else
				{
					isString = false;
				}
			}
			else
			{
				if(isString == false)
				{
					if(character == '[' || character == '{' || character == ',' || character == ':')
					{
						isSimpleValue = false;
						break;
					}
				}
			}
		}
		
		if(isSimpleValue == true)
		{
			m_value = data;
			m_valid = true;
			return;
		}
	}
	
	
	char bracketOpening = '{';
	char bracketClosing = '}';
	//	Delete leading and trailing Garbage and detect Object or Array
	{
		std::size_t position_objectBraces	= data.find('{');
		std::size_t position_arrayBraces	= data.find('[');
		
		uint32 position_start = position_objectBraces;
		if(position_arrayBraces < position_objectBraces)
		{
			position_start = position_arrayBraces;
			bracketOpening = '[';
			bracketClosing = ']';
			
			m_isArray = true;
		}
		
		uint32 position_end = data.findMatchingBracket(position_start, bracketOpening, bracketClosing);
		if(position_end == data.get_size())
		{
			return;
		}
		data = data.sub(position_start, position_end - position_start + 1);
	}
	
	
	
	const uint32 numberOfCharacters = data.get_size();
	
	bool isString = false;
	uint32 startOfElement = 1;
	for(uint32 i = startOfElement; i < numberOfCharacters - 1; i++)
	{
		const char& character(data[i]);
		if(character == '\"')
		{
			if(isString == false)
			{
				isString = true;
			}
			else
			{
				isString = false;
			}
		}
		else
		{
			if(isString == false)
			{
				if(character == ',' || character == bracketClosing)
				{
					String child_data(data.sub(startOfElement, i - startOfElement));
					String name_child("");
					if(m_isArray == false)
					{
						Array components(child_data.explodeIfNotInStringQuotes(':'));
						if(components.get_size() != 2)
						{
							return;
						}
						name_child = components[0];
						if(name_child[0] != '\"' || name_child[name_child.get_size() - 1] != '\"')
						{
							return;
						}
						name_child.erase(0, 1);
						name_child.erase(name_child.get_size() - 1, 1);
						
						
						
						child_data = components[1];
					}
					
					JSON* json = new JSON(name_child, child_data);
					if(json->is_valid() == false)
					{
						return;
					}
					m_children += json;
					startOfElement = i + 1;
					continue;
				}
				if(character == '{')
				{
					uint32 position = data.findMatchingBracket(i, '{', '}');
					if(position == numberOfCharacters)
					{
						return;
					}
					
					String name_child("");
					if(i != startOfElement)
					{
						name_child = data.sub(startOfElement, i - startOfElement);
						if(name_child[name_child.get_size() - 1] != ':')
						{
							return;
						}
						name_child.erase(name_child.get_size() - 1, 1);
						if(name_child[0] != '\"' || name_child[name_child.get_size() - 1] != '\"')
						{
							return;
						}
						name_child.erase(0, 1);
						name_child.eraseFromEnd(1);
					}
					
					JSON* json = new JSON(name_child, data.sub(i, position - i + 1));
					if(json->is_valid() == false)
					{
						return;
					}
					m_children += json;
					
					
					
					i = position + 1;
					startOfElement = i + 1;
					
					//	Check for correct Syntax
					{
						const char& character_next(data[i]);
						if(character_next != bracketClosing && character_next != ',')
						{
							return;
						}
					}
					
					continue;
				}
				if(character == '[')
				{
					uint32 position = data.findMatchingBracket(i, '[', ']');
					if(position == numberOfCharacters)
					{
						return;
					}
					
					String name_child("");
					if(i != startOfElement)
					{
						name_child = data.sub(startOfElement, i - startOfElement);
						if(name_child[name_child.get_size() - 1] != ':')
						{
							return;
						}
						name_child.eraseFromEnd(1);
						if(name_child[0] != '\"' || name_child[name_child.get_size() - 1] != '\"')
						{
							return;
						}
						name_child.erase(0, 1);
						name_child.eraseFromEnd(1);
					}
					
					JSON* json = new JSON(name_child, data.sub(i, position - i + 1));
					if(json->is_valid() == false)
					{
						return;
					}
					m_children += json;
					
					
					
					i = position + 1;
					startOfElement = i + 1;
					
					//	Check for correct Syntax
					{
						const char& character_next(data[i]);
						if(character_next != bracketClosing && character_next != ',')
						{
							return;
						}
					}
					
					continue;
				}
			}
		}
	}
	
	if(startOfElement < numberOfCharacters - 1)
	{
		String child_data(data.sub(startOfElement, numberOfCharacters - startOfElement - 1));
		
		String name_child("");
		if(m_isArray == false)
		{
			Array components(child_data.explodeIfNotInStringQuotes(':'));
			if(components.get_size() != 2)
			{
				return;
			}
			name_child = components[0];
			if(name_child[0] != '\"' || name_child[name_child.get_size() - 1] != '\"')
			{
				return;
			}
			name_child.erase(0, 1);
			name_child.eraseFromEnd(1);
			
			
			
			child_data = components[1];
		}
		
		JSON* json = new JSON(name_child, child_data);
		if(json->is_valid() == false)
		{
			return;
		}
		m_children += json;
	}
	
	m_valid = true;
}


JSON::~JSON()
{
	for(auto& i: m_children)
	{
		if(i != nullptr)
		{
			delete i;
		}
	}
}








String JSON::get_valueWithoutQuotes() const
{
	if(m_valid == false)
	{
		return("");
	}
	String value(m_value);
	if(value.get_size() >= 2)
	{
		if(value[0] == '\"' && value[value.get_size() - 1] == '\"')
		{
			value.erase(0, 1);
			value.eraseFromEnd(1);
		}
	}
	return(value);
}


bool JSON::has_child(String childName) const
{
	if(m_valid == false)
	{
		return(false);
	}
	const uint32 numberOfChildren = m_children.get_size();
	for(uint32 i = 0; i < numberOfChildren; i++)
	{
		const JSON* child = m_children[i];
		if(child->m_name == childName)
		{
			return(true);
		}
	}
	return(false);
}


JSON* JSON::get_child(String childName) const
{
	if(m_valid == false)
	{
		return(nullptr);
	}
	const uint32 numberOfChildren = m_children.get_size();
	for(uint32 i = 0; i < numberOfChildren; i++)
	{
		JSON* child = m_children[i];
		if(child->m_name == childName)
		{
			return(child);
		}
	}
	return(nullptr);
}


JSON* JSON::get_child(uint32 childNumber) const
{
	if(m_valid == false)
	{
		return(nullptr);
	}
	if(childNumber >= m_children.get_size())
	{
		return(nullptr);
	}
	return(m_children[childNumber]);
}







void JSON::erase()
{
	m_isArray = false;
	m_name.erase();
	m_value.erase();
	
	for(auto& i: m_children)
	{
		delete i;
	}
	m_children.erase();
	
	m_valid = true;
}







JSON& JSON::operator+=(JSON* child)
{
	if(m_valid == true)
	{
		if(m_isArray == true)
		{
			if(child->m_name.get_size() > 0)
			{
				return(*this);
			}
		}
		m_children += child;
	}
	return(*this);
}


JSON& JSON::operator+=(Array<String> array)
{
	if(m_valid == true)
	{
		if(m_children.get_size() == 0 || m_isArray == true)
		{
			m_isArray = true;
			
			for(auto& i: array)
			{
				operator+=(new JSON("", i));
			}
		}
	}
	return(*this);
}


JSON& JSON::operator+=(Array<JSON*> array)
{
	if(m_valid == true)
	{
		if(m_children.get_size() == 0 || m_isArray == true)
		{
			m_isArray = true;
			
			for(auto& i: array)
			{
				operator+=(i);
			}
		}
	}
	return(*this);
}


JSON& JSON::operator-=(JSON* child)
{
	if(m_valid == true)
	{
		for(uint32 i = 0; i < m_children.get_size(); i++)
		{
			JSON* child2 = m_children[i];
			if(child2 == child)
			{
				m_children.erase(i);
				i--;
			}
		}
	}
	return(*this);
}


JSON& JSON::operator=(const JSON& json)
{
	erase();
	
	m_isArray		= json.m_isArray;
	m_name			= json.m_name;
	m_value			= json.m_value;
	m_valid			= json.m_valid;
	
	for(auto& i: json.m_children)
	{
		m_children += new JSON(*i);
	}
	return(*this);
}







/*
String JSON::transformToString(bool prettyPrinting, uint32 treeDepth) const
{
	if(m_valid == false)
	{
		return("");
	}
	
	String fill(treeDepth, '\t');
	
	
	char closingBracket = '}';
	String data;
	if(pretty == true)
	{
		data += fill;
	}
	
	if(m_isArray == true)
	{
		if(m_name.get_size() > 0)
		{
			data += "\"" + m_name + "\":";
			if(pretty == true)
			{
				data += '\n';
				data += fill;
			}
		}
		data += '[';
		if(pretty == true)
		{
			data += '\n';
		}
		closingBracket = ']';
	}
	else
	{
		if(m_children.get_size() == 0)
		{
			if(m_name.get_size() > 0)
			{
				data += "\"" + m_name + "\":";
				if(pretty == true)
				{
					data += ' ';
				}
			}
			data += m_value;
			return(data);
		}
		else
		{
			if(m_name.get_size() > 0 && treeDepth > 0)
			{
				data += "\"" + m_name + "\":";
				if(pretty == true)
				{
					data += '\n';
					data += fill;
				}
			}
			data += '{';
			if(pretty == true)
			{
				data += '\n';
			}
		}
	}
	
	
	
	const uint32 numberOfChildren = m_children.get_size();
	for(uint32 i= 0; i < numberOfChildren; i++)
	{
		data += m_children[i]->transformToString(treeDepth + 1, pretty);
		if(i < numberOfChildren - 1)
		{
			data += ',';
		}
		if(pretty == true)
		{
			data += '\n';
		}
	}
	if(numberOfChildren == 0)
	{
		if(pretty == true)
		{
			data += Array<char>('\t', treeDepth + 1) + '\n';
		}
	}
	
	if(pretty == true)
	{
		data += fill;
	}
	data += closingBracket;
	
	
	return(data);
}*/
