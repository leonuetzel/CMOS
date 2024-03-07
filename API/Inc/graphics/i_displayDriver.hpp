#pragma once

#include "rectGraphic.hpp"





class I_DisplayDriver
{
	public:
		
		typedef struct
		{
			int16 h_total;
			int16 h_active;
			int16 h_backporch;
			int16 h_frontporch;
			int16 h_syncwidth;
			int16 v_total;
			int16 v_active;
			int16 v_backporch;
			int16 v_frontporch;
			int16 v_syncwidth;
		}s_displayData;
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		virtual feedback set_thread()																							= 0;
		virtual uint16 get_wakeUpInterrupt()																			= 0;
		
		virtual feedback start()																									= 0;
		virtual feedback stop()																										= 0;
		virtual feedback set_layerBuffer(uint32 layer, Color* buffer)							= 0;
		virtual feedback clear(uint32 layer)																			= 0;
		virtual feedback set_colorBackground(uint8 red, uint8 green, uint8 blue)	= 0;
		virtual feedback set_layerAlpha(uint8 layer, uint8 alpha)									= 0;
		virtual feedback set_pixel(Vec2& position, Color color, uint32 layer)			= 0;
		
		virtual s_displayData& get_displayData()																	= 0;
		virtual Vec2 get_displayDimensions()																			= 0;
		virtual RectGraphic& get_layerData(uint32 layer)													= 0;
		virtual uint32 get_numberOfLayers()																				= 0;
		virtual uint32 get_fps()																									= 0;
		virtual Color get_colorBackground()																				= 0;
};