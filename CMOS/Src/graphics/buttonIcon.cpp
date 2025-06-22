#include "../../Inc/graphics/graphics.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

CODE_RAM void ButtonIcon::onUpdate(Element& element)
{
	ButtonIcon& buttonIcon = (ButtonIcon&) element;
	
	
	//	Full Rebuild
	if(buttonIcon.m_rebuildRequested == true)
	{
		const Vec2 iconPosition((buttonIcon.get_size() - buttonIcon.m_icon.get_size()) / 2);
		if(buttonIcon.m_isPressed == false)
		{
			buttonIcon.draw_icon(buttonIcon.m_icon, iconPosition);
			buttonIcon.draw_frame(buttonIcon.m_colorFrame);
		}
		else
		{
			//	Invert Icon Colors
			const Color* iconData = buttonIcon.m_icon.get_data();
			const Vec2 iconSize = buttonIcon.m_icon.get_size();
			const uint32 numberOfPixel = iconSize.x * iconSize.y;
			Color* iconDataInverted = new Color[numberOfPixel];
			for(uint32 i = 0; i < numberOfPixel; i++)
			{
				iconDataInverted[i] = iconData[i].invert();
			}
			Icon iconInverted(iconSize, iconDataInverted);
			
			buttonIcon.draw_icon(iconInverted, iconPosition);
			buttonIcon.draw_frame(buttonIcon.m_colorFrame);
			
			delete[] iconDataInverted;
		}
	}
	
	
	//	Normal Update
	if(buttonIcon.m_updateRequested == true)
	{
		
	}
	
	
	//	Execute User Update
	if(buttonIcon.m_function_onUpdate != nullptr)
	{
		buttonIcon.m_function_onUpdate(element);
	}
}


CODE_RAM void ButtonIcon::onCallback(Element& element)
{
	ButtonIcon& buttonIcon = (ButtonIcon&) element;
	
	CMOS& cmos = CMOS::get();
	
	uint32 counter_ms = 0;
	while(m_touchEvent != Graphics::e_touchEvent::RELEASE)
	{
		if(m_touchEvent == Graphics::e_touchEvent::TOUCH)
		{
			buttonIcon.m_isPressed = true;
			buttonIcon.m_rebuildRequested = true;
		}
		
		counter_ms++;
		
		
		//	Unlock Semaphore so that an Update can take Place
		cmos.semaphore_unlock(&element);
		cmos.sleep_ms(1);
		cmos.semaphore_lock(&element);
	}
	
	
	//	Draw Button in un-pressed Condition
	buttonIcon.m_isPressed = false;
	buttonIcon.m_rebuildRequested = true;
	cmos.semaphore_unlock(&element);
	cmos.sleep_ms(1);
	cmos.semaphore_lock(&element);
	
	
	//	Execute User Callback
	if(buttonIcon.m_function_onCallback != nullptr && counter_ms >= buttonIcon.m_pressTime_ms && buttonIcon.m_touchValid == true)
	{
		buttonIcon.m_function_onCallback(element);
		buttonIcon.m_updateRequested = true;
	}
}


CODE_RAM void ButtonIcon::onChangePage(Element& element)
{
	ButtonIcon& buttonIcon = (ButtonIcon&) element;
	
	if(buttonIcon.m_function_onChangePage != nullptr)
	{
		buttonIcon.m_function_onChangePage(element);
	}
}


CODE_RAM void ButtonIcon::onChangeLayer(Element& element)
{
	ButtonIcon& buttonIcon = (ButtonIcon&) element;
	
	if(buttonIcon.m_function_onChangeLayer != nullptr)
	{
		buttonIcon.m_function_onChangeLayer(element);
	}
}


CODE_RAM void ButtonIcon::onChangePosition(Element& element)
{
	ButtonIcon& buttonIcon = (ButtonIcon&) element;
	
	if(buttonIcon.m_function_onChangePosition != nullptr)
	{
		buttonIcon.m_function_onChangePosition(element);
	}
}


CODE_RAM void ButtonIcon::onChangeSize(Element& element)
{
	ButtonIcon& buttonIcon = (ButtonIcon&) element;
	
	if(buttonIcon.m_function_onChangeSize != nullptr)
	{
		buttonIcon.m_function_onChangeSize(element);
	}
}


CODE_RAM void ButtonIcon::onChangePageActual(Element& element)
{
	ButtonIcon& buttonIcon = (ButtonIcon&) element;
	
	if(buttonIcon.m_function_onChangePageActual != nullptr)
	{
		buttonIcon.m_function_onChangePageActual(element);
	}
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

ButtonIcon::~ButtonIcon()
{
	
}