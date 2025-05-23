#include "../../Inc/graphics/graphics.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/

MessageBox* MessageBox::m_messageBox = nullptr;



/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

void MessageBox::update(Element& element)
{
	if(m_messageBox != nullptr)
	{
		element.draw_string(m_messageBox->m_message, Element::e_align::CENTER, Fonts::CalibriLight24, Colors::white, true);
	}
}


void MessageBox::callback(Element& element)
{
	if(m_messageBox == nullptr)
	{
		return;
	}
	
	
	//	Check if another Button has already been pressed
	if(m_messageBox->m_userChoice != e_button::NONE)
	{
		return;
	}
	
	
	//	Enter User Choice
	Button* button = (Button*) &element;
	for(uint32 i = 0; i < 6; i++)
	{
		if(m_messageBox->m_buttons[i] == button)
		{
			m_messageBox->m_userChoice = (e_button) i;
			break;
		}
	}
}



/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

MessageBox::MessageBox(e_type type, const String& message)
	:	m_message(message),
		m_userChoice(e_button::NONE)
{
	if(m_messageBox != nullptr)
	{
		return;
	}
	
	
	//	Set all Button Pointer to null
	for(auto& i: m_buttons)
	{
		i = nullptr;
	}
	
	
	//	Get Display Dimensions
	Graphics& graphics = Graphics::get();
	const Vec2 displaySize = graphics.get_displayDimensions();
	
	
	//	Button Size
	constexpr Vec2 buttonSize = Vec2(150, 100);
	
	
	//	Center Position of Middle Button
	const Vec2 center = Vec2(displaySize.x / 2, displaySize.y / 3);
	
	
	//	Create Buttons depending on Type
	switch(type)
	{
		case e_type::OK_:
		{
			//	OK Button
			Button* button_ok = new Button
			(
				Element
				(
					Rect(center - buttonSize / 2, buttonSize),
					graphics.get_pageActual(),
					graphics.get_numberOfLayers() - 1,
					nullptr,
					callback,
					0,
					Element::e_frameType::ROUNDED
				),
				"OK",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::royal_blue,
				c_pressTime
			);
			graphics += button_ok;
			m_buttons[(uint8) e_button::OK_] = button_ok;
		}
		break;
		
		case e_type::OK_CANCEL:
		{
			//	Bottom Left Button Position for a single Button
			Vec2 position(center - buttonSize / 2);
			
			
			//	OK Button
			Button* button_ok = new Button
			(
				Element
				(
					Rect(Vec2(position.x - buttonSize.x, position.y), buttonSize),
					graphics.get_pageActual(),
					graphics.get_numberOfLayers() - 1,
					nullptr,
					callback,
					0,
					Element::e_frameType::ROUNDED
				),
				"OK",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::royal_blue,
				c_pressTime
			);
			graphics += button_ok;
			m_buttons[(uint8) e_button::OK_] = button_ok;
			
			
			//	Cancel Button
			Button* button_cancel = new Button
			(
				Element
				(
					Rect(Vec2(position.x + buttonSize.x, position.y), buttonSize),
					graphics.get_pageActual(),
					graphics.get_numberOfLayers() - 1,
					nullptr,
					callback,
					0,
					Element::e_frameType::ROUNDED
				),
				"Cancel",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::yellow,
				c_pressTime
			);
			graphics += button_cancel;
			m_buttons[(uint8) e_button::CANCEL] = button_cancel;
		}
		break;
		
		case e_type::RETRY_CANCEL:
		{
			//	Bottom Left Button Position for a single Button
			Vec2 position(center - buttonSize / 2);
			
			
			//	Retry Button
			Button* button_retry = new Button
			(
				Element
				(
					Rect(Vec2(position.x - buttonSize.x, position.y), buttonSize),
					graphics.get_pageActual(),
					graphics.get_numberOfLayers() - 1,
					nullptr,
					callback,
					0,
					Element::e_frameType::ROUNDED
				),
				"Retry",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::turquoise,
				c_pressTime
			);
			graphics += button_retry;
			m_buttons[(uint8) e_button::RETRY] = button_retry;
			
			
			//	Cancel Button
			Button* button_cancel = new Button
			(
				Element
				(
					Rect(Vec2(position.x + buttonSize.x, position.y), buttonSize),
					graphics.get_pageActual(),
					graphics.get_numberOfLayers() - 1,
					nullptr,
					callback,
					0,
					Element::e_frameType::ROUNDED
				),
				"Cancel",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::yellow,
				c_pressTime
			);
			graphics += button_cancel;
			m_buttons[(uint8) e_button::CANCEL] = button_cancel;
		}
		break;
		
		case e_type::RETRY_CANCEL_CONTINUE:
		{
			//	Bottom Left Button Position for a single Button
			Vec2 position(center - buttonSize / 2);
			
			
			//	Retry Button
			Button* button_retry = new Button
			(
				Element
				(
					Rect(Vec2(position.x - 2 * buttonSize.x, position.y), buttonSize),
					graphics.get_pageActual(),
					graphics.get_numberOfLayers() - 1,
					nullptr,
					callback,
					0,
					Element::e_frameType::ROUNDED
				),
				"Retry",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::turquoise,
				c_pressTime
			);
			graphics += button_retry;
			m_buttons[(uint8) e_button::RETRY] = button_retry;
			
			
			//	Cancel Button
			Button* button_cancel = new Button
			(
				Element
				(
					Rect(position, buttonSize),
					graphics.get_pageActual(),
					graphics.get_numberOfLayers() - 1,
					nullptr,
					callback,
					0,
					Element::e_frameType::ROUNDED
				),
				"Cancel",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::yellow,
				c_pressTime
			);
			graphics += button_cancel;
			m_buttons[(uint8) e_button::CANCEL] = button_cancel;
			
			
			//	Continue Button
			Button* button_continue = new Button
			(
				Element
				(
					Rect(Vec2(position.x + 2 * buttonSize.x, position.y), buttonSize),
					graphics.get_pageActual(),
					graphics.get_numberOfLayers() - 1,
					nullptr,
					callback,
					0,
					Element::e_frameType::ROUNDED
				),
				"Continue",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::yellow,
				c_pressTime
			);
			graphics += button_continue;
			m_buttons[(uint8) e_button::CONTINUE] = button_continue;
		}
		break;
		
		case e_type::YES_NO:
		{
			//	Bottom Left Button Position for a single Button
			Vec2 position(center - buttonSize / 2);
			
			
			//	Yes Button
			Button* button_yes = new Button
			(
				Element
				(
					Rect(Vec2(position.x - buttonSize.x, position.y), buttonSize),
					graphics.get_pageActual(),
					graphics.get_numberOfLayers() - 1,
					nullptr,
					callback,
					0,
					Element::e_frameType::ROUNDED
				),
				"Yes",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::green,
				c_pressTime
			);
			graphics += button_yes;
			m_buttons[(uint8) e_button::YES] = button_yes;
			
			
			//	No Button
			Button* button_no = new Button
			(
				Element
				(
					Rect(Vec2(position.x + buttonSize.x, position.y), buttonSize),
					graphics.get_pageActual(),
					graphics.get_numberOfLayers() - 1,
					nullptr,
					callback,
					0,
					Element::e_frameType::ROUNDED
				),
				"No",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::red,
				c_pressTime
			);
			graphics += button_no;
			m_buttons[(uint8) e_button::NO] = button_no;
		}
		break;
		
		case e_type::YES_NO_CANCEL:
		{
			//	Bottom Left Button Position for a single Button
			Vec2 position(center - buttonSize / 2);
			
			
			//	Yes Button
			Button* button_yes = new Button
			(
				Element
				(
					Rect(Vec2(position.x - 2 * buttonSize.x, position.y), buttonSize),
					graphics.get_pageActual(),
					graphics.get_numberOfLayers() - 1,
					nullptr,
					callback,
					0,
					Element::e_frameType::ROUNDED
				),
				"Yes",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::green,
				c_pressTime
			);
			graphics += button_yes;
			m_buttons[(uint8) e_button::YES] = button_yes;
			
			
			//	No Button
			Button* button_no = new Button
			(
				Element
				(
					Rect(position, buttonSize),
					graphics.get_pageActual(),
					graphics.get_numberOfLayers() - 1,
					nullptr,
					callback,
					0,
					Element::e_frameType::ROUNDED
				),
				"No",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::red,
				c_pressTime
			);
			graphics += button_no;
			m_buttons[(uint8) e_button::NO] = button_no;
			
			
			//	Cancel Button
			Button* button_cancel = new Button
			(
				Element
				(
					Rect(Vec2(position.x + 2 * buttonSize.x, position.y), buttonSize),
					graphics.get_pageActual(),
					graphics.get_numberOfLayers() - 1,
					nullptr,
					callback,
					0,
					Element::e_frameType::ROUNDED
				),
				"Cancel",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::yellow,
				c_pressTime
			);
			graphics += button_cancel;
			m_buttons[(uint8) e_button::CANCEL] = button_cancel;
		}
		break;
		
		default:
		{
			return;
		}
		break;
	}
	m_messageBox = this;
	
	
	//	Message Element
	const Vec2 textSize = Vec2(displaySize / 5);
	const Vec2 textPosition = Vec2(displaySize.x / 2, (2 * displaySize.y) / 3) - textSize / 2;
	Element* text = new Element
	(
		Rect(textPosition, textSize),
		graphics.get_pageActual(),
		graphics.get_numberOfLayers() - 1,
		update,
		nullptr,
		0,
		Element::e_frameType::NONE
	);
	graphics += text;
	m_text = text;
	
	
	//	Set Blending to dim other Elements and disable Touch Function of other Layers
	const uint32 numberOfLayers = graphics.get_numberOfLayers();
	for(uint32 i = 0; i < numberOfLayers - 1; i++)
	{
		graphics.set_layerAlpha(i, 30);
		graphics.set_layerTouchability(i, false);
	}
	
	
	//	Wait for User to choose
	CMOS& cmos = CMOS::get();
	while(m_userChoice == e_button::NONE)
	{
		cmos.sleep_ms(20);
	}
}


MessageBox::~MessageBox()
{
	//	Free Memory (Element Destructor will detach itself from Graphics)
	CMOS& cmos = CMOS::get();
	Graphics& graphics = Graphics::get();
	for(auto& i: m_buttons)
	{
		if(i != nullptr)
		{
			graphics -= i;
			while(cmos.semaphore_doesExist(i) == true)
			{
				cmos.sleep_ms(5);
			}
			delete i;
		}
	}
	if(m_text != nullptr)
	{
		graphics -= m_text;
		while(cmos.semaphore_doesExist(m_text) == true)
		{
			cmos.sleep_ms(5);
		}
		delete m_text;
	}
	
	
	//	Reset Blending and Touchability
	const uint32 numberOfLayers = graphics.get_numberOfLayers();
	for(uint32 i = 0; i < numberOfLayers - 1; i++)
	{
		graphics.set_layerAlpha(i, 0xFF);
		graphics.set_layerTouchability(i, true);
	}
	
	
	m_messageBox = nullptr;
}







MessageBox::e_button MessageBox::getUserChoice()
{
	return(m_userChoice);
}