// xll_instrument.h - Two row array of floating point time in years and cash flows.
#pragma once
#include "instrument/tmx_instrument.h"
#include "curve/tmx_curve_pwflat.h"
#include "date/tmx_date_business_day.h"
#include "date/tmx_date_day_count.h"
#include "date/tmx_date_periodic.h"
#include "bondxll.h"

#undef interface

namespace xll {

	// Enum string from frequency enum
#define TMX_DATE_FREQUENCY_STRING(a, b, c, d) \
	if (h == tmx::date::frequency::b) return CATEGORY "_FREQUENCY_" #a;
	constexpr const char* frequency_string(tmx::date::frequency h)
	{
		TMX_DATE_FREQUENCY(TMX_DATE_FREQUENCY_STRING)
	else return nullptr;
	}
#undef TMX_FREQUENCY_STRING

	// Enum string from handle
#define TMX_DAY_COUNT_STRING(a, b, c) if (h == to_handle(tmx::date::day_count_##b)) return CATEGORY "_DAY_COUNT_" #a;
	inline const char* day_count_string(HANDLEX h)
	{
		TMX_DAY_COUNT(TMX_DAY_COUNT_STRING)
			return nullptr;
	}
#undef TMX_DAY_COUNT_STRING

	// Enum string from business day roll convention.
#define TMX_DATE_BUSINESS_DAY_STRING(a, b, c) if (h == tmx::date::business_day::roll::b) return CATEGORY "_BUSINESS_DAY_" #a;
	inline const char* business_day_string(tmx::date::business_day::roll h)
	{
		TMX_DATE_BUSINESS_DAY(TMX_DATE_BUSINESS_DAY_STRING)
			return nullptr;
	}
#undef TMX_DATE_BUSINESS_DAY_STRING

	// TMX_DATE_HOLIDAY_CALENDAR(TMX_DATE_HOLIDAY_CALENDAR_STRING)
	// Enum string from calendar.
#define TMX_DATE_HOLIDAY_CALENDAR_STRING(a, b, c) if (h == tmx::date::holiday::calendar::b) return CATEGORY "_HOLIDAY_CALENDAR_" #a;
	inline const char* holiday_calendar_string(tmx::date::holiday::calendar_t h)
	{
		TMX_DATE_HOLIDAY_CALENDAR(TMX_DATE_HOLIDAY_CALENDAR_STRING)
			return nullptr;
	}
#undef TMX_DATE_HOLIDAY_CALENDAR_STRING


	// Instruments are two row array of times and cash flows.
	// Assumes lifetime of x.
	inline auto instrument_iterable(FPX& uc)
	{
		ensure(uc.rows() == 2);

		auto u = fms::iterable::counted(fms::iterable::ptr(begin(uc)), columns(uc));
		auto c = fms::iterable::counted(fms::iterable::ptr(begin(uc) + columns(uc)), columns(uc));

		return tmx::instrument::iterable(u, c);
	}
	// Return value type.
	inline FPX instrument(const FP12& time, const FP12& cash)
	{
		ensure(size(time) == size(cash));
		FPX uc(2, size(time));
		std::copy(begin(time), end(time), uc.array());
		std::copy(begin(cash), end(cash), uc.array() + size(time));

		return uc;
	}
	// Return value type.
	template<class IU, class IC>
	inline FPX instrument(const tmx::instrument::iterable<IU, IC>& i)
	{
		int n = static_cast<int>(size(i));
		FPX uc(2, n);	
		copy(i.time(), uc.array());
		copy(i.cash(), uc.array() + n);

		return uc;
	}

} // namespace xll