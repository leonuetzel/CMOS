#include "../../Inc/graphics/graphics.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

CODE_RAM void PageSwitchButton::writeText()
{
	//	Convert Page Number to String
	const String pageNumberAsString(Graphics::get().get_pageActual());
	
	
	//	Draw Text
	draw_string("<", e_align::CENTER_LEFT, *m_font, m_colorText);
	draw_string(">", e_align::CENTER_RIGHT, *m_font, m_colorText);
	draw_string(pageNumberAsString, e_align::CENTER, *m_font, m_colorText);
}







CODE_RAM void PageSwitchButton::onUpdate(Element& element)
{
	PageSwitchButton& pageSwitchButton = (PageSwitchButton&) element;
	
	
	//	Write actual Page Number
	pageSwitchButton.writeText();
	
	
	//	Execute User Update first to be able to display Changes after
	if(pageSwitchButton.m_function_onUpdate != nullptr)
	{
		pageSwitchButton.m_function_onUpdate(pageSwitchButton);
	}
}


CODE_RAM void PageSwitchButton::onCallback(Element& element)
{
	PageSwitchButton& pageSwitchButton = (PageSwitchButton&) element;
	
	Graphics& graphics = Graphics::get();
	const uint8 c_numberOfPages = graphics.get_numberOfPages();
	
	Vec2 size = pageSwitchButton.get_size();
	
	int8 add = 0;
	if(pageSwitchButton.m_touchPosition.x > size.x / 2)
	{
		add = 1;
	}
	if(pageSwitchButton.m_touchPosition.x < size.x / 2)
	{
		add = -1;
	}
	
	
	
	int8 pageActual = graphics.get_pageActual();
	pageActual += add;
	if(pageActual < 0)
	{
		pageActual = c_numberOfPages - 1;
	}
	if((uint8) pageActual >= c_numberOfPages)
	{
		pageActual = 0;
	}
	graphics.set_pageActual(pageActual);
	
	
	
	
	
	if(pageSwitchButton.m_function_onCallback != nullptr)
	{
		pageSwitchButton.m_function_onCallback(element);
		pageSwitchButton.m_updateRequested = true;
	}
}


CODE_RAM void PageSwitchButton::onChangePage(Element& element)
{
	PageSwitchButton& pageSwitchButton = (PageSwitchButton&) element;
	
	if(pageSwitchButton.m_function_onChangePage != nullptr)
	{
		pageSwitchButton.m_function_onChangePage(element);
	}
}


CODE_RAM void PageSwitchButton::onChangeLayer(Element& element)
{
	PageSwitchButton& pageSwitchButton = (PageSwitchButton&) element;
	
	if(pageSwitchButton.m_function_onChangeLayer != nullptr)
	{
		pageSwitchButton.m_function_onChangeLayer(element);
	}
}


CODE_RAM void PageSwitchButton::onChangePosition(Element& element)
{
	PageSwitchButton& pageSwitchButton = (PageSwitchButton&) element;
	
	if(pageSwitchButton.m_function_onChangePosition != nullptr)
	{
		pageSwitchButton.m_function_onChangePosition(element);
	}
}


CODE_RAM void PageSwitchButton::onChangeSize(Element& element)
{
	PageSwitchButton& pageSwitchButton = (PageSwitchButton&) element;
	
	if(pageSwitchButton.m_function_onChangeSize != nullptr)
	{
		pageSwitchButton.m_function_onChangeSize(element);
	}
}


CODE_RAM void PageSwitchButton::onChangePageActual(Element& element)
{
	PageSwitchButton& pageSwitchButton = (PageSwitchButton&) element;
	
	const uint8 pageActual = Graphics::get().get_pageActual();
	pageSwitchButton.set_page(pageActual);
	
	if(pageSwitchButton.m_function_onChangePageActual != nullptr)
	{
		pageSwitchButton.m_function_onChangePageActual(element);
	}
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

PageSwitchButton::~PageSwitchButton()
{
	
}