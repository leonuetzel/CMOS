#pragma once

#include "../cmos.hpp"





class Icon
{
	public:
		
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		const Vec2 m_size;
		const Color* m_data;
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		constexpr inline Icon(Vec2 size, const Color* data);
		
		constexpr const Color* get_data() const;
		constexpr inline Vec2 get_size() const;
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

constexpr inline Icon::Icon(Vec2 size, const Color* data)
	:	m_size(size),
		m_data(data)
{
	
}







constexpr	const Color* Icon::get_data() const
{
	return(m_data);
}


constexpr inline Vec2 Icon::get_size() const
{
	return(m_size);
}