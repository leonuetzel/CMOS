#pragma once

#include "../cmos.hpp"
#include "element.hpp"





class Button: public Element
{
	public:
		
		
		
		
		
		
		
	private:
		
		struct
		{
			String text;
			const Font* font;
			Color color;
		}m_textActual, m_textNew;
		
		Color m_colorBackground;
		Color m_colorFrame;
		Color m_colorPressed;
		uint32 m_pressTime_ms;
		bool m_isPressed;
		
		f_element m_function_onUpdate;
		f_element m_function_onCallback;
		f_element m_function_onChangePage;
		f_element m_function_onChangeLayer;
		f_element m_function_onChangePosition;
		f_element m_function_onChangeSize;
		f_element m_function_onChangePageActual;
		
		static void onUpdate(Element& element);
		static void onCallback(Element& element);
		static void onChangePage(Element& element);
		static void onChangeLayer(Element& element);
		static void onChangePosition(Element& element);
		static void onChangeSize(Element& element);
		static void onChangePageActual(Element& element);
		
		
		
		
		
	protected:
	
		const Font* m_font;
		Color m_colorText;
		
		
		
		
		
	public:
		
		inline Button(Element element, String text, const Font& font, Color colorText, Color colorBackground, Color colorFrame, Color colorPressed, uint32 pressTime_ms);
		virtual ~Button();
		
		constexpr inline void				set_function_onUpdate(f_element onUpdateFunction);
		constexpr inline void				set_function_onCallback(f_element onCallbackFunction);
		constexpr inline void				set_function_onChangePage(f_element onChangePageFunction);
		constexpr inline void				set_function_onChangeLayer(f_element onChangeLayerFunction);
		constexpr inline void				set_function_onChangePosition(f_element onChangePositionFunction);
		constexpr inline void				set_function_onChangeSize(f_element onChangeSizeFunction);
		constexpr inline void				set_function_onChangePageActual(f_element onChangePageActualFunction);
		constexpr inline f_element	get_function_onUpdate() const;
		constexpr inline f_element	get_function_onCallback() const;
		constexpr inline f_element	get_function_onChangePage() const;
		constexpr inline f_element	get_function_onChangeLayer() const;
		constexpr inline f_element	get_function_onChangePosition() const;
		constexpr inline f_element	get_function_onChangeSize() const;
		constexpr inline f_element	get_function_onChangePageActual() const;
		
		inline String get_text() const;
		constexpr inline const Font& get_font() const;
		constexpr inline Color get_colorText() const;
		constexpr inline Color get_colorBackground() const;
		constexpr inline Color get_colorFrame() const;
		constexpr inline Color get_colorPressed() const;
		constexpr inline uint32 get_pressTime_ms() const;
		
		inline void set_text(String text);
		constexpr inline void set_font(const Font& font);
		constexpr inline void set_colorText(Color color);
		constexpr inline void set_colorBackground(Color color);
		constexpr inline void set_colorFrame(Color color);
		constexpr inline void set_colorPressed(Color color);
		constexpr inline void set_pressTime_ms(uint32 pressTime_ms);
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

inline Button::Button(Element element, String text, const Font& font, Color colorText, Color colorBackground, Color colorFrame, Color colorPressed, uint32 pressTime_ms)
	:	Element(element),
		m_textActual
		{
			.text = text,
			.font = &font,
			.color = colorText
		},
		m_textNew(m_textActual),
		m_colorBackground(colorBackground),
		m_colorFrame(colorFrame),
		m_colorPressed(colorPressed),
		m_pressTime_ms(pressTime_ms),
		m_isPressed(false),
		
		m_function_onUpdate(Element::get_function_onUpdate()),
		m_function_onCallback(Element::get_function_onCallback()),
		m_function_onChangePage(Element::get_function_onChangePage()),
		m_function_onChangeLayer(Element::get_function_onChangeLayer()),
		m_function_onChangePosition(Element::get_function_onChangePosition()),
		m_function_onChangeSize(Element::get_function_onChangeSize()),
		m_function_onChangePageActual(Element::get_function_onChangePageActual()),
		
		m_font(&font),
		m_colorText(colorText)
{
	Element::set_function_onUpdate(onUpdate);
	Element::set_function_onCallback(onCallback);
	Element::set_function_onChangePage(onChangePage);
	Element::set_function_onChangeLayer(onChangeLayer);
	Element::set_function_onChangePosition(onChangePosition);
	Element::set_function_onChangeSize(onChangeSize);
	Element::set_function_onChangePageActual(onChangePageActual);
}







constexpr inline void Button::set_function_onUpdate(f_element onUpdateFunction)
{
	m_function_onUpdate = onUpdateFunction;
}


constexpr inline void Button::set_function_onCallback(f_element onCallbackFunction)
{
	m_function_onCallback = onCallbackFunction;
}


constexpr inline void Button::set_function_onChangePage(f_element onChangePageFunction)
{
	m_function_onChangePage = onChangePageFunction;
}


constexpr inline void Button::set_function_onChangeLayer(f_element onChangeLayerFunction)
{
	m_function_onChangeLayer = onChangeLayerFunction;
}


constexpr inline void Button::set_function_onChangePosition(f_element onChangePositionFunction)
{
	m_function_onChangePosition = onChangePositionFunction;
}


constexpr inline void Button::set_function_onChangeSize(f_element onChangeSizeFunction)
{
	m_function_onChangeSize = onChangeSizeFunction;
}


constexpr inline void Button::set_function_onChangePageActual(f_element onChangePageActualFunction)
{
	m_function_onChangePageActual = onChangePageActualFunction;
}


constexpr inline Element::f_element Button::get_function_onUpdate() const
{
	return(m_function_onUpdate);
}


constexpr inline Element::f_element Button::get_function_onCallback() const
{
	return(m_function_onCallback);
}


constexpr inline Element::f_element Button::get_function_onChangePage() const
{
	return(m_function_onChangePage);
}


constexpr inline Element::f_element Button::get_function_onChangeLayer() const
{
	return(m_function_onChangeLayer);
}


constexpr inline Element::f_element Button::get_function_onChangePosition() const
{
	return(m_function_onChangePosition);
}


constexpr inline Element::f_element Button::get_function_onChangeSize() const
{
	return(m_function_onChangeSize);
}


constexpr inline Element::f_element Button::get_function_onChangePageActual() const
{
	return(m_function_onChangePageActual);
}







inline String Button::get_text() const
{
	return(m_textActual.text);
}


constexpr inline const Font& Button::get_font() const
{
	return(*m_textActual.font);
}


constexpr inline Color Button::get_colorText() const
{
	return(m_textActual.color);
}


constexpr inline Color Button::get_colorBackground() const
{
	return(m_colorBackground);
}


constexpr inline Color Button::get_colorFrame() const
{
	return(m_colorFrame);
}


constexpr inline Color Button::get_colorPressed() const
{
	return(m_colorPressed);
}


constexpr inline uint32 Button::get_pressTime_ms() const
{
	return(m_pressTime_ms);
}








inline void Button::set_text(String text)
{
	if(text != m_textNew.text)
	{
		m_textNew.text = text;
		m_updateRequested = true;
	}
}


constexpr inline void Button::set_font(const Font& font)
{
	m_font = &font;
	if(m_textNew.font != &font)
	{
		m_textNew.font = &font;
		m_updateRequested = true;
	}
}


constexpr inline void Button::set_colorText(Color color)
{
	m_colorText = color;
	if(m_textNew.color != color)
	{
		m_textNew.color = color;
		m_updateRequested = true;
	}
}


constexpr inline void Button::set_colorBackground(Color color)
{
	if(m_colorBackground != color)
	{
		m_colorBackground = color;
		m_rebuildRequested = true;
	}
}


constexpr inline void Button::set_colorFrame(Color color)
{
	if(m_colorFrame != color)
	{
		m_colorFrame = color;
		m_rebuildRequested = true;
	}
}


constexpr inline void Button::set_colorPressed(Color color)
{
	m_colorPressed = color;
}


constexpr inline void Button::set_pressTime_ms(uint32 pressTime_ms)
{
	m_pressTime_ms = pressTime_ms;
}