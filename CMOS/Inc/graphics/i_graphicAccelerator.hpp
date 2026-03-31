#pragma once

#include "../defines.hpp"
#include "rectGraphic.hpp"





class I_GraphicAccelerator
{
	public:
		
		typedef void (*f_callback)();
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		//	Colors the whole output buffer with a single color
		virtual void fillRectangleWithSingleColor(const RectGraphic& output, Color color) = 0;
		
		//	Colors a rectangle with dimensions "rectangle" in the output buffer with a single color
		//	The position of the rectangle is related to the output buffers coordinate system
		virtual void drawfilledRectangleWithSingleColor(const RectGraphic& output, Color color, Rect rectangle) = 0;
		
		//	Copies the whole foreground buffer to the output buffer by placing the foreground rectangle inside the output rectangle
		//	The position of the foreground rectangle is related to the output buffers coordinate system
		virtual void copyForegroundToOutput(const RectGraphic& output, const RectGraphic& foreground, uint8 alphaFactor = 0xFF) = 0;
		
		//	Copies the specified rectangle from the foreground buffer to the specified position in the output buffer
		//	The position of the foreground rectangle and the position in the output buffer is related to the output buffers coordinate system
		virtual void copyForegroundRectangleToOutputPosition(const RectGraphic& output, const RectGraphic& foreground, Rect rectangle, Vec2 position, uint8 alphaFactor = 0xFF) = 0;
		
		//	Blends the foreground buffer with the background buffer and copies the result to the output buffer
		//	The alpha factors can be used to set the transparency of the foreground and background buffers. 0xFF means fully opaque, 0x00 means fully transparent.
		//	The position of the foreground and background rectangles is related to the output buffers coordinate system
		virtual void blendForegroundAndBackgroundToOutput(const RectGraphic& output, const RectGraphic& foreground, const RectGraphic& background, uint8 alphaFactorForeground = 0xFF, uint8 alphaFactorBackground = 0xFF) = 0;
		
		//	Blends the foreground rectangle (constant color) with the background buffer and copies the result to the output buffer
		//	The position of the foreground and background rectangles is related to the output buffers coordinate system
		virtual void blendBackgroundWithConstantForegroundToOutput(const RectGraphic& output, const Rect& foreground, const RectGraphic& background, Color color_foreground) = 0;
		
		//	Blends the foreground buffer with the background rectangle (constant color) and copies the result to the output buffer
		//	The position of the foreground and background rectangles is related to the output buffers coordinate system
		virtual void blendForegroundWithConstantBackgroundToOutput(const RectGraphic& output, const RectGraphic& foreground, const Rect& background, Color color_background) = 0;
};