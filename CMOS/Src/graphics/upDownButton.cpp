#include "../../Inc/graphics/graphics.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/

Array<Pair<UpDownButton*, Array<Element*>>> UpDownButton::m_upDownButtons;



/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

CODE_RAM void UpDownButton::onUpdate(Element& element)
{
	//	Get this Pointer
	UpDownButton* thisPointer = nullptr;
	e_elementID elementID = e_elementID::BUTTON_INCREMENT;
	for(auto& i: m_upDownButtons)
	{
		const Array<Element*> elements = i.second();
		for(uint32 j = 0; j < 4; j++)
		{
			if(elements[j] == &element)
			{
				thisPointer = i.first();
				elementID = (e_elementID) j;
				break;
			}
		}
		if(thisPointer != nullptr)
		{
			break;
		}
	}
	if(thisPointer == nullptr)
	{
		return;
	}
	UpDownButton& upDownButton = *thisPointer;
	
	
	//	Full Rebuild
	if(element.isRebuildRequested() == true)
	{
		
	}
	
	
	//	Normal Update
	if(element.isUpdateRequested() == true)
	{
		//	Limit Value to given Boundaries
		upDownButton.m_value = Math::min(upDownButton.m_value, upDownButton.m_maxValue);
		upDownButton.m_value = Math::max(upDownButton.m_value, upDownButton.m_minValue);
		
		
		//	Rebuild only if its a pure Element, Buttons handle this themselves
		if(elementID == e_elementID::ELEMENT_NAME || elementID == e_elementID::ELEMENT_VALUE)
		{
			String text;
			if(elementID == e_elementID::ELEMENT_NAME)
			{
				text = upDownButton.m_variableName;
			}
			else
			{
				text = String(upDownButton.m_value);
			}
			
			const Button& button(upDownButton.m_buttonIncrement);
			element.draw_background(button.get_colorBackground());
			element.draw_frame(button.get_colorFrame());
			element.draw_string(text, Element::e_align::CENTER, button.get_font(), upDownButton.m_colorText);
		}
	}
	
	
	//	Execute User Update
	if(upDownButton.m_function_onUpdate != nullptr)
	{
		upDownButton.m_function_onUpdate(element);
	}
}


CODE_RAM void UpDownButton::onCallback_buttonIncrement(Element& element)
{
	Button& button = (Button&) element;
	
	
	//	Get this Pointer
	UpDownButton* thisPointer = nullptr;
	for(auto& i: m_upDownButtons)
	{
		const Array<Element*> elements = i.second();
		if(elements[(uint32) e_elementID::BUTTON_INCREMENT] == &button)
		{
			thisPointer = i.first();
			break;
		}
	}
	if(thisPointer == nullptr)
	{
		return;
	}
	UpDownButton& upDownButton = *thisPointer;
	
	
	//	Increment
	if(upDownButton.m_wrapAround == false)
	{
		if(upDownButton.m_value < upDownButton.m_maxValue)
		{
			if(upDownButton.m_maxValue - upDownButton.m_value > upDownButton.m_stepSize)
			{
				upDownButton.m_value += upDownButton.m_stepSize;
				upDownButton.m_elementValue.requestUpdate();
			}
			else
			{
				upDownButton.m_value = upDownButton.m_maxValue;
				upDownButton.m_elementValue.requestUpdate();
			}
		}
	}
	else
	{
		uint16 newValueWithOverflow = ((uint16) upDownButton.m_value) + upDownButton.m_stepSize;
		if(newValueWithOverflow > upDownButton.m_maxValue)
		{
			newValueWithOverflow -= upDownButton.m_maxValue + 1;
			newValueWithOverflow += upDownButton.m_minValue;
		}
		upDownButton.m_value = newValueWithOverflow;
		upDownButton.m_elementValue.requestUpdate();
	}
	
	
	//	User Callback
	if(upDownButton.m_function_onCallback != nullptr)
	{
		upDownButton.m_function_onCallback(element);
		button.requestUpdate();
	}
}


CODE_RAM void UpDownButton::onCallback_buttonDecrement(Element& element)
{
	Button& button = (Button&) element;
	
	
	//	Get this Pointer
	UpDownButton* thisPointer = nullptr;
	for(auto& i: m_upDownButtons)
	{
		const Array<Element*> elements = i.second();
		if(elements[(uint32) e_elementID::BUTTON_DECREMENT] == &button)
		{
			thisPointer = i.first();
			break;
		}
	}
	if(thisPointer == nullptr)
	{
		return;
	}
	UpDownButton& upDownButton = *thisPointer;
	
	
	//	Decrement
	if(upDownButton.m_wrapAround == false)
	{
		if(upDownButton.m_value > upDownButton.m_minValue)
		{
			if(upDownButton.m_value - upDownButton.m_minValue > upDownButton.m_stepSize)
			{
				upDownButton.m_value -= upDownButton.m_stepSize;
				upDownButton.m_elementValue.requestUpdate();
			}
			else
			{
				upDownButton.m_value = upDownButton.m_minValue;
				upDownButton.m_elementValue.requestUpdate();
			}
		}
	}
	else
	{
		int16 newValueWithOverflow = ((int16) upDownButton.m_value) - upDownButton.m_stepSize;
		if(newValueWithOverflow < upDownButton.m_minValue)
		{
			newValueWithOverflow = upDownButton.m_minValue - newValueWithOverflow;
			newValueWithOverflow = upDownButton.m_maxValue - newValueWithOverflow + 1;
		}
		upDownButton.m_value = newValueWithOverflow;
		upDownButton.m_elementValue.requestUpdate();
	}
	
	
	//	User Callback
	if(upDownButton.m_function_onCallback != nullptr)
	{
		upDownButton.m_function_onCallback(element);
		button.requestUpdate();
	}
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

UpDownButton::~UpDownButton()
{
	for(uint32 i = 0; i < m_upDownButtons.get_size(); i++)
	{
		if(m_upDownButtons[i].first() == this)
		{
			m_upDownButtons.erase(i);
			return;
		}
	}
}