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
		
		static constexpr uint8 printableStart	= 0x20;
		static constexpr uint8 printableEnd		= 0x7E;
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		const uint8 m_height;
		const int8 m_minimumPositionY;
		const uint8* m_glyphData;
		const uint8* m_glyphAdvanceWidth;
		const uint8* m_glyphDescription;
		
		
		//	Constructor and Destructor
		Font(const Font&) = delete;
		
		
		//	Member Functions
		
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		constexpr inline Font(uint8 height, int8 minimumPositionY, const uint8* glyphData, const uint8* glyphAdvanceWidth, const uint8* glyphDescription);
		inline ~Font();
		
		constexpr inline uint8 get_height() const;
		constexpr inline int8 get_minimumPositionY() const;
		constexpr inline uint8 get_advanceWidth(char character) const;
		constexpr inline const s_glyphDescription get_glyph(char character) const;
		static constexpr inline bool isPrintable(char character);
		
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

constexpr inline Font::Font(uint8 height, int8 minimumPositionY, const uint8* glyphData, const uint8* glyphAdvanceWidth, const uint8* glyphDescription)
	:	m_height(height),
		m_minimumPositionY(minimumPositionY),
		m_glyphData(glyphData),
		m_glyphAdvanceWidth(glyphAdvanceWidth),
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


constexpr	inline uint8 Font::get_advanceWidth(char character) const
{
	if(isPrintable(character) == false)
	{
		return(0);
	}
	return(m_glyphAdvanceWidth[character - printableStart]);
}


constexpr	inline const Font::s_glyphDescription Font::get_glyph(char character) const
{
	s_glyphDescription glyphDescription{0, Rect(0, 0, 0, 0), nullptr};
	if(isPrintable(character) == false)
	{
		return(glyphDescription);
	}
	
	
	//	Glyph Table starts with the Data for Glyph 0x20
	character -= printableStart;
	
	
	//	Get Glyph Data
	//	They are formatted in the Byte Stream as
	//	Byte-Stream[0] = Bitmap Index MSB
	//	Byte-Stream[1] = Bitmap Index LSB
	//	Byte-Stream[2] = Box Position X
	//	Byte-Stream[3] = Box Position Y
	//	Byte-Stream[4] = Box Size X
	//	Byte-Stream[5] = Box Size Y
	const uint8* glyphDescriptionByteStream = m_glyphDescription + 6 * character;
	
	const uint16 bitmapIndex = (glyphDescriptionByteStream[0] << 8) | glyphDescriptionByteStream[1];
	glyphDescription.data = (uint8*) m_glyphData + bitmapIndex;
	
	glyphDescription.advanceWidth = m_glyphAdvanceWidth[(uint8) character];
	glyphDescription.box.position.x = (int8) glyphDescriptionByteStream[2];
	glyphDescription.box.position.y = (int8) glyphDescriptionByteStream[3];
	glyphDescription.box.size.x = glyphDescriptionByteStream[4];
	glyphDescription.box.size.y = glyphDescriptionByteStream[5];
	
	
	//	Read Bitmap Index (the Index where the Glyph Data starts in the Data Array)
	glyphDescription.data = (uint8*) &m_glyphData[bitmapIndex];
	
	
	return(glyphDescription);
}


constexpr	inline bool Font::isPrintable(char character)
{
	if(character < printableStart || character > printableEnd)
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