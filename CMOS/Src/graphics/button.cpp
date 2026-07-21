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
	
	
	//	Full rebuild
	if(button.isRebuildRequested() == true)
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
	
	
	if(button.m_textActual.text != button.m_textNew.text || button.m_textActual.font != button.m_textNew.font || button.m_textActual.color != button.m_textNew.color)
	{
		button.draw_string(button.m_textActual.text, Element::e_align::CENTER, *button.m_textActual.font, button.m_colorBackground, true);
		
		button.m_textActual.text	= button.m_textNew.text;
		button.m_textActual.font	= button.m_textNew.font;
		button.m_textActual.color	= button.m_textNew.color;
		
		button.draw_string(button.m_textActual.text, Element::e_align::CENTER, *button.m_textActual.font, button.m_textActual.color, true);
	}
	
	
	//	Execute user update
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
	while(m_touchEvent != I_TouchController::e_touchEvent::RELEASE)
	{
		if(m_touchEvent == I_TouchController::e_touchEvent::TOUCH)
		{
			button.m_isPressed = true;
			button.requestRebuild();
			button.requestUpdate();
		}
		
		counter_ms++;
		
		
		//	Unlock semaphore so that an update can take place
		cmos.semaphore_unlock(&element);
		cmos.sleep_ms(1);
		cmos.semaphore_lock(&element);
	}
	
	
	//	Draw button in un-pressed condition
	button.m_isPressed = false;
	button.requestRebuild();
	button.requestUpdate();
	
	cmos.semaphore_unlock(&element);
	while(button.isRebuildRequested() == true)
	{
		cmos.sleep_ms(1);
	}
	cmos.semaphore_lock(&element);
	
	
	//	Execute user callback
	if(button.m_function_onCallback != nullptr && counter_ms >= button.m_pressTime_ms && button.m_touchValid == true)
	{
		button.m_function_onCallback(element);
		button.requestUpdate();
	}
}


CODE_RAM void Button::onChangeMyPage(Element& element)
{
	Button& button = (Button&) element;
	
	if(button.m_function_onChangeMyPage != nullptr)
	{
		button.m_function_onChangeMyPage(element);
	}
}


CODE_RAM void Button::onChangeShape(Element& element)
{
	Button& button = (Button&) element;
	
	if(button.m_function_onChangeShape != nullptr)
	{
		button.m_function_onChangeShape(element);
	}
}


CODE_RAM void Button::onChangeCurrentPage(Element& element)
{
	Button& button = (Button&) element;
	
	if(button.m_function_onChangeCurrentPage != nullptr)
	{
		button.m_function_onChangeCurrentPage(element);
	}
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

Button::~Button()
{
	
}