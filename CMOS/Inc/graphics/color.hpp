#pragma once

#include "../defines.hpp"





class Color
{
	public:
		
		uint8 blue;
		uint8 green;
		uint8 red;
		uint8 alpha;
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		constexpr inline Color();
		constexpr inline Color(uint8 r, uint8 g, uint8 b, uint8 a);
		
		constexpr inline Color invert() const;
		
		constexpr inline Color& operator=(const Color& color);
		constexpr inline bool operator==(const Color& color) const;
		constexpr inline bool operator!=(const Color& color) const;
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

constexpr inline Color::Color()
	:	blue(0),
		green(0),
		red(0),
		alpha(0)
{
	
}


constexpr inline Color::Color(uint8 r, uint8 g, uint8 b, uint8 a)
	:	blue(b),
		green(g),
		red(r),
		alpha(a)
{
	
}







constexpr inline Color Color::invert() const
{
	return(Color(0xFF - red, 0xFF - green, 0xFF - blue, alpha));
}







constexpr inline Color& Color::operator=(const Color& color)
{
	alpha	= color.alpha;
	red		= color.red;
	green	= color.green;
	blue	= color.blue;
	return(*this);
}


constexpr inline bool Color::operator==(const Color& color) const
{
	if(alpha != color.alpha)
	{
		return(false);
	}
	if(red != color.red)
	{
		return(false);
	}
	if(green != color.green)
	{
		return(false);
	}
	if(blue != color.blue)
	{
		return(false);
	}
	return(true);
}


constexpr inline bool Color::operator!=(const Color& color) const
{
	return(!operator==(color));
}