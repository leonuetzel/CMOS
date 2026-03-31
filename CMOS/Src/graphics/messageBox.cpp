#include "../../Inc/graphics/graphics.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/

UniqueArray<MessageBox*> MessageBox::m_messageBoxes;



/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

void MessageBox::update(Element& element)
{
	//	Find the MessageBox which has to be updated
	MessageBox* messageBox = nullptr;
	for(auto& i: m_messageBoxes)
	{
		if(i->m_page == element.get_page())
		{
			messageBox = i;
			break;
		}
	}
	if(messageBox == nullptr)
	{
		return;
	}
	
	
	//	Update message text
	element.draw_string(messageBox->m_message, Element::e_align::CENTER, Fonts::CalibriLight24, Colors::white, true);
}


void MessageBox::callback(Element& element)
{
	//	Find the MessageBox which has been interacted with
	MessageBox* messageBox = nullptr;
	for(auto& i: m_messageBoxes)
	{
		if(i->m_page == element.get_page())
		{
			messageBox = i;
			break;
		}
	}
	if(messageBox == nullptr)
	{
		return;
	}
	
	
	//	Check if another button has already been pressed
	if(messageBox->m_userChoice != e_button::NONE)
	{
		return;
	}
	
	
	//	Enter user choice
	Button* button = (Button*) &element;
	for(uint32 i = 0; i < 6; i++)
	{
		if(messageBox->m_buttons[i] == button)
		{
			messageBox->m_userChoice = (e_button) i;
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
	:	m_pageOnCreation(Graphics::get().get_currentPage()),
		m_page(Graphics::get().add_page()),
		m_message(message),
		m_userChoice(e_button::NONE)
{
	if(m_page == 0xFF)
	{
		return;
	}
	
	
	//	Set all button pointer to null
	for(auto& i: m_buttons)
	{
		i = nullptr;
	}
	
	
	//	Get display dimensions
	Graphics& graphics = Graphics::get();
	const Vec2 displaySize = graphics.get_displayDimensions();
	
	
	//	Show MessageBox on its own page which has been freshly created by the constructor (m_page)
	if(graphics.set_currentPage(m_page) != OK)
	{
		return;
	}
	
	
	//	Button size
	constexpr Vec2 buttonSize = Vec2(150, 100);
	
	
	//	Center position of middle button
	const Vec2 center = Vec2(displaySize.x / 2, displaySize.y / 3);
	
	
	//	Create buttons depending on type
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
					m_page,
					0,
					nullptr
				),
				"OK",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::royal_blue,
				c_pressTime
			);
			button_ok->set_function_onCallback(callback);
			button_ok->set_frameType(Element::e_frameType::ROUNDED);
			graphics += button_ok;
			m_buttons[(uint8) e_button::OK_] = button_ok;
		}
		break;
		
		case e_type::OK_CANCEL:
		{
			//	Bottom left button position for a single button
			Vec2 position(center - buttonSize / 2);
			
			
			//	OK button
			Button* button_ok = new Button
			(
				Element
				(
					Rect(Vec2(position.x - buttonSize.x, position.y), buttonSize),
					m_page,
					0,
					nullptr
				),
				"OK",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::royal_blue,
				c_pressTime
			);
			button_ok->set_function_onCallback(callback);
			button_ok->set_frameType(Element::e_frameType::ROUNDED);
			graphics += button_ok;
			m_buttons[(uint8) e_button::OK_] = button_ok;
			
			
			//	Cancel button
			Button* button_cancel = new Button
			(
				Element
				(
					Rect(Vec2(position.x + buttonSize.x, position.y), buttonSize),
					m_page,
					0,
					nullptr
				),
				"Cancel",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::yellow,
				c_pressTime
			);
			button_cancel->set_function_onCallback(callback);
			button_cancel->set_frameType(Element::e_frameType::ROUNDED);
			graphics += button_cancel;
			m_buttons[(uint8) e_button::CANCEL] = button_cancel;
		}
		break;
		
		case e_type::RETRY_CANCEL:
		{
			//	Bottom left button position for a single button
			Vec2 position(center - buttonSize / 2);
			
			
			//	Retry button
			Button* button_retry = new Button
			(
				Element
				(
					Rect(Vec2(position.x - buttonSize.x, position.y), buttonSize),
					m_page,
					0,
					nullptr
				),
				"Retry",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::turquoise,
				c_pressTime
			);
			button_retry->set_function_onCallback(callback);
			button_retry->set_frameType(Element::e_frameType::ROUNDED);
			graphics += button_retry;
			m_buttons[(uint8) e_button::RETRY] = button_retry;
			
			
			//	Cancel button
			Button* button_cancel = new Button
			(
				Element
				(
					Rect(Vec2(position.x + buttonSize.x, position.y), buttonSize),
					m_page,
					0,
					nullptr
				),
				"Cancel",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::yellow,
				c_pressTime
			);
			button_cancel->set_function_onCallback(callback);
			button_cancel->set_frameType(Element::e_frameType::ROUNDED);
			graphics += button_cancel;
			m_buttons[(uint8) e_button::CANCEL] = button_cancel;
		}
		break;
		
		case e_type::RETRY_CANCEL_CONTINUE:
		{
			//	Bottom left button position for a single button
			Vec2 position(center - buttonSize / 2);
			
			
			//	Retry button
			Button* button_retry = new Button
			(
				Element
				(
					Rect(Vec2(position.x - 2 * buttonSize.x, position.y), buttonSize),
					m_page,
					0,
					nullptr
				),
				"Retry",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::turquoise,
				c_pressTime
			);
			button_retry->set_function_onCallback(callback);
			button_retry->set_frameType(Element::e_frameType::ROUNDED);
			graphics += button_retry;
			m_buttons[(uint8) e_button::RETRY] = button_retry;
			
			
			//	Cancel button
			Button* button_cancel = new Button
			(
				Element
				(
					Rect(position, buttonSize),
					m_page,
					0,
					nullptr
				),
				"Cancel",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::yellow,
				c_pressTime
			);
			button_cancel->set_function_onCallback(callback);
			button_cancel->set_frameType(Element::e_frameType::ROUNDED);
			graphics += button_cancel;
			m_buttons[(uint8) e_button::CANCEL] = button_cancel;
			
			
			//	Continue button
			Button* button_continue = new Button
			(
				Element
				(
					Rect(Vec2(position.x + 2 * buttonSize.x, position.y), buttonSize),
					m_page,
					0,
					nullptr
				),
				"Continue",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::yellow,
				c_pressTime
			);
			button_continue->set_function_onCallback(callback);
			button_continue->set_frameType(Element::e_frameType::ROUNDED);
			graphics += button_continue;
			m_buttons[(uint8) e_button::CONTINUE] = button_continue;
		}
		break;
		
		case e_type::YES_NO:
		{
			//	Bottom Left button position for a single button
			Vec2 position(center - buttonSize / 2);
			
			
			//	Yes button
			Button* button_yes = new Button
			(
				Element
				(
					Rect(Vec2(position.x - buttonSize.x, position.y), buttonSize),
					m_page,
					0,
					nullptr
				),
				"Yes",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::green,
				c_pressTime
			);
			button_yes->set_function_onCallback(callback);
			button_yes->set_frameType(Element::e_frameType::ROUNDED);
			graphics += button_yes;
			m_buttons[(uint8) e_button::YES] = button_yes;
			
			
			//	No button
			Button* button_no = new Button
			(
				Element
				(
					Rect(Vec2(position.x + buttonSize.x, position.y), buttonSize),
					m_page,
					0,
					nullptr
				),
				"No",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::red,
				c_pressTime
			);
			button_no->set_function_onCallback(callback);
			button_no->set_frameType(Element::e_frameType::ROUNDED);
			graphics += button_no;
			m_buttons[(uint8) e_button::NO] = button_no;
		}
		break;
		
		case e_type::YES_NO_CANCEL:
		{
			//	Bottom Left button position for a single button
			Vec2 position(center - buttonSize / 2);
			
			
			//	Yes button
			Button* button_yes = new Button
			(
				Element
				(
					Rect(Vec2(position.x - 2 * buttonSize.x, position.y), buttonSize),
					m_page,
					0,
					nullptr
				),
				"Yes",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::green,
				c_pressTime
			);
			button_yes->set_function_onCallback(callback);
			button_yes->set_frameType(Element::e_frameType::ROUNDED);
			graphics += button_yes;
			m_buttons[(uint8) e_button::YES] = button_yes;
			
			
			//	No button
			Button* button_no = new Button
			(
				Element
				(
					Rect(position, buttonSize),
					m_page,
					0,
					nullptr
				),
				"No",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::red,
				c_pressTime
			);
			button_no->set_function_onCallback(callback);
			button_no->set_frameType(Element::e_frameType::ROUNDED);
			graphics += button_no;
			m_buttons[(uint8) e_button::NO] = button_no;
			
			
			//	Cancel button
			Button* button_cancel = new Button
			(
				Element
				(
					Rect(Vec2(position.x + 2 * buttonSize.x, position.y), buttonSize),
					m_page,
					0,
					nullptr
				),
				"Cancel",
				Fonts::CalibriLight24,
				Colors::white,
				Colors::black,
				Colors::silver,
				Colors::yellow,
				c_pressTime
			);
			button_cancel->set_function_onCallback(callback);
			button_cancel->set_frameType(Element::e_frameType::ROUNDED);
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
	
	
	//	Message element
	const Vec2 textSize = Vec2(displaySize / 5);
	const Vec2 textPosition = Vec2(displaySize.x / 2, (2 * displaySize.y) / 3) - textSize / 2;
	Element* text = new Element
	(
		Rect(textPosition, textSize),
		m_page,
		0,
		update
	);
	graphics += text;
	m_text = text;
	
	
	//	Save pointer to this MessageBox in static member for update and callback functions
	m_messageBoxes += this;
	
	
	//	Wait for user to choose
	CMOS& cmos = CMOS::get();
	while(m_userChoice == e_button::NONE)
	{
		cmos.sleep_ms(20);
	}
}


MessageBox::~MessageBox()
{
	//	Free memory (Element destructor will detach itself from Graphics)
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
	
	
	//	Remove page where the MessageBox was shown
	graphics.remove_page(m_page);
	
	
	//	Go back to the page where the MessageBox was created
	graphics.set_currentPage(m_pageOnCreation);
	
	
	//	Remove pointer to this MessageBox from static member
	m_messageBoxes.erase(this);
}







MessageBox::e_button MessageBox::getUserChoice()
{
	return(m_userChoice);
}