// bondxll.h
#pragma once
#include "../bondlib/tmx_date_day_count.h"
#include "xll24/xll.h"

#undef interface

#ifndef CATEGORY
#define CATEGORY "TMX"
#endif

namespace xll {

// TMX_DAY_COUNT_, date::day_count_t, description
#define TMX_DAY_COUNT(X) \
	X(30_360, isma30360, "Each year is assumed to have 12 months and 360 days, with each month consisting of exactly 30 days.") \
	X(30E_360, isma30360eom, "Each year is assumed to have 12 months and 360 days, with each month consisting of exactly 30 days. The end-of-month rule is applied.") \
	X(ACTUAL_ACTUAL, isdaactualactual, "Actual days per year.") \
	X(ACTUAL_360, actual360, "Actual days divided by 360.") \
	X(ACTUAL_365, actual365fixed, "Actual days divided by 365.") \

// Enum string from handle
#define TMX_DAY_COUNT_STRING(a, b, c) if (h == to_handle(tmx::date::day_count_##b)) return CATEGORY "_DAY_COUNT_" #a;
	inline const char* day_count_string(HANDLEX h)
	{
		TMX_DAY_COUNT(TMX_DAY_COUNT_STRING)
		return CATEGORY "_DAY_COUNT_INVALID";
	}
#undef TMX_DAY_COUNT_STRING

// TMX_FREQUENCY_, date::frequency, description
#define TMX_FREQUENCY(X) \
	X(ANNUALLY, annually, "Yearly payments.") \
	X(SEMIANNUALLY, semiannually, "2 payments per year.") \
	X(QUARTERLY, quarterly, "4 payments per year.") \
	X(MONTHLY, monthly, "12 payments per year.") \


// Enum string from frequency enum
#define TMX_FREQUENCY_STRING(a, b, c) if (h == tmx::date::frequency::##b) return CATEGORY "_FREQUENCY_" #a;
	inline const char* frequency_string(tmx::date::frequency h)
	{
		TMX_FREQUENCY(TMX_FREQUENCY_STRING)
		return CATEGORY "_FREQUENCY_INVALID";
	}
#undef TMX_FREQUENCY_STRING

} // namespace xll