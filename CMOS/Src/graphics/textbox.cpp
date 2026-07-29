#include "../../Inc/graphics/graphics.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

CODE_RAM void Textbox::onUpdate(Element& element)
{
	Textbox& textbox = (Textbox&) element;
	
	
	//	Full rebuild
	if(textbox.isRebuildRequested() == true)
	{
		//	Draw background
		textbox.draw_background(textbox.m_colorBackground);
		
		
		//	Redraw all lines
		for(auto& i: textbox.m_lines)
		{
			i.redrawNecessary = true;
		}
	}
	
	
	//	Execute user update first to be able to display changes after
	if(textbox.m_function_onUpdate != nullptr)
	{
		textbox.m_function_onUpdate(element);
	}
	
	
	//	Draw all lines - they will only be drawn if the s_line.redrawNecessary flag is set
	for(uint32 i = 0; i < textbox.m_lines.get_size(); i++)
	{
		textbox.drawLine(i);
	}
	
	
	//	Draw frame after all lines - otherwise the line clearing would clear parts of the frame
	textbox.draw_frame(textbox.m_colorFrame);
}


CODE_RAM void Textbox::onCallback(Element& element)
{
	Textbox& textbox = (Textbox&) element;
	
	CMOS& cmos = CMOS::get();
	
	
	//	Save position of first touch contact so that we can later decide, if the user wanted to scroll or select a line
	if(m_touchEvent != I_TouchController::e_touchEvent::TOUCH)
	{
		return;
	}
	const Vec2 touchPositionStart = textbox.m_touchPosition;
	
	
	//	Scrolling animation
	bool isScrolling = false;
	const uint32 scrollingInPixelsAtStart = textbox.m_scrollingInPixels;
	while(m_touchEvent != I_TouchController::e_touchEvent::RELEASE)
	{
		const Vec2 touchPositionNow = textbox.m_touchPosition;
		
		
		//	Determine if the user is scrolling
		const Vec2 touchMovement = touchPositionNow - touchPositionStart;
		if(isScrolling == false)
		{
			if(touchMovement.absolute() > textbox.get_lineHeight())
			{
				isScrolling = true;
			}
			cmos.sleep_ms(10);
		}
		else
		{
			if(textbox.m_lines.get_size() > 1 && textbox.m_lines.get_size() > (uint32) (textbox.size.y - 2) / textbox.get_lineHeight())
			{
				//	Scroll only if there are enough lines to scroll
				
				
				//	Calculate new scrolling position
				textbox.m_scrollingInPixels = scrollingInPixelsAtStart + touchMovement.y;
				
				
				//	Dont allow to scroll above line 0
				if(textbox.m_scrollingInPixels < 0)
				{
					textbox.m_scrollingInPixels = 0;
				}
				
				
				//	Dont allow to scroll below last line
				const uint32 scrollingInLines = textbox.m_scrollingInPixels / textbox.get_lineHeight();
				uint32 numberOfDisplayableLines = (textbox.size.y - 2) / textbox.get_lineHeight();
				if(textbox.size.y - 2 % textbox.get_lineHeight() != 0)
				{
					numberOfDisplayableLines++;
				}
				
				if(scrollingInLines >= textbox.m_lines.get_size() - numberOfDisplayableLines)
				{
					bool decrementing = false;
					while(textbox.isLineInScrollingWindow(textbox.m_lines.get_size() - 2) == false)
					{
						textbox.m_scrollingInPixels--;
						decrementing = true;
					}
					if(decrementing == true)
					{
						textbox.m_scrollingInPixels++;
					}
				}
				
				
				//	Update all lines
				for(auto& i: textbox.m_lines)
				{
					i.redrawNecessary = true;
				}
				textbox.requestRebuild();
				
				
				//	Unlock semaphore so that an update can take place
				cmos.semaphore_unlock(&element);
				cmos.sleep_ms(10);
				cmos.semaphore_lock(&element);
			}
			else
			{
				//	If there are not enough lines to scroll, simply wait for the user to release the touch
				cmos.sleep_ms(10);
			}
		}
	}
	
	
	//	Check for line selection
	if(isScrolling == false)
	{
		//	Save old touched state of the line
		bool wasLineTouched = false;
		const uint32 lineSelected = textbox.get_line(textbox.m_touchPosition.y);
		if(lineSelected < textbox.m_lines.get_size())
		{
			s_line& line = textbox.m_lines[lineSelected];
			if(line.touchable == true)
			{
				wasLineTouched = line.isTouched;
			}
		}
		
		
		//	Update every line that is marked as touched right now
		for(auto& i: textbox.m_lines)
		{
			if(i.isTouched == true)
			{
				i.isTouched = false;
				i.redrawNecessary = true;
			}
		}
		
		
		//	Update newly touched line
		if(lineSelected < textbox.m_lines.get_size())
		{
			s_line& line = textbox.m_lines[lineSelected];
			if(line.touchable == true)
			{
				line.isTouched = !wasLineTouched;
				line.redrawNecessary = true;
			}
		}
	}
	
	
	//	Set event for subsequent callbacks
	if(isScrolling == false)
	{
		textbox.m_event = e_event::LINE_SELECTED;
	}
	else
	{
		textbox.m_event = e_event::SCROLLING;
	}
	
	
	//	Execute user callback
	if(textbox.m_function_onCallback != nullptr && textbox.get_lineTouched() < textbox.get_numberOfLines())
	{
		textbox.m_function_onCallback(element);
		textbox.requestUpdate();
	}
}


CODE_RAM void Textbox::onChangeMyPage(Element& element)
{
	
}


CODE_RAM void Textbox::onChangeShape(Element& element)
{
	Textbox& textbox = (Textbox&) element;
	
	
	//	Rebuild the whole textbox
	textbox.requestRebuild();
}


CODE_RAM void Textbox::onChangeCurrentPage(Element& element)
{
	
}



/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/

CODE_RAM void Textbox::drawLine(uint32 lineNumber)
{
	//	Boundary check
	if(lineNumber >= m_lines.get_size())
	{
		return;
	}
	
	
	//	Dont draw line if its not necessary or its not visible to the user
	if(m_lines[lineNumber].redrawNecessary == false || isLineInScrollingWindow(lineNumber) == false)
	{
		return;
	}
	
	
	//	Determine the colors in which the line will be drawn
	s_line& line = m_lines[lineNumber];
	
	Color colorLineBackground = line.colorLine;
	Color colorText = line.colorText;
	if(line.isTouched == true)
	{
		colorLineBackground = line.colorLineTouched;
		colorText = line.colorTextTouched;
	}
	
	
	//	If text stayed the same, we need to redraw the whole line because something else changed (like scrolling, etc...)
	const int16 y = get_lineCoordinate(lineNumber);
	if(line.text == line.displayed.text || m_scrollingInPixels != line.displayed.scrollingInPixels)
	{
		//	Draw rectangle
		Rectangle lineRectangle;
		lineRectangle.position = Vec2(1 + line.xOffset, y);
		lineRectangle.size = Vec2(size.x - 2 - line.xOffset, get_lineHeight());
		draw_rectangleFilled(lineRectangle, colorLineBackground);
		
		
		//	Draw text
		const Vec2 bottomLeftPosition(get_align((e_align) e_align_x::LEFT, line.text, m_font, defaultTextDistanceFromBorder).x + line.xOffset, y - m_font.get_minimumPositionY());
		draw_string(line.text, bottomLeftPosition, m_font, colorText);
	}
	else
	{
		//	If text changed, we just draw the old text in background color and then draw the new text
		//	This way we reduce the number of memory accesses and therefore the time needed to draw
		const int16 scrollingDifference = line.displayed.scrollingInPixels - m_scrollingInPixels;
		const Vec2 bottomLeftPositionNew(get_align((e_align) e_align_x::LEFT, line.text, m_font, defaultTextDistanceFromBorder).x + line.xOffset, y - m_font.get_minimumPositionY());
		const Vec2 bottomLeftPositionOld(bottomLeftPositionNew.x, bottomLeftPositionNew.y + scrollingDifference);
		
		draw_string(line.displayed.text, bottomLeftPositionOld, m_font, colorLineBackground);
		draw_string(line.text, bottomLeftPositionNew, m_font, colorText);
	}
	
	
	//	Update line state
	line.displayed.colorLine = colorLineBackground;
	line.displayed.text = line.text;
	line.displayed.scrollingInPixels = m_scrollingInPixels;
	line.redrawNecessary = false;
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

Textbox::Textbox(Element element, const Font& font)
	:	Element(element),
		m_lines(),
		m_font(font),
		m_colorBackground(Colors::black),
		m_colorFrame(Colors::silver),
		m_scrollingInPixels(0),
		m_event(e_event::NONE),
		
		m_function_onUpdate(Element::get_function_onUpdate()),
		m_function_onCallback(Element::get_function_onCallback()),
		m_function_onChangeMyPage(Element::get_function_onChangeMyPage()),
		m_function_onChangeShape(Element::get_function_onChangeShape())
{
	Element::set_function_onUpdate(onUpdate);
	Element::set_function_onCallback(onCallback);
	Element::set_function_onChangeMyPage(onChangeMyPage);
	Element::set_function_onChangeShape(onChangeShape);
	Element::set_function_onChangeCurrentPage(onChangeCurrentPage);
}


Textbox::~Textbox()
{
	
}







feedback Textbox::erase_line(uint32 lineNumber)
{
	//	No more lines to erase
	if(m_lines.get_size() == 0)
	{
		return(FAIL);
	}
	
	
	//	Default case: erase last line
	if(lineNumber == c_lineNumberInvalid)
	{
		lineNumber = m_lines.get_size() - 1;
	}
	
	
	//	Erase line
	if(m_lines.erase(lineNumber) != OK)
	{
		return(FAIL);
	}
	
	
	requestRebuild();
	return(OK);
}


feedback Textbox::set_numberOfLines(uint32 numberOfLines)
{
	const uint32 numberOfLinesOld = m_lines.get_size();
	if(numberOfLines == numberOfLinesOld)
	{
		return(OK);
	}
	
	
	//	Add new lines
	if(numberOfLines > numberOfLinesOld)
	{
		s_line line;
		line.text 						= String();
		line.colorLine				= Colors::black;
		line.colorLineTouched	= Colors::white;
		line.colorText				= Colors::white;
		line.colorTextTouched	= Colors::black;
		line.xOffset					= 0;
		line.touchable				= true;
		line.isTouched				= false;
		line.redrawNecessary	= true;
		
		m_lines += Array<s_line>(line, numberOfLines - numberOfLinesOld);
		requestRebuild();
		return(OK);
	}
	
	
	//	Erase some lines
	if(numberOfLines < numberOfLinesOld)
	{
		m_lines.eraseFromEnd(numberOfLinesOld - numberOfLines);
		requestRebuild();
		return(OK);
	}
	return(FAIL);
}