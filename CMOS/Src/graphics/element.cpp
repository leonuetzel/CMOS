#include "../../Inc/graphics/graphics.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/

I_GraphicAccelerator* Element::m_graphicAccelerator;
I_TouchController::e_touchEvent Element::m_touchEvent;



/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

CODE_RAM void Element::onUpdate()
{
	if(m_visible == false)
	{
		return;
	}
	
	
	//	Lock semaphore of this element to prevent data altering by callback function
	CMOS& cmos = CMOS::get();
	if(cmos.semaphore_lock(this) != OK)
	{
		return;
	}
	
	
	//	Call update function if it is set
	if(m_function_onUpdate != nullptr)
	{
		m_function_onUpdate(*this);
	}
	
	
	//	Clear rebuild request flag
	m_rebuildRequested = false;
	
	
	//	Unlock semaphore
	cmos.semaphore_unlock(this);
}


CODE_RAM void Element::onCallback()
{
	//	Lock semaphore of this element to prevent data altering by update function
	CMOS& cmos = CMOS::get();
	if(cmos.semaphore_lock(this) != OK)
	{
		return;
	}
	
	
	//	Do callback stuff
	if(m_function_onCallback != nullptr)
	{
		m_function_onCallback(*this);
	}
	
	
	//	Unlock semaphore
	cmos.semaphore_unlock(this);
}


CODE_RAM void Element::onChangeCurrentPage()
{
	if(m_function_onChangeCurrentPage != nullptr)
	{
		m_function_onChangeCurrentPage(*this);
	}
}







CODE_RAM feedback Element::draw_rectangleFilledManual(Rect rectangle, Color color)
{
	if(rectangle.size.x <= 0 || rectangle.size.y <= 0)
	{
		return(FAIL);
	}
	
	for(int32 y = 0; y < rectangle.size.y; y++)
	{
		for(int32 x = 0; x < rectangle.size.x; x++)
		{
			const Vec2 pixel(rectangle.position.x + x, rectangle.position.y + y);
			set_pixel(pixel, color);
		}
	}
	return(OK);
}


CODE_RAM void Element::syncFramebuffers()
{
	//	Clone backbuffer if necessary
	if(m_areBothFramebuffersIdentical == false)
	{
		const RectGraphic backBuffer(m_backBufferShape, m_backBufferData[m_backbufferIndex]);
		const RectGraphic frontBuffer(m_backBufferShape, m_backBufferData[1 - m_backbufferIndex]);
		
		
		//	Lock semaphore for this element to prevent data altering by update and callback function
		CMOS& cmos = CMOS::get();
		if(cmos.semaphore_lock(this) != OK)
		{
			return;
		}
		
		
		//	Copy the element's visible buffer to the backbuffer
		m_graphicAccelerator->copyForegroundRectangleToOutputPosition(backBuffer, frontBuffer, *this, position);
		
		
		//	Unlock semaphore
		cmos.semaphore_unlock(this);
		
		
		//	Reset flag
		m_areBothFramebuffersIdentical = true;
	}
}


CODE_RAM feedback Element::clearFromBothFramebuffers()
{
	//	Lock semaphore for this element to prevent data altering by update and callback function
	CMOS& cmos = CMOS::get();
	if(cmos.semaphore_isOwnedByRunningThread(this) != true)
	{
		return(FAIL);
	}
	
	
	//	Clear the element's area on both framebuffers
	for(auto& i: m_backBufferData)
	{
		const RectGraphic backbuffer(m_backBufferShape, i);
		m_graphicAccelerator->drawfilledRectangleWithSingleColor(backbuffer, Colors::transparent, *this);
	}
	
	
	//	Unlock semaphore
	cmos.semaphore_unlock(this);
	return(OK);
}



/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/

CODE_RAM bool Element::containsPoint(Vec2 point)
{
	Vec2 center(size / 2);
	
	switch(m_frameType)
	{
		case e_frameType::RECTANGLE:
			return(contains(point));
			
		case e_frameType::ROUND:
			if(size.x == size.y)
			{
				if(Vec2(point - position - center).absolute() > center.x + 1)
				{
					return(false);
				}
				else
				{
					return(true);
				}
			}
			if(size.x > size.y)
			{
				Vec2 pointCleaned = point - position;
				int16 y_div2 = size.y / 2;
				
				if(pointCleaned.x >= y_div2 && pointCleaned.x <= size.x - 1 - y_div2)
				{
					if(pointCleaned.y >= 0 && pointCleaned.y < size.y)
					{
						return(true);
					}
					else
					{
						return(false);
					}
				}
				if(pointCleaned.x < y_div2)
				{
					Vec2 center(y_div2, y_div2);
					Vec2 diff(pointCleaned - center);
					if(diff.absolute() > y_div2 + 1)
					{
						return(false);
					}
					else
					{
						return(true);
					}
				}
				if(pointCleaned.x > size.x - 1 - y_div2)
				{
					Vec2 center(size.x - 1 - y_div2, y_div2);
					Vec2 diff(pointCleaned - center);
					if(diff.absolute() > y_div2 + 1)
					{
						return(false);
					}
					else
					{
						return(true);
					}
				}
			}
			if(size.x < size.y)
			{
				Vec2 pointCleaned = point - position;
				int16 x_div2 = size.x / 2;
				
				if(pointCleaned.y >= x_div2 && pointCleaned.y <= size.y - 1 - x_div2)
				{
					if(pointCleaned.x >= 0 && pointCleaned.x < size.x)
					{
						return(true);
					}
					else
					{
						return(false);
					}
				}
				if(pointCleaned.y < x_div2)
				{
					Vec2 center(x_div2, x_div2);
					Vec2 diff(pointCleaned - center);
					if(diff.absolute() > x_div2 + 1)
					{
						return(false);
					}
					else
					{
						return(true);
					}
				}
				if(pointCleaned.y > size.y - 1 - x_div2)
				{
					Vec2 center(x_div2, size.y - 1 - x_div2);
					Vec2 diff(pointCleaned - center);
					if(diff.absolute() > x_div2 + 1)
					{
						return(false);
					}
					else
					{
						return(true);
					}
				}
			}
			return(false);
			
		case e_frameType::ROUNDED:
			{
				Vec2 pointCleaned = point - position;
				
				if(contains(point) == false)
				{
					return(false);
				}
				
				if(pointCleaned.x < c_frameRoundness && pointCleaned.y < c_frameRoundness)
				{
					Vec2 center(c_frameRoundness, c_frameRoundness);
					Vec2 diff(pointCleaned - center);
					if((int16) diff.absolute() > c_frameRoundness)
					{
						return(false);
					}
					else
					{
						return(true);
					}
				}
				
				if(pointCleaned.x > size.x - 1 - c_frameRoundness && pointCleaned.y < c_frameRoundness)
				{
					Vec2 center(size.x - 1 - c_frameRoundness, c_frameRoundness);
					Vec2 diff(pointCleaned - center);
					if((int16) diff.absolute() > c_frameRoundness)
					{
						return(false);
					}
					else
					{
						return(true);
					}
				}
				
				if(pointCleaned.x < c_frameRoundness && pointCleaned.y > size.y - 1 - c_frameRoundness)
				{
					Vec2 center(c_frameRoundness, size.y - 1 - c_frameRoundness);
					Vec2 diff(pointCleaned - center);
					if((int16) diff.absolute() > c_frameRoundness)
					{
						return(false);
					}
					else
					{
						return(true);
					}
				}
				
				if(pointCleaned.x > size.x - 1 - c_frameRoundness && pointCleaned.y > size.y - 1 - c_frameRoundness)
				{
					Vec2 center(size.x - 1 - c_frameRoundness, size.y - 1 - c_frameRoundness);
					Vec2 diff(pointCleaned - center);
					if((int16) diff.absolute() > c_frameRoundness)
					{
						return(false);
					}
					else
					{
						return(true);
					}
				}
				
				return(true);
			}
			
		case e_frameType::NONE:
			return(contains(point));
			
		default:
			return(false);
	}
	
	return(false);
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

CODE_RAM Element::Element(Rect shape, uint8 page, uint32 updatePeriodInFrames, f_element onUpdateFunction)
	:	Rectangle(shape),
		m_page(page),
		m_updatePeriodInFrames(updatePeriodInFrames),
		m_function_onUpdate(onUpdateFunction),
		
		m_visible(true),
		m_touchable(true),
		m_function_onCallback(nullptr),
		m_function_onChangeMyPage(nullptr),
		m_function_onChangeShape(nullptr),
		m_function_onChangeCurrentPage(nullptr),
		
		m_backBufferShape(Graphics::get().m_frameBufferShape),
		m_backBufferData(),
		m_backbufferIndex(Graphics::get().m_frameBufferIndex),
		
		m_frameCounterAtLastUpdate(0),
		m_areBothFramebuffersIdentical(true),
		
		m_frameType(e_frameType::NONE),
		m_touchPosition(Vec2(0, 0)),
		m_touchValid(false)
{
	Graphics& graphics = Graphics::get();
	for(uint32 i = 0; i < 2; i++)
	{
		m_backBufferData[i] = graphics.m_frameBufferData[i];
	}
}


CODE_RAM Element::~Element()
{
	Graphics::get() -= this;
}







CODE_RAM feedback Element::set_page(uint32 newPage)
{
	Graphics& graphics = Graphics::get();
	if(newPage >= graphics.get_numberOfPages())
	{
		return(FAIL);
	}
	
	
	//	Check overlap with other elements on the new page
	for(auto& i: graphics.m_elements)
	{
		if(i != this)
		{
			if(i->m_page == newPage)
			{
				if(doesOverlap(*i) == true)
				{
					return(FAIL);
				}
			}
		}
	}
	
	
	//	Lock Semaphore for this Element
	CMOS& cmos = CMOS::get();
	if(cmos.semaphore_lock(this) != OK)
	{
		return(FAIL);
	}
	
	
	//	Set new Page and call "onChangePage" function
	m_page = newPage;
	if(m_function_onChangeMyPage != nullptr)
	{
		m_function_onChangeMyPage(*this);
	}
	
	
	//	Unlock semaphore
	return(cmos.semaphore_unlock(this));
}


CODE_RAM feedback Element::set_position(Vec2 newPosition)
{
	Graphics& graphics = Graphics::get();
	
	//	Check display borders
	if(newPosition.x < 0 || newPosition.y < 0)
	{
		return(FAIL);
	}
	
	const Rect newRect(newPosition, size);
	const Vec2 topRight(newRect.get_topRightCorner());
	if(topRight.x > m_backBufferShape.size.x || topRight.y > m_backBufferShape.size.y)
	{
		return(FAIL);
	}
	
	
	//	Check overlap with other elements
	for(auto& i: graphics.m_elements)
	{
		if(i != this)
		{
			if(i->m_page == m_page)
			{
				if(newRect.doesOverlap(*i) == true)
				{
					return(FAIL);
				}
			}
		}
	}
	
	
	//	Lock semaphore for this element
	CMOS& cmos = CMOS::get();
	if(cmos.semaphore_lock(this) != OK)
	{
		return(FAIL);
	}
	
	
	//	Set new position and call "onChangeShape" function
	if(m_page == graphics.get_currentPage())
	{
		position = newPosition;
	}
	if(m_function_onChangeShape != nullptr)
	{
		m_function_onChangeShape(*this);
	}
	
	
	//	Unlock Semaphore
	return(cmos.semaphore_unlock(this));
}


CODE_RAM feedback Element::set_size(Vec2 newSize)
{
	Graphics& graphics = Graphics::get();
	
	//	Check display borders
	const Rect newRect(position, newSize);
	const Vec2 topRight(newRect.get_topRightCorner());
	if(topRight.x > m_backBufferShape.size.x || topRight.y > m_backBufferShape.size.y)
	{
		return(FAIL);
	}
	
	
	//	Check minimum dimensions
	if(newSize.x < c_minimumSideLength || newSize.y < c_minimumSideLength)
	{
		return(FAIL);
	}
	
	
	//	Check overlap with other elements
	for(auto& i: graphics.m_elements)
	{
		if(i != this)
		{
			if(i->m_page == m_page)
			{
				if(newRect.doesOverlap(*i) == true)
				{
					return(FAIL);
				}
			}
		}
	}
	
	
	//	Lock semaphore for this element
	CMOS& cmos = CMOS::get();
	if(cmos.semaphore_lock(this) != OK)
	{
		return(FAIL);
	}
	
	
	//	Set new size and call "onChangeShape" function
	if(m_page == graphics.get_currentPage())
	{
		size = newSize;
	}
	if(m_function_onChangeShape != nullptr)
	{
		m_function_onChangeShape(*this);
	}
	
	
	//	Unlock semaphore
	return(cmos.semaphore_unlock(this));
}







CODE_RAM Vec2 Element::get_align(e_align align, const String& string, const Font& font, int16 distanceFromBorder)
{
	Vec2 bottomLeftPosition(0, 0);
	
	Vec2 stringBox = get_stringBox(string, font);
	
	uint8 x_align =  ((uint32) align) & 0x3;
	uint8 y_align = (((uint32) align) & 0xC) >> 2;
	
	
	
	switch(x_align)
	{
		case 0:
		{
			bottomLeftPosition.x = distanceFromBorder;
		}
		break;
		
		case 1:
		{
			bottomLeftPosition.x = (size.x - stringBox.x) / 2;
		}
		break;
		
		case 2:
		{
			bottomLeftPosition.x = size.x - 1 - distanceFromBorder - stringBox.x;
		}
		break;
		
		default:
		{
			bottomLeftPosition.x = 0;
		}
		break;
	}
	
	
	
	switch(y_align)
	{
		case 0:
		{
			bottomLeftPosition.y = distanceFromBorder;
		}
		break;
		
		case 1:
		{
			bottomLeftPosition.y = (size.y - stringBox.y) / 2;
			
			//	Shift the text a bit up to account for empty space generated by the font's baseline
			bottomLeftPosition.y += font.get_height() / 5;
		}
		break;
		
		case 2:
		{
			bottomLeftPosition.y = size.y - 1 - stringBox.y - distanceFromBorder;
		}
		break;
		
		default:
		{
			bottomLeftPosition.y = 0;
		}
		break;
	}
	
	
	return(bottomLeftPosition);
}







Vec2 Element::get_stringBox(const String& string, const Font& font, bool multiLine, int16 distanceFromBorder)
{
	if(multiLine == false)
	{
		//	We need to read the advance width of every character in the string
		Vec2 stringBox(0, font.get_height());
		for(auto& i: string)
		{
			stringBox.x += font.get_advanceWidth(i);
		}
		return(stringBox);
	}
	
	
	//	Multi line support
	//	We need to read the advance width of every character in the string until the element size is reached
	//	So we now calculate the line width for every line
	Array<int16> lineWidth(0);
	for(auto& i: string)
	{
		if(font.isPrintable(i) == true)
		{
			//	Calculate the Line Width with one more Character
			int16& lineWidthActual = lineWidth[lineWidth.get_size() - 1];
			const int16 characterWidth = font.get_advanceWidth(i);
			const int16 lineWidthNew = lineWidthActual + characterWidth;
			
			
			//	Check if the new line width is too long
			if(lineWidthNew > size.x - 2 * distanceFromBorder)
			{
				//	New line width is too long
				//	Save the old line width and start a new line
				lineWidth += characterWidth;
			}
			else
			{
				//	New line width fits into the element
				//	Save the new line width
				lineWidthActual = lineWidthNew;
			}
		}
	}
	
	//	Now we have the line width for every line
	//	The string box is the maximum line width and the number of lines times the font height
	Vec2 stringBox(0, lineWidth.get_size() * font.get_height());
	for(uint32 i = 0; i < lineWidth.get_size(); i++)
	{
		if(lineWidth[i] > stringBox.x)
		{
			stringBox.x = lineWidth[i];
		}
	}
	return(stringBox);
}







CODE_RAM feedback Element::set_pixel(Vec2 pixelPosition, Color color)
{
	//	Check if backbuffer is available
	if(m_backBufferData[m_backbufferIndex] == nullptr)
	{
		return(FAIL);
	}
	
	
	//	Dont set pixel if the element is not on the current page
	static Graphics& graphics = Graphics::get();
	if(m_page != graphics.get_currentPage())
	{
		return(FAIL);
	}
	
	
	//	Check if pixel is inside the element
	const Vec2 positionAbsolute(position + pixelPosition);
	if(containsPoint(positionAbsolute) == false)
	{
		return(FAIL);
	}
	
	
	//	Choose the correct backbuffer
	Color* const pixel = m_backBufferData[m_backbufferIndex] + ((m_backBufferShape.size.y - positionAbsolute.y - 1) * m_backBufferShape.size.x + positionAbsolute.x);
	*pixel = color;
	m_areBothFramebuffersIdentical = false;
	return(OK);
}


CODE_RAM void Element::draw_frame(Color color)
{
	switch(m_frameType)
	{
		case e_frameType::RECTANGLE:
		{
			draw_rectangle(Rectangle(0, 0, size.x, size.y), color);
		}
		break;
			
		case e_frameType::ROUND:
		{
			if(size.x == size.y)
			{
				draw_circle(size / 2, size.x / 2 - 1, color);
			}
			if(size.x > size.y)
			{
				int16 y_div2 = size.y / 2;
				for(uint32 i = 0; (int16) i < size.x - size.y; i++)
				{
					int16 x = y_div2 + i;
					set_pixel(Vec2(x, 0), 					color);
					set_pixel(Vec2(x, size.y - 1), color);
				}
				Vec2 center_1(y_div2, y_div2);
				Vec2 center_2(size.x - 1 - y_div2, y_div2);
				draw_circle(center_1, y_div2 - 1, color, 90,	181);
				draw_circle(center_2, y_div2 - 1, color, 270,	181);
			}
			if(size.x < size.y)
			{
				int16 x_div2 = size.x / 2;
				for(uint32 i = 0; (int16) i < size.y - size.x; i++)
				{
					int16 y = x_div2 + i;
					set_pixel(Vec2(0,					y),	color);
					set_pixel(Vec2(size.x - 1,	y), color);
				}
				Vec2 center_1(x_div2, x_div2);
				Vec2 center_2(x_div2, size.y - 1 - x_div2);
				draw_circle(center_1, x_div2 - 1, color, 180,	181);
				draw_circle(center_2, x_div2 - 1, color, 0,		181);
			}
		}
		break;
			
		case e_frameType::ROUNDED:
		{
			for(uint32 i = c_frameRoundness; (int16) i < size.x - 1 - c_frameRoundness; i++)
			{
				set_pixel(Vec2(i, 0), color);
				set_pixel(Vec2(i, size.y - 1), color);
			}
			for(uint32 i = c_frameRoundness; (int16) i < size.y - 1 - c_frameRoundness; i++)
			{
				set_pixel(Vec2(0, i), color);
				set_pixel(Vec2(size.x - 1, i), color);
			}
			draw_circle(Vec2(size.x - 1 - c_frameRoundness,	size.y - 1 - c_frameRoundness), c_frameRoundness, color, 0, 	91);
			draw_circle(Vec2(c_frameRoundness,								size.y - 1 - c_frameRoundness), c_frameRoundness, color, 90, 	91);
			draw_circle(Vec2(c_frameRoundness,								c_frameRoundness),							c_frameRoundness, color, 180, 91);
			draw_circle(Vec2(size.x - 1 - c_frameRoundness,	c_frameRoundness),							c_frameRoundness, color, 270, 91);
		}break;
			
		case e_frameType::NONE:
		{
			
		}
		break;
			
		default:
		{
			
		}
		break;
	}
}


CODE_RAM void Element::draw_background(Color color)
{
	switch(m_frameType)
	{
		case e_frameType::RECTANGLE:
		{
			draw_rectangleFilled(Rect(Vec2(0, 0), size), color);
		}
		break;
		
		case e_frameType::ROUND:
		{
			if(size.x == size.y)
			{
				draw_circleFilled(size / 2, size.x / 2 - 1, color);
			}
			if(size.x > size.y)
			{
				int16 y_div2 = size.y / 2;
				Rectangle mid(y_div2, 0, size.x - size.y, size.y);
				draw_rectangleFilled(mid, color);
				
				Vec2 center_1(y_div2, y_div2);
				Vec2 center_2(size.x - 1 - y_div2, y_div2);
				draw_circleFilled(center_1, y_div2 - 1, color, 90,	181);
				draw_circleFilled(center_2, y_div2 - 1, color, 270,	181);
			}
			if(size.x < size.y)
			{
				int16 x_div2 = size.x / 2;
				Rectangle mid(0, x_div2, size.y, size.y - size.x);
				draw_rectangleFilled(mid, color);
				
				Vec2 center_1(x_div2, x_div2);
				Vec2 center_2(x_div2, size.y - 1 - x_div2);
				draw_circleFilled(center_1, x_div2 - 1, color, 180,	181);
				draw_circleFilled(center_2, x_div2 - 1, color, 0,		181);
			}
		}
		break;
			
		case e_frameType::ROUNDED:
		{
			//	Rectangle Center
			draw_rectangleFilled(	Rectangle(c_frameRoundness,								c_frameRoundness,								size.x - 2 * c_frameRoundness,	size.y - 2 * c_frameRoundness	), color);
			
			//	Rectangle Bottom
			draw_rectangleFilled(	Rectangle(c_frameRoundness,								0, 															size.x - 2 * c_frameRoundness,	c_frameRoundness							), color);
			
			//	Rectangle Top
			draw_rectangleFilled(	Rectangle(c_frameRoundness,								size.y - 1 - c_frameRoundness,	size.x - 2 * c_frameRoundness,	c_frameRoundness							), color);
			
			//	Rectangle Left
			draw_rectangleFilled(	Rectangle(0,															c_frameRoundness,								c_frameRoundness,								size.y - 2 * c_frameRoundness	), color);
			
			//	Rectangle Right
			draw_rectangleFilled(	Rectangle(size.x - 1 - c_frameRoundness,	c_frameRoundness,								c_frameRoundness,								size.y - 2 * c_frameRoundness	), color);
			
			//	Circle Top Right
			draw_circleFilled(		Vec2(			size.x - 1 - c_frameRoundness,	size.y - 1 - c_frameRoundness), c_frameRoundness, color, 0,		91);
			
			//	Circle Top Left
			draw_circleFilled(		Vec2(			c_frameRoundness,								size.y - 1 - c_frameRoundness), c_frameRoundness, color, 90,	91);
			
			//	Circle Bottom Left
			draw_circleFilled(		Vec2(			c_frameRoundness,								c_frameRoundness),							c_frameRoundness, color, 180,	91);
			
			//	Circle Bottom Right
			draw_circleFilled(		Vec2(			size.x - 1 - c_frameRoundness,	c_frameRoundness),							c_frameRoundness, color, 270,	91);
		}
		break;
		
		case e_frameType::NONE:
		{
			draw_rectangleFilled(Rect(Vec2(0, 0), size), color);
		}
		break;
			
		default:
		{
			
		}
		break;
	}
}







CODE_RAM feedback Element::draw_char(const Font::s_glyphDescription& glyphDescription, Vec2 bottomLeftPosition, Color color)
{
	//	Check for valid data
	const uint8* fontData = glyphDescription.data;
	if(fontData == nullptr)
	{
		return(FAIL);
	}
	
	
	//	Adjust the bottom Left position according to the glyph description
	bottomLeftPosition += glyphDescription.box.position;
	
	
	//	Data are one bit per pixel
	//	The first pixel is in the top-left corner
	//	Bit order is from left to right with bit 7 being the left-most pixel
	//	The data is a real bitstream, not necessarily being aligned to the box boundaries
	const uint32 numberOfPixels = glyphDescription.box.size.x * glyphDescription.box.size.y;
	uint32 pixelCounter = 0;
	Vec2 pixelPosition(0, glyphDescription.box.size.y - 1);
	while(pixelCounter < numberOfPixels)
	{
		//	Calculate the next bit
		const uint32 byteCounter = pixelCounter / 8;
		const uint32 bitCounter = 7 - (pixelCounter % 8);
		
		
		//	Get pixel data
		const bool bit = bit::isSet(fontData[byteCounter], bitCounter);
		
		
		//	Draw pixel
		if(bit == true)
		{
			set_pixel(bottomLeftPosition + pixelPosition, color);
		}
		
		
		//	Update pixel position
		pixelPosition.x++;
		if(pixelPosition.x >= glyphDescription.box.size.x)
		{
			pixelPosition.x = 0;
			pixelPosition.y--;
		}
		pixelCounter++;
	}
	return(OK);
}


CODE_RAM feedback Element::draw_char(char character, Vec2 bottomLeftPosition, const Font& font, Color color)
{
	//	Non-printable Characters wont be printed
	if(font.isPrintable(character) == false)
	{
		return(FAIL);
	}
	
	
	//	Get Glyph Description
	const Font::s_glyphDescription glyphDescription = font.get_glyph(character);
	
	
	//	Check for valid Data
	const uint8* fontData = glyphDescription.data;
	if(fontData == nullptr)
	{
		return(FAIL);
	}
	
	
	return(draw_char(glyphDescription, bottomLeftPosition, color));
}


CODE_RAM feedback Element::draw_string(String string, Vec2 bottomLeftPosition, const Font& font, Color color)
{
	for(uint32 i = 0; i < string.get_size(); i++)
	{
		//	Get Character to draw
		const char character = string[i];
		
		
		//	Non-printable Characters wont be drawn
		if(font.isPrintable(character) == false)
		{
			continue;
		}
		
		
		//	Get Glyph Description for the Advance Width
		const Font::s_glyphDescription glyphDescription = font.get_glyph(character);
		
		
		//	Draw Character
		draw_char(glyphDescription, bottomLeftPosition, color);
		
		
		//	Update the Bottom Left Position
		//	Advance Width is the distance to the next Character
		bottomLeftPosition.x += glyphDescription.advanceWidth;
	}
	return(OK);
}


CODE_RAM Array<uint32> Element::splitStringToMultiLine(const String& string, const Font& font, int16 distanceFromBorder)
{
	//	This Function splits a String into multiple Lines
	//	It returns an Array with the Number of Characters in each Line
	
	
	//	Element too small
	if(size.x <= 2 * distanceFromBorder)
	{
		return(Array<uint32>());
	}
	
	
	//	Calculate the maximum Line Width
	const uint32 maxUsablePixelPerLine = size.x - 2 * distanceFromBorder;
	
	
	//	Now we split the String into Lines and note the Position of the last Line Break Character to enable String Splits at natural Breaks
	uint32 positionOfLastLineBreakCharacter = string.get_size();
	constexpr char lineBreakCharacters[] = {' ', '\n', '\t', '-'};
	
	
	Array<uint32> charactersPerLine(0);
	uint32 lineLengthInPixel = 0;
	for(uint32 i = 0; i < string.get_size(); i++)
	{
		const char& character = string[i];
		
		
		//	Check if the Character is a Line Break Character
		for(auto& j: lineBreakCharacters)
		{
			if(character == j)
			{
				//	Character is a Line Break Character
				//	So we save the Position of the last Line Break Character
				positionOfLastLineBreakCharacter = i;
				break;
			}
		}
		
		
		//	Get the Advance Width of the Character
		const uint8 advanceWidthCharacter = font.get_advanceWidth(character);
		
		
		//	Check if the Character fits into the current Line
		uint32& charactersPerActualLine = charactersPerLine[charactersPerLine.get_size() - 1];
		if(lineLengthInPixel + advanceWidthCharacter > maxUsablePixelPerLine)
		{
			//	Character does not fit into the current Line
			//	So we need to start a new Line
			//	We try to do natural Line Breaks, so we need to check if there is a Line Break Character in the current Line
			if(positionOfLastLineBreakCharacter != string.get_size())
			{
				//	There is a Line Break Character in the current Line
				//	So we split the String at this Position
				
				
				//	Calculate the Sum of Characters in previous Lines
				uint32 sumOfCharactersAlreadyInLines = 0;
				for(auto& j: charactersPerLine)
				{
					sumOfCharactersAlreadyInLines += j;
				}
				charactersPerActualLine += positionOfLastLineBreakCharacter - sumOfCharactersAlreadyInLines + 1;
				i = positionOfLastLineBreakCharacter;
				
				
				//	Reset the Position of the last Line Break Character
				positionOfLastLineBreakCharacter = string.get_size();
				
				
				charactersPerLine += 0;
				lineLengthInPixel = 0;
			}
			else
			{
				charactersPerLine += 1;
				lineLengthInPixel = advanceWidthCharacter;
			}
		}
		else
		{
			//	Character fits into the current Line
			charactersPerActualLine++;
			lineLengthInPixel += advanceWidthCharacter;
		}
	}
	return(charactersPerLine);
}


CODE_RAM feedback Element::draw_string(String string, e_align_x align_x, uint32 y, const Font& font, Color color, bool multiLine, int16 distanceFromBorder)
{
	if(multiLine == true && get_stringBox(string, font).x > size.x - 2 * distanceFromBorder)
	{
		const Array<uint32> lineLengths = splitStringToMultiLine(string, font, distanceFromBorder);
		
		const uint32 numberOfLines = lineLengths.get_size();
		uint32 indexStart = 0;
		for(uint32 i = 0; i < numberOfLines; i++)
		{
			//	Get the Y Position for the current Line
			const uint32 y_line = y - i * font.get_height();
			
			
			//	Get the String for the current Line
			const uint32 lineLength = lineLengths[i];
			const String line = string.sub(indexStart, lineLength);
			
			
			//	Draw the String
			draw_string(line, align_x, y_line, font, color);
			
			
			//	Update the Start Index for the next Line
			indexStart += lineLength;
		}
	}
	else
	{
		Vec2 bottomLeftPosition(0, y);
		
		bottomLeftPosition.x = get_align((e_align) align_x, string, font, distanceFromBorder).x;
		return(draw_string(string, bottomLeftPosition, font, color));
	}
	return(OK);
}


CODE_RAM feedback Element::draw_string(String string, e_align_y align_y, uint32 x, const Font& font, Color color, bool multiLine, int16 distanceFromBorder)
{
	if(multiLine == true && get_stringBox(string, font).x > size.x)
	{
		const Array<uint32> lineLengths = splitStringToMultiLine(string, font, distanceFromBorder);
		const uint32 numberOfLines = lineLengths.get_size();
		
		
		//	Get starting Y Position for the first Line
		Vec2 bottomLeftPosition(x, 0);
		bottomLeftPosition.y = get_align((e_align) align_y, string, font, distanceFromBorder).y;
		
		
		//	We need to distinguish between Aligning Styles here
		switch(align_y)
		{
			case e_align_y::TOP:
			{
				uint32 indexStart = 0;
				for(uint32 i = 0; i < numberOfLines; i++)
				{
					//	Get the String for the current Line
					const uint32 lineLength = lineLengths[i];
					const String line = string.sub(indexStart, lineLength);
					
					
					//	Draw the String
					draw_string(line, bottomLeftPosition, font, color);
					
					
					//	Update the Bottom Left Position for the next Line
					bottomLeftPosition.y -= font.get_height();
					
					
					//	Update the Start Index for the next Line
					indexStart += lineLength;
				}
			}
			break;
			
			case e_align_y::CENTER:
			{
				uint8 indexStart = 0;
				bottomLeftPosition.y += ((numberOfLines - 1) / 2.0f) * font.get_height();
				for(uint32 i = 0; i < numberOfLines; i++)
				{
					//	Get the String for the current Line
					const uint32 lineLength = lineLengths[i];
					const String line = string.sub(indexStart, lineLength);
					
					
					//	Draw the String
					draw_string(line, bottomLeftPosition, font, color);
					
					
					//	Update the Bottom Left Position for the next Line
					bottomLeftPosition.y -= font.get_height();
					
					
					//	Update the Start Index for the next Line
					indexStart += lineLength;
				}
			}
			break;
			
			case e_align_y::BOTTOM:
			{
				uint32 indexStart = string.get_size();
				for(uint32 i = 0; i < numberOfLines; i++)
				{
					//	Update the Start Index for the next Line
					const uint32 lineLength = lineLengths[numberOfLines - i - 1];
					indexStart -= lineLength;
					
					
					//	Get the String for the current Line
					const String line = string.sub(indexStart, lineLength);
					
					
					//	Draw the String
					draw_string(line, bottomLeftPosition, font, color);
					
					
					//	Update the Bottom Left Position for the next Line
					bottomLeftPosition.y += font.get_height();
					if(bottomLeftPosition.y > size.y - 1 - distanceFromBorder)
					{
						//	We reached the Top Border of the Element
						//	So we stop drawing here
						break;
					}
				}
			}
			break;
			
			default:
			{
				
			}
			break;
		}
	}
	else
	{
		Vec2 bottomLeftPosition(x, 0);
		
		bottomLeftPosition.y = get_align((e_align) align_y, string, font, distanceFromBorder).y;
		return(draw_string(string, bottomLeftPosition, font, color));
	}
	return(OK);
}


CODE_RAM feedback Element::draw_string(String string, e_align align, const Font& font, Color color, bool multiLine, int16 distanceFromBorder)
{
	if(multiLine == true && get_stringBox(string, font).x > size.x)
	{
		const Array<uint32> lineLengths = splitStringToMultiLine(string, font, distanceFromBorder);
		const uint32 numberOfLines = lineLengths.get_size();
		
		
		//	Get starting Y Position for the first Line
		Vec2 bottomLeftPosition = get_align(align, string, font, distanceFromBorder);
		
		
		//	We need to distinguish between Aligning Styles here
		if(align == e_align::TOP_LEFT || align == e_align::TOP_CENTER || align == e_align::TOP_RIGHT)
		{
			uint32 indexStart = 0;
			for(uint32 i = 0; i < numberOfLines; i++)
			{
				//	Get the String for the current Line
				const uint32 lineLength = lineLengths[i];
				const String line = string.sub(indexStart, lineLength);
				
				
				//	Calculate the Bottom Left Position for the current Line
				bottomLeftPosition.x = get_align(align, line, font, distanceFromBorder).x;	
				
				
				//	Draw the String
				draw_string(line, bottomLeftPosition, font, color);
				
				
				//	Update the Bottom Left Position for the next Line
				bottomLeftPosition.y -= font.get_height();
				
				
				//	Update the Start Index for the next Line
				indexStart += lineLength;
			}
		}
		if(align == e_align::CENTER_LEFT || align == e_align::CENTER || align == e_align::CENTER)
		{
			uint32 indexStart = 0;
			bottomLeftPosition.y += ((numberOfLines - 1) / 2.0f) * font.get_height();
			for(uint32 i = 0; i < numberOfLines; i++)
			{
				//	Get the String for the current Line
				const uint32 lineLength = lineLengths[i];
				const String line = string.sub(indexStart, lineLength);
				
				
				//	Calculate the Bottom Left Position for the current Line
				bottomLeftPosition.x = get_align(align, line, font, distanceFromBorder).x;
				
				
				//	Draw the String
				draw_string(line, bottomLeftPosition, font, color);
				
				
				//	Update the Bottom Left Position for the next Line
				bottomLeftPosition.y -= font.get_height();
				
				
				//	Update the Start Index for the next Line
				indexStart += lineLength;
			}
		}
		if(align == e_align::BOTTOM_LEFT || align == e_align::BOTTOM_CENTER || align == e_align::BOTTOM_RIGHT)
		{
			uint32 indexStart = string.get_size();
			for(uint32 i = 0; i < numberOfLines; i++)
			{
				//	Update the Start Index for the next Line
				const uint32 lineLength = lineLengths[numberOfLines - i - 1];
				indexStart -= lineLength;
				
				
				//	Get the String for the current Line
				const String line = string.sub(indexStart, lineLength);
				
				
				//	Calculate the Bottom Left Position for the current Line
				bottomLeftPosition.x = get_align(align, line, font, distanceFromBorder).x;
				
				
				//	Draw the String
				draw_string(line, bottomLeftPosition, font, color);
				
				
				//	Update the Bottom Left Position for the next Line
				bottomLeftPosition.y += font.get_height();
				if(bottomLeftPosition.y > size.y - 1 - distanceFromBorder)
				{
					//	We reached the Top Border of the Element
					//	So we stop drawing here
					break;
				}
			}
		}
	}
	else
	{
		const Vec2 bottomLeftPosition = get_align(align, string, font, distanceFromBorder);
		return(draw_string(string, bottomLeftPosition, font, color));
	}
	return(OK);
}







CODE_RAM void Element::draw_icon(const Icon& icon, Vec2 bottomLeftPosition)
{
	const Vec2 iconSize = icon.get_size();
	const Color* iconData = icon.get_data();
	if(iconData == nullptr)
	{
		return;
	}
	
	for(int16 x = 0; x < iconSize.x; x++)
	{
		for(int16 y = 0; y < iconSize.y; y++)
		{
			const Vec2 position(x, y);
			set_pixel(bottomLeftPosition + position, iconData[y * iconSize.x + x]);
		}
	}
}







CODE_RAM feedback Element::draw_line(Vec2 start, Vec2 end, Color color)
{
	Vec2 direction(start, end);
	
	if(direction.x == 0 && direction.y == 0)
	{
		return(OK);
	}
	
	if(direction.x == 0)
	{
		int8 sign_y = Math::sign(direction.y);
		uint32 c_absolute = Math::absolute(direction.y);
		for(uint32 y = 0; y < c_absolute + 1; y++)
		{
			Vec2 pixel(start.x, start.y + y * sign_y);
			set_pixel(pixel, color);
		}
		return(OK);
	}
	
	if(direction.y == 0)
	{
		int8 sign_x = Math::sign(direction.x);
		const uint32 c_absolute = Math::absolute(direction.x);
		for(uint32 x = 0; x < c_absolute + 1; x++)
		{
			Vec2 pixel(start.x + x * sign_x, start.y);
			set_pixel(pixel, color);
		}
		return(OK);
	}
	
	
	int8 sign_x = Math::sign(direction.x);
	uint32 max = Math::absolute(direction.x);
	
	for(uint32 i = 0; i < max + 1; i++)
	{
		double y = start.y + (((double) i * direction.y) / max);
		Vec2 pixel(start.x + i * sign_x, y);
		
		set_pixel(pixel, color);
	}
	
	return(OK);
}


CODE_RAM feedback Element::draw_rectangle(Rectangle rectangle, Color color)
{
	if(rectangle.size.x <= 0 || rectangle.size.y <= 0)
	{
		return(FAIL);
	}
	
	Rectangle rect_check(position + rectangle.position, rectangle.size);
	if(contains(rect_check) == false)
	{
		return(FAIL);
	}
	
	Vec2 position_1;
	Vec2 position_2;
	
	position_1.y = 0;
	position_2.y = rectangle.size.y - 1;
	for(int16 x = 0; x < rectangle.size.x; x++)
	{
		position_1.x = x;
		position_2.x = x;
		set_pixel(rectangle.position + position_1, color);
		set_pixel(rectangle.position + position_2, color);
	}
	
	position_1.x = 0;
	position_2.x = rectangle.size.x - 1;;
	for(int16 y = 0; y < rectangle.size.y; y++)
	{
		position_1.y = y;
		position_2.y = y;
		set_pixel(rectangle.position + position_1, color);
		set_pixel(rectangle.position + position_2, color);
	}
	
	return(OK);
}


CODE_RAM feedback Element::draw_rectangleFilled(Rectangle rectangle, Color color)
{
	Graphics& graphics = Graphics::get();
	if(rectangle.size.x <= 0 || rectangle.size.y <= 0 || m_page != graphics.get_currentPage())
	{
		return(FAIL);
	}
	
	
	//	Check if rectangle can be drawn by using the graphic accelerator - if not, use the manual drawing function
	//	It can be drawn if the rectangle is completely inside the element (not crossing the frame - depends on frame type)
	Vec2 bottomLeft(position + rectangle.position);
	Vec2 bottomRight(position + rectangle.position);
	Vec2 topLeft(position + rectangle.position);
	Vec2 topRight(position + rectangle.get_topRightCorner());
	
	bottomRight.x += rectangle.size.x - 1;
	topLeft.y += rectangle.size.y - 1;
	
	if(containsPoint(bottomLeft) == false || containsPoint(bottomRight) == false || containsPoint(topLeft) == false || containsPoint(topRight) == false)
	{
		return(draw_rectangleFilledManual(rectangle, color));
	}
	
	
	//	Choose the correct backbuffer and draw the rectangle by using the graphic accelerator
	rectangle.position += position + m_backBufferShape.position;
	const RectGraphic backbufferActive(m_backBufferShape, m_backBufferData[m_backbufferIndex]);
	m_graphicAccelerator->drawfilledRectangleWithSingleColor(backbufferActive, color, rectangle);
	
	
	//	Since the graphic accelerator has drawn directly into the backbuffer, we can set the flag that both framebuffers are not identical
	m_areBothFramebuffersIdentical = false;
	
	
	return(OK);
}


CODE_RAM feedback Element::draw_circle(Vec2 center, uint32 radius, Color color)
{
	int32 f = 1 - radius;
	int32 ddF_x = 0;
	int32 ddF_y = -2 * radius;
	int32 x = 0;
	int32 y = radius;
	
	set_pixel(Vec2(center.x, center.y + radius), color);
	set_pixel(Vec2(center.x, center.y - radius), color);
	set_pixel(Vec2(center.x + radius, center.y), color);
	set_pixel(Vec2(center.x - radius, center.y), color);
	
	while(x < y)
	{
		if(f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;
		
		set_pixel(Vec2(center.x + x, center.y + y), color);
		set_pixel(Vec2(center.x - x, center.y + y), color);
		set_pixel(Vec2(center.x + x, center.y - y), color);
		set_pixel(Vec2(center.x - x, center.y - y), color);
		set_pixel(Vec2(center.x + y, center.y + x), color);
		set_pixel(Vec2(center.x - y, center.y + x), color);
		set_pixel(Vec2(center.x + y, center.y - x), color);
		set_pixel(Vec2(center.x - y, center.y - x), color);
	}
	
	return(OK);
}


CODE_RAM feedback Element::draw_circle(Vec2 center, uint32 radius, Color color, double angleStart, double anglePart)
{
	if(anglePart >= 360)
	{
		return(draw_circle(center, radius, color));
	}
	
	while(angleStart >= 360)
	{
		angleStart -= 360;
	}
	
	int32 f = 1 - radius;
	int32 ddF_x = 0;
	int32 ddF_y = -2 * radius;
	int32 x = 0;
	int32 y = radius;
	
	Vec2 pixel;
	double angle = 0;
	double angleEnd = angleStart + anglePart;
	if(angleEnd >= 360)
	{
		angleEnd -= 360;
	}
	
	pixel.x = center.x;
	pixel.y = center.y + radius;
	angle = (pixel - center).angleToX();
	if(Math::isAngleBetween(angle, angleStart, angleEnd) == true)
	{
		set_pixel(pixel, color);
	}
	
	pixel.x = center.x;
	pixel.y = center.y - radius;
	angle = (pixel - center).angleToX();
	if(Math::isAngleBetween(angle, angleStart, angleEnd) == true)
	{
		set_pixel(pixel, color);
	}
	
	pixel.x = center.x + radius;
	pixel.y = center.y;
	angle = (pixel - center).angleToX();
	if(Math::isAngleBetween(angle, angleStart, angleEnd) == true)
	{
		set_pixel(pixel, color);
	}
	
	pixel.x = center.x - radius;
	pixel.y = center.y;
	angle = (pixel - center).angleToX();
	if(Math::isAngleBetween(angle, angleStart, angleEnd) == true)
	{
		set_pixel(pixel, color);
	}
	
	
	
	while(x < y)
	{
		if(f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;
		
		
		
		pixel.x = center.x + x;
		pixel.y = center.y + y;
		angle = (pixel - center).angleToX();
		if(Math::isAngleBetween(angle, angleStart, angleEnd) == true)
		{
			set_pixel(pixel, color);
		}
		
		pixel.x = center.x - x;
		pixel.y = center.y + y;
		angle = (pixel - center).angleToX();
		if(Math::isAngleBetween(angle, angleStart, angleEnd) == true)
		{
			set_pixel(pixel, color);
		}
		
		pixel.x = center.x + x;
		pixel.y = center.y - y;
		angle = (pixel - center).angleToX();
		if(Math::isAngleBetween(angle, angleStart, angleEnd) == true)
		{
			set_pixel(pixel, color);
		}
		
		pixel.x = center.x - x;
		pixel.y = center.y - y;
		angle = (pixel - center).angleToX();
		if(Math::isAngleBetween(angle, angleStart, angleEnd) == true)
		{
			set_pixel(pixel, color);
		}
		
		pixel.x = center.x + y;
		pixel.y = center.y + x;
		angle = (pixel - center).angleToX();
		if(Math::isAngleBetween(angle, angleStart, angleEnd) == true)
		{
			set_pixel(pixel, color);
		}
		
		pixel.x = center.x - y;
		pixel.y = center.y + x;
		angle = (pixel - center).angleToX();
		if(Math::isAngleBetween(angle, angleStart, angleEnd) == true)
		{
			set_pixel(pixel, color);
		}
		
		pixel.x = center.x + y;
		pixel.y = center.y - x;
		angle = (pixel - center).angleToX();
		if(Math::isAngleBetween(angle, angleStart, angleEnd) == true)
		{
			set_pixel(pixel, color);
		}
		
		pixel.x = center.x - y;
		pixel.y = center.y - x;
		angle = (pixel - center).angleToX();
		if(Math::isAngleBetween(angle, angleStart, angleEnd) == true)
		{
			set_pixel(pixel, color);
		}
	}
	
	return(OK);
}


CODE_RAM feedback Element::draw_circleFilled(Vec2 center, uint32 radius, Color color)
{
	uint32 r_square = radius * radius;
	for(int16 y = 0; y <= (int16) radius; y++)
	{
		for(int16 x = 0; x <= (int16) radius; x++)
		{
			uint32 temp = x * x + y * y;
			if(temp <= r_square)
			{
				set_pixel(Vec2(center.x + x, center.y + y), color);
				set_pixel(Vec2(center.x - x, center.y + y), color);
				set_pixel(Vec2(center.x + x, center.y - y), color);
				set_pixel(Vec2(center.x - x, center.y - y), color);
			}
		}
	}
	return(OK);
}


CODE_RAM feedback Element::draw_circleFilled(Vec2 center, uint32 radius, Color color, double angleStart, double anglePart)
{
	if(anglePart >= 360)
	{
		return(draw_circleFilled(center, radius, color));
	}
	
	while(angleStart >= 360)
	{
		angleStart -= 360;
	}
	
	uint32 r_square = radius * radius;
	
	Vec2 pixel;
	double angle = 0;
	double angleEnd = angleStart + anglePart;
	if(angleEnd >= 360)
	{
		angleEnd -= 360;
	}
	
	for(int16 y = 0; y <= (int16) radius; y++)
	{
		for(int16 x = 0; x <= (int16) radius; x++)
		{
			uint32 temp = x * x + y * y;
			if(temp <= r_square)
			{
				pixel.x = center.x + x;
				pixel.y = center.y + y;
				angle = (pixel - center).angleToX();
				if(Math::isAngleBetween(angle, angleStart, angleEnd) == true)
				{
					set_pixel(pixel, color);
				}
				
				pixel.x = center.x - x;
				pixel.y = center.y + y;
				angle = (pixel - center).angleToX();
				if(Math::isAngleBetween(angle, angleStart, angleEnd) == true)
				{
					set_pixel(pixel, color);
				}
				
				pixel.x = center.x + x;
				pixel.y = center.y - y;
				angle = (pixel - center).angleToX();
				if(Math::isAngleBetween(angle, angleStart, angleEnd) == true)
				{
					set_pixel(pixel, color);
				}
				
				pixel.x = center.x - x;
				pixel.y = center.y - y;
				angle = (pixel - center).angleToX();
				if(Math::isAngleBetween(angle, angleStart, angleEnd) == true)
				{
					set_pixel(pixel, color);
				}
			}
		}
	}
	return(OK);
}


CODE_RAM feedback Element::draw_circle(Vec2 center, uint32 radius, f_color color)
{
	int32 f = 1 - radius;
	int32 ddF_x = 0;
	int32 ddF_y = -2 * radius;
	int32 x = 0;
	int32 y = radius;

	set_pixel(Vec2(center.x, center.y + radius), color(Vec2(center.x, center.y + radius), size));
	set_pixel(Vec2(center.x, center.y - radius), color(Vec2(center.x, center.y - radius), size));
	set_pixel(Vec2(center.x + radius, center.y), color(Vec2(center.x + radius, center.y), size));
	set_pixel(Vec2(center.x - radius, center.y), color(Vec2(center.x - radius, center.y), size));

	while(x < y)
	{
		if(f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;

		set_pixel(Vec2(center.x + x, center.y + y), color(Vec2(center.x + x, center.y + y), size));
		set_pixel(Vec2(center.x - x, center.y + y), color(Vec2(center.x - x, center.y + y), size));
		set_pixel(Vec2(center.x + x, center.y - y), color(Vec2(center.x + x, center.y - y), size));
		set_pixel(Vec2(center.x - x, center.y - y), color(Vec2(center.x - x, center.y - y), size));
		set_pixel(Vec2(center.x + y, center.y + x), color(Vec2(center.x + y, center.y + x), size));
		set_pixel(Vec2(center.x - y, center.y + x), color(Vec2(center.x - y, center.y + x), size));
		set_pixel(Vec2(center.x + y, center.y - x), color(Vec2(center.x + y, center.y - x), size));
		set_pixel(Vec2(center.x - y, center.y - x), color(Vec2(center.x - y, center.y - x), size));
	}
	
	return(OK);
}


CODE_RAM feedback Element::draw_circleFilled(Vec2 center, uint32 radius, f_color color)
{
	uint32 r_square = radius * radius;
	for(int16 y = 0; y <= (int16) radius; y++)
	{
		for(int16 x = 0; x <= (int16) radius; x++)
		{
			uint32 temp = x * x + y * y;
			if(temp <= r_square)
			{
				set_pixel(Vec2(center.x + x, center.y + y), color(Vec2(center.x + x, center.y + y), size));
				set_pixel(Vec2(center.x - x, center.y + y), color(Vec2(center.x + x, center.y + y), size));
				set_pixel(Vec2(center.x + x, center.y - y), color(Vec2(center.x + x, center.y + y), size));
				set_pixel(Vec2(center.x - x, center.y - y), color(Vec2(center.x + x, center.y + y), size));
			}
		}
	}
	return(OK);
}