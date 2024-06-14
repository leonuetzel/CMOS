#pragma once

#include "../cmos.hpp"





class Font
{
	public:
		
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		const Vec2 m_size;
		const uint8* m_data;
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		constexpr inline Font(Vec2 size, const uint8* data);
		
		constexpr const uint8* get_character(char character) const;
		constexpr inline Vec2 get_size() const;
		
		inline bool operator==(const Font& font) const;
		inline bool operator!=(const Font& font) const;
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

constexpr inline Font::Font(Vec2 size, const uint8* data)
	:	m_size(size),
		m_data(data)
{
	
}







constexpr	const uint8* Font::get_character(char character) const
{
	const uint32 bytesPerColumn = m_size.y / 8;
	const uint32 bytesPerCharacter = bytesPerColumn * m_size.x;
	
	return(m_data + character * bytesPerCharacter);
}


constexpr inline Vec2 Font::get_size() const
{
	return(m_size);
}







inline bool Font::operator==(const Font& font) const
{
	if(m_size != font.m_size)
	{
		return(false);
	}
	if(m_data != font.m_data)
	{
		return(false);
	}
	return(true);
}


inline bool Font::operator!=(const Font& font) const
{
	return(!operator==(font));
}