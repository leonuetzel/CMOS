#pragma once

#include "../defines.hpp"
#include "../rectangle.hpp"
#include "color.hpp"





typedef Vector2<int16> Vec2;
typedef Rectangle<int16> Rect;



class RectGraphic: public Rect
{
	public:
		
		Color* data;
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		constexpr inline RectGraphic();
		constexpr inline RectGraphic(Rect rectangle);
		constexpr inline RectGraphic(Rect rectangle, Color* data);
		constexpr inline RectGraphic(int16 position_x, int16 position_y, int16 size_x, int16 size_y);
		
		constexpr inline bool is_dataValid() const;
		constexpr inline Color* get_data(Rect rectangle) const;
		
		constexpr inline RectGraphic& operator=(const RectGraphic& rectangle);
		constexpr inline RectGraphic& operator=(const Rect& rectangle);
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

constexpr inline RectGraphic::RectGraphic()
	:	data(nullptr)
{
	
}


constexpr inline RectGraphic::RectGraphic(Rect rectangle)
	: Rect(rectangle),
		data(nullptr)
{
	
}


constexpr inline RectGraphic::RectGraphic(Rect rectangle, Color* data)
	: Rect(rectangle),
		data(data)
{
	
}


constexpr inline RectGraphic::RectGraphic(int16 position_x, int16 position_y, int16 size_x, int16 size_y)
	: Rect(position_x, position_y, size_x, size_y),
		data(nullptr)
{
	
}







constexpr inline bool RectGraphic::is_dataValid() const
{
	if(data != nullptr)
	{
		return(true);
	}
	return(false);
}


constexpr inline Color* RectGraphic::get_data(Rect rectangle) const
{
	Vec2 position_relative = rectangle.position - position; 
	return(data + (size.y - position_relative.y - rectangle.size.y) * size.x + position_relative.x);
}







constexpr inline RectGraphic& RectGraphic::operator=(const RectGraphic& rectangle)
{
	Rect::operator=(rectangle);
	data = rectangle.data;
	
	return(*this);
}


constexpr inline RectGraphic& RectGraphic::operator=(const Rect& rectangle)
{
	Rect::operator=(rectangle);
	
	return(*this);
}