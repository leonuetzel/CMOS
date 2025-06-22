#pragma once

#include "../cmos.hpp"
#include "element.hpp"





class ButtonIcon: public Element
{
	public:
		
		
		
		
		
		
		
	private:
		
		const Icon& m_icon;
		Color m_colorFrame;
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
		
		
		
		
		
	public:
		
		inline ButtonIcon(Element element, const Icon& icon, Color colorFrame, uint32 pressTime_ms);
		virtual ~ButtonIcon();
		
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
		m_colorFrame(colorFrame),
		m_pressTime_ms(pressTime_ms),
		m_isPressed(false),
		
		m_function_onUpdate(Element::get_function_onUpdate()),
		m_function_onCallback(Element::get_function_onCallback()),
		m_function_onChangePage(Element::get_function_onChangePage()),
		m_function_onChangeLayer(Element::get_function_onChangeLayer()),
		m_function_onChangePosition(Element::get_function_onChangePosition()),
		m_function_onChangeSize(Element::get_function_onChangeSize()),
		m_function_onChangePageActual(Element::get_function_onChangePageActual())
{
	Element::set_function_onUpdate(onUpdate);
	Element::set_function_onCallback(onCallback);
	Element::set_function_onChangePage(onChangePage);
	Element::set_function_onChangeLayer(onChangeLayer);
	Element::set_function_onChangePosition(onChangePosition);
	Element::set_function_onChangeSize(onChangeSize);
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


constexpr inline void ButtonIcon::set_function_onChangeLayer(f_element onChangeLayerFunction)
{
	m_function_onChangeLayer = onChangeLayerFunction;
}


constexpr inline void ButtonIcon::set_function_onChangePosition(f_element onChangePositionFunction)
{
	m_function_onChangePosition = onChangePositionFunction;
}


constexpr inline void ButtonIcon::set_function_onChangeSize(f_element onChangeSizeFunction)
{
	m_function_onChangeSize = onChangeSizeFunction;
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


constexpr inline Element::f_element ButtonIcon::get_function_onChangeLayer() const
{
	return(m_function_onChangeLayer);
}


constexpr inline Element::f_element ButtonIcon::get_function_onChangePosition() const
{
	return(m_function_onChangePosition);
}


constexpr inline Element::f_element ButtonIcon::get_function_onChangeSize() const
{
	return(m_function_onChangeSize);
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