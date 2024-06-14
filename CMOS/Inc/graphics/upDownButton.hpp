#pragma once

#include "../cmos.hpp"
#include "button.hpp"
#include "graphicsManager.hpp"





class UpDownButton
{
	public:
		
		
		
		
		
		
		
	private:
		
		enum class e_elementID
		{
			BUTTON_INCREMENT	= 0,
			BUTTON_DECREMENT	= 1,
			ELEMENT_NAME			= 2,
			ELEMENT_VALUE			= 3
		};
		
		Element::f_element m_function_onUpdate;
		Element::f_element m_function_onCallback;
		
		static Array<Pair<UpDownButton*, Array<Element*>>> m_upDownButtons;
		static constexpr uint32 c_pressTime_ms = 25;
		
		Button m_buttonIncrement;
		Button m_buttonDecrement;
		Element m_elementName;
		Element m_elementValue;
		
		String m_variableName;
		uint8 m_value;
		uint8 m_minValue;
		uint8 m_maxValue;
		bool m_wrapAround;
		uint8 m_stepSize;
		Color m_colorText;
		
		
		static void onUpdate(Element& element);
		static void onCallback_buttonIncrement(Element& element);
		static void onCallback_buttonDecrement(Element& element);
		
		
		
		
		
	public:
		
		inline UpDownButton(Element element, const Font& font, Color colorText, Color colorBackground, Color colorFrame, Color colorPressed, String variableName = "", uint8 initValue = 0, uint8 minValue = 0, uint8 maxValue = 255, bool wrapAround = false, uint8 stepSize = 1);
		virtual ~UpDownButton();
		
		inline void set_variableName(const String& variableName);
		constexpr inline feedback set_minValue(uint8 minValue);
		constexpr inline feedback set_maxValue(uint8 maxValue);
		constexpr inline void wrapAround(bool allowWrapAround);
		constexpr inline void set_stepSize(uint8 stepSize);
		
		inline const Array<Element*> operator()();
		
		constexpr inline uint8 get_value() const;
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

inline UpDownButton::UpDownButton(Element element, const Font& font, Color colorText, Color colorBackground, Color colorFrame, Color colorPressed, String variableName, uint8 initValue, uint8 minValue, uint8 maxValue, bool wrapAround, uint8 stepSize)
	:	m_function_onUpdate(element.get_function_onUpdate()),
		m_function_onCallback(element.get_function_onCallback()),
		
		m_buttonIncrement
		(
			Element
			(
				Rect
				(
					element.get_position().x,
					element.get_position().y + 2 * element.get_size().x,
					element.get_size().x,
					element.get_size().x
				),
				element.get_page(),
				element.get_layer(),
				onUpdate,
				onCallback_buttonIncrement,
				element.get_updateRate_ms(),
				Element::e_frameType::ROUND
			),
			"+",
			font,
			Colors::green,
			colorBackground,
			colorFrame,
			colorPressed,
			c_pressTime_ms
		),
		m_buttonDecrement
		(
			Element
			(
				Rect
				(
					element.get_position().x,
					element.get_position().y,
					element.get_size().x,
					element.get_size().x
				),
				element.get_page(),
				element.get_layer(),
				onUpdate,
				onCallback_buttonDecrement,
				element.get_updateRate_ms(),
				Element::e_frameType::ROUND
			),
			"-",
			font,
			Colors::red,
			colorBackground,
			colorFrame,
			colorPressed,
			c_pressTime_ms
		),
		m_elementName
		(
			Rect
			(
				element.get_position().x,
				element.get_position().y + 3 * element.get_size().x,
				element.get_size().x,
				element.get_size().x
			),
			element.get_page(),
			element.get_layer(),
			onUpdate,
			nullptr,
			0,
			Element::e_frameType::ROUND
		),
		m_elementValue
		(
			Rect
			(
				element.get_position().x,
				element.get_position().y + element.get_size().x,
				element.get_size().x,
				element.get_size().x
			),
			element.get_page(),
			element.get_layer(),
			onUpdate,
			nullptr,
			0,
			Element::e_frameType::ROUND
		),
		
		m_variableName(variableName),
		m_value(initValue),
		m_minValue(minValue),
		m_maxValue(maxValue),
		m_wrapAround(wrapAround),
		m_stepSize(stepSize),
		m_colorText(colorText)
{
	Pair<UpDownButton*, Array<Element*>> upDownButton;
	upDownButton.first() = this;
	upDownButton.second() += &m_buttonIncrement;
	upDownButton.second() += &m_buttonDecrement;
	upDownButton.second() += &m_elementName;
	upDownButton.second() += &m_elementValue;
	
	m_upDownButtons += upDownButton;
}







inline void UpDownButton::set_variableName(const String& variableName)
{
	m_variableName = variableName;
	m_elementName.requestRebuild();
}


constexpr inline feedback UpDownButton::set_minValue(uint8 minValue)
{
	//	Minimum Value cant be bigger than maximum Value
	if(minValue > m_maxValue)
	{
		return(FAIL);
	}
	
	m_minValue = minValue;
	m_elementValue.requestRebuild();
	return(OK);
}


constexpr inline feedback UpDownButton::set_maxValue(uint8 maxValue)
{
	//	Maximum Value cant be smaller than minimum Value
	if(maxValue < m_minValue)
	{
		return(FAIL);
	}
	
	m_maxValue = maxValue;
	m_elementValue.requestRebuild();
	return(OK);
}


constexpr inline void UpDownButton::wrapAround(bool allowWrapAround)
{
	m_wrapAround = allowWrapAround;
}


constexpr inline void UpDownButton::set_stepSize(uint8 stepSize)
{
	m_stepSize = stepSize;
}







inline const Array<Element*> UpDownButton::operator()()
{
	Array<Element*> array;
	array += &m_buttonIncrement;
	array += &m_buttonDecrement;
	array += &m_elementName;
	array += &m_elementValue;
	
	return(array);
}







constexpr inline uint8 UpDownButton::get_value() const
{
	return(m_value);
}