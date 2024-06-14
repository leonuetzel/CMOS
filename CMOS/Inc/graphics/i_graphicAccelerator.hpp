#pragma once

#include "../defines.hpp"
#include "rectGraphic.hpp"





class I_GraphicAccelerator
{
	public:
		
		typedef void (*f_callback)();
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		virtual bool is_available() const																																																																															= 0;
		virtual uint16 get_wakeUpInterrupt() const																																																																										= 0;
		
		virtual void draw_rectangleFull(const RectGraphic& output, Color							color,			f_callback					callback = nullptr)																															= 0;
		virtual void draw_rectangleFull(const RectGraphic& output, Color							color,			Rect								rectangle,				f_callback	callback = nullptr)																= 0;
		virtual void draw_rectangleFull(const RectGraphic& output, const RectGraphic& foreground, f_callback					callback = nullptr)																															= 0;
		virtual void draw_rectangleFull(const RectGraphic& output, const RectGraphic& foreground, const RectGraphic&	background,				f_callback	callback = nullptr)																= 0;
		virtual void draw_rectangleFull(const RectGraphic& output, const Rect&				foreground, const RectGraphic&	background,				Color				color_foreground,	f_callback callback = nullptr)	= 0;
		virtual void draw_rectangleFull(const RectGraphic& output, const RectGraphic&	foreground, const Rect&					background,				Color				color_background,	f_callback callback = nullptr)	= 0;
		
		virtual void cutOut(const RectGraphic& destination, const RectGraphic& source, f_callback callback = nullptr)																																									= 0;
};