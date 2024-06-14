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
		f_element m_function_onChangeLayer;
		f_element m_function_onChangePosition;
		f_element m_function_onChangeSize;
		f_element m_function_onChangePageActual;
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		void writePageNumber(uint8 pageNumber);
		
		static void onUpdate(Element& element);
		static void onCallback(Element& element);
		static void onChangePage(Element& element);
		static void onChangeLayer(Element& element);
		static void onChangePosition(Element& element);
		static void onChangeSize(Element& element);
		static void onChangePageActual(Element& element);
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		inline PageSwitchButton(Button button);
		virtual ~PageSwitchButton();
		
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
		m_function_onChangeLayer(Button::get_function_onChangeLayer()),
		m_function_onChangePosition(Button::get_function_onChangePosition()),
		m_function_onChangeSize(Button::get_function_onChangeSize()),
		m_function_onChangePageActual(Button::get_function_onChangePageActual())
{
	Button::set_function_onUpdate(onUpdate);
	Button::set_function_onCallback(onCallback);
	Button::set_function_onChangePage(onChangePage);
	Button::set_function_onChangeLayer(onChangeLayer);
	Button::set_function_onChangePosition(onChangePosition);
	Button::set_function_onChangeSize(onChangeSize);
	Button::set_function_onChangePageActual(onChangePageActual);
	
	writePageNumber(Graphics::get().get_pageActual());
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


constexpr inline void PageSwitchButton::set_function_onChangeLayer(f_element onChangeLayerFunction)
{
	m_function_onChangeLayer = onChangeLayerFunction;
}


constexpr inline void PageSwitchButton::set_function_onChangePosition(f_element onChangePositionFunction)
{
	m_function_onChangePosition = onChangePositionFunction;
}


constexpr inline void PageSwitchButton::set_function_onChangeSize(f_element onChangeSizeFunction)
{
	m_function_onChangeSize = onChangeSizeFunction;
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


constexpr inline Element::f_element PageSwitchButton::get_function_onChangeLayer() const
{
	return(m_function_onChangeLayer);
}


constexpr inline Element::f_element PageSwitchButton::get_function_onChangePosition() const
{
	return(m_function_onChangePosition);
}


constexpr inline Element::f_element PageSwitchButton::get_function_onChangeSize() const
{
	return(m_function_onChangeSize);
}


constexpr inline Element::f_element PageSwitchButton::get_function_onChangePageActual() const
{
	return(m_function_onChangePageActual);
}