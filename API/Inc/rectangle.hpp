#pragma once

#include "defines.hpp"
#include "mathBasic.hpp"
#include "vector2.hpp"





template<typename dataType>
class Rectangle
{
	public:
		
		Vector2<dataType> position;
		Vector2<dataType> size;
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		constexpr inline Rectangle();
		constexpr inline Rectangle(Vector2<dataType> position, Vector2<dataType> size);
		constexpr inline Rectangle(dataType position_x, dataType position_y, dataType size_x, dataType size_y);
		
		constexpr inline Vector2<dataType> get_topRightCorner() const;
		constexpr inline double get_surface() const;
		
		constexpr inline bool contains(const Vector2<dataType> v) const;
		constexpr inline bool contains(const Rectangle& rectangle) const;
		
		bool doesOverlap(const Rectangle& rectangle) const;
		Rectangle get_overlap(const Rectangle& rectangle) const;
		inline double get_overlapSurface(const Rectangle& rectangle) const;
		
		constexpr inline bool operator==(const Rectangle& rectangle) const;
		
		constexpr inline Rectangle& operator=(const Rectangle& rectangle);
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

template<typename dataType>
constexpr inline Rectangle<dataType>::Rectangle()
	:	position(0, 0),
		size(0, 0)
{
	
}


template<typename dataType>
constexpr inline Rectangle<dataType>::Rectangle(Vector2<dataType> position, Vector2<dataType> size)
	: position(Vector2<dataType>(position.x, position.y)),
		size(Vector2<dataType>(size.x, size.y))
{
	
}


template<typename dataType>
constexpr inline Rectangle<dataType>::Rectangle(dataType position_x, dataType position_y, dataType size_x, dataType size_y)
	:	position(Vector2<dataType>(position_x, position_y)),
		size(Vector2<dataType>(size_x, size_y))
{
	
}







template<typename dataType>
constexpr inline Vector2<dataType> Rectangle<dataType>::get_topRightCorner() const
{
	Vector2<dataType> sizeSign(size.sign());
	return(Vector2<dataType>(position.x + size.x - sizeSign.x, position.y + size.y - sizeSign.y));
}


template<typename dataType>
constexpr inline double Rectangle<dataType>::get_surface() const
{
	return(Math::absolute(size.x * size.y));
}







template<typename dataType>
constexpr inline bool Rectangle<dataType>::contains(const Vector2<dataType> v) const
{
	if(v.x >= position.x && v.x < position.x + size.x && v.y >= position.y && v.y < position.y + size.y)
	{
		return(true);
	}
	return(false);
}


template<typename dataType>
constexpr inline bool Rectangle<dataType>::contains(const Rectangle& rectangle) const
{
	//	Bottom and Left Side
	if(rectangle.position.x < position.x || rectangle.position.y < position.y)
	{
		return(false);
	}
	
	
	//	Right Side
	if(rectangle.position.x + rectangle.size.x > position.x + size.x)
	{
		return(false);
	}
	
	
	//	Top Side
	if(rectangle.position.y + rectangle.size.y > position.y + size.y)
	{
		return(false);
	}
	return(true);
}







template<typename dataType>
bool Rectangle<dataType>::doesOverlap(const Rectangle& rectangle) const
{
	//	Check if "this" is contained in "rectangle"
	if(position.x >= rectangle.position.x && position.x + size.x <= rectangle.position.x + rectangle.size.x)
	{
		if(position.y >= rectangle.position.y && position.y + size.y <= rectangle.position.y + rectangle.size.y)
		{
			return(true);
		}
	}
	
	
	
	//	Check if "rectangle" is contained in "this"
	if(rectangle.position.x >= position.x && rectangle.position.x + rectangle.size.x <= position.x + size.x)
	{
		if(rectangle.position.y >= position.y && rectangle.position.y + rectangle.size.y <= position.y + size.y)
		{
			return(true);
		}
	}
	
	
	
	if(position.x >= rectangle.position.x + rectangle.size.x)
	{
		return(false);
	}
	if(position.x + size.x <= rectangle.position.x)
	{
		return(false);
	}
	if(position.y + size.y <= rectangle.position.y)
	{
		return(false);
	}
	if(position.y >= rectangle.position.y + rectangle.size.y)
	{
		return(false);
	}
	return(true);
}


template<typename dataType>
Rectangle<dataType> Rectangle<dataType>::get_overlap(const Rectangle& rectangle) const
{
	Rectangle overlap;
	overlap.position = {max(position.x, rectangle.position.x), max(position.y, rectangle.position.y)};
	overlap.size = {min(position.x + size.x, rectangle.position.x + rectangle.size.x) - overlap.position.x, min(position.y + size.y, rectangle.position.y + rectangle.size.y) - overlap.position.y};
	
	if(overlap.position.x >= overlap.position.x + overlap.size.x || overlap.position.y >= overlap.position.y + overlap.size.y)
	{
		return(Rectangle({0, 0}, {0, 0}));
	}
	return(overlap);
}


template<typename dataType>
inline double Rectangle<dataType>::get_overlapSurface(const Rectangle& rectangle) const
{
	Rectangle	overlap = get_overlap(rectangle);
	return(overlap.get_surface());
}







template<typename dataType>
constexpr inline bool Rectangle<dataType>::operator==(const Rectangle& rectangle) const
{
	if(position == rectangle.position && size == rectangle.size)
	{
		return(true);
	}
	return(false);
}


template<typename dataType>
constexpr inline Rectangle<dataType>& Rectangle<dataType>::operator=(const Rectangle& rectangle)
{
	position = rectangle.position;
	size = rectangle.size;
	
	return(*this);
}