// bondxll.h
#pragma once
#include "../bondlib/tmx_date_day_count.h"
#include "../bondlib/tmx_date_business_day.h"
#include "xll24/xll.h"

#undef interface

#ifndef CATEGORY
#define CATEGORY "TMX"
#endif

namespace xll {

// Enum string from frequency enum
#define TMX_DATE_FREQUENCY_STRING(a, b, c, d) if (h == tmx::date::frequency::##b) return CATEGORY "_FREQUENCY_" #a;
	inline const char* frequency_string(tmx::date::frequency h)
	{
		TMX_DATE_FREQUENCY(TMX_DATE_FREQUENCY_STRING)
		return CATEGORY "_FREQUENCY_INVALID";
	}
#undef TMX_FREQUENCY_STRING

	// Enum string from handle
#define TMX_DAY_COUNT_STRING(a, b, c) if (h == to_handle(tmx::date::day_count_##b)) return CATEGORY "_DAY_COUNT_" #a;
	inline const char* day_count_string(HANDLEX h)
	{
		TMX_DAY_COUNT(TMX_DAY_COUNT_STRING)
			return CATEGORY "_DAY_COUNT_INVALID";
	}
#undef TMX_DAY_COUNT_STRING

// Enum string from business day roll convention.
#define TMX_DATE_BUSINESS_DAY_ROLL_STRING(a, b, c) if (h == tmx::date::business_day::roll::##b) return CATEGORY "_BUSINESS_DAY_ROLL_" #a;
	inline const char* business_day_roll_string(tmx::date::business_day::roll h)
	{
		TMX_DATE_BUSINESS_DAY_ROLL(TMX_DATE_BUSINESS_DAY_ROLL_STRING)
			return CATEGORY "_BUSINESS_DAY_INVALID";
	}
#undef TMX_DATE_BUSINESS_DAY_ROLL_STRING

// Enum string from calendar.
#define TMX_DATE_HOLIDAY_CALENDAR_STRING(a, b, c) if (h == to_handle(tmx::date::holiday::calendar::##b)) return CATEGORY "_HOLIDAY_CALENDAR_" #a;
	inline const char* holiday_calendar_string(HANDLEX h)
	{
		TMX_DATE_HOLIDAY_CALENDAR(TMX_DATE_HOLIDAY_CALENDAR_STRING)
			return CATEGORY "_HOLIDAY_CALENDAR_INVALID";
	}
#undef TMX_DATE_HOLIDAY_CALENDAR_STRING

} // namespace xll