#include "../bondlib/tmx_date_day_count.h"
#include "bondxll.h"

using namespace tmx;
using namespace xll;

#define BDE_URL "https://github.com/bloomberg/bde/blob/main/groups/bbl/bbldc/bbldc_basic"

// TODO: (Tianxin) add all the day count fraction types

// TMX_DAY_COUNT, date::day_count_t, description, BDE file
#define TMX_DAY_COUNT(X) \
X(30_360, isma30360, "Each year is assumed to have 12 months and 360 days, with each month consisting of exactly 30 days.") \
X(30E_360, isma30360eom, "Each year is assumed to have 12 months and 360 days, with each month consisting of exactly 30 days. The end-of-month rule is applied.") \
X(ACTUAL_ACTUAL, isdaactualactual, "Actual days per year.") \
X(ACTUAL_360, actual360, "Actual days divided by 360.") \
X(ACTUAL_365, actual365fixed, "Actual days divided by 365.")

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

#define TMX_FREQUENCY(X) \
X(ANNUALLY, annually, "Yearly payments.") \
X(SEMIANNUALLY, semiannually, "2 payments per year.") \
X(QUARTERLY, quarterly, "4 payments per year.") \
X(MONTHLY, monthly, "12 payments per year.") \

#define TMX_FREQUENCY_ENUM(a, b, c) XLL_CONST(WORD, TMX_FREQUENCY_##a, (WORD)tmx::date::frequency::b, c, CATEGORY " Enum", "")
TMX_FREQUENCY(TMX_FREQUENCY_ENUM)
#undef TMX_FREQUENCY_ENUM

#define TMX_FREQUENCY_ENUM(a, b, c) OPER("TMX_FREQUENCY_" #a),
OPER tmx_frequency_enum({
	TMX_FREQUENCY(TMX_FREQUENCY_ENUM)
	});
#undef TMX_FREQUENCY_ENUM

XLL_CONST(LPOPER, TMX_FREQUENCY_ENUM, &tmx_frequency_enum, "Payment frequencies.", CATEGORY " Enum", "https://www.investopedia.com/terms/c/compounding.asp")

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

		date::ymd y0 = to_ymd(d0);
		date::ymd y1 = to_ymd(d1);

		result = _dcf(y0, y1);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}
