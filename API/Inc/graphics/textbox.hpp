#pragma once

#include "../cmos.hpp"
#include "element.hpp"





class Textbox: public Element
{
	public:
		
		enum class e_event
		{
			NONE,
			SCROLLING,
			LINE_SELECTED
		};
		
		
		
		
		
	private:
		
		typedef struct
		{
			//	Text to display
			String text;
			
			
			//	Colors of that Line
			Color colorLine;
			Color colorLineTouched;
			Color colorText;
			Color colorTextTouched;
			
			
			//	Offset from left
			int16 xOffset;
			
			
			//	Touch
			bool touchable;
			bool isTouched;
			
			
			//	Flag needed for redrawing
			bool redrawNecessary;
			
			
			//	Actual displayed Line, so that we dont need to redraw every Line every Update
			struct
			{
				String text;
				Color colorLine;
				int16 scrollingInPixels;
			}displayed;
		}s_line;
		
		Array<s_line> m_lines;
		
		const Font& m_font;
		Color m_colorBackground;
		Color m_colorFrame;
		int16 m_scrollingInPixels;
		e_event m_event;
		
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
		
		constexpr inline bool isLineInScrollingWindow(uint32 lineNumber) const;
		
		
		
		
		
	protected:
		
		static constexpr int16 c_lineSpacing = 5;
		static constexpr uint32 c_lineNumberInvalid = 0xFFFFFFFF;
		
		constexpr inline int16 get_lineCoordinate(uint32 lineNumber) const;
		constexpr inline uint32 get_line(int16 y) const;
		constexpr inline int16 get_lineHeight() const;
		constexpr inline Color get_colorBackground() const;
		void drawLine(uint32 lineNumber);
		bool drawText(uint32 lineNumber, uint8 lineTouchedNew, bool forceDraw = false);
		
		
		
		
		
	public:
		
		Textbox(Element element, const Font& font);
		virtual ~Textbox();
		
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
		
		constexpr inline uint32 get_lineTouched() const;
		constexpr inline const Font& get_font() const;
		inline const String& get_text(uint32 lineNumber);
		inline bool is_touchable(uint32 lineNumber) const;
		inline Color get_colorText(uint32 lineNumber) const;
		inline Color get_colorTextTouched(uint32 lineNumber) const;
		inline Color get_colorLine(uint32 lineNumber) const;
		inline Color get_colorLineTouched(uint32 lineNumber) const;
		inline int16 get_xOffset(uint32 lineNumber) const;
		
		inline feedback set_text(const String& text, uint32 lineNumber);
		inline feedback set_lineTouchability(bool touchable);
		inline feedback set_lineTouchability(bool touchable, uint32 lineNumber);
		inline feedback set_colorText(Color color);
		inline feedback set_colorText(Color color, uint32 lineNumber);
		inline feedback set_colorTextTouched(Color color);
		inline feedback set_colorTextTouched(Color color, uint32 lineNumber);
		inline feedback set_colorLine(Color color);
		inline feedback set_colorLine(Color color, uint32 lineNumber);
		inline feedback set_colorLineTouched(Color color);
		inline feedback set_colorLineTouched(Color color, uint32 lineNumber);
		inline feedback set_xOffset(int16 xOffset);
		inline feedback set_xOffset(int16 xOffset, uint32 lineNumber);
		inline void reset_touchFlags();
		inline e_event get_event() const;
		
		
		//	Line Adding and Removal
		inline feedback add_line();
		feedback erase_line(uint32 lineNumber = c_lineNumberInvalid);
		feedback set_numberOfLines(uint32 numberOfLines);
		constexpr inline uint32 get_numberOfLines() const;
		constexpr inline float get_scrollPosition() const;
		
		
		constexpr inline void clear();
		constexpr inline feedback clear(uint32 lineNumber);
};



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

constexpr inline bool Textbox::isLineInScrollingWindow(uint32 lineNumber) const
{
	const int16 y = get_lineCoordinate(lineNumber);
	if(y < size.y - 1 && y + get_lineHeight() > 1)
	{
		return(true);
	}
	return(false);
}



/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/

constexpr inline int16 Textbox::get_lineCoordinate(uint32 lineNumber) const
{
	return(size.y - 1 - ((int16) lineNumber + 1) * (m_font.get_height() + c_lineSpacing) + m_scrollingInPixels);
}


constexpr inline uint32 Textbox::get_line(int16 y) const
{
	//	Y Position is out of Frame
	if(y < 0 || y >= size.y)
	{
		return(c_lineNumberInvalid);
	}
	
	
	//	Calculate Line Number
	const uint32 lineNumber = (size.y - 1 - y + m_scrollingInPixels) / (m_font.get_height() + c_lineSpacing);
	
	
	//	If Line is not displayable anymore, return invalid
	if(isLineInScrollingWindow(lineNumber) == false)
	{
		return(c_lineNumberInvalid);
	}
	return(lineNumber);
}


constexpr inline int16 Textbox::get_lineHeight() const
{
	return(m_font.get_height() + c_lineSpacing);
}


constexpr inline Color Textbox::get_colorBackground() const
{
	return(m_colorBackground);
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

constexpr inline void Textbox::set_function_onUpdate(f_element onUpdateFunction)
{
	m_function_onUpdate = onUpdateFunction;
}


constexpr inline void Textbox::set_function_onCallback(f_element onCallbackFunction)
{
	m_function_onCallback = onCallbackFunction;
}


constexpr inline void Textbox::set_function_onChangePage(f_element onChangePageFunction)
{
	m_function_onChangePage = onChangePageFunction;
}


constexpr inline void Textbox::set_function_onChangeLayer(f_element onChangeLayerFunction)
{
	m_function_onChangeLayer = onChangeLayerFunction;
}


constexpr inline void Textbox::set_function_onChangePosition(f_element onChangePositionFunction)
{
	m_function_onChangePosition = onChangePositionFunction;
}


constexpr inline void Textbox::set_function_onChangeSize(f_element onChangeSizeFunction)
{
	m_function_onChangeSize = onChangeSizeFunction;
}


constexpr inline Element::f_element Textbox::get_function_onUpdate() const
{
	return(m_function_onUpdate);
}


constexpr inline Element::f_element Textbox::get_function_onCallback() const
{
	return(m_function_onCallback);
}


constexpr inline Element::f_element Textbox::get_function_onChangePage() const
{
	return(m_function_onChangePage);
}


constexpr inline Element::f_element Textbox::get_function_onChangeLayer() const
{
	return(m_function_onChangeLayer);
}


constexpr inline Element::f_element Textbox::get_function_onChangePosition() const
{
	return(m_function_onChangePosition);
}


constexpr inline Element::f_element Textbox::get_function_onChangeSize() const
{
	return(m_function_onChangeSize);
}







constexpr inline uint32 Textbox::get_lineTouched() const
{
	for(uint32 i = 0; i < m_lines.get_size(); i++)
	{
		if(m_lines[i].isTouched == true)
		{
			return(i);
		}
	}
	return(c_lineNumberInvalid);
}


constexpr inline const Font& Textbox::get_font() const
{
	return(m_font);
}


inline const String& Textbox::get_text(uint32 lineNumber)
{
	return(m_lines[lineNumber].text);
}


inline bool Textbox::is_touchable(uint32 lineNumber) const
{
	return(m_lines[lineNumber].touchable);
}


inline Color Textbox::get_colorText(uint32 lineNumber) const
{
	return(m_lines[lineNumber].colorText);
}


inline Color Textbox::get_colorTextTouched(uint32 lineNumber) const
{
	return(m_lines[lineNumber].colorTextTouched);
}


inline Color Textbox::get_colorLine(uint32 lineNumber) const
{
	return(m_lines[lineNumber].colorLine);
}


inline Color Textbox::get_colorLineTouched(uint32 lineNumber) const
{
	return(m_lines[lineNumber].colorLineTouched);
}







inline feedback Textbox::set_text(const String& text, uint32 lineNumber)
{
	if(lineNumber < m_lines.get_size())
	{
		bool redrawNecessary = false;
		if(text != m_lines[lineNumber].text)
		{
			redrawNecessary = true;
		}
		m_lines[lineNumber].text = text;
		m_lines[lineNumber].redrawNecessary = redrawNecessary;
		
		
		//	Update Textbox only if the Line is visible to the User
		if(isLineInScrollingWindow(lineNumber) == true)
		{
			m_updateRequested = true;
		}
		return(OK);
	}
	return(FAIL);
}


inline feedback Textbox::set_lineTouchability(bool touchable)
{
	for(auto& i: m_lines)
	{
		i.touchable = touchable;
		if(touchable == false)
		{
			i.isTouched = false;
		}
		i.redrawNecessary = true;
	}
	m_updateRequested = true;
	return(OK);
}


inline feedback Textbox::set_lineTouchability(bool touchable, uint32 lineNumber)
{
	if(lineNumber < m_lines.get_size())
	{
		m_lines[lineNumber].touchable = touchable;
		if(touchable == false)
		{
			m_lines[lineNumber].isTouched = false;
		}
		m_lines[lineNumber].redrawNecessary = true;
		
		
		//	Update Textbox only if the Line is visible to the User
		if(isLineInScrollingWindow(lineNumber) == true)
		{
			m_updateRequested = true;
		}
		return(OK);
	}
	return(FAIL);
}


inline feedback Textbox::set_colorText(Color color)
{
	for(auto& i: m_lines)
	{
		i.colorText = color;
		i.redrawNecessary = true;
	}
	m_updateRequested = true;
	return(OK);
}


inline feedback Textbox::set_colorText(Color color, uint32 lineNumber)
{
	if(lineNumber < m_lines.get_size())
	{
		m_lines[lineNumber].colorText = color;
		m_lines[lineNumber].redrawNecessary = true;
		
		
		//	Update Textbox only if the Line is visible to the User
		if(isLineInScrollingWindow(lineNumber) == true)
		{
			m_updateRequested = true;
		}
		return(OK);
	}
	return(FAIL);
}


inline feedback Textbox::set_colorTextTouched(Color color)
{
	for(auto& i: m_lines)
	{
		i.colorTextTouched = color;
		i.redrawNecessary = true;
	}
	m_updateRequested = true;
	return(OK);
}


inline feedback Textbox::set_colorTextTouched(Color color, uint32 lineNumber)
{
	if(lineNumber < m_lines.get_size())
	{
		m_lines[lineNumber].colorTextTouched = color;
		m_lines[lineNumber].redrawNecessary = true;
		
		
		//	Update Textbox only if the Line is visible to the User
		if(isLineInScrollingWindow(lineNumber) == true)
		{
			m_updateRequested = true;
		}
		return(OK);
	}
	return(FAIL);
}


inline feedback Textbox::set_colorLine(Color color)
{
	for(auto& i: m_lines)
	{
		i.colorLine = color;
		i.redrawNecessary = true;
	}
	m_updateRequested = true;
	return(OK);
}


inline feedback Textbox::set_colorLine(Color color, uint32 lineNumber)
{
	if(lineNumber < m_lines.get_size())
	{
		m_lines[lineNumber].colorLine = color;
		m_lines[lineNumber].redrawNecessary = true;
		
		
		//	Update Textbox only if the Line is visible to the User
		if(isLineInScrollingWindow(lineNumber) == true)
		{
			m_updateRequested = true;
		}
		return(OK);
	}
	return(FAIL);
}


inline feedback Textbox::set_colorLineTouched(Color color)
{
	for(auto& i: m_lines)
	{
		i.colorLineTouched = color;
		i.redrawNecessary = true;
	}
	m_updateRequested = true;
	return(OK);
}


inline feedback Textbox::set_colorLineTouched(Color color, uint32 lineNumber)
{
	if(lineNumber < m_lines.get_size())
	{
		m_lines[lineNumber].colorLineTouched = color;
		m_lines[lineNumber].redrawNecessary = true;
		
		
		//	Update Textbox only if the Line is visible to the User
		if(isLineInScrollingWindow(lineNumber) == true)
		{
			m_updateRequested = true;
		}
		return(OK);
	}
	return(FAIL);
}


inline feedback Textbox::set_xOffset(int16 xOffset)
{
	for(auto& i: m_lines)
	{
		i.xOffset = xOffset;
		i.redrawNecessary = true;
	}
	m_updateRequested = true;
	return(OK);
}


inline feedback Textbox::set_xOffset(int16 xOffset, uint32 lineNumber)
{
	if(lineNumber < m_lines.get_size())
	{
		m_lines[lineNumber].xOffset = xOffset;
		m_lines[lineNumber].redrawNecessary = true;
		
		
		//	Update Textbox only if the Line is visible to the User
		if(isLineInScrollingWindow(lineNumber) == true)
		{
			m_updateRequested = true;
		}
		return(OK);
	}
	return(FAIL);
}


inline void Textbox::reset_touchFlags()
{
	for(auto& i: m_lines)
	{
		if(i.isTouched == true)
		{
			i.isTouched = false;
			i.redrawNecessary = true;
		}
	}
	m_updateRequested = true;
}


inline Textbox::e_event Textbox::get_event() const
{
	return(m_event);
}







inline feedback Textbox::add_line()
{
	return(set_numberOfLines(m_lines.get_size() + 1));
}


constexpr inline uint32 Textbox::get_numberOfLines() const
{
	return(m_lines.get_size());
}


constexpr inline float Textbox::get_scrollPosition() const
{
	const float lineHeight = get_lineHeight();
	return(m_scrollingInPixels / lineHeight);
}







constexpr inline void Textbox::clear()
{
	for(auto& i: m_lines)
	{
		i.text.erase();
		i.redrawNecessary = true;
	}
	m_updateRequested = true;
}


constexpr inline feedback Textbox::clear(uint32 lineNumber)
{
	if(lineNumber < m_lines.get_size())
	{
		m_lines[lineNumber].text.erase();
		m_lines[lineNumber].redrawNecessary = true;
		
		
		//	Update Textbox only if the Line is visible to the User
		if(isLineInScrollingWindow(lineNumber) == true)
		{
			m_updateRequested = true;
		}
		return(OK);
	}
	return(FAIL);
}