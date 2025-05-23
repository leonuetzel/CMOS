#pragma once

#include "graphicsManager.hpp"





class Element	:	protected Rect
{
	public:
		
		enum class e_align_x: uint8
		{
			LEFT		= 0x0,
			CENTER	= 0x1,
			RIGHT		= 0x2
		};
		
		enum class e_align_y: uint8
		{
			BOTTOM	= 0x0,
			CENTER	= 0x1,
			TOP			= 0x2
		};
		
		enum class e_align: uint8
		{
			BOTTOM_LEFT			= 0x0,
			BOTTOM_CENTER		= 0x1,
			BOTTOM_RIGHT		= 0x2,
			CENTER_LEFT			= 0x4,
			CENTER					= 0x5,
			CENTER_RIGHT		= 0x6,
			TOP_LEFT				= 0x8,
			TOP_CENTER			= 0x9,
			TOP_RIGHT				= 0xA
		};
		
		enum class e_frameType: uint8
		{
			RECTANGLE,
			ROUND,
			ROUNDED,
			NONE
		};
		
		
		
		typedef void (*f_element)(Element& element);
		typedef Color (*f_color)(Vec2 pixelPosition, Vec2 rectangleSize);
		
		static constexpr int16 defaultTextDistanceFromBorder = 2;
		
		
		
		
		
	private:
		
		//	Static Member
		static RectGraphic* m_backBuffer;
		static I_GraphicAccelerator* m_graphicAccelerator;
		
		
		//	Non-static Member
		uint64 m_ticks;
		
		bool m_visible;
		bool m_touchable;
		
		f_element m_function_onUpdate;
		f_element m_function_onCallback;
		f_element m_function_onChangePage;
		f_element m_function_onChangeLayer;
		f_element m_function_onChangePosition;
		f_element m_function_onChangeSize;
		f_element m_function_onChangePageActual;
		uint16 m_updateRate_ms;
		
		uint8 m_page;
		uint8 m_layer;
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		void onUpdate();
		void onCallback();
		void onChangePageActual();
		
		feedback draw_rectangleFilledManual(Rect rectangle, Color color);
		
		
		//	Friends
		friend class Graphics;
		
		
		
		
		
	protected:
		
		static constexpr int16 c_frameRoundness = 8;
		
		e_frameType m_frameType;
		bool m_rebuildRequested;
		bool m_updateRequested;
		
		
		//	Touch Related
		Vec2 m_touchPosition;
		static Graphics::e_touchEvent m_touchEvent;
		bool m_touchValid;
		
		
		bool containsPoint(Vec2 point);
		
		
		
		
		
	public:
		
		static constexpr int16 c_minimumSideLength = c_frameRoundness * 2;
		
		constexpr inline Element(Rectangle shape, uint8 page, uint8 layer, f_element onUpdateFunction, f_element onCallbackFunction, uint32 updateRate_ms, e_frameType frameType);
		virtual ~Element();
		
		constexpr inline feedback		set_function_onUpdate(f_element onUpdateFunction);
		constexpr inline void				set_function_onCallback(f_element onCallbackFunction);
		constexpr inline void				set_function_onChangePage(f_element onChangePageFunction);
		constexpr inline void				set_function_onChangeLayer(f_element onChangeLayerFunction);
		constexpr inline void				set_function_onChangePosition(f_element onChangePositionFunction);
		constexpr inline void				set_function_onChangeSize(f_element onChangeSizeFunction);
		constexpr inline void				set_function_onChangePageActual(f_element onChangePageActualFunction);
		constexpr inline f_element	get_function_onUpdate() const;
		constexpr inline f_element	get_function_onCallback() const;
		constexpr inline f_element	get_function_onChangePage() const;
		constexpr inline f_element	get_function_onChangeLayer() const;
		constexpr inline f_element	get_function_onChangePosition() const;
		constexpr inline f_element	get_function_onChangeSize() const;
		constexpr inline f_element	get_function_onChangePageActual() const;
		
		constexpr inline void set_updateRate_ms(uint32 updateRate_ms);
		constexpr inline void set_visibility(bool visible);
		constexpr inline void set_touchability(bool touchable);
		feedback set_page(uint32 newPage);
		feedback set_layer(uint32 newLayer);
		feedback set_position(Vec2 newPosition);
		feedback set_size(Vec2 newSize);
		
		constexpr inline Vec2 get_position() const;
		constexpr inline Vec2 get_size() const;
		constexpr inline uint32 get_page() const;
		constexpr inline uint32 get_layer() const;
		constexpr inline uint32 get_updateRate_ms() const;
		constexpr inline e_frameType get_frameType() const;
		constexpr inline bool get_visibility() const;
		constexpr inline bool get_touchability() const;
		inline Graphics::e_touchEvent get_touchEvent() const;
		constexpr inline Vec2 get_touchPosition() const;
		constexpr inline bool get_touchValid() const;
		Vec2 get_align(e_align align, const String& string, const Font& font, int16 distanceFromBorder);
		
		Vec2 get_stringBox(const String& string, const Font& font, bool multiLine = false, int16 distanceFromBorder = defaultTextDistanceFromBorder);
		
		feedback set_pixel(Vec2 pixelPosition, Color color);
		inline void clear();
		void draw_frame(Color color);
		void draw_background(Color color);
		
		feedback draw_char(const Font::s_glyphDescription& glyphDescription, Vec2 bottomLeftPosition, Color color);
		feedback draw_char(	 				char character,	Vec2 bottomLeftPosition, 			const Font& font, Color color);
		feedback draw_string(				String string,	Vec2 bottomLeftPosition, 			const Font& font, Color color);
		Array<String> splitStringToMultiLine(const String& string,								const Font& font,																				int16 distanceFromBorder = defaultTextDistanceFromBorder);
		feedback draw_string(String string,	e_align_x align_x, uint32 y, 					const Font& font, Color color, bool multiLine = false, 	int16 distanceFromBorder = defaultTextDistanceFromBorder);
		feedback draw_string(String string,	e_align_y align_y, uint32 x, 					const Font& font, Color color, bool multiLine = false, 	int16 distanceFromBorder = defaultTextDistanceFromBorder);
		feedback draw_string(String string,	e_align align,												const Font& font, Color color, bool multiLine = false, 	int16 distanceFromBorder = defaultTextDistanceFromBorder);
		
		void draw_icon(const Icon& icon, Vec2 bottomLeftPosition);
		
		feedback draw_line(Vec2 start, Vec2 end, Color color);
		feedback draw_rectangle(Rect rectangle, Color color);
		feedback draw_rectangleFilled(Rect rectangle, Color color);
		feedback draw_circle(Vec2 center, uint32 radius, Color color);
		feedback draw_circle(Vec2 center, uint32 radius, Color color, double angleStart, double anglePart);
		feedback draw_circleFilled(Vec2 center, uint32 radius, Color color);
		feedback draw_circleFilled(Vec2 center, uint32 radius, Color color, double angleStart, double anglePart);
		feedback draw_circle(Vec2 center, uint32 radius, f_color color);
		feedback draw_circleFilled(Vec2 center, uint32 radius, f_color color);
		
		constexpr inline void requestRebuild();
		constexpr inline void requestUpdate();
		
		constexpr inline bool isRebuildRequested() const;
		constexpr inline bool isUpdateRequested() const;
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

constexpr inline Element::Element(Rect shape, uint8 page, uint8 layer, f_element onUpdateFunction, f_element onCallbackFunction, uint32 updateRate_ms, e_frameType frameType)
	:	Rectangle(shape),
		m_ticks(0),
		m_visible(true),
		m_touchable(true),
		m_function_onUpdate(onUpdateFunction),
		m_function_onCallback(onCallbackFunction),
		m_function_onChangePage(nullptr),
		m_function_onChangeLayer(nullptr),
		m_function_onChangePosition(nullptr),
		m_function_onChangeSize(nullptr),
		m_function_onChangePageActual(nullptr),
		m_updateRate_ms(updateRate_ms),
		m_page(page),
		m_layer(layer),
		m_frameType(frameType),
		m_rebuildRequested(false),
		m_updateRequested(false),
		m_touchValid(false)
{
	
}







constexpr inline feedback Element::set_function_onUpdate(f_element onUpdateFunction)
{
	if(onUpdateFunction == nullptr)
	{
		return(FAIL);
	}
	m_function_onUpdate = onUpdateFunction;
	return(OK);
}


constexpr inline void Element::set_function_onCallback(f_element onCallbackFunction)
{
	m_function_onCallback = onCallbackFunction;
}


constexpr inline void Element::set_function_onChangePage(f_element onChangePageFunction)
{
	m_function_onChangePage = onChangePageFunction;
}


constexpr inline void Element::set_function_onChangeLayer(f_element onChangeLayerFunction)
{
	m_function_onChangeLayer = onChangeLayerFunction;
}


constexpr inline void Element::set_function_onChangePosition(f_element onChangePositionFunction)
{
	m_function_onChangePosition = onChangePositionFunction;
}


constexpr inline void Element::set_function_onChangeSize(f_element onChangeSizeFunction)
{
	m_function_onChangeSize = onChangeSizeFunction;
}


constexpr inline void Element::set_function_onChangePageActual(f_element onChangePageActualFunction)
{
	m_function_onChangePageActual = onChangePageActualFunction;
}


constexpr inline Element::f_element Element::get_function_onUpdate() const
{
	return(m_function_onUpdate);
}


constexpr inline Element::f_element Element::get_function_onCallback() const
{
	return(m_function_onCallback);
}


constexpr inline Element::f_element Element::get_function_onChangePage() const
{
	return(m_function_onChangePage);
}


constexpr inline Element::f_element Element::get_function_onChangeLayer() const
{
	return(m_function_onChangeLayer);
}


constexpr inline Element::f_element Element::get_function_onChangePosition() const
{
	return(m_function_onChangePosition);
}


constexpr inline Element::f_element Element::get_function_onChangeSize() const
{
	return(m_function_onChangeSize);
}


constexpr inline Element::f_element Element::get_function_onChangePageActual() const
{
	return(m_function_onChangePageActual);
}







constexpr inline void Element::set_updateRate_ms(uint32 updateRate_ms)
{
	m_updateRate_ms = updateRate_ms;
}


constexpr inline void Element::set_visibility(bool visible)
{
	if(m_visible == visible)
	{
		return;
	}
	m_visible = visible;
	
	
	
	if(m_visible == false)
	{
		clear();
	}
	else
	{
		m_rebuildRequested = true;
		m_updateRequested = true;
	}
}


constexpr inline void Element::set_touchability(bool touchable)
{
	m_touchable = touchable;
}







constexpr inline Vec2 Element::get_position() const
{
	return(position);
}


constexpr inline Vec2 Element::get_size() const
{
	return(size);
}


constexpr inline uint32 Element::get_page() const
{
	return(m_page);
}


constexpr inline uint32 Element::get_layer() const
{
	return(m_layer);
}


constexpr inline uint32 Element::get_updateRate_ms() const
{
	return(m_updateRate_ms);
}


constexpr inline Element::e_frameType Element::get_frameType() const
{
	return(m_frameType);
}


constexpr inline bool Element::get_visibility() const
{
	return(m_visible);
}


constexpr inline bool Element::get_touchability() const
{
	return(m_touchable);
}


inline Graphics::e_touchEvent Element::get_touchEvent() const
{
	return(m_touchEvent);
}


constexpr inline Vec2 Element::get_touchPosition() const
{
	return(m_touchPosition);
}


constexpr inline bool Element::get_touchValid() const
{
	return(m_touchValid);
}







inline void Element::clear()
{
	draw_rectangleFilled(Rect(Vec2(0, 0), size), Colors::transparent);
}







constexpr inline void Element::requestRebuild()
{
	m_rebuildRequested = true;
}


constexpr inline void Element::requestUpdate()
{
	m_updateRequested = true;
}







constexpr inline bool Element::isRebuildRequested() const
{
	return(m_rebuildRequested);
}


constexpr inline bool Element::isUpdateRequested() const
{
	return(m_updateRequested);
}