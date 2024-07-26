#include "date/tmx_date_day_count.h"
#include "date/tmx_date_business_day.h"
#include "date/tmx_date_holiday_calendar.h"
#include "date/tmx_date_periodic.h"
#include "bondxll.h"

// TODO: Define enums as constants. Use xlfEvaluate instead of X-Macros.

using namespace tmx;
using namespace xll;

// Day count conventions
#define DAY_COUNT_URL "https://en.wikipedia.org/wiki/Day_count_convention"

//XLL_CONST(HANDLEX, TMX_DAY_COUNT_ACTUAL_ACTUAL, safe_handle(date::day_count_isdaactualactual), "Actual days per year.", CATEGORY " Enum", BDE_URL  ".cpp")
#define TMX_DAY_COUNT_ENUM(a, b, c) XLL_CONST(HANDLEX, TMX_DAY_COUNT_##a, safe_handle(date::day_count_##b), c, CATEGORY " Enum", DAY_COUNT_URL)
TMX_DAY_COUNT(TMX_DAY_COUNT_ENUM)
#undef TMX_DAY_COUNT_ENUM

// All day count fractions as string names.
#define TMX_DAY_COUNT_ENUM(a, b, c) OPER("TMX_DAY_COUNT_" #a),
OPER tmx_day_count_enum({
	TMX_DAY_COUNT(TMX_DAY_COUNT_ENUM)
});
#undef TMX_DAY_COUNT_ENUM

XLL_CONST(LPOPER, TMX_DAY_COUNT_ENUM, &tmx_day_count_enum, "Day count fraction types.", CATEGORY " Enum", DAY_COUNT_URL)

#define TMX_DATE_FREQUENCY_ENUM(a, b, c, d) XLL_CONST(WORD, TMX_FREQUENCY_##a, (WORD)date::frequency::b, d, CATEGORY " Enum", "")
TMX_DATE_FREQUENCY(TMX_DATE_FREQUENCY_ENUM)
#undef TMX_DATE_FREQUENCY_ENUM

#define TMX_DATE_FREQUENCY_ENUM(a, b, c, d) OPER("TMX_FREQUENCY_" #a),
OPER tmx_frequency_enum({
	TMX_DATE_FREQUENCY(TMX_DATE_FREQUENCY_ENUM)
	});
#undef TMX_DATE_FREQUENCY_ENUM

XLL_CONST(LPOPER, TMX_FREQUENCY_ENUM, &tmx_frequency_enum, "Payment frequencies.", CATEGORY " Enum", "https://www.investopedia.com/terms/c/compounding.asp")

// Roll conventions
#define BUSINESS_DAY_URL "https://en.wikipedia.org/wiki/Date_rolling"

#define TMX_DATE_BUSINESS_DAY_ENUM(a, b, c) XLL_CONST(WORD, TMX_BUSINESS_DAY_##a, (WORD)date::business_day::roll::b, c, CATEGORY " Enum", BUSINESS_DAY_URL)
TMX_DATE_BUSINESS_DAY(TMX_DATE_BUSINESS_DAY_ENUM)
#undef TMX_DATE_BUSINESS_DAY_ENUM

// All roll conventions as string names.
#define TMX_DATE_BUSINESS_DAY_ENUM(a, b, c) OPER("TMX_BUSINESS_DAY_" #a),
OPER tmx_date_business_day_enum({
	TMX_DATE_BUSINESS_DAY(TMX_DATE_BUSINESS_DAY_ENUM)
	});
#undef TMX_DATE_BUSINESS_DAY_ENUM

XLL_CONST(LPOPER, TMX_BUSINESS_DAY_ENUM, &tmx_date_business_day_enum, "Business day roll conventions.", CATEGORY " Enum", BUSINESS_DAY_URL)

// Calendars
#define HOLIDAY_CALENDAR_URL "https://www.sifma.org/resources/general/holiday-schedule/#us"

#define TMX_DATE_HOLIDAY_CALENDAR_ENUM(a, b, c) XLL_CONST(HANDLEX, TMX_HOLIDAY_CALENDAR_##a, safe_handle(date::holiday::calendar::b), c, CATEGORY " Enum", HOLIDAY_CALENDAR_URL)
TMX_DATE_HOLIDAY_CALENDAR(TMX_DATE_HOLIDAY_CALENDAR_ENUM)
#undef TMX_DATE_HOLIDAY_CALENDAR_ENUM

// All calendars as string names.
#define TMX_DATE_HOLIDAY_CALENDAR_ENUM(a, b, c) OPER("TMX_HOLIDAY_CALENDAR_" #a),
OPER tmx_holiday_calendar_enum({
	TMX_DATE_HOLIDAY_CALENDAR(TMX_DATE_HOLIDAY_CALENDAR_ENUM)
	});
#undef TMX_DATE_HOLIDAY_CALENDAR_ENUM

XLL_CONST(LPOPER, TMX_HOLIDAY_CALENDAR_ENUM, &tmx_holiday_calendar_enum, "Holiday calendars.", CATEGORY " Enum", HOLIDAY_CALENDAR_URL)

AddIn xai_date_holiday_calendar(
	Function(XLL_LPOPER, "xll_date_holiday_calendar", CATEGORY ".DATE.HOLIDAY")
	.Arguments({
		Arg(XLL_HANDLEX, "calendar", "is a TMX_HOLIDAY_CALENDAR_* enumeration.", "=ENUM(\"TMX_HOLIDAY_CALENDAR_WEEKEND\")"),
		Arg(XLL_DOUBLE, "date", "is an Excel date.", "=TODAY()"),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return true if date is in the holiday calendar.")
);
LPOPER WINAPI xll_date_holiday_calendar(HANDLEX calendar, double d)
{
#pragma XLLEXPORT
	static OPER result;

	try {
		date::holiday::calendar::calendar_t _calendar = reinterpret_cast<date::holiday::calendar::calendar_t>(safe_pointer<date::holiday::calendar::calendar_t>(calendar));
		ensure(_calendar);

		date::ymd y = to_days(d);

		result = _calendar(y);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		result = ErrValue;
	}

	return &result;
}

AddIn xai_date_periodic(
	Function(XLL_FP, "xll_date_periodic", CATEGORY ".DATE.PERIODIC")
	.Arguments({
		Arg(XLL_INT, "frequency", "is a payment frequency. Default is semiannual."),
		Arg(XLL_DOUBLE, "from", "is the date prior to first periodic date."),
		Arg(XLL_DOUBLE, "to", "is the last date."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return periodic dates after from ending at to.")
);
_FP12* WINAPI xll_date_periodic(date::frequency f, double d0, double d1)
{
#pragma XLLEXPORT
	static xll::FPX result;

	try {
		result.resize(0, 0);
		date::ymd y0 = to_days(d0);
		date::ymd y1 = to_days(d1);

		auto p = date::periodic(f, y0, y1);

		int n = static_cast<int>(p.size());
		if (n == 0) {
			return nullptr;
		}
		result.resize(n, 1);
		for (int i = 0; i < n; ++i) {
			result[i] = from_days(*p++);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return nullptr;
	}

	return result.get();
}

AddIn xai_date_addyears(
	Function(XLL_DOUBLE, "xll_date_addyears", CATEGORY ".DATE.ADDYEARS")
	.Arguments({
		Arg(XLL_DOUBLE, "date", "is an Excel date.", "=TODAY()"),
		Arg(XLL_DOUBLE, "years", "is the number of years to add.", 10),
		})
		.Category(CATEGORY)
	.FunctionHelp("Add years to date. ADDYEARS(d0, DIFFYEARS(d1, d0)) = d1")
);
double WINAPI xll_date_addyears(double d, double y)
{
#pragma XLLEXPORT

	return d + y*date::seconds_per_year/86400;
}

AddIn xai_date_diffyears(
	Function(XLL_DOUBLE, "xll_date_diffyears", CATEGORY ".DATE.DIFFYEARS")
	.Arguments({
		Arg(XLL_DOUBLE, "d1", "is an Excel date."),
		Arg(XLL_DOUBLE, "d0", "is an Excel date."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return the difference of d1 and d0 in years. DIFFYEARS(ADDYEARS(d, y), d)) = y.")
);
double WINAPI xll_date_diffyears(double d1, double d0)
{
#pragma XLLEXPORT

	return (d1 - d0)*86400/date::seconds_per_year;
}

AddIn xai_date_addyear(
	Function(XLL_DOUBLE, "xll_date_addyear", CATEGORY ".DATE.ADDYEAR")
	.Arguments({
		Arg(XLL_DOUBLE, "date", "is an Excel date."),
		Arg(XLL_INT, "years", "is the number of years to add."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Add years to date.")
);
double WINAPI xll_date_addyear(double d, int y)
{
#pragma XLLEXPORT

	date::ymd t = to_days(d);
	t += std::chrono::years(y);

	return from_days(t);
}

AddIn xai_date_addmonth(
	Function(XLL_DOUBLE, "xll_date_addmonth", CATEGORY ".DATE.ADDMONTH")
	.Arguments({
		Arg(XLL_DOUBLE, "date", "is an Excel date."),
		Arg(XLL_INT, "months", "is the number of months to add."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Add months to date.")
);
double WINAPI xll_date_addmonth(double d, int y)
{
#pragma XLLEXPORT

	date::ymd t = to_days(d);
	t += std::chrono::months(y);

	return from_days(t);
}

AddIn xai_date_dcf(
	Function(XLL_DOUBLE, "xll_date_dcf", CATEGORY ".DATE.DAY_COUNT")
	.Arguments({
		Arg(XLL_DOUBLE, "from", "is an Excel date.", "1/1/2024"),
		Arg(XLL_DOUBLE, "to", "is an Excel date.", "7/1/2024"),
		Arg(XLL_HANDLEX, "dcb", "is a day count basis. Default is 30/360.", "=ENUM(\"TMX_DAY_COUNT_30_360\")"),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return the day count fraction from d0 to d1.")
);
double WINAPI xll_date_dcf(double d0, double d1, HANDLEX dcf)
{
#pragma XLLEXPORT
	double result = std::numeric_limits<double>::quiet_NaN();

	try {
		if (!dcf) {
			dcf = safe_handle(&date::day_count_isma30360);
		}
		date::day_count_t _dcf = reinterpret_cast<date::day_count_t>(safe_pointer<date::day_count_t>(dcf));
		ensure(_dcf);

		date::ymd y0 = to_days(d0);
		date::ymd y1 = to_days(d1);

		result = _dcf(y1, y0);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}
