#pragma once

#include "defines.hpp"





class I_Flash
{
	public:
		
		
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	protected:
		
		constexpr inline I_Flash();
		inline ~I_Flash();
		
		
		
		
		
	public:
		
		virtual feedback writePage(uint32* data, uint32 pageNumber)	= 0;
		virtual feedback erase(uint32 pageNumber)										= 0;
		virtual feedback erase()																		= 0;
		
		virtual uint32 get_pageSize() const													= 0;
		virtual uint32 get_numberOfPages() const										= 0;
		virtual uint32 get_size() const															= 0;
		virtual uint32 get_smallestProgrammableBlockSize() const		= 0;
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

constexpr inline I_Flash::I_Flash()
{
	
}


inline I_Flash::~I_Flash()
{
	
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

