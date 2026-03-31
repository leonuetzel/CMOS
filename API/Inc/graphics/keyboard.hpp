#pragma once

#include "graphicsManager.hpp"
#include "button.hpp"





class Keyboard
{
	public:
		
		enum class e_key
		{
			NONE,
			LAYOUT,
			ENTER,
			SHIFT,
			LETTER,
			NUMBER,
			SIGN,
			DELETE
		};
		
		
		
		
		
	private:
		
		//	Static Member
		enum class e_caseMode
		{
			LOWER_CASE,
			UPPER_CASE,
			FIRST_UPPER
		};
		
		typedef struct
		{
			Button* button;
			Rectangle<float> area;
			String primary_text;
			String secondary_text;
			e_key primary_ID;
			e_key secondary_ID;
		}Key;
		
		static const Key c_layout[44];
		static UniqueArray<Keyboard*> m_keyboards;
		
		
		//	Non-static Member
		const uint8 m_pageOnCreation;
		const uint8 m_page;
		Array<Key> m_keys;
		Element* m_text;
		String m_buffer;
		e_caseMode m_caseMode;
		bool m_upperCase;
		bool m_primaryMode;
		bool m_enterPressed;
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		static void update(Element& element);
		static void callback(Element& element);
		
		void switchCase();
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		Keyboard();
		Keyboard(const Keyboard& keyboard) = delete;
		~Keyboard();
		
		String operator()(bool showNow = true, bool waitForEnter = true);
		
		void show();
		void hide();
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

