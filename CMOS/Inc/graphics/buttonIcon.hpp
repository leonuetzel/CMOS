#pragma once

#include "../cmos.hpp"
#include "element.hpp"





class ButtonIcon: public Element
{
	public:
		
		
		
		
		
		
		
	private:
		
		const Icon& m_icon;
		Color* m_iconDataInverted;
		Color m_colorFrame;
		uint32 m_pressTime_ms;
		bool m_isPressed;
		
		f_element m_function_onUpdate;
		f_element m_function_onCallback;
		f_element m_function_onChangePage;
		f_element m_function_onChangeShape;
		f_element m_function_onChangePageActual;
		
		static void onUpdate(Element& element);
		static void onCallback(Element& element);
		static void onChangePage(Element& element);
		static void onChangeShape(Element& element);
		static void onChangePageActual(Element& element);
		
		
		
		
		
	public:
		
		inline ButtonIcon(Element element, const Icon& icon, Color colorFrame, uint32 pressTime_ms);
		virtual ~ButtonIcon();
		
		constexpr inline void				set_function_onUpdate(f_element onUpdateFunction);
		constexpr inline void				set_function_onCallback(f_element onCallbackFunction);
		constexpr inline void				set_function_onChangePage(f_element onChangePageFunction);
		constexpr inline void				set_function_onChangeShape(f_element onChangeShapeFunction);
		constexpr inline void				set_function_onChangePageActual(f_element onChangePageActualFunction);
		constexpr inline f_element	get_function_onUpdate() const;
		constexpr inline f_element	get_function_onCallback() const;
		constexpr inline f_element	get_function_onChangePage() const;
		constexpr inline f_element	get_function_onChangeShape() const;
		constexpr inline f_element	get_function_onChangePageActual() const;
		
		constexpr inline uint32 get_pressTime_ms() const;
		
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

inline ButtonIcon::ButtonIcon(Element element, const Icon& icon, Color colorFrame, uint32 pressTime_ms)
	:	Element(element),
		m_icon(icon),
		m_iconDataInverted(nullptr),
		m_colorFrame(colorFrame),
		m_pressTime_ms(pressTime_ms),
		m_isPressed(false),
		
		m_function_onUpdate(Element::get_function_onUpdate()),
		m_function_onCallback(Element::get_function_onCallback()),
		m_function_onChangePage(Element::get_function_onChangePage()),
		m_function_onChangeShape(Element::get_function_onChangeShape()),
		m_function_onChangePageActual(Element::get_function_onChangePageActual())
{
	Element::set_function_onUpdate(onUpdate);
	Element::set_function_onCallback(onCallback);
	Element::set_function_onChangePage(onChangePage);
	Element::set_function_onChangeShape(onChangeShape);
	Element::set_function_onChangePageActual(onChangePageActual);
}







constexpr inline void ButtonIcon::set_function_onUpdate(f_element onUpdateFunction)
{
	m_function_onUpdate = onUpdateFunction;
}


constexpr inline void ButtonIcon::set_function_onCallback(f_element onCallbackFunction)
{
	m_function_onCallback = onCallbackFunction;
}


constexpr inline void ButtonIcon::set_function_onChangePage(f_element onChangePageFunction)
{
	m_function_onChangePage = onChangePageFunction;
}


constexpr inline void ButtonIcon::set_function_onChangeShape(f_element onChangeShapeFunction)
{
	m_function_onChangeShape = onChangeShapeFunction;
}


constexpr inline void ButtonIcon::set_function_onChangePageActual(f_element onChangePageActualFunction)
{
	m_function_onChangePageActual = onChangePageActualFunction;
}


constexpr inline Element::f_element ButtonIcon::get_function_onUpdate() const
{
	return(m_function_onUpdate);
}


constexpr inline Element::f_element ButtonIcon::get_function_onCallback() const
{
	return(m_function_onCallback);
}


constexpr inline Element::f_element ButtonIcon::get_function_onChangePage() const
{
	return(m_function_onChangePage);
}


constexpr inline Element::f_element ButtonIcon::get_function_onChangeShape() const
{
	return(m_function_onChangeShape);
}


constexpr inline Element::f_element ButtonIcon::get_function_onChangePageActual() const
{
	return(m_function_onChangePageActual);
}







constexpr inline uint32 ButtonIcon::get_pressTime_ms() const
{
	return(m_pressTime_ms);
}







constexpr inline void ButtonIcon::set_pressTime_ms(uint32 pressTime_ms)
{
	m_pressTime_ms = pressTime_ms;
}