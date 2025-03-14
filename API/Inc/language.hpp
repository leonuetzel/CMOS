#pragma once

#include "defines.hpp"





class Language
{
	public:
		
		enum class e_language: uint8
		{
			ENGLISH,
			GERMAN,
			FRANCAIS
		};
		
		static constexpr uint32 c_numberOfLanguages = (uint32) e_language::FRANCAIS + 1;
		
		
		
		
		
	private:
		
		e_language m_language;
		
		
		
		
		
	public:
		
		
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

