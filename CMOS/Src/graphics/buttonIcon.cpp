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
	
	
	//	Full rebuild
	if(buttonIcon.isRebuildRequested() == true)
	{
		const Vec2 iconPosition((buttonIcon.get_size() - buttonIcon.m_icon.get_size()) / 2);
		if(buttonIcon.m_isPressed == false)
		{
			buttonIcon.draw_icon(buttonIcon.m_icon, iconPosition);
			buttonIcon.draw_frame(buttonIcon.m_colorFrame);
		}
		else
		{
			//	Invert icon colors only once
			const Vec2 iconSize = buttonIcon.m_icon.get_size();
			if(buttonIcon.m_iconDataInverted == nullptr)
			{
				const Color* iconData = buttonIcon.m_icon.get_data();
				
				const uint32 numberOfPixel = iconSize.x * iconSize.y;
				Color* iconDataInverted = new Color[numberOfPixel];
				
				for(uint32 i = 0; i < numberOfPixel; i++)
				{
					iconDataInverted[i] = iconData[i].invert();
				}
				
				buttonIcon.m_iconDataInverted = iconDataInverted;
			}
			
			
			//	Draw inverted icon
			const Icon iconInverted(iconSize, buttonIcon.m_iconDataInverted);
			buttonIcon.draw_icon(iconInverted, iconPosition);
			
			
			//	Draw frame
			buttonIcon.draw_frame(buttonIcon.m_colorFrame);
		}
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
	while(m_touchEvent != I_TouchController::e_touchEvent::RELEASE)
	{
		if(m_touchEvent == I_TouchController::e_touchEvent::TOUCH)
		{
			buttonIcon.m_isPressed = true;
			buttonIcon.requestRebuild();
			buttonIcon.requestUpdate();
		}
		
		counter_ms++;
		
		
		//	Unlock semaphore so that an update can take place
		cmos.semaphore_unlock(&element);
		cmos.sleep_ms(1);
		cmos.semaphore_lock(&element);
	}
	
	
	//	Draw button in un-pressed condition
	buttonIcon.m_isPressed = false;
	buttonIcon.requestRebuild();
	buttonIcon.requestUpdate();
	
	cmos.semaphore_unlock(&element);
	while(buttonIcon.isRebuildRequested() == true)
	{
		cmos.sleep_ms(1);
	}
	cmos.semaphore_lock(&element);
	
	
	//	Execute user callback
	if(buttonIcon.m_function_onCallback != nullptr && counter_ms >= buttonIcon.m_pressTime_ms && buttonIcon.m_touchValid == true)
	{
		buttonIcon.m_function_onCallback(element);
		buttonIcon.requestUpdate();
	}
}


CODE_RAM void ButtonIcon::onChangeMyPage(Element& element)
{
	ButtonIcon& buttonIcon = (ButtonIcon&) element;
	
	if(buttonIcon.m_function_onChangeMyPage != nullptr)
	{
		buttonIcon.m_function_onChangeMyPage(element);
	}
}


CODE_RAM void ButtonIcon::onChangeShape(Element& element)
{
	ButtonIcon& buttonIcon = (ButtonIcon&) element;
	
	if(buttonIcon.m_function_onChangeShape != nullptr)
	{
		buttonIcon.m_function_onChangeShape(element);
	}
}


CODE_RAM void ButtonIcon::onChangeCurrentPage(Element& element)
{
	ButtonIcon& buttonIcon = (ButtonIcon&) element;
	
	if(buttonIcon.m_function_onChangeCurrentPage != nullptr)
	{
		buttonIcon.m_function_onChangeCurrentPage(element);
	}
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

ButtonIcon::~ButtonIcon()
{
	if(m_iconDataInverted != nullptr)
	{
		delete[] m_iconDataInverted;
		m_iconDataInverted = nullptr;
	}
}