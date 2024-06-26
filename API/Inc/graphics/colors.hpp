#pragma once

#include "color.hpp"





namespace Colors
{
	constexpr Color red										(0xFF, 0x00, 0x00, 0xFF);
	constexpr Color green									(0x00, 0xFF, 0x00, 0xFF);
	constexpr Color blue									(0x00, 0x00, 0xFF, 0xFF);
	constexpr Color yellow								(0xFF, 0xFF, 0x00, 0xFF);
	constexpr Color orange								(0xFF, 0xAA, 0x00, 0xFF);
	constexpr Color grey									(0x80, 0x80, 0x80, 0xFF);
	constexpr Color brown									(0x59, 0x35, 0x1F, 0xFF);
	constexpr Color violet								(0x80, 0x00, 0x80, 0xFF);
	constexpr Color pink									(0xFF, 0x00, 0xFF, 0xFF);
	constexpr Color olive									(0x80, 0x80, 0x00, 0xFF);
	constexpr Color silver								(0xC0, 0xC0, 0xC0, 0xFF);
	constexpr Color aqua									(0x00, 0xFF, 0xFF, 0xFF);
	constexpr Color navy									(0x00, 0x00, 0x80, 0xFF);
	constexpr Color white									(0xFF, 0xFF, 0xFF, 0xFF);
	constexpr Color black									(0x00, 0x00, 0x00, 0xFF);
	constexpr Color medium_aquamarine			(0x66, 0xCD, 0xAA, 0xFF);
	constexpr Color royal_blue						(0x41, 0x69, 0xE1, 0xFF);
	constexpr Color sky_blue							(0x87, 0xCE, 0xEB, 0xFF);
	constexpr Color slate_blue						(0x6A, 0x5A, 0xCD, 0xFF);
	constexpr Color aquamarine						(0x7F, 0xFF, 0xD4, 0xFF);
	constexpr Color turquoise							(0x40, 0xE0, 0xD0, 0xFF);
	constexpr Color sandy_brown						(0xF4, 0xA4, 0x60, 0xFF);
	constexpr Color beige									(0xF5, 0xF5, 0xDC, 0xFF);
	constexpr Color chocolate							(0xD2, 0x69, 0x1E, 0xFF);
	constexpr Color peru									(0xCD, 0x85, 0x3F, 0xFF);
	constexpr Color tan										(0xD2, 0xB4, 0x8C, 0xFF);
	constexpr Color dark_slate_grey				(0x2F, 0x4F, 0x4F, 0xFF);
	constexpr Color slate_grey						(0x70, 0x80, 0x90, 0xFF);
	constexpr Color dark_green						(0x00, 0x64, 0x00, 0xFF);
	constexpr Color dark_khaki						(0xBD, 0xB7, 0x6B, 0xFF);
	constexpr Color forest_green					(0x22, 0x8B, 0x22, 0xFF);
	constexpr Color green_yellow					(0xAD, 0xFF, 0x2F, 0xFF);
	constexpr Color lawn_green						(0x7C, 0xFC, 0x00, 0xFF);
	constexpr Color light_sea_green				(0x20, 0xB2, 0xAA, 0xFF);
	constexpr Color lime_green						(0x32, 0xCD, 0x32, 0xFF);
	constexpr Color medium_spring_green		(0x00, 0xFA, 0x9A, 0xFF);
	constexpr Color chartreuse						(0x7F, 0xFF, 0x00, 0xFF);
	constexpr Color dark_orange						(0xFF, 0x8C, 0x00, 0xFF);
	constexpr Color light_coral						(0xF0, 0x80, 0x80, 0xFF);
	constexpr Color peach_puff						(0xFF, 0xDA, 0xB9, 0xFF);
	constexpr Color coral									(0xFF, 0x7F, 0x50, 0xFF);
	constexpr Color salmon								(0xFA, 0x80, 0x72, 0xFF);
	constexpr Color dark_sienna						(0xA0, 0x52, 0x2D, 0xFF);
	constexpr Color sienna								(0xFF, 0x82, 0x47, 0xFF);
	constexpr Color deep_pink							(0xFF, 0x14, 0x93, 0xFF);
	constexpr Color hot_pink							(0xFF, 0x69, 0xB4, 0xFF);
	constexpr Color medium_violet_red			(0xC7, 0x15, 0x85, 0xFF);
	constexpr Color orange_red						(0xFF, 0x45, 0x00, 0xFF);
	constexpr Color violet_red						(0xD0, 0x20, 0x90, 0xFF);
	constexpr Color firebrick							(0xB2, 0x22, 0x22, 0xFF);
	constexpr Color tomato								(0xFF, 0x63, 0x47, 0xFF);
	constexpr Color dark_magenta					(0x8B, 0x00, 0x8B, 0xFF);
	constexpr Color dark_orchid						(0x99, 0x32, 0xCC, 0xFF);
	constexpr Color medium_orchid					(0xBA, 0x55, 0xD3, 0xFF);
	constexpr Color dark_violet						(0x94, 0x00, 0xD3, 0xFF);
	constexpr Color lavender							(0xE6, 0xE6, 0xFA, 0xFF);
	constexpr Color maroon								(0xB0, 0x30, 0x60, 0xFF);
	constexpr Color purple								(0xA0, 0x20, 0xF0, 0xFF);
	constexpr Color dark_purple						(0x7D, 0x26, 0xCD, 0xFF);
	constexpr Color navajo_white					(0xFF, 0xDE, 0xAD, 0xFF);
	constexpr Color wheat									(0xF5, 0xDE, 0xB3, 0xFF);
	constexpr Color dark_goldenrod				(0xB8, 0x86, 0x0B, 0xFF);
	constexpr Color light_goldenrod				(0xEE, 0xDD, 0x82, 0xFF);
	constexpr Color gold									(0xFF, 0xD7, 0x00, 0xFF);
	constexpr Color moccassin							(0xFF, 0xE4, 0xB5, 0xFF);
	constexpr Color sixteenth_transparent	(0x00, 0x00, 0x00, 0xEF);
	constexpr Color eigth_transparent			(0x00, 0x00, 0x00, 0xDF);
	constexpr Color quarter_transparent		(0x00, 0x00, 0x00, 0xBF);
	constexpr Color half_transparent			(0x00, 0x00, 0x00, 0x7F);
	constexpr Color transparent						(0x00, 0x00, 0x00, 0x00);
};