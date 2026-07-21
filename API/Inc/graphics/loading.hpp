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
		f_element m_function_onChangeMyPage;
		f_element m_function_onChangeShape;
		f_element m_function_onChangeCurrentPage;
		
		static void onUpdate(Element& element);
		static void onCallback(Element& element);
		static void onChangeMyPage(Element& element);
		static void onChangeShape(Element& element);
		static void onChangeCurrentPage(Element& element);
		
		
		
		
		
	public:
		
		inline Loading(Element element, const Font& font, Color colorText, Color colorBackground, Color colorFrame, Color colorCircles, uint32 numberOfCircles, uint32 bigCircleRadius, uint32 angleBetweenTwoCircles_degree, uint32 circleRadius);
		inline Loading(Element element, const Font& font, Color colorText, Color colorBackground, Color colorFrame, f_color colorCircles, uint32 numberOfCircles, uint32 bigCircleRadius, uint32 angleBetweenTwoCircles_degree, uint32 circleRadius);
		virtual ~Loading();
		
		constexpr inline void				set_function_onUpdate(f_element onUpdateFunction);
		constexpr inline void				set_function_onCallback(f_element onCallbackFunction);
		constexpr inline void				set_function_onChangeMyPage(f_element onChangeMyPageFunction);
		constexpr inline void				set_function_onChangeShape(f_element onChangeShapeFunction);
		constexpr inline void				set_function_onChangeCurrentPage(f_element onChangeCurrentPageFunction);
		constexpr inline f_element	get_function_onUpdate() const;
		constexpr inline f_element	get_function_onCallback() const;
		constexpr inline f_element	get_function_onChangeMyPage() const;
		constexpr inline f_element	get_function_onChangeShape() const;
		constexpr inline f_element	get_function_onChangeCurrentPage() const;
		
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
		m_function_onChangeMyPage(Element::get_function_onChangeMyPage()),
		m_function_onChangeShape(Element::get_function_onChangeShape())
{
	Element::set_function_onUpdate(onUpdate);
	Element::set_function_onCallback(onCallback);
	Element::set_function_onChangeMyPage(onChangeMyPage);
	Element::set_function_onChangeShape(onChangeShape);
	
	set_updatePeriodInFrames(1);
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
		m_function_onChangeMyPage(Element::get_function_onChangeMyPage()),
		m_function_onChangeShape(Element::get_function_onChangeShape())
{
	Element::set_function_onUpdate(onUpdate);
	Element::set_function_onCallback(onCallback);
	Element::set_function_onChangeMyPage(onChangeMyPage);
	Element::set_function_onChangeShape(onChangeShape);
	
	set_updatePeriodInFrames(1);
}







constexpr inline void Loading::set_function_onUpdate(f_element onUpdateFunction)
{
	m_function_onUpdate = onUpdateFunction;
}


constexpr inline void Loading::set_function_onCallback(f_element onCallbackFunction)
{
	m_function_onCallback = onCallbackFunction;
}


constexpr inline void Loading::set_function_onChangeMyPage(f_element onChangeMyPageFunction)
{
	m_function_onChangeMyPage = onChangeMyPageFunction;
}


constexpr inline void Loading::set_function_onChangeShape(f_element onChangeShapeFunction)
{
	m_function_onChangeShape = onChangeShapeFunction;
}


constexpr inline Element::f_element Loading::get_function_onUpdate() const
{
	return(m_function_onUpdate);
}


constexpr inline Element::f_element Loading::get_function_onCallback() const
{
	return(m_function_onCallback);
}


constexpr inline Element::f_element Loading::get_function_onChangeMyPage() const
{
	return(m_function_onChangeMyPage);
}


constexpr inline Element::f_element Loading::get_function_onChangeShape() const
{
	return(m_function_onChangeShape);
}