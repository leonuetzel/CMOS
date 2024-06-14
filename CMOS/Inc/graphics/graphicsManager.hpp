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
		
		enum class e_touchEvent: uint8
		{
			INVALID		= 0,
			TOUCH			= 1,
			CONTACT		= 2,
			RELEASE		= 3
		};
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		I_DisplayDriver* m_displayDriver;
		I_GraphicAccelerator* m_graphicAccelerator;
		
		bool m_initialized;
		uint8 m_thread_ID;
		
		Array<Element*> m_elements;
		uint8 m_fps;
		uint8 c_numberOfLayers;
		uint8 c_numberOfPages;
		uint8 m_pageActual;
		
		bool m_reloadRequested;
		
		Color m_backgroundColor;
		RectGraphic m_background;
		bool m_background_visible;
		Array<bool> m_layerTouchability;
		
		
		//	Constructor and Destructor
		constexpr inline Graphics();
		Graphics(const Graphics& graphics) = delete;
		inline ~Graphics();
		
		
		//	Member Functions
		void manager();
		
		feedback clear_layer(uint8 layer);
		
		
		//	Friends
		friend class Element;
		
		
		
		
		
	public:
		
		feedback init(I_GraphicAccelerator& graphicAccelerator, I_DisplayDriver& displayDriver, I_Semaphore& semaphore, uint8 numberOfPages, const Array<Color*>& backBuffer, Color* backgroundBuffer);
		static Graphics& get();
		
		inline feedback set_backgroundColor(Color backgroundColor);
		
		
		inline Vec2 get_displayDimensions();
		inline Rect get_layerData(uint8 layer);
		constexpr inline uint8 get_numberOfLayers() const;
		inline feedback set_layerAlpha(uint8 layer, uint8 alpha);
		inline feedback set_layerTouchability(uint8 layer, bool touchable);
		
		constexpr inline uint8 get_fps();
		
		constexpr inline feedback set_pageActual(uint8 page);
		constexpr inline uint8 get_pageActual() const;
		constexpr inline uint8 get_numberOfPages() const;
		
		constexpr inline void set_background_visibility(bool visible);
		constexpr inline feedback set_pixel_background(Vec2 pixelPosition, Color color);
		
		inline void transferLayerToFrontbuffer();
		inline feedback transferLayerToFrontbuffer(uint8 layer);
		
		inline bool contains(Element& element) const;
		inline bool contains(Element* element) const;
		inline Array<Element*> list() const;
		void erase();
		
		void register_touchData(Vec2 touchPoint, e_touchEvent touchEvent);
		
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

constexpr inline Graphics::Graphics()
	:	m_displayDriver(nullptr),
		m_graphicAccelerator(nullptr),
		
		m_initialized(false),
		m_thread_ID(CMOS::threadID_invalid),
		
		m_elements(),
		m_fps(0),
		c_numberOfLayers(0),
		c_numberOfPages(0),
		m_pageActual(0),
		
		m_reloadRequested(true),
		
		m_backgroundColor(Colors::transparent),
		m_background(),
		m_background_visible(false)
{
	m_elements.erase();
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

inline feedback Graphics::set_backgroundColor(Color backgroundColor)
{
	if(m_displayDriver->set_colorBackground(backgroundColor.red, backgroundColor.green, backgroundColor.blue) == OK)
	{
		m_backgroundColor = backgroundColor;
		return(OK);
	}
	return(FAIL);
}







inline Vec2 Graphics::get_displayDimensions()
{
	return(m_displayDriver->get_displayDimensions());
}


inline Rect Graphics::get_layerData(uint8 layer)
{
	return(m_displayDriver->get_layerData(layer));
}


constexpr inline uint8 Graphics::get_numberOfLayers() const
{
	return(c_numberOfLayers);
}


inline feedback Graphics::set_layerAlpha(uint8 layer, uint8 alpha)
{
	return(m_displayDriver->set_layerAlpha(layer, alpha));
}


inline feedback Graphics::set_layerTouchability(uint8 layer, bool touchable)
{
	if(layer < c_numberOfLayers)
	{
		m_layerTouchability[layer] = touchable;
		return(OK);
	}
	return(FAIL);
}







constexpr inline uint8 Graphics::get_fps()
{
	return(m_fps);
}







constexpr inline feedback Graphics::set_pageActual(uint8 page)
{
	if(page < c_numberOfPages)
	{
		m_pageActual = page;
		m_reloadRequested = true;
		return(OK);
	}
	
	return(FAIL);
}


constexpr inline uint8 Graphics::get_pageActual() const
{
	return(m_pageActual);
}


constexpr inline uint8 Graphics::get_numberOfPages() const
{
	return(c_numberOfPages);
}







constexpr inline void Graphics::set_background_visibility(bool visible)
{
	m_background_visible = visible;
}


constexpr inline feedback Graphics::set_pixel_background(Vec2 pixelPosition, Color color)
{
	if(m_background.contains(pixelPosition) == false || m_background.data == nullptr)
	{
		return(FAIL);
	}
	
	Color* pixel = m_background.data + ((m_background.size.y - pixelPosition.y - 1) * m_background.size.x + pixelPosition.x);
	*pixel = color;
	
	return(OK);
}







inline void Graphics::transferLayerToFrontbuffer()
{
	CMOS::get().send_mail(m_thread_ID, c_numberOfLayers);
}


inline feedback Graphics::transferLayerToFrontbuffer(uint8 layer)
{
	if(layer < c_numberOfLayers)
	{
		CMOS::get().send_mail(m_thread_ID, layer);
		return(OK);
	}
	return(FAIL);
}







inline bool Graphics::contains(Element& element) const
{
	return(m_elements.contains(&element));
}


inline bool Graphics::contains(Element* element) const
{
	return(m_elements.contains(element));
}


inline Array<Element*> Graphics::list() const
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