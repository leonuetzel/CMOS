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
	
	
	//	Full Rebuild
	if(textbox.m_rebuildRequested == true)
	{
		//	Draw Background and Frame first
		textbox.draw_background(textbox.m_colorBackground);
		textbox.draw_frame(textbox.m_colorFrame);
		
		
		//	Draw Frame
		e_frameType frameType = textbox.get_frameType();
		if(frameType == e_frameType::ROUND || frameType == e_frameType::ROUNDED)
		{
			textbox.draw_frame(textbox.m_colorFrame);
		}
		
		
		//	Redraw all Lines
		for(auto& i: textbox.m_lines)
		{
			i.redrawNecessary = true;
		}
		for(uint32 i = 0; i < textbox.m_lines.get_size(); i++)
		{
			textbox.drawLine(i);
		}
		textbox.m_updateRequested = true;
	}
	
	
	//	Execute User Update first to be able to display Changes after
	if(textbox.m_function_onUpdate != nullptr)
	{
		textbox.m_function_onUpdate(element);
	}
	
	
	//	Normal Update
	if(textbox.m_updateRequested == true)
	{
		//	Draw all Lines - they will only be drawn if the s_line.redrawNecessary Flag is set
		for(uint32 i = 0; i < textbox.m_lines.get_size(); i++)
		{
			textbox.drawLine(i);
		}
		textbox.draw_frame(textbox.m_colorFrame);
	}
}


CODE_RAM void Textbox::onCallback(Element& element)
{
	Textbox& textbox = (Textbox&) element;
	
	CMOS& cmos = CMOS::get();
	
	
	//	Save Position of first Touch Contact so that we can later decide, if the User wanted to scroll or select a Line
	if(m_touchEvent != Graphics::e_touchEvent::TOUCH)
	{
		return;
	}
	const Vec2 touchPositionStart = textbox.m_touchPosition;
	
	
	//	Scrolling Animation
	bool isScrolling = false;
	const uint32 scrollingInPixelsAtStart = textbox.m_scrollingInPixels;
	while(m_touchEvent != Graphics::e_touchEvent::RELEASE)
	{
		const Vec2 touchPositionNow = textbox.m_touchPosition;
		
		
		//	Determine if the User is scrolling
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
				//	Scroll only if there are enough Lines to scroll
				
				
				//	Calculate new scrolling Position
				textbox.m_scrollingInPixels = scrollingInPixelsAtStart + touchMovement.y;
				
				
				//	Dont allow to scroll above Line 0
				if(textbox.m_scrollingInPixels < 0)
				{
					textbox.m_scrollingInPixels = 0;
				}
				
				
				//	Dont allow to scroll below last Line
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
				
				
				//	Update all Lines
				for(auto& i: textbox.m_lines)
				{
					i.redrawNecessary = true;
				}
				textbox.m_rebuildRequested = true;
				
				
				//	Unlock Semaphore so that an Update can take Place
				cmos.semaphore_unlock(&element);
				cmos.sleep_ms(10);
				cmos.semaphore_lock(&element);
			}
			else
			{
				//	If there are not enough Lines to scroll, simply wait for the User to release the Touch
				cmos.sleep_ms(10);
			}
		}
	}
	
	
	//	Check for Line Selection
	if(isScrolling == false)
	{
		//	Save old touched State of the Line
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
		
		
		//	Update every Line that is marked as touched right now
		for(auto& i: textbox.m_lines)
		{
			if(i.isTouched == true)
			{
				i.isTouched = false;
				i.redrawNecessary = true;
			}
		}
		
		
		//	Update newly touched Line
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
	
	
	//	Set Event for subsequent Callbacks
	if(isScrolling == false)
	{
		textbox.m_event = e_event::LINE_SELECTED;
	}
	else
	{
		textbox.m_event = e_event::SCROLLING;
	}
	
	
	//	Execute User Callback
	if(textbox.m_function_onCallback != nullptr && textbox.get_lineTouched() < textbox.get_numberOfLines())
	{
		textbox.m_function_onCallback(element);
		textbox.m_updateRequested = true;
	}
}


CODE_RAM void Textbox::onChangePage(Element& element)
{
	
}


CODE_RAM void Textbox::onChangeLayer(Element& element)
{
	
}


CODE_RAM void Textbox::onChangePosition(Element& element)
{
	
}


CODE_RAM void Textbox::onChangeSize(Element& element)
{
	Textbox& textbox = (Textbox&) element;
	
	
	//	Rebuild the whole Textbox
	textbox.m_rebuildRequested = true;
}



/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/

CODE_RAM void Textbox::drawLine(uint32 lineNumber)
{
	//	Boundary Check
	if(lineNumber >= m_lines.get_size())
	{
		return;
	}
	
	
	//	Dont draw Line if its not necessary or its not visible to the User
	if(m_lines[lineNumber].redrawNecessary == false || isLineInScrollingWindow(lineNumber) == false)
	{
		return;
	}
	
	
	//	Determine the Colors in which the Line will be drawn
	s_line& line = m_lines[lineNumber];
	
	Color colorLineBackground = line.colorLine;
	Color colorText = line.colorText;
	if(line.isTouched == true)
	{
		colorLineBackground = line.colorLineTouched;
		colorText = line.colorTextTouched;
	}
	
	
	//	If Text stayed the same, we need to redraw the whole Line because something else changed (like scrolling, etc...)
	const int16 y = get_lineCoordinate(lineNumber);
	if(line.text == line.displayed.text || m_scrollingInPixels != line.displayed.scrollingInPixels)
	{
		//	Draw Rectangle
		Rectangle lineRectangle;
		lineRectangle.position = Vec2(1 + line.xOffset, y);
		lineRectangle.size = Vec2(size.x - 2 - line.xOffset, get_lineHeight());
		draw_rectangleFilled(lineRectangle, colorLineBackground);
		
		
		//	Draw Text
		const Vec2 bottomLeftPosition(get_align(size, (e_align) e_align_x::LEFT, line.text.get_size(), m_font).x + line.xOffset, y);
		draw_string(line.text, bottomLeftPosition, m_font, colorText);
	}
	else
	{
		//	If Text changed, we just draw the old Text in Background Color and then draw the new Text
		//	This Way we reduce the Number of Memory Accesses and therefore the Time needed to draw
		const int16 scrollingDifference = line.displayed.scrollingInPixels - m_scrollingInPixels;
		const Vec2 bottomLeftPositionNew(get_align(size, (e_align) e_align_x::LEFT, line.text.get_size(), m_font).x + line.xOffset, y);
		const Vec2 bottomLeftPositionOld(bottomLeftPositionNew.x, bottomLeftPositionNew.y + scrollingDifference);
		
		draw_string(line.displayed.text, bottomLeftPositionOld, m_font, colorLineBackground);
		draw_string(line.text, bottomLeftPositionNew, m_font, colorText);
	}
	
	
	//	Update Line State
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
		m_function_onChangePage(Element::get_function_onChangePage()),
		m_function_onChangeLayer(Element::get_function_onChangeLayer()),
		m_function_onChangePosition(Element::get_function_onChangePosition()),
		m_function_onChangeSize(Element::get_function_onChangeSize())
{
	Element::set_function_onUpdate(onUpdate);
	Element::set_function_onCallback(onCallback);
	Element::set_function_onChangePage(onChangePage);
	Element::set_function_onChangeLayer(onChangeLayer);
	Element::set_function_onChangePosition(onChangePosition);
	Element::set_function_onChangeSize(onChangeSize);
}


Textbox::~Textbox()
{
	
}







feedback Textbox::erase_line(uint32 lineNumber)
{
	//	No more Lines to erase
	if(m_lines.get_size() == 0)
	{
		return(FAIL);
	}
	
	
	//	Default Case: Erase last Line
	if(lineNumber == c_lineNumberInvalid)
	{
		lineNumber = m_lines.get_size() - 1;
	}
	
	
	//	Erase Line
	if(m_lines.erase(lineNumber) != OK)
	{
		return(FAIL);
	}
	
	
	m_rebuildRequested = true;
	return(OK);
}


feedback Textbox::set_numberOfLines(uint32 numberOfLines)
{
	const uint32 numberOfLinesOld = m_lines.get_size();
	if(numberOfLines == numberOfLinesOld)
	{
		return(OK);
	}
	
	
	//	Add new Lines
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
		m_rebuildRequested = true;
		return(OK);
	}
	
	
	//	Erase some Lines
	if(numberOfLines < numberOfLinesOld)
	{
		m_lines.eraseFromEnd(numberOfLinesOld - numberOfLines);
		m_rebuildRequested = true;
		return(OK);
	}
	return(FAIL);
}