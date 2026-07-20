#pragma once

#include "i_displayDriver.hpp"





namespace Displays
{
	constexpr I_DisplayDriver::s_displayData NEWHEAVEN_NHD_5_0_800480AF_ASXP =
	{
		.h_active									=  800,
		.h_backporch							=    4,
		.h_frontporch							=    8,
		.h_syncwidth							=    4,
		
		.v_active									=  480,
		.v_backporch							=    4,
		.v_frontporch							=    8,
		.v_syncwidth							=    4,
		
		.pixelClockFrequency			= 25000000,
		
		.hSyncActiveHigh 					= true,
		.vSyncActiveHigh 					= true,
		.notDataEnableActiveHigh	= false,
		.pixelClockActiveHigh			= true
	};
	
	
	constexpr I_DisplayDriver::s_displayData NEWHEAVEN_NHD_7_0_800480AF_ASXP =
	{
		.h_active									=  800,
		.h_backporch							=    8,
		.h_frontporch							=    8,
		.h_syncwidth							=    4,
		
		.v_active									=  480,
		.v_backporch							=   15,
		.v_frontporch							=   16,
		.v_syncwidth							=    4,
		
		.pixelClockFrequency			= 25000000,
		
		.hSyncActiveHigh 					= true,
		.vSyncActiveHigh 					= true,
		.notDataEnableActiveHigh	= true,
		.pixelClockActiveHigh			= false
	};
	
	
	constexpr I_DisplayDriver::s_displayData NEWHEAVEN_NHD_10_0_1024600BF_ASXP =
	{
		.h_active									= 1024,
		.h_backporch							=   60,
		.h_frontporch							=  160,
		.h_syncwidth							=  100,
		
		.v_active									=  600,
		.v_backporch							=   13,
		.v_frontporch							=   12,
		.v_syncwidth							=   10,
		
		.pixelClockFrequency			= 42000000,
		
		.hSyncActiveHigh 					= false,
		.vSyncActiveHigh 					= false,
		.notDataEnableActiveHigh	= false,
		.pixelClockActiveHigh			= false
	};
};