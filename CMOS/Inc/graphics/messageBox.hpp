#pragma once

#include "graphicsManager.hpp"
#include "button.hpp"





class MessageBox
{
	public:
		
		enum class e_type
		{
			OK_,
			OK_CANCEL,
			RETRY_CANCEL,
			RETRY_CANCEL_CONTINUE,
			YES_NO,
			YES_NO_CANCEL
		};
		
		enum class e_button
		{
			OK_				= 0,
			CANCEL		= 1,
			YES				= 2,
			NO				= 3,
			RETRY			= 4,
			CONTINUE	= 5,
			NONE			= 6,
		};
		
		
		
		
		
	private:
		
		//	Static Member
		static constexpr uint32 c_pressTime = 25;
		static MessageBox* m_messageBox;
		
		
		//	Non-static Member
		const String m_message;
		Button* m_buttons[6];
		Element* m_text;
		e_button m_userChoice;
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		static void update(Element& element);
		static void callback(Element& element);
		
		
		//	Friends
		
		
		
		
		
		
	public:
		
		MessageBox(e_type type, const String& message);
		~MessageBox();
		
		e_button getUserChoice();
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

