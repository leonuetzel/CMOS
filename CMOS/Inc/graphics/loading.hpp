#pragma once

#include "../cmos.hpp"
#include "element.hpp"





class Loading: public Element
{
	public:
		
		
		
		
		
		
		
	private:
		
		static constexpr double c_stepWidth = 0.1;
		
		double m_index;
		
		uint8* m_progress;
		uint8 m_progress_old;
		const Font& m_font;
		Color m_colorText;
		Color m_colorBackground;
		Color m_colorFrame;
		Color m_colorCircles;
		f_color m_colorFunction;
		
		uint32 m_numberOfCircles;
		uint32 m_bigCircleRadius;
		uint32 m_circleRadius;
		float m_angleRad;
		
		
		f_element m_function_onUpdate;
		f_element m_function_onCallback;
		f_element m_function_onChangePage;
		f_element m_function_onChangeLayer;
		f_element m_function_onChangePosition;
		f_element m_function_onChangeSize;
		
		static void onUpdate(Element& element);
		static void onCallback(Element& element);
		static void onChangePage(Element& element);
		static void onChangeLayer(Element& element);
		static void onChangePosition(Element& element);
		static void onChangeSize(Element& element);
		
		
		
		
		
	public:
		
		inline Loading(Element element, const Font& font, Color colorText, Color colorBackground, Color colorFrame, Color colorCircles, uint32 numberOfCircles, uint32 bigCircleRadius, uint32 angleBetweenTwoCircles_degree, uint32 circleRadius);
		inline Loading(Element element, const Font& font, Color colorText, Color colorBackground, Color colorFrame, f_color colorCircles, uint32 numberOfCircles, uint32 bigCircleRadius, uint32 angleBetweenTwoCircles_degree, uint32 circleRadius);
		virtual ~Loading();
		
		constexpr inline void				set_function_onUpdate(f_element onUpdateFunction);
		constexpr inline void				set_function_onCallback(f_element onCallbackFunction);
		constexpr inline void				set_function_onChangePage(f_element onChangePageFunction);
		constexpr inline void				set_function_onChangeLayer(f_element onChangeLayerFunction);
		constexpr inline void				set_function_onChangePosition(f_element onChangePositionFunction);
		constexpr inline void				set_function_onChangeSize(f_element onChangeSizeFunction);
		constexpr inline f_element	get_function_onUpdate() const;
		constexpr inline f_element	get_function_onCallback() const;
		constexpr inline f_element	get_function_onChangePage() const;
		constexpr inline f_element	get_function_onChangeLayer() const;
		constexpr inline f_element	get_function_onChangePosition() const;
		constexpr inline f_element	get_function_onChangeSize() const;
		
		void set_progressInformation(uint8* progress);
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

inline Loading::Loading(Element element, const Font& font, Color colorText, Color colorBackground, Color colorFrame, Color colorCircles, uint32 numberOfCircles, uint32 bigCircleRadius, uint32 angleBetweenTwoCircles_degree, uint32 circleRadius)
	:	Element(element),
		m_index(0),
		
		m_progress(nullptr),
		m_progress_old(0),
		m_font(font),
		m_colorText(colorText),
		m_colorBackground(colorBackground),
		m_colorFrame(colorFrame),
		m_colorCircles(colorCircles),
		m_colorFunction(nullptr),
		
		m_numberOfCircles(numberOfCircles),
		m_bigCircleRadius(bigCircleRadius),
		m_circleRadius(circleRadius),
		m_angleRad(Math::degToRad<float>(angleBetweenTwoCircles_degree)),
		
		m_function_onUpdate(Element::get_function_onUpdate()),
		m_function_onCallback(Element::get_function_onCallback()),
		m_function_onChangePage(Element::get_function_onChangePage()),
		m_function_onChangeLayer(Element::get_function_onChangeLayer()),
		m_function_onChangePosition(Element::get_function_onChangePosition()),
		m_function_onChangeSize(Element::get_function_onChangeSize())
{
	Element::set_function_onUpdate(onUpdate);
	Element::set_function_onCallback(onCallback);
	Element::set_function_onChangePage(onChangePage);
	Element::set_function_onChangeLayer(onChangeLayer);
	Element::set_function_onChangePosition(onChangePosition);
	Element::set_function_onChangeSize(onChangeSize);
	
	set_updateRate_ms(1);
}


inline Loading::Loading(Element element, const Font& font, Color colorText, Color colorBackground, Color colorFrame, f_color colorCircles, uint32 numberOfCircles, uint32 bigCircleRadius, uint32 angleBetweenTwoCircles_degree, uint32 circleRadius)
	:	Element(element),
		m_index(0),
		
		m_progress(nullptr),
		m_progress_old(0),
		m_font(font),
		m_colorText(colorText),
		m_colorBackground(colorBackground),
		m_colorFrame(colorFrame),
		m_colorCircles(Colors::transparent),
		m_colorFunction(colorCircles),
		
		m_numberOfCircles(numberOfCircles),
		m_bigCircleRadius(bigCircleRadius),
		m_circleRadius(circleRadius),
		m_angleRad(Math::degToRad<float>(angleBetweenTwoCircles_degree)),
		
		m_function_onUpdate(Element::get_function_onUpdate()),
		m_function_onCallback(Element::get_function_onCallback()),
		m_function_onChangePage(Element::get_function_onChangePage()),
		m_function_onChangeLayer(Element::get_function_onChangeLayer()),
		m_function_onChangePosition(Element::get_function_onChangePosition()),
		m_function_onChangeSize(Element::get_function_onChangeSize())
{
	Element::set_function_onUpdate(onUpdate);
	Element::set_function_onCallback(onCallback);
	Element::set_function_onChangePage(onChangePage);
	Element::set_function_onChangeLayer(onChangeLayer);
	Element::set_function_onChangePosition(onChangePosition);
	Element::set_function_onChangeSize(onChangeSize);
	
	set_updateRate_ms(1);
}







constexpr inline void Loading::set_function_onUpdate(f_element onUpdateFunction)
{
	m_function_onUpdate = onUpdateFunction;
}


constexpr inline void Loading::set_function_onCallback(f_element onCallbackFunction)
{
	m_function_onCallback = onCallbackFunction;
}


constexpr inline void Loading::set_function_onChangePage(f_element onChangePageFunction)
{
	m_function_onChangePage = onChangePageFunction;
}


constexpr inline void Loading::set_function_onChangeLayer(f_element onChangeLayerFunction)
{
	m_function_onChangeLayer = onChangeLayerFunction;
}


constexpr inline void Loading::set_function_onChangePosition(f_element onChangePositionFunction)
{
	m_function_onChangePosition = onChangePositionFunction;
}


constexpr inline void Loading::set_function_onChangeSize(f_element onChangeSizeFunction)
{
	m_function_onChangeSize = onChangeSizeFunction;
}


constexpr inline Element::f_element Loading::get_function_onUpdate() const
{
	return(m_function_onUpdate);
}


constexpr inline Element::f_element Loading::get_function_onCallback() const
{
	return(m_function_onCallback);
}


constexpr inline Element::f_element Loading::get_function_onChangePage() const
{
	return(m_function_onChangePage);
}


constexpr inline Element::f_element Loading::get_function_onChangeLayer() const
{
	return(m_function_onChangeLayer);
}


constexpr inline Element::f_element Loading::get_function_onChangePosition() const
{
	return(m_function_onChangePosition);
}


constexpr inline Element::f_element Loading::get_function_onChangeSize() const
{
	return(m_function_onChangeSize);
}