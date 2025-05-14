#include "../../Inc/graphics/graphics.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/

RectGraphic* 						Element::m_backBuffer;
I_GraphicAccelerator* 	Element::m_graphicAccelerator;
Graphics::e_touchEvent	Element::m_touchEvent;



/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

CODE_RAM void Element::onUpdate()
{
	if(m_visible == false)
	{
		return;
	}
	
	
	//	Lock Semaphore of this Element to prevent Data Altering by Callback Function
	CMOS& cmos = CMOS::get();
	if(cmos.semaphore_lock(this) != OK)
	{
		return;
	}
	m_function_onUpdate(*this);
	
	m_rebuildRequested = false;
	m_updateRequested = false;
	
	
	//	Unlock Semaphore
	cmos.semaphore_unlock(this);
}


CODE_RAM void Element::onCallback()
{
	//	Lock Semaphore of this Element to prevent Data Altering by Update Function
	CMOS& cmos = CMOS::get();
	if(cmos.semaphore_lock(this) != OK)
	{
		return;
	}
	
	
	//	Do Callback Stuff
	if(m_function_onCallback != nullptr)
	{
		m_function_onCallback(*this);
	}
	
	
	//	Unlock Semaphore
	cmos.semaphore_unlock(this);
}


CODE_RAM void Element::onChangePageActual()
{
	if(m_function_onChangePageActual != nullptr)
	{
		m_function_onChangePageActual(*this);
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
			Vec2 pixel(rectangle.position.x + x, rectangle.position.y + y);
			set_pixel(pixel, color);
		}
	}
	
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
	
	for(uint32 i = 0; i < graphics.m_elements.get_size(); i++)
	{
		Element& element = *graphics.m_elements[i];
		if(&element != this)
		{
			if(element.m_layer == m_layer && element.m_page == newPage)
			{
				if(doesOverlap(element) == true)
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
	
	
	if(m_page == graphics.get_pageActual())
	{
		//	Clear old Data in Backbuffer and transfer to Frontbuffer to make Changes visible
		clear();
		graphics.transferLayerToFrontbuffer(m_layer);
	}
	
	
	//	Set new Page and rebuild Element if new Page is actual Page
	m_page = newPage;
	if(m_page == graphics.get_pageActual())
	{
		m_rebuildRequested = true;
		m_updateRequested = true;
	}
	if(m_function_onChangePage != nullptr)
	{
		m_function_onChangePage(*this);
	}
	
	
	//	Unlock Semaphore
	return(cmos.semaphore_unlock(this));
}


CODE_RAM feedback Element::set_layer(uint32 newLayer)
{
	Graphics& graphics = Graphics::get();
	if(newLayer >= graphics.get_numberOfLayers())
	{
		return(FAIL);
	}
	
	for(uint32 i = 0; i < graphics.m_elements.get_size(); i++)
	{
		Element& element = *graphics.m_elements[i];
		if(&element != this)
		{
			if(element.m_page == m_page && element.m_layer == newLayer)
			{
				if(doesOverlap(element) == true)
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
	
	
	if(m_page == graphics.get_pageActual())
	{
		//	Clear old Data in Backbuffer
		clear();
		
		
		//	Set new Layer and rebuild Element
		m_layer = newLayer;
		m_rebuildRequested = true;
		m_updateRequested = true;
	}
	if(m_function_onChangeLayer != nullptr)
	{
		m_function_onChangeLayer(*this);
	}
	
	
	//	Unlock Semaphore
	return(cmos.semaphore_unlock(this));
}


CODE_RAM feedback Element::set_position(Vec2 newPosition)
{
	Graphics& graphics = Graphics::get();
	
	//	Check Display Borders
	if(newPosition.x < 0 || newPosition.y < 0)
	{
		return(FAIL);
	}
	
	Rect newRect(newPosition, size);
	Vec2 topRight(newRect.get_topRightCorner());
	if(topRight.x > m_backBuffer[m_layer].size.x || topRight.y > m_backBuffer[m_layer].size.y)
	{
		return(FAIL);
	}
	
	
	//	Check overlap with other Elements
	for(uint32 i = 0; i < graphics.m_elements.get_size(); i++)
	{
		Element& element = *graphics.m_elements[i];
		if(&element != this)
		{
			if(element.m_layer == m_layer && element.m_page == m_page)
			{
				if(newRect.doesOverlap(element) == true)
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
	
	
	//	Make Changes visible only if Element is on currently shown Page
	if(m_page == graphics.get_pageActual())
	{
		//	Clear old Data in Backbuffer
		clear();
		
		
		//	Set new Position and rebuild Element
		position = newPosition;
		m_rebuildRequested = true;
		m_updateRequested = true;
	}
	if(m_function_onChangePosition != nullptr)
	{
		m_function_onChangePosition(*this);
	}
	
	
	//	Unlock Semaphore
	return(cmos.semaphore_unlock(this));
}


CODE_RAM feedback Element::set_size(Vec2 newSize)
{
	Graphics& graphics = Graphics::get();
	
	//	Check Display Borders
	Rect newRect(position, newSize);
	Vec2 topRight(newRect.get_topRightCorner());
	if(topRight.x > m_backBuffer[m_layer].size.x || topRight.y > m_backBuffer[m_layer].size.y)
	{
		return(FAIL);
	}
	
	
	//	Check Minimum Dimensions
	if(newSize.x < c_minimumSideLength || newSize.y < c_minimumSideLength)
	{
		return(FAIL);
	}
	
	
	//	Check overlap with other Elements
	for(uint32 i = 0; i < graphics.m_elements.get_size(); i++)
	{
		Element& element = *graphics.m_elements[i];
		if(&element != this)
		{
			if(element.m_layer == m_layer && element.m_page == m_page)
			{
				if(newRect.doesOverlap(element) == true)
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
	
	
	//	Make Changes visible only if Element is on currently shown Page
	if(m_page == graphics.get_pageActual())
	{
		//	Clear old Data in Backbuffer
		clear();
		
		
		//	Set new Size and rebuild Element
		size = newSize;
		m_rebuildRequested = true;
		m_updateRequested = true;
	}
	if(m_function_onChangeSize != nullptr)
	{
		m_function_onChangeSize(*this);
	}
	
	
	//	Unlock Semaphore
	return(cmos.semaphore_unlock(this));
}







CODE_RAM Vec2 Element::get_align(Vec2 boxSize, e_align align, uint32 stringLength, const Font& font)
{
	Vec2 bottomLeftPosition(0, 0);
	
	const Vec2 fontSize = font.get_size();
	Vec2 pixel(stringLength * fontSize.x, fontSize.y);
	
	uint8 x_align =  ((uint32) align) & 0x3;
	uint8 y_align = (((uint32) align) & 0xC) >> 2;
	
	
	
	switch(x_align)
	{
		case 0:
		{
			bottomLeftPosition.x = 1;
		}
		break;
		
		case 1:
		{
			bottomLeftPosition.x = (boxSize.x - pixel.x) / 2;
		}
		break;
		
		case 2:
		{
			bottomLeftPosition.x = boxSize.x - 1 - pixel.x;
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
			bottomLeftPosition.y = 1;
		}
		break;
		
		case 1:
		{
			bottomLeftPosition.y = (boxSize.y - pixel.y) / 2;
		}
		break;
		
		case 2:
		{
			bottomLeftPosition.y = boxSize.y - 1 - pixel.y;
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







Vec2 Element::get_minimumDimension(uint32 stringLength, const Font& font, bool multiLine)
{
	const Vec2 fontSize = font.get_size();
	Vec2 minimumSize;
	
	
	//	font.x + 1 Distance between two Signs + 2 Frame + 2 Distance to Frame
	minimumSize.x = (stringLength * fontSize.x) + stringLength - 1 + 2 + 2;
	
	
	//	font.y + 2 Frame + 2 Distance to Frame
	minimumSize.y = fontSize.y + 2 + 2;
	
	
	//	Multi Line Support
	if(multiLine == true)
	{
		Graphics& graphics = Graphics::get();
		const Vec2 displayDimensions = graphics.get_displayDimensions();
		
		
		const uint32 charactersPerLine = (displayDimensions.x - 2 - 2 + 1) / (fontSize.x + 1);
		minimumSize.x = (charactersPerLine * fontSize.x) + charactersPerLine - 1 + 2 + 2;
		minimumSize.y = stringLength / charactersPerLine;
		if(stringLength % charactersPerLine != 0)
		{
			minimumSize.y++;
		}
		minimumSize.y *= fontSize.y + 2;
		minimumSize.y += 2;
	}
	return(minimumSize);
}







CODE_RAM feedback Element::set_pixel(Vec2 pixelPosition, Color color)
{
	static Graphics& graphics = Graphics::get();
	if(m_backBuffer[m_layer].data == nullptr || m_page != graphics.get_pageActual())
	{
		return(FAIL);
	}
	
	Vec2 positionAbsolute(position + pixelPosition);
	
	if(containsPoint(positionAbsolute) == false)
	{
		return(FAIL);
	}
	
	RectGraphic& buffer = m_backBuffer[m_layer];
	Color* pixel = buffer.data + ((buffer.size.y - positionAbsolute.y - 1) * buffer.size.x + positionAbsolute.x);
	*pixel = color;
	
	return(OK);
}


CODE_RAM void Element::draw_frame(Color color)
{
	switch(m_frameType)
	{
		case e_frameType::RECTANGLE:
			draw_rectangle(Rectangle(0, 0, size.x, size.y), color);
			break;
			
		case e_frameType::ROUND:
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
			break;
			
		case e_frameType::ROUNDED:
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
			break;
			
		case e_frameType::NONE:
			break;
			
		default:
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







CODE_RAM feedback Element::draw_char(char character, Vec2 bottomLeftPosition, const Font& font, Color color)
{
	const Vec2 characterSize = font.get_size();
	bottomLeftPosition.x += 2;
	
	
	const uint8* fontData = font.get_character(character);
	if(fontData == nullptr)
	{
		return(FAIL);
	}
	
	
	//	Draw
	const int32 bytesPerColumn = characterSize.y / 8;
	for(int16 x = 0; x < characterSize.x; x++)
	{
		for(int16 i = 0; i < bytesPerColumn; i++)
		{
			const uint8 data = fontData[x * bytesPerColumn + i];
			for(int16 yInner = 0; yInner < 8; yInner++)
			{
				if(bit::isSet(data, yInner) == true)
				{
					set_pixel(Vec2(bottomLeftPosition.x + x, bottomLeftPosition.y + i * 8 + yInner), color);
				}
			}
		}
	}
	return(OK);
}


CODE_RAM feedback Element::draw_string(String string, Vec2 bottomLeftPosition, const Font& font, Color color, bool multiLine)
{
	const Vec2 letterSize = font.get_size();
	
	for(uint32 i = 0; i < string.get_size(); i++)
	{
		draw_char(string[i], bottomLeftPosition, font, color);
		bottomLeftPosition.x += letterSize.x;
	}
	return(OK);
}


CODE_RAM Array<String> Element::splitStringToMultiLine(const String& string, const Font& font)
{
	Array<String> output;
	if(get_minimumDimension(string.get_size(), font).x > size.x)
	{
		//	Multi-Line needed
		
		
		//	Explode String into Words
		Array<String> words(string.explode(' '));
		
		
		String line;
		while(1)
		{
			//	Put as much Words into a Line as possible
			if(words.get_size() > 0)
			{
				//	Append a Word to the line
				if(line.get_size() > 0)
				{
					line += ' ';
				}
				line += words[0];
				
				
				//	Check if it fits into the Line
				if(get_minimumDimension(line.get_size(), font).x > size.x)
				{
					//	It doesnt fit - we need to remove the last Word added
					//	Check if we have more than one Word in the Line already
					if(line.contains(' ') == true)
					{
						//	We have more than one Word - remove the last one and its corresponding Whitespace
						line.eraseFromEnd(words[0].get_size() + 1);
					}
					else
					{
						//	We have only one Word, so we need to split it up
						
						
						//	Erase a single Character from that Word until it fits
						while(get_minimumDimension(line.get_size(), font).x > size.x)
						{
							line.eraseFromEnd(1);
						}
						
						
						//	Erase the same Number of Characters from the Word in the overall Word Buffer
						words[0].erase(line.get_size());
					}
					
					
					//	Now we know that the Line is full - put it into the Output Buffer
					output += line;
					line.erase();
				}
				else
				{
					//	It fits into the Line - remove the Word from the overall Word Buffer
					words.erase(0);
				}
			}
			else
			{
				output += line;
				line.erase();
				break;
			}
		}
	}
	else
	{
		output += string;
	}
	return(output);
}


CODE_RAM feedback Element::draw_string(String string, e_align_x align_x, uint32 y, const Font& font, Color color, bool multiLine)
{
	if(multiLine == true && get_minimumDimension(string.get_size(), font).x > size.x)
	{
		const Array<String> lines = splitStringToMultiLine(string, font);
		
		const uint32 numberOfLines = lines.get_size();
		for(uint32 i = 0; i < numberOfLines; i++)
		{
			const uint32 y_line = y - i * (font.get_size().y + 2);
			draw_string(lines[i], align_x, y_line, font, color);
		}
	}
	else
	{
		Vec2 bottomLeftPosition(0, y);
		
		bottomLeftPosition.x = get_align(size, (e_align) align_x, string.get_size(), font).x;
		return(draw_string(string, bottomLeftPosition, font, color));
	}
	return(OK);
}


CODE_RAM feedback Element::draw_string(String string, e_align_y align_y, uint32 x, const Font& font, Color color, bool multiLine)
{
	if(multiLine == true && get_minimumDimension(string.get_size(), font).x > size.x)
	{
		const Array<String> lines = splitStringToMultiLine(string, font);
		const uint32 numberOfLines = lines.get_size();
		
		
		//	Get starting Y Position for the first Line
		Vec2 bottomLeftPosition(x, 0);
		bottomLeftPosition.y = get_align(size, (e_align) align_y, string.get_size(), font).y;
		
		
		//	We need to distinguish between Aligning Styles here
		switch(align_y)
		{
			case e_align_y::TOP:
			{
				for(uint32 i = 0; i < numberOfLines; i++)
				{
					draw_string(lines[i], bottomLeftPosition, font, color);
					bottomLeftPosition.y -= font.get_size().y + 2;
				}
			}
			break;
			
			case e_align_y::CENTER:
			{
				bottomLeftPosition.y += ((numberOfLines - 1) / 2.0f) * (font.get_size().y + 2);
				for(uint32 i = 0; i < numberOfLines; i++)
				{
					draw_string(lines[i], bottomLeftPosition, font, color);
					bottomLeftPosition.y -= font.get_size().y + 2;
				}
			}
			break;
			
			case e_align_y::BOTTOM:
			{
				for(uint32 i = 0; i < numberOfLines; i++)
				{
					draw_string(lines[numberOfLines - i - 1], bottomLeftPosition, font, color);
					bottomLeftPosition.y += font.get_size().y + 2;
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
		
		bottomLeftPosition.y = get_align(size, (e_align) align_y, string.get_size(), font).y;
		return(draw_string(string, bottomLeftPosition, font, color));
	}
	return(OK);
}


CODE_RAM feedback Element::draw_string(String string, e_align align, const Font& font, Color color, bool multiLine)
{
	if(multiLine == true && get_minimumDimension(string.get_size(), font).x > size.x)
	{
		const Array<String> lines = splitStringToMultiLine(string, font);
		const uint32 numberOfLines = lines.get_size();
		
		
		//	Get starting Y Position for the first Line
		Vec2 bottomLeftPosition = get_align(size, align, string.get_size(), font);
		
		
		//	We need to distinguish between Aligning Styles here
		if(align == e_align::TOP_LEFT || align == e_align::TOP_CENTER || align == e_align::TOP_RIGHT)
		{
			for(uint32 i = 0; i < numberOfLines; i++)
			{
				bottomLeftPosition.x = get_align(size, align, lines[i].get_size(), font).x;
				draw_string(lines[i], bottomLeftPosition, font, color);
				bottomLeftPosition.y -= font.get_size().y + 2;
			}
		}
		if(align == e_align::CENTER_LEFT || align == e_align::CENTER || align == e_align::CENTER)
		{
			bottomLeftPosition.y += ((numberOfLines - 1) / 2.0f) * (font.get_size().y + 2);
			for(uint32 i = 0; i < numberOfLines; i++)
			{
				bottomLeftPosition.x = get_align(size, align, lines[i].get_size(), font).x;
				draw_string(lines[i], bottomLeftPosition, font, color);
				bottomLeftPosition.y -= font.get_size().y + 2;
			}
		}
		if(align == e_align::BOTTOM_LEFT || align == e_align::BOTTOM_CENTER || align == e_align::BOTTOM_RIGHT)
		{
			for(uint32 i = 0; i < numberOfLines; i++)
			{
				bottomLeftPosition.x = get_align(size, align, lines[i].get_size(), font).x;
				draw_string(lines[numberOfLines - i - 1], bottomLeftPosition, font, color);
				bottomLeftPosition.y += font.get_size().y + 2;
			}
		}
	}
	else
	{
		const Vec2 bottomLeftPosition = get_align(size, align, string.get_size(), font);
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
	if(rectangle.size.x <= 0 || rectangle.size.y <= 0 || m_page != graphics.get_pageActual())
	{
		return(FAIL);
	}
	
	
	//	Check if Rectangle can be drawn by using the Graphic DMA - if not, use the manual Drawing Function
	//	It can be drawn if the Rectangle is completely inside the Element (not crossing the Frame - depends on Frame Type)
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
	
	
	rectangle.position += position + m_backBuffer[m_layer].position;
	m_graphicAccelerator->draw_rectangleFull(m_backBuffer[m_layer], color, rectangle);
	while(m_graphicAccelerator->is_available() == false)
	{
		#if defined(CORTEX_M7)
			CMOS::get().sleep_100us(1);
		#endif
		
		#if defined(CORTEX_M0) || defined(CORTEX_M0P) || defined(CORTEX_M3) || defined(CORTEX_M4)
			CMOS::get().sleep_ms(1);
		#endif
	}
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