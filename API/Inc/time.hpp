#pragma once

#include "defines.hpp"





class Time
{
	public:
		
		enum class e_weekday: uint8
		{
			INVALID						= 0x0,
			MONDAY						= 0x1,
			TUESDAY						= 0x2,
			WEDNESDAY					= 0x3,
			THURSDAY					= 0x4,
			FRIDAY						= 0x5,
			SATURDAY					= 0x6,
			SUNDAY						= 0x7
		};
		
		static constexpr uint32 c_numberOfWeekdays = (uint32) e_weekday::SUNDAY + 1;
		
		uint8 second;
		uint8 minute;
		uint8 hour;
		uint8 day;
		uint8 month;
		uint16 year;
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		constexpr inline Time();
		constexpr inline Time(uint32 seconds);
		constexpr inline Time(uint8 _second, uint8 _minute, uint32 _hour, uint8 _day, uint8 _month, uint16 _year);
		
		e_weekday get_weekday() const;
		
		constexpr inline Time operator++(int seconds);
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

constexpr inline Time::Time()
	:	second	(0),
		minute	(0),
		hour		(0),
		day			(1),
		month		(1),
		year		(2000)
{
	
}


constexpr inline Time::Time(uint32 seconds)
	:	second(seconds % 60),
		minute((seconds / 60) % 60),
		hour(seconds / 3600),
		day(0),
		month(0),
		year(0)
{
	
}


constexpr inline Time::Time(uint8 _second, uint8 _minute, uint32 _hour, uint8 _day, uint8 _month, uint16 _year)
	:	second(_second),
		minute(_minute),
		hour(_hour),
		day(_day),
		month(_month),
		year(_year)
{
	
}







constexpr inline Time Time::operator++(int seconds)
{
	second++;
	if(second >= 60)
	{
		second = 0;
		minute++;
		if(minute >= 60)
		{
			minute = 0;
			hour++;
			if(hour >= 24)
			{
				hour = 0;
				day++;
				uint8 day_max = 31;
				if(month == 4 || month == 6 || month == 9 || month == 11)
				{
					day_max = 30;
				}
				if(month == 2)
				{
					if(year % 4 != 0)
					{
						day_max = 29;
					}
					else
					{
						day_max = 28;
					}
				}
				if(day >= day_max)
				{
					day = 1;
					month++;
					if(month >= 12)
					{
						month = 1;
						year++;
					}
				}
			}
		}
	}
	return(*this);
}