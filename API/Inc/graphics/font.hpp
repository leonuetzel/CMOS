#pragma once

#include "../cmos.hpp"





class Font
{
	public:
		
		typedef struct
		{
			uint8 advanceWidth;
			Rect box;
			uint8* data;
		}s_glyphDescription;
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		const uint8 m_height;
		const int8 m_minimumPositionY;
		const uint8* m_glyphData;
		const uint8* m_glyphDescription;
		
		
		//	Constructor and Destructor
		Font(const Font&) = delete;
		
		
		//	Member Functions
		
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		constexpr inline Font(uint8 height, int8 minimumPositionY, const uint8* glyphData, const uint8* glyphDescription);
		inline ~Font();
		
		constexpr inline uint8 get_height() const;
		constexpr inline int8 get_minimumPositionY() const;
		constexpr inline const s_glyphDescription get_glyph(char character) const;
		constexpr inline bool isPrintable(char character) const;
		
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

constexpr inline Font::Font(uint8 height, int8 minimumPositionY, const uint8* glyphData, const uint8* glyphDescription)
	:	m_height(height),
		m_minimumPositionY(minimumPositionY),
		m_glyphData(glyphData),
		m_glyphDescription(glyphDescription)
{
	
}


inline Font::~Font()
{
	
}







constexpr uint8 Font::get_height() const
{
	return(m_height);
}


constexpr int8 Font::get_minimumPositionY() const
{
	return(m_minimumPositionY);
}


constexpr	inline const Font::s_glyphDescription Font::get_glyph(char character) const
{
	s_glyphDescription glyphDescription{0, Rect(0, 0, 0, 0), nullptr};
	if(isPrintable(character) == false)
	{
		return(glyphDescription);
	}
	
	
	//	Glyph Table starts with the Data for Glyph 0x20
	character -= 0x20;
	
	
	//	Get Glyph Data
	//	They are formatted in the Byte Stream as
	//	Byte-Stream[0] = Bitmap Index MSB
	//	Byte-Stream[1] = Bitmap Index LSB
	//	Byte-Stream[2] = Advance Width
	//	Byte-Stream[3] = Box Position X
	//	Byte-Stream[4] = Box Position Y
	//	Byte-Stream[5] = Box Size X
	//	Byte-Stream[6] = Box Size Y
	const uint8* glyphDescriptionByteStream = m_glyphDescription + 7 * character;
	
	const uint16 bitmapIndex = (glyphDescriptionByteStream[0] << 8) | glyphDescriptionByteStream[1];
	glyphDescription.data = (uint8*) m_glyphData + bitmapIndex;
	
	glyphDescription.advanceWidth = glyphDescriptionByteStream[2];
	glyphDescription.box.position.x = (int8) glyphDescriptionByteStream[3];
	glyphDescription.box.position.y = (int8) glyphDescriptionByteStream[4];
	glyphDescription.box.size.x = glyphDescriptionByteStream[5];
	glyphDescription.box.size.y = glyphDescriptionByteStream[6];
	
	
	//	Read Bitmap Index (the Index where the Glyph Data starts in the Data Array)
	glyphDescription.data = (uint8*) &m_glyphData[bitmapIndex];
	
	
	return(glyphDescription);
}


constexpr	inline bool Font::isPrintable(char character) const
{
	if(character < 0x20 || character > 0x7E)
	{
		return(false);
	}
	return(true);
}







inline bool Font::operator==(const Font& font) const
{
	if(m_glyphData != font.m_glyphData)
	{
		return(false);
	}
	if(m_glyphDescription != font.m_glyphDescription)
	{
		return(false);
	}
	return(true);
}


inline bool Font::operator!=(const Font& font) const
{
	return(!operator==(font));
}