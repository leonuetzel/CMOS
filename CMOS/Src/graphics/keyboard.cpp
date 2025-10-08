#include "../../Inc/graphics/graphics.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/

const Keyboard::Key Keyboard::c_layout[] = 
{
	//	Line 0
	{nullptr, Rectangle<float>(0.00, 0.00, 0.14, 0.19), "!#="  , "ABC"  , e_key::LAYOUT	, e_key::LAYOUT},
	{nullptr, Rectangle<float>(0.15, 0.00, 0.10, 0.19), ","    , ";"    , e_key::SIGN  	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.26, 0.00, 0.49, 0.19), " "    , " "    , e_key::SIGN  	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.76, 0.00, 0.10, 0.19), "."    , ":"    , e_key::SIGN  	, e_key::SIGN},
	
	//	Line 0 - 1
	{nullptr, Rectangle<float>(0.87, 0.00, 0.13, 0.40), "Enter", "Enter", e_key::ENTER 	, e_key::ENTER},
	
	//	Line 1
	{nullptr, Rectangle<float>(0.00, 0.21, 0.13, 0.19), "Shift", ""     , e_key::SHIFT	, e_key::NONE},
	{nullptr, Rectangle<float>(0.14, 0.21, 0.08, 0.19), "Y"    , ""     , e_key::LETTER	, e_key::NONE},
	{nullptr, Rectangle<float>(0.23, 0.21, 0.08, 0.19), "X"    , ""     , e_key::LETTER	, e_key::NONE},
	{nullptr, Rectangle<float>(0.32, 0.21, 0.08, 0.19), "C"    , ""     , e_key::LETTER	, e_key::NONE},
	{nullptr, Rectangle<float>(0.41, 0.21, 0.08, 0.19), "V"    , ""     , e_key::LETTER	, e_key::NONE},
	{nullptr, Rectangle<float>(0.50, 0.21, 0.08, 0.19), "B"    , ""     , e_key::LETTER	, e_key::NONE},
	{nullptr, Rectangle<float>(0.59, 0.21, 0.08, 0.19), "N"    , ""     , e_key::LETTER	, e_key::NONE},
	{nullptr, Rectangle<float>(0.68, 0.21, 0.08, 0.19), "M"    , ""     , e_key::LETTER	, e_key::NONE},
	{nullptr, Rectangle<float>(0.77, 0.21, 0.09, 0.19), "-"    , "_"    , e_key::SIGN		, e_key::SIGN},
	
	//	Line 2
	{nullptr, Rectangle<float>(0.11, 0.42, 0.08, 0.19), "A"     , ""    , e_key::LETTER	, e_key::NONE},
	{nullptr, Rectangle<float>(0.20, 0.42, 0.08, 0.19), "S"     , ""    , e_key::LETTER	, e_key::NONE},
	{nullptr, Rectangle<float>(0.29, 0.42, 0.08, 0.19), "D"     , ""    , e_key::LETTER	, e_key::NONE},
	{nullptr, Rectangle<float>(0.38, 0.42, 0.08, 0.19), "F"     , ""    , e_key::LETTER	, e_key::NONE},
	{nullptr, Rectangle<float>(0.47, 0.42, 0.08, 0.19), "G"     , ""    , e_key::LETTER	, e_key::NONE},
	{nullptr, Rectangle<float>(0.56, 0.42, 0.08, 0.19), "H"     , ""    , e_key::LETTER	, e_key::NONE},
	{nullptr, Rectangle<float>(0.65, 0.42, 0.08, 0.19), "J"     , ""    , e_key::LETTER	, e_key::NONE},
	{nullptr, Rectangle<float>(0.74, 0.42, 0.08, 0.19), "K"     , ""    , e_key::LETTER	, e_key::NONE},
	{nullptr, Rectangle<float>(0.83, 0.42, 0.08, 0.19), "L"     , ""    , e_key::LETTER	, e_key::NONE},
	
	//	Line 3
	{nullptr, Rectangle<float>(0.05, 0.63, 0.08, 0.19), "Q"     , "+"   , e_key::LETTER	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.14, 0.63, 0.08, 0.19), "W"     , "~"   , e_key::LETTER	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.23, 0.63, 0.08, 0.19), "E"     , "*"   , e_key::LETTER	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.32, 0.63, 0.08, 0.19), "R"     , "'"   , e_key::LETTER	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.41, 0.63, 0.08, 0.19), "T"     , "#"   , e_key::LETTER	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.50, 0.63, 0.08, 0.19), "Z"     , "-"   , e_key::LETTER	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.59, 0.63, 0.08, 0.19), "U"     , "{"   , e_key::LETTER	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.68, 0.63, 0.08, 0.19), "I"     , "["   , e_key::LETTER	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.77, 0.63, 0.08, 0.19), "O"     , "]"   , e_key::LETTER	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.86, 0.63, 0.08, 0.19), "P"     , "}"   , e_key::LETTER	, e_key::SIGN},
	
	//	Line 4
	{nullptr, Rectangle<float>(0.00, 0.84, 0.08, 0.19), "1"     , "!"   , e_key::NUMBER	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.09, 0.84, 0.08, 0.19), "2"     , "\""  , e_key::NUMBER	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.18, 0.84, 0.08, 0.19), "3"     , ""    , e_key::NUMBER	, e_key::NONE},
	{nullptr, Rectangle<float>(0.27, 0.84, 0.08, 0.19), "4"     , "$"   , e_key::NUMBER	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.36, 0.84, 0.08, 0.19), "5"     , "%"   , e_key::NUMBER	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.45, 0.84, 0.08, 0.19), "6"     , "&"   , e_key::NUMBER	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.54, 0.84, 0.08, 0.19), "7"     , "/"   , e_key::NUMBER	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.63, 0.84, 0.08, 0.19), "8"     , "("   , e_key::NUMBER	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.72, 0.84, 0.08, 0.19), "9"     , ")"   , e_key::NUMBER	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.81, 0.84, 0.08, 0.19), "0"     , "="   , e_key::NUMBER	, e_key::SIGN},
	{nullptr, Rectangle<float>(0.90, 0.84, 0.10, 0.19), "Del"  , "Del"  , e_key::DELETE	, e_key::DELETE}
};



/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

Keyboard::Keyboard()
	:	m_caseMode(e_caseMode::FIRST_UPPER),
		m_upperCase(true),
		m_primaryMode(true),
		m_enterPressed(false)
{
	Graphics& graphics = Graphics::get();
	const Vec2 displayDimensions = graphics.get_displayDimensions();
	const Vec2 keyboardDimensions = Vec2(displayDimensions.x * 1.0f, displayDimensions.y * 0.5f);
	
	
	//	Create Buttons
	for(auto& i: c_layout)
	{
		//	Create Button
		Button* button = new Button
		(
			Element
			(
				Rect
				(
					i.area.position.x * keyboardDimensions.x,
					i.area.position.y * keyboardDimensions.y,
					i.area.size.x * keyboardDimensions.x,
					i.area.size.y * keyboardDimensions.y
				),
				graphics.get_pageActual(),
				graphics.get_numberOfLayers() - 1,
				nullptr,
				callback,
				0,
				Element::e_frameType::ROUNDED
			),
			i.primary_text,
			Fonts::CalibriLight16,
			Colors::white,
			Colors::black,
			Colors::silver,
			Colors::royal_blue,
			10
		);
		
		
		//	Create Key to save
		Key key = i;
		key.button = button;
		
		
		//	Save Key
		m_keys += key;
	}
	
	
	//	Create Text Display
	m_text = new Element
	(
		Rect(0, displayDimensions.y * 0.65f, displayDimensions.x, 30),
		graphics.get_pageActual(),
		graphics.get_numberOfLayers() - 1,
		update,
		nullptr,
		0,
		Element::e_frameType::ROUNDED
	);
}


Keyboard::~Keyboard()
{
	(*this)(false);
	for(auto& i: m_keys)
	{
		delete i.button;
	}
	if(m_text != nullptr)
	{
		delete m_text;
	}
}







void Keyboard::callback(Element& element)
{
	Button& button = (Button&) element;
	
	
	//	Check for valid Button
	Keyboard& keyboard = Keyboard::get();
	Key* key = nullptr;
	for(auto& i: keyboard.m_keys)
	{
		if(i.button == &button)
		{
			key = &i;
		}
	}
	if(key == nullptr)
	{
		return;
	}
	
	
	//	Read Key Category
	e_key keyCategory = key->primary_ID;
	if(keyboard.m_primaryMode == false)
	{
		keyCategory = key->secondary_ID;
	}
	
	
	switch(keyCategory)
	{
		case e_key::LAYOUT:
		{
			if(keyboard.m_primaryMode == true)
			{
				for(auto& i: keyboard.m_keys)
				{
					i.button->set_text(i.secondary_text);
				}
				keyboard.m_primaryMode = false;
			}
			else
			{
				for(auto& i: keyboard.m_keys)
				{
					i.button->set_text(i.primary_text);
				}
				keyboard.m_primaryMode = true;
			}
		}
		break;
		
		case e_key::ENTER:
		{
			keyboard.m_enterPressed = true;
			keyboard(false, false);
		}
		break;
		
		case e_key::SHIFT:
		{
			switch(keyboard.m_caseMode)
			{
				case e_caseMode::LOWER_CASE:
				{
					keyboard.m_caseMode = e_caseMode::FIRST_UPPER;
					if(keyboard.m_upperCase == false)
					{
						keyboard.switchCase();
					}
				}
				break;
				
				case e_caseMode::UPPER_CASE:
				{
					keyboard.m_caseMode = e_caseMode::LOWER_CASE;
					if(keyboard.m_upperCase == true)
					{
						keyboard.switchCase();
					}
				}
				break;
				
				case e_caseMode::FIRST_UPPER:
				{
					keyboard.m_caseMode = e_caseMode::UPPER_CASE;
					if(keyboard.m_upperCase == false)
					{
						keyboard.switchCase();
					}
				}
				break;
				
				default:
				{
					
				}
				break;
			}
		}
		break;
		
		case e_key::LETTER:
		case e_key::NUMBER:
		case e_key::SIGN:
		{
			keyboard.m_buffer += button.get_text();
			if(keyboard.m_buffer.get_size() > 0)
			{
				if(keyboard.m_caseMode != e_caseMode::UPPER_CASE)
				{
					if(keyboard.m_upperCase == true)
					{
						keyboard.switchCase();
					}
					if(keyboard.m_caseMode == e_caseMode::FIRST_UPPER)
					{
						keyboard.m_caseMode = e_caseMode::LOWER_CASE;
					}
				}
			}
		}
		break;
		
		case e_key::DELETE:
		{
			keyboard.m_buffer.eraseFromEnd(1);
			if(keyboard.m_buffer.get_size() == 0)
			{
				keyboard.m_caseMode = e_caseMode::FIRST_UPPER;
				if(keyboard.m_upperCase == false)
				{
					keyboard.switchCase();
				}
			}
		}
		break;
		
		default:
		{
			
		}
		break;
	}
	
	
	//	Update Text Display
	if(keyCategory != e_key::ENTER)
	{
		keyboard.m_text->requestUpdate();
	}
}


void Keyboard::update(Element& element)
{
	element.draw_background(Colors::black);
	element.draw_frame(Colors::silver),
	element.draw_string(Keyboard::get().m_buffer, Element::e_align::CENTER, Fonts::CalibriLight16, Colors::white);
}







void Keyboard::switchCase()
{
	if(m_upperCase == true)
	{
		for(auto& i: m_keys)
		{
			e_key keyCategory = i.primary_ID;
			if(m_primaryMode == false)
			{
				keyCategory = i.secondary_ID;
			}
			
			
			if(keyCategory == e_key::LETTER)
			{
				Button& letterButton = *i.button;
				
				String text = letterButton.get_text();
				text.lowerCase();
				letterButton.set_text(text);
			}
		}
		m_upperCase = false;
	}
	else
	{
		for(auto& i: m_keys)
		{
			e_key keyCategory = i.primary_ID;
			if(m_primaryMode == false)
			{
				keyCategory = i.secondary_ID;
			}
			
			
			if(keyCategory == e_key::LETTER)
			{
				Button& letterButton = *i.button;
				
				String text = letterButton.get_text();
				text.upperCase();
				letterButton.set_text(text);
			}
		}
		m_upperCase = true;
	}
}



/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

Keyboard& Keyboard::get()
{
	static Keyboard keyboard;
	return(keyboard);
}







String Keyboard::operator()(bool show, bool waitForEnter)
{
	Graphics& graphics = Graphics::get();
	
	
	//	Display or Hide Keyboard
	if(show == true)
	{
		const uint8 pageActual = graphics.get_pageActual();
		for(auto& i: m_keys)
		{
			graphics += i.button;
			i.button->set_page(pageActual);
		}
		graphics += m_text;
		m_text->set_page(pageActual);
		
		
		//	Set Blending to dim other Elements and disable Touch Function of other Layers
		const uint32 numberOfLayers = graphics.get_numberOfLayers();
		for(uint32 i = 0; i < numberOfLayers - 1; i++)
		{
			graphics.set_layerAlpha(i, 30);
			graphics.set_layerTouchability(i, false);
		}
	}
	else
	{
		for(auto& i: m_keys)
		{
			graphics -= i.button;
		}
		graphics -= m_text;
		
		
		//	Reset Blending and Touchability
		const uint32 numberOfLayers = graphics.get_numberOfLayers();
		for(uint32 i = 0; i < numberOfLayers - 1; i++)
		{
			graphics.set_layerAlpha(i, 0xFF);
			graphics.set_layerTouchability(i, true);
		}
	}
	
	
	//	Reset State
	m_caseMode = e_caseMode::FIRST_UPPER;
	if(m_upperCase == false)
	{
		switchCase();
	}
	m_primaryMode = true;
	m_enterPressed = false;
	
	
	//	Wait for Enter
	String userInput = m_buffer;
	m_buffer.erase();
	
	if(show == true && waitForEnter == true)
	{
		CMOS& cmos = CMOS::get();
		while(m_enterPressed == false)
		{
			cmos.sleep_ms(20);
		}
		userInput += (*this)(false);
	}
	
	
	//	Return User Input
	return(userInput);
}