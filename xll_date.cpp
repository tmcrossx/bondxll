#include "../bondlib/tmx_date_day_count.h"
#include "../bondlib/tmx_date_holiday_calendar.h"
#include "bondxll.h"
#include "xll24/excel_clock.h"

using namespace tmx;
using namespace xll;

#define BDE_URL "https://github.com/bloomberg/bde/blob/main/groups/bbl/bbldc/bbldc_basic"

//XLL_CONST(HANDLEX, TMX_DAY_COUNT_ACTUAL_ACTUAL, safe_handle(date::day_count_isdaactualactual), "Actual days per year.", CATEGORY " Enum", BDE_URL  ".cpp")
#define TMX_DAY_COUNT_ENUM(a, b, c) XLL_CONST(HANDLEX, TMX_DAY_COUNT_##a, safe_handle(date::day_count_##b), c, CATEGORY " Enum", BDE_URL #b ".cpp")
TMX_DAY_COUNT(TMX_DAY_COUNT_ENUM)
#undef TMX_DAY_COUNT_ENUM

// All day count fractions as string names.
#define TMX_DAY_COUNT_ENUM(a, b, c) OPER("TMX_DAY_COUNT_" #a),
OPER tmx_day_count_enum({
	TMX_DAY_COUNT(TMX_DAY_COUNT_ENUM)
});
#undef TMX_DAY_COUNT_ENUM

XLL_CONST(LPOPER, TMX_DAY_COUNT_ENUM, &tmx_day_count_enum, "Day count fraction types.", CATEGORY " Enum", BDE_URL)

#define TMX_DATE_FREQUENCY_ENUM(a, b, c, d) XLL_CONST(WORD, TMX_FREQUENCY_##a, (WORD)tmx::date::frequency::b, d, CATEGORY " Enum", "")
TMX_DATE_FREQUENCY(TMX_DATE_FREQUENCY_ENUM)
#undef TMX_DATE_FREQUENCY_ENUM

#define TMX_DATE_FREQUENCY_ENUM(a, b, c, d) OPER("TMX_FREQUENCY_" #a),
OPER tmx_frequency_enum({
	TMX_DATE_FREQUENCY(TMX_DATE_FREQUENCY_ENUM)
	});
#undef TMX_DATE_FREQUENCY_ENUM

XLL_CONST(LPOPER, TMX_FREQUENCY_ENUM, &tmx_frequency_enum, "Payment frequencies.", CATEGORY " Enum", "https://www.investopedia.com/terms/c/compounding.asp")

// Roll conventions
//TODO: #define SIFMA_URL "https://www.sifma.org/resources/general/holiday-schedule/#us"

#define TMX_DATE_BUSINESS_DAY_ROLL_ENUM(a, b, c) XLL_CONST(WORD, TMX_BUSINESS_DAY_ROLL_##a, (WORD)date::business_day::roll::b, c, CATEGORY " Enum", "")
TMX_DATE_BUSINESS_DAY_ROLL(TMX_DATE_BUSINESS_DAY_ROLL_ENUM)
#undef TMX_DATE_BUSINESS_DAY_ROLL_ENUM

// All roll conventions as string names.
#define TMX_DATE_BUSINESS_DAY_ROLL_ENUM(a, b, c) OPER("TMX_BUSINESS_DAY_ROLL_" #a),
OPER tmx_date_business_day_enum({
	TMX_DATE_BUSINESS_DAY_ROLL(TMX_DATE_BUSINESS_DAY_ROLL_ENUM)
	});
#undef TMX_DATE_BUSINESS_DAY_ROLL_ENUM

// Calendars
#define SIFMA_URL "https://www.sifma.org/resources/general/holiday-schedule/#us"

#define TMX_DATE_HOLIDAY_CALENDAR_ENUM(a, b, c) XLL_CONST(HANDLEX, TMX_HOLIDAY_CALENDAR_##a, safe_handle(date::holiday::calendar::b), c, CATEGORY " Enum", SIFMA_URL)
TMX_DATE_HOLIDAY_CALENDAR(TMX_DATE_HOLIDAY_CALENDAR_ENUM)
#undef TMX_DATE_HOLIDAY_CALENDAR_ENUM

// All calendars as string names.
#define TMX_DATE_HOLIDAY_CALENDAR_ENUM(a, b, c) OPER("TMX_HOLIDAY_CALENDAR_" #a),
OPER tmx_holiday_calendar_enum({
	TMX_DATE_HOLIDAY_CALENDAR(TMX_DATE_HOLIDAY_CALENDAR_ENUM)
	});
#undef TMX_DATE_HOLIDAY_CALENDAR_ENUM

XLL_CONST(LPOPER, TMX_HOLIDAY_CALENDAR_ENUM, &tmx_holiday_calendar_enum, "Holiday calendars.", CATEGORY " Enum", BDE_URL)

AddIn xai_date_holiday_calendar(
	Function(XLL_LPOPER, "xll_date_holiday_calendar", CATEGORY ".DATE.HOLIDAY.CALENDAR")
	.Arguments({
		Arg(XLL_HANDLEX, "calendar", "is a holiday calendar."),
		Arg(XLL_DOUBLE, "date", "is an Excel date."),
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

AddIn xai_date_addyears(
	Function(XLL_DOUBLE, "xll_date_addyears", CATEGORY ".DATE.ADDYEARS")
	.Arguments({
		Arg(XLL_DOUBLE, "date", "is an Excel date."),
		Arg(XLL_DOUBLE, "years", "is the number of years to add."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Add years to date.")
);
double WINAPI xll_date_addyears(double d, double y)
{
#pragma XLLEXPORT

	return d + y*date::days_per_year;
}

AddIn xai_date_diffyears(
	Function(XLL_DOUBLE, "xll_date_diffyears", CATEGORY ".DATE.DIFFYEARS")
	.Arguments({
		Arg(XLL_DOUBLE, "d1", "is an Excel date."),
		Arg(XLL_DOUBLE, "d0", "is an Excel date."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return d1 - d0 in years.")
);
double WINAPI xll_date_diffyears(double d1, double d0)
{
#pragma XLLEXPORT

	return (d1 - d0)/date::days_per_year;
}

AddIn xai_date_dcf(
	Function(XLL_DOUBLE, "xll_date_dcf", CATEGORY ".DATE.DAY_COUNT")
	.Arguments({
		Arg(XLL_DOUBLE, "d0", "is an Excel date."),
		Arg(XLL_DOUBLE, "d1", "is an Excel date."),
		Arg(XLL_HANDLEX, "dcf", "is a day count fraction. Default is 30/360."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return day count fraction from d0 to d1.")
);
double WINAPI xll_date_dcf(double d0, double d1, HANDLEX dcf)
{
#pragma XLLEXPORT
	double result = INVALID_HANDLEX;

	try {
		if (!dcf) {
			dcf = safe_handle(&date::day_count_isma30360);
		}
		date::day_count_t _dcf = reinterpret_cast<date::day_count_t>(safe_pointer<date::day_count_t>(dcf));
		ensure(_dcf);

		date::ymd y0 = to_days(d0);
		date::ymd y1 = to_days(d1);

		result = _dcf(y0, y1);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}
