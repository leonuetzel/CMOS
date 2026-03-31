#include "../../Inc/graphics/graphics.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

CODE_RAM void PageSwitchButton::writeText()
{
	//	Convert page number to string
	const String pageNumberAsString(Graphics::get().get_currentPage());
	
	
	//	Draw text
	draw_string("<", e_align::CENTER_LEFT, *m_font, m_colorText);
	draw_string(">", e_align::CENTER_RIGHT, *m_font, m_colorText);
	draw_string(pageNumberAsString, e_align::CENTER, *m_font, m_colorText);
}







CODE_RAM void PageSwitchButton::onUpdate(Element& element)
{
	PageSwitchButton& pageSwitchButton = (PageSwitchButton&) element;
	
	
	//	Write current page number
	pageSwitchButton.writeText();
	
	
	//	Execute user update first to be able to display changes after
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
	
	
	
	int8 pageCurrent = graphics.get_currentPage();
	pageCurrent += add;
	if(pageCurrent < 0)
	{
		pageCurrent = c_numberOfPages - 1;
	}
	if((uint8) pageCurrent >= c_numberOfPages)
	{
		pageCurrent = 0;
	}
	graphics.set_currentPage(pageCurrent);
	
	
	
	
	
	if(pageSwitchButton.m_function_onCallback != nullptr)
	{
		pageSwitchButton.m_function_onCallback(element);
		pageSwitchButton.requestUpdate();
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


CODE_RAM void PageSwitchButton::onChangeShape(Element& element)
{
	PageSwitchButton& pageSwitchButton = (PageSwitchButton&) element;
	
	if(pageSwitchButton.m_function_onChangeShape != nullptr)
	{
		pageSwitchButton.m_function_onChangeShape(element);
	}
}


CODE_RAM void PageSwitchButton::onChangePageActual(Element& element)
{
	PageSwitchButton& pageSwitchButton = (PageSwitchButton&) element;
	
	const uint8 pageCurrent = Graphics::get().get_currentPage();
	pageSwitchButton.set_page(pageCurrent);
	
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