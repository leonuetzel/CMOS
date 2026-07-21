#pragma once

#include "../defines.hpp"
#include "../pair.hpp"





class I_TouchController
{
	public:
		
		enum class e_touchEvent: uint8
		{
			INVALID		= 0,
			TOUCH			= 1,
			CONTACT		= 2,
			RELEASE		= 3
		};
		
		enum class e_touchGuesture: uint8
		{
			INVALID,
			NONE,
			MOVE_UP,
			MOVE_RIGHT,
			MOVE_DOWN,
			MOVE_LEFT,
			ZOOM_IN,
			ZOOM_OUT
		};
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		virtual e_touchGuesture getTouchGuesture()															= 0;
		virtual uint8 getNumberOfTouchPoints()																	= 0;
		virtual Pair<Vec2, e_touchEvent> getTouchPointInformation(uint8 index)	= 0;
};