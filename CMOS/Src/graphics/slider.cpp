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
	
	
	//	Execute user update first to be able to display changes after
	if(slider.m_function_onUpdate != nullptr)
	{
		slider.m_function_onUpdate(slider);
	}
	
	
	//	Full rebuild
	if(slider.isRebuildRequested() == true)
	{
		slider.draw_background(slider.m_colorBackground);
		slider.draw_frame(slider.m_colorFrame);
	}
	
	
	//	Take temporary variables to avoid multiple accesses
	const uint8 percentActualShown = slider.m_percentActualShown;
	const uint8 percentNew = slider.m_percentNewFromCallback;
	
	if(percentActualShown != percentNew || slider.isRebuildRequested() == true)
	{
		//	Delete old circle
		slider.draw_circleFilled(slider.get_center(percentActualShown), c_radius, slider.m_colorBackground);
		
		
		//	Draw middle line
		slider.draw_middleLine(percentNew);
		
		
		//	Draw new circle
		slider.draw_circleFilled(slider.get_center(percentNew), c_radius, slider.m_colorCircle);
		
		
		//	Draw text
		slider.draw_string(slider.m_text, e_align::TOP_CENTER, slider.m_font, slider.m_colorText);
		
		
		//	Update actual shown value
		slider.m_percentActualShown = percentNew;
	}
}


CODE_RAM void Slider::onCallback(Element& element)
{
	Slider& slider = (Slider&) element;
	
	CMOS& cmos = CMOS::get();
	
	bool firstTimeCalculated = false;
	while(m_touchEvent != I_TouchController::e_touchEvent::RELEASE || firstTimeCalculated == false)
	{
		firstTimeCalculated = true;
		uint8 percent = slider.calculatePercentage();
		
		if(percent != slider.m_percentNewFromCallback)
		{
			//	Update slider value
			slider.m_percentNewFromCallback = percent;
			
			
			//	Execute user update first to be able to display changes after
			if(slider.m_function_onCallback != nullptr)
			{
				slider.m_function_onCallback(element);
			}
			
			
			//	Request update to draw the new value
			slider.requestUpdate();
			
			
			//	Unlock semaphore so that an update can take place
			cmos.semaphore_unlock(&element);
			cmos.sleep_ms(1);
			cmos.semaphore_lock(&element);
		}
		
		cmos.sleep_ms(10);
	}
}


CODE_RAM void Slider::onChangePage(Element& element)
{
	
}


CODE_RAM void Slider::onChangeShape(Element& element)
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
	//	Check that line thickness is drawable
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