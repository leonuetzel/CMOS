#include "../../Inc/graphics/graphics.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

CODE_RAM void Button::onUpdate(Element& element)
{
	Button& button = (Button&) element;
	
	
	//	Full Rebuild
	if(button.m_rebuildRequested == true)
	{
		if(button.m_isPressed == false)
		{
			button.draw_background(button.m_colorBackground);
		}
		else
		{
			button.draw_background(button.m_colorPressed);
		}
		button.draw_frame(button.m_colorFrame);
		button.draw_string(button.m_textActual.text, Element::e_align::CENTER, *button.m_textActual.font, button.m_textActual.color, true);
	}
	
	
	//	Normal Update
	if(button.m_updateRequested == true)
	{
		if(button.m_textActual.text != button.m_textNew.text || button.m_textActual.font != button.m_textNew.font || button.m_textActual.color != button.m_textNew.color)
		{
			button.draw_string(button.m_textActual.text, Element::e_align::CENTER, *button.m_textActual.font, button.m_colorBackground, true);
			
			button.m_textActual.text	= button.m_textNew.text;
			button.m_textActual.font	= button.m_textNew.font;
			button.m_textActual.color	= button.m_textNew.color;
			
			button.draw_string(button.m_textActual.text, Element::e_align::CENTER, *button.m_textActual.font, button.m_textActual.color, true);
		}
	}
	
	
	//	Execute User Update
	if(button.m_function_onUpdate != nullptr)
	{
		button.m_function_onUpdate(button);
	}
}


CODE_RAM void Button::onCallback(Element& element)
{
	Button& button = (Button&) element;
	
	CMOS& cmos = CMOS::get();
	
	uint32 counter_ms = 0;
	while(m_touchEvent != Graphics::e_touchEvent::RELEASE)
	{
		if(m_touchEvent == Graphics::e_touchEvent::TOUCH)
		{
			button.m_isPressed = true;
			button.m_rebuildRequested = true;
		}
		
		counter_ms++;
		
		
		//	Unlock Semaphore so that an Update can take Place
		cmos.semaphore_unlock(&element);
		cmos.sleep_ms(1);
		cmos.semaphore_lock(&element);
	}
	
	
	//	Draw Button in un-pressed Condition
	button.m_isPressed = false;
	button.m_rebuildRequested = true;
	cmos.semaphore_unlock(&element);
	cmos.sleep_ms(1);
	cmos.semaphore_lock(&element);
	
	
	//	Execute User Callback
	if(button.m_function_onCallback != nullptr && counter_ms >= button.m_pressTime_ms && button.m_touchValid == true)
	{
		button.m_function_onCallback(element);
		button.m_updateRequested = true;
	}
}


CODE_RAM void Button::onChangePage(Element& element)
{
	Button& button = (Button&) element;
	
	if(button.m_function_onChangePage != nullptr)
	{
		button.m_function_onChangePage(element);
	}
}


CODE_RAM void Button::onChangeLayer(Element& element)
{
	Button& button = (Button&) element;
	
	if(button.m_function_onChangeLayer != nullptr)
	{
		button.m_function_onChangeLayer(element);
	}
}


CODE_RAM void Button::onChangePosition(Element& element)
{
	Button& button = (Button&) element;
	
	if(button.m_function_onChangePosition != nullptr)
	{
		button.m_function_onChangePosition(element);
	}
}


CODE_RAM void Button::onChangeSize(Element& element)
{
	Button& button = (Button&) element;
	
	if(button.m_function_onChangeSize != nullptr)
	{
		button.m_function_onChangeSize(element);
	}
}


CODE_RAM void Button::onChangePageActual(Element& element)
{
	Button& button = (Button&) element;
	
	if(button.m_function_onChangePageActual != nullptr)
	{
		button.m_function_onChangePageActual(element);
	}
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

Button::~Button()
{
	
}