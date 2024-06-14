#include "../../Inc/graphics/graphics.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

CODE_RAM void Loading::onUpdate(Element& element)
{
	Loading& loading = (Loading&) element;
	
	
	//	Execute User Update first to be able to display Changes after
	if(loading.m_function_onUpdate != nullptr)
	{
		loading.m_function_onUpdate(element);
	}
	
	
	//	Full Rebuild
	if(loading.m_rebuildRequested == true)
	{
		loading.draw_background(loading.m_colorBackground);
		loading.draw_frame(loading.m_colorFrame);
	}
	
	
	//	Normal Update
	if(loading.m_updateRequested == true)
	{
		Vec2 center = loading.get_size() / 2;
		
		for(uint32 i = 0; i < loading.m_numberOfCircles; i++)
		{
			Vec2 position;
			position.x = center.x - loading.m_bigCircleRadius * cos(loading.m_index + i * loading.m_angleRad);
			position.y = center.y + loading.m_bigCircleRadius * sin(loading.m_index + i * loading.m_angleRad);
			loading.draw_circleFilled(position, loading.m_circleRadius, loading.m_colorBackground);
		}
		
		loading.m_index += c_stepWidth;
		
		for(uint32 i = 0; i < loading.m_numberOfCircles; i++)
		{
			Vec2 position;
			position.x = center.x - loading.m_bigCircleRadius * cos(loading.m_index + i * loading.m_angleRad);
			position.y = center.y + loading.m_bigCircleRadius * sin(loading.m_index + i * loading.m_angleRad);
			if(loading.m_colorFunction == nullptr)
			{
				loading.draw_circleFilled(position, loading.m_circleRadius, loading.m_colorCircles);
			}
			else
			{
				loading.draw_circleFilled(position, loading.m_circleRadius, loading.m_colorFunction);
			}
		}
		
		if(loading.m_progress != nullptr)
		{
			loading.draw_string(String(loading.m_progress_old) + "%", Element::e_align::CENTER, loading.m_font, Colors::transparent);
			loading.m_progress_old = (*loading.m_progress);
			loading.draw_string(String(loading.m_progress_old) + "%", e_align::CENTER, loading.m_font, loading.m_colorText);
		}
	}
}


CODE_RAM void Loading::onCallback(Element& element)
{
	Loading& loading = (Loading&) element;
	
	if(loading.m_function_onCallback != nullptr)
	{
		loading.m_function_onCallback(element);
		loading.m_updateRequested = true;;
	}
}


CODE_RAM void Loading::onChangePage(Element& element)
{
	
}


CODE_RAM void Loading::onChangeLayer(Element& element)
{
	
}


CODE_RAM void Loading::onChangePosition(Element& element)
{
	
}


CODE_RAM void Loading::onChangeSize(Element& element)
{
	
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

Loading::~Loading()
{
	
}







CODE_RAM void Loading::set_progressInformation(uint8* progress)
{
	m_progress = progress;
	if(m_progress == nullptr)
	{
		draw_string(String(m_progress_old) + "%", Element::e_align::CENTER, m_font, Colors::transparent);
		m_progress_old = 0;
	}
}