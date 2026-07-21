#pragma once

#include "../cmos.hpp"
#include "font.hpp"
#include "fonts.hpp"
#include "icon.hpp"
#include "icons.hpp"
#include "rectGraphic.hpp"





class Element;

class Graphics
{
	public:
		
		
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		I_DisplayDriver* m_displayDriver;
		I_GraphicAccelerator* m_graphicAccelerator;
		
		Rect m_frameBufferShape;
		Color* m_frameBufferData[2];
		uint8 m_frameBufferIndex;
		
		bool m_pageChangedActionNecessary;
		
		uint8 m_fps;
		Color m_backgroundColor;
		
		uint8 m_numberOfPages;
		uint8 m_pageCurrent;
		Array<Element*> m_elements;
		
		
		//	Constructor and Destructor
		inline Graphics();
		Graphics(const Graphics& graphics) = delete;
		inline ~Graphics();
		
		
		//	Member Functions
		void manager();
		
		
		//	Friends
		friend class Element;
		
		
		
		
		
	public:
		
		feedback init(I_GraphicAccelerator& graphicAccelerator, I_DisplayDriver& displayDriver, Rect frameShape, Color* frameBuffer1, Color* frameBuffer2);
		static Graphics& get();
		
		
		//	Display
		inline Vec2 get_displayDimensions() const;
		constexpr inline uint8 get_fps() const;
		inline feedback set_backgroundColor(Color backgroundColor);
		constexpr inline Color get_backgroundColor() const;
		
		
		//	Pages
		uint8 add_page();
		feedback remove_page(uint8 pageNumber);
		constexpr inline uint8 get_numberOfPages() const;
		constexpr inline feedback set_currentPage(uint8 page);
		constexpr inline uint8 get_currentPage() const;
		
		
		//	Elements
		inline bool containsElement(Element& element) const;
		inline bool containsElement(Element* element) const;
		inline Array<Element*> listElements() const;
		void eraseAllElements();
		
		void register_touchData(Vec2 touchPoint, I_TouchController::e_touchEvent touchEvent);
		
		Graphics& operator+=(Element& element);
		Graphics& operator-=(Element& element);
		inline Graphics& operator+=(Element* element);
		inline Graphics& operator-=(Element* element);
		inline Graphics& operator+=(const Array<Element*> elements);
		inline Graphics& operator-=(const Array<Element*> elements);
};



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

inline Graphics::Graphics()
	:	m_displayDriver(nullptr),
		m_graphicAccelerator(nullptr),
		
		m_frameBufferShape(Vec2(0, 0), Vec2(0, 0)),
		m_frameBufferData(),
		m_frameBufferIndex(0),
		
		m_pageChangedActionNecessary(false),
		
		m_fps(0),
		m_backgroundColor(Colors::transparent),
		
		m_numberOfPages(1),
		m_pageCurrent(0),
		
		m_elements()
{
	m_frameBufferData[0] = nullptr;
	m_frameBufferData[1] = nullptr;
}


inline Graphics::~Graphics()
{
	
}



/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

inline Vec2 Graphics::get_displayDimensions() const
{
	return(m_displayDriver->get_displayDimensions());
}


constexpr inline uint8 Graphics::get_fps() const
{
	return(m_fps);
}


inline feedback Graphics::set_backgroundColor(Color backgroundColor)
{
	if(m_displayDriver->set_colorBackground(backgroundColor) == OK)
	{
		m_backgroundColor = backgroundColor;
		return(OK);
	}
	return(FAIL);
}


constexpr inline Color Graphics::get_backgroundColor() const
{
	return(m_backgroundColor);
}







constexpr inline uint8 Graphics::get_numberOfPages() const
{
	return(m_numberOfPages);
}


constexpr inline feedback Graphics::set_currentPage(uint8 page)
{
	if(page < m_numberOfPages)
	{
		m_pageCurrent = page;
		m_pageChangedActionNecessary = true;
		return(OK);
	}
	return(FAIL);
}


constexpr inline uint8 Graphics::get_currentPage() const
{
	return(m_pageCurrent);
}







inline bool Graphics::containsElement(Element& element) const
{
	return(m_elements.contains(&element));
}


inline bool Graphics::containsElement(Element* element) const
{
	return(m_elements.contains(element));
}


inline Array<Element*> Graphics::listElements() const
{
	return(m_elements);
}







inline Graphics& Graphics::operator+=(Element* element)
{
	return(operator+=(*element));
}


inline Graphics& Graphics::operator-=(Element* element)
{
	return(operator-=(*element));
}


inline Graphics& Graphics::operator+=(const Array<Element*> elements)
{
	for(auto& i: elements)
	{
		operator+=(i);
	}
	return(*this);
}


inline Graphics& Graphics::operator-=(const Array<Element*> elements)
{
	for(auto& i: elements)
	{
		operator-=(i);
	}
	return(*this);
}