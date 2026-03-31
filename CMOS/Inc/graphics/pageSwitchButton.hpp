#pragma once

#include "../cmos.hpp"
#include "button.hpp"
#include "graphicsManager.hpp"





class PageSwitchButton: public Button
{
	public:
		
		
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		f_element m_function_onUpdate;
		f_element m_function_onCallback;
		f_element m_function_onChangePage;
		f_element m_function_onChangeShape;
		f_element m_function_onChangePageActual;
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		void writeText();
		
		static void onUpdate(Element& element);
		static void onCallback(Element& element);
		static void onChangePage(Element& element);
		static void onChangeShape(Element& element);
		static void onChangePageActual(Element& element);
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		inline PageSwitchButton(Button button);
		virtual ~PageSwitchButton();
		
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

inline PageSwitchButton::PageSwitchButton(Button button)
	:	Button(button),
		m_function_onUpdate(Button::get_function_onUpdate()),
		m_function_onCallback(Button::get_function_onCallback()),
		m_function_onChangePage(Button::get_function_onChangePage()),
		m_function_onChangeShape(Button::get_function_onChangeShape()),
		m_function_onChangePageActual(Button::get_function_onChangePageActual())
{
	Button::set_function_onUpdate(onUpdate);
	Button::set_function_onCallback(onCallback);
	Button::set_function_onChangePage(onChangePage);
	Button::set_function_onChangeShape(onChangeShape);
	Button::set_function_onChangePageActual(onChangePageActual);
	
	writeText();
}







constexpr inline void PageSwitchButton::set_function_onUpdate(f_element onUpdateFunction)
{
	m_function_onUpdate = onUpdateFunction;
}


constexpr inline void PageSwitchButton::set_function_onCallback(f_element onCallbackFunction)
{
	m_function_onCallback = onCallbackFunction;
}


constexpr inline void PageSwitchButton::set_function_onChangePage(f_element onChangePageFunction)
{
	m_function_onChangePage = onChangePageFunction;
}


constexpr inline void PageSwitchButton::set_function_onChangeShape(f_element onChangeShapeFunction)
{
	m_function_onChangeShape = onChangeShapeFunction;
}


constexpr inline void PageSwitchButton::set_function_onChangePageActual(f_element onChangePageActualFunction)
{
	m_function_onChangePageActual = onChangePageActual;
}


constexpr inline Element::f_element PageSwitchButton::get_function_onUpdate() const
{
	return(m_function_onUpdate);
}


constexpr inline Element::f_element PageSwitchButton::get_function_onCallback() const
{
	return(m_function_onCallback);
}


constexpr inline Element::f_element PageSwitchButton::get_function_onChangePage() const
{
	return(m_function_onChangePage);
}


constexpr inline Element::f_element PageSwitchButton::get_function_onChangeShape() const
{
	return(m_function_onChangeShape);
}


constexpr inline Element::f_element PageSwitchButton::get_function_onChangePageActual() const
{
	return(m_function_onChangePageActual);
}