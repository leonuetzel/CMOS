#include "../../Inc/graphics/graphics.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

CODE_RAM void Slider::onUpdate(Element& element)
{
	Slider& slider = (Slider&) element;
	
	
	//	Execute User Update first to be able to display Changes after
	if(slider.m_function_onUpdate != nullptr)
	{
		slider.m_function_onUpdate(slider);
	}
	
	
	//	Full Rebuild
	if(slider.m_rebuildRequested == true)
	{
		slider.draw_background(slider.m_colorBackground);
		slider.draw_frame(slider.m_colorFrame);
	}
	
	
	//	Normal Update
	if(slider.m_updateRequested == true)
	{
		uint8 percentNew = slider.m_percentNewFromCallback;
		if(slider.m_percentActualShown != percentNew || slider.m_rebuildRequested == true)
		{
			slider.draw_circleFilled(slider.get_center(slider.m_percentActualShown), c_radius, slider.m_colorBackground);
			slider.draw_middleLine(percentNew);
			slider.draw_circleFilled(slider.get_center(percentNew), c_radius, slider.m_colorCircle);
			slider.draw_string(slider.m_text, e_align::TOP_CENTER, slider.m_font, slider.m_colorText);
			slider.m_percentActualShown = percentNew;
		}
	}
}


CODE_RAM void Slider::onCallback(Element& element)
{
	Slider& slider = (Slider&) element;
	
	CMOS& cmos = CMOS::get();
	
	bool firstTimeCalculated = false;
	while(m_touchEvent != Graphics::e_touchEvent::RELEASE || firstTimeCalculated == false)
	{
		firstTimeCalculated = true;
		uint8 percent = slider.calculatePercentage();
		
		if(percent != slider.m_percentNewFromCallback)
		{
			slider.m_percentNewFromCallback = percent;
			
			if(slider.m_function_onCallback != nullptr)
			{
				slider.m_function_onCallback(element);
			}
			slider.m_updateRequested = true;
		}
		
		cmos.sleep_ms(10);
	}
}


CODE_RAM void Slider::onChangePage(Element& element)
{
	
}


CODE_RAM void Slider::onChangeLayer(Element& element)
{
	
}


CODE_RAM void Slider::onChangePosition(Element& element)
{
	
}


CODE_RAM void Slider::onChangeSize(Element& element)
{
	
}







CODE_RAM uint8 Slider::calculatePercentage()
{
	int32 nominator = 100 * (m_touchPosition.x - c_radius - 5);
	int32 denominator = size.x - 2 * c_radius - 10;
	int8 percent = nominator / denominator;
	
	if(percent < 0)
	{
		percent = 0;
	}
	if(percent > 100)
	{
		percent = 100;
	}
	
	return(percent);
}


CODE_RAM void Slider::draw_middleLine(uint8 percent)
{
	//	Check that Line Thickness is drawable
	static_assert(Slider::c_lineThickness % 2 == 1, "Slider::c_lineThickness in slider.hpp must be an odd Number.");
	
	Vec2 center = get_center(percent);
	int16 lineYPosition = center.y - (c_lineThickness - 1) / 2;
	
	Rectangle left
	(
		c_distanceFromBorder,
		lineYPosition,
		center.x - c_distanceFromBorder,
		c_lineThickness
	);
	Rectangle right
	(
		center.x,
		lineYPosition,
		size.x - center.x - c_distanceFromBorder - 1,
		c_lineThickness
	);
	draw_rectangleFilled(left,	m_colorLineLeft);
	draw_rectangleFilled(right,	m_colorLineRight);
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

Slider::~Slider()
{
	
}