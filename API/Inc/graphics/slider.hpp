#pragma once

#include "../cmos.hpp"
#include "element.hpp"





class Slider: public Element
{
	public:
		
		
		
		
		
		
		
	private:
		
		static constexpr uint32 c_radius							= 8;
		static constexpr uint32 c_lineThickness				= 3;
		static constexpr uint32 c_distanceFromBorder	= 5;
		
		String m_text;
		const Font& m_font;
		Color m_colorText;
		Color m_colorBackground;
		Color m_colorFrame;
		Color m_colorLineLeft;
		Color m_colorLineRight;
		Color m_colorCircle;
		
		uint8 m_percentActualShown;
		uint8 m_percentNewFromCallback;
		
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
		
		uint8 calculatePercentage();
		constexpr inline Vec2 get_center(uint8 percent);
		void draw_middleLine(uint8 percent);
		
		
		
		
		
	public:
		
		inline Slider(Element element, String text, const Font& font, Color colorText, Color colorBackground, Color colorFrame, double initPercent);
		virtual ~Slider();
		
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
		
		constexpr inline feedback set_colorLine(Color colorBottom, Color colorTop);
		constexpr inline feedback set_colorCircle(Color color);
		
		constexpr inline Color get_colorLineBottom() const;
		constexpr inline Color get_colorLineTop() const;
		constexpr inline Color get_colorCircle() const;
		constexpr inline uint8 get_percent() const;
};



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

constexpr inline Vec2 Slider::get_center(uint8 percent)
{
	constexpr uint32 c_centerDistanceFromBorder = c_radius + c_distanceFromBorder;
	
	Vec2 size = get_size();
	const uint32 c_usableLength = size.x - 2 * c_centerDistanceFromBorder - 1;
	
	Vec2 center
	(
		c_radius + c_distanceFromBorder + (percent * c_usableLength) / 100,
		size.y / 3
	);
	
	return(center);
}



/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

inline Slider::Slider(Element element, String text, const Font& font, Color colorText, Color colorBackground, Color colorFrame, double initPercent)
	:	Element(element),
		m_text(text),
		m_font(font),
		m_colorText(colorText),
		m_colorBackground(colorBackground),
		m_colorFrame(colorFrame),
		m_colorLineLeft(Colors::blue),
		m_colorLineRight(Colors::white),
		m_colorCircle(Colors::silver),
		
		m_percentActualShown(initPercent),
		m_percentNewFromCallback(m_percentActualShown),
		
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
}







constexpr inline void Slider::set_function_onUpdate(f_element onUpdateFunction)
{
	m_function_onUpdate = onUpdateFunction;
}


constexpr inline void Slider::set_function_onCallback(f_element onCallbackFunction)
{
	m_function_onCallback = onCallbackFunction;
}


constexpr inline void Slider::set_function_onChangePage(f_element onChangePageFunction)
{
	m_function_onChangePage = onChangePageFunction;
}


constexpr inline void Slider::set_function_onChangeLayer(f_element onChangeLayerFunction)
{
	m_function_onChangeLayer = onChangeLayerFunction;
}


constexpr inline void Slider::set_function_onChangePosition(f_element onChangePositionFunction)
{
	m_function_onChangePosition = onChangePositionFunction;
}


constexpr inline void Slider::set_function_onChangeSize(f_element onChangeSizeFunction)
{
	m_function_onChangeSize = onChangeSizeFunction;
}


constexpr inline Element::f_element Slider::get_function_onUpdate() const
{
	return(m_function_onUpdate);
}


constexpr inline Element::f_element Slider::get_function_onCallback() const
{
	return(m_function_onCallback);
}


constexpr inline Element::f_element Slider::get_function_onChangePage() const
{
	return(m_function_onChangePage);
}


constexpr inline Element::f_element Slider::get_function_onChangeLayer() const
{
	return(m_function_onChangeLayer);
}


constexpr inline Element::f_element Slider::get_function_onChangePosition() const
{
	return(m_function_onChangePosition);
}


constexpr inline Element::f_element Slider::get_function_onChangeSize() const
{
	return(m_function_onChangeSize);
}







constexpr inline feedback Slider::set_colorLine(Color colorBottom, Color colorTop)
{
	m_colorLineLeft = colorBottom;
	m_colorLineRight = colorTop;
	return(OK);
}


constexpr inline feedback Slider::set_colorCircle(Color color)
{
	m_colorCircle = color;
	return(OK);
}







constexpr inline Color Slider::get_colorLineBottom() const
{
	return(m_colorLineLeft);
}


constexpr inline Color Slider::get_colorLineTop() const
{
	return(m_colorLineRight);
}


constexpr inline Color Slider::get_colorCircle() const
{
	return(m_colorCircle);
}


constexpr inline uint8 Slider::get_percent() const
{
	return(m_percentNewFromCallback);
}