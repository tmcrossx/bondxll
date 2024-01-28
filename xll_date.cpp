#include "../bondlib/tmx_date.h"
#include "../bondlib/tmx_date_day_count.h"
#include "bondxll.h"

using namespace tmx;
using namespace xll;

/*
XLL_CONST(HANDLEX, TMX_DAY_COUNT_YEARS, safe_handle(date::dcf_years), "Approximate days per year.", CATEGORY " Enum", "")
XLL_CONST(HANDLEX, TMX_DAY_COUNT_ACTUAL_ACTUAL, safe_handle(date::dcf_actual_actual), "Actual days per year.", CATEGORY " Enum", "")
XLL_CONST(HANDLEX, TMX_DAY_COUNT_30_360, safe_handle(date::dcf_30_360), "30 days per month, 360 days per year.", CATEGORY " Enum", "")
XLL_CONST(HANDLEX, TMX_DAY_COUNT_ACTUAL_360, safe_handle(date::dcf_actual_360), "Actual days divided by 360.", CATEGORY " Enum", "")
XLL_CONST(HANDLEX, TMX_DAY_COUNT_ACTUAL_365, safe_handle(date::dcf_actual_365), "Actual days divided by 365.", CATEGORY " Enum", "")
*/
XLL_CONST(WORD, TMX_FREQUENCY_ANNUALLY, (WORD)tmx::date::frequency::annually, "Yearly payments.", CATEGORY " Enum", "")
XLL_CONST(WORD, TMX_FREQUENCY_SEMIANNUALLY, (WORD)tmx::date::frequency::semiannually, "2 payments per year.", CATEGORY " Enum", "")
XLL_CONST(WORD, TMX_FREQUENCY_QUARTERLY, (WORD)tmx::date::frequency::quarterly, "4 payments per year.", CATEGORY " Enum", "")
XLL_CONST(WORD, TMX_FREQUENCY_MONTHLY, (WORD)tmx::date::frequency::monthly, "12 payments per year.", CATEGORY " Enum", "")

// TODO: move elsewhere
AddIn xai_enum(
	Function(XLL_LPOPER, "xll_enum", "ENUM")
	.Arguments({
		Arg(XLL_CSTRING, "enum", "is the string representation of an enumeration.")
		})
	.Category(CATEGORY)
	.FunctionHelp("Return evaluate a string representation of an enumeration.")
);
LPOPER WINAPI xll_enum(const XCHAR* e)
{
#pragma XLLEXPORT
	static OPER o;

	try {
		o = Excel(xlfEvaluate, OPER("=") & OPER(e) & OPER("()"));
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return (LPOPER)&ErrValue;
	}

	return &o;
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

	return d + y/date::days_per_year;
}

AddIn xai_date_add_ymd(
	Function(XLL_DOUBLE, "xll_date_add_ymd", CATEGORY ".DATE.ADD_YMD")
	.Arguments({
		Arg(XLL_DOUBLE, "date", "is an Excel date."),
		Arg(XLL_LONG, "years", "is the number of years to add."),
		Arg(XLL_WORD, "months", "is the number of months to add."),
		Arg(XLL_WORD, "days", "is the number of days to add."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Add years to date.")
);
double WINAPI xll_date_add_ymd(double t, LONG y, WORD m, WORD d)
{
#pragma XLLEXPORT

	auto u = as_days(t);
	u += std::chrono::days{ d };
	auto v = std::chrono::year_month_day{ u };
	v += std::chrono::years(y);
	v += std::chrono::months(m);
	auto w = std::chrono::sys_days{ v };

	return excel_clock::from_sys(w).time_since_epoch().count();
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
double WINAPI xll_date_diffyears(double d0, double d1)
{
#pragma XLLEXPORT

	return (d1 - d0)/date::days_per_year;
}

AddIn xai_date_dcf(
	Function(XLL_DOUBLE, "xll_date_dcf", CATEGORY ".DATE_DCF")
	.Arguments({
		Arg(XLL_HANDLEX, "dcf", "is a day count fraction."),
		Arg(XLL_DOUBLE, "d0", "is an Excel date."),
		Arg(XLL_DOUBLE, "d1", "is an Excel date."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return day count fraction from d0 to d1.")
);
double WINAPI xll_date_dcf(HANDLEX dcf, double d0, double d1)
{
#pragma XLLEXPORT
	double result = INVALID_HANDLEX;

	try {
		date::day_count_t* _dcf = safe_pointer<date::day_count_t>(dcf);
		if (!_dcf) {
			XLL_ERROR(__FUNCTION__ ": invalid day count fraction");

			return INVALID_HANDLEX;
		}

		result = (*_dcf)(as_days(d0), as_days(d1));
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}
/*
AddIn xai_date_dcf_years(
	Function(XLL_DOUBLE, "xll_date_dcf_years", CATEGORY ".DATE.DCF_YEARS")
	.Arguments({
		Arg(XLL_DOUBLE, "d0", "is an Excel date."),
		Arg(XLL_DOUBLE, "d1", "is an Excel date."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return time in years from d0 to d1.")
);

double WINAPI xll_date_dcf_years(double d0, double d1)
{
#pragma XLLEXPORT
	return xll_date_dcf(xll_TMX_DAY_COUNT_YEARS(), d0, d1);
}

AddIn xai_date_dcf_actual_360(
	Function(XLL_DOUBLE, "xll_date_dcf_actual_360", CATEGORY ".DATE.DCF_ACTUAL_360")
	.Arguments({
		Arg(XLL_DOUBLE, "d0", "is an Excel date."),
		Arg(XLL_DOUBLE, "d1", "is an Excel date."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return Actual/360 day count fraction from d0 to d1.")
);
double WINAPI xll_date_dcf_actual_360(double d0, double d1)
{
#pragma XLLEXPORT
	return xll_date_dcf(xll_TMX_DAY_COUNT_ACTUAL_360(), d0, d1);
}
AddIn xai_date_dcf_actual_365(
	Function(XLL_DOUBLE, "xll_date_dcf_actual_365", CATEGORY ".DATE.DCF_ACTUAL_365")
	.Arguments({
		Arg(XLL_DOUBLE, "d0", "is an Excel date."),
		Arg(XLL_DOUBLE, "d1", "is an Excel date."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return Actual/365 day count fraction from d0 to d1.")
);
double WINAPI xll_date_dcf_actual_365(double d0, double d1)
{
#pragma XLLEXPORT
	return xll_date_dcf(xll_TMX_DAY_COUNT_ACTUAL_365(), d0, d1);
}
AddIn xai_date_dcf_actual_actual(
	Function(XLL_DOUBLE, "xll_date_dcf_actual_actual", CATEGORY ".DATE.DCF_ACTUAL_ACTUAL")
	.Arguments({
		Arg(XLL_DOUBLE, "d0", "is an Excel date."),
		Arg(XLL_DOUBLE, "d1", "is an Excel date."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return time Actual/Actual day count fraction from d0 to d1.")
);
double WINAPI xll_date_dcf_actual_actual(double d0, double d1)
{
#pragma XLLEXPORT
	return xll_date_dcf(xll_TMX_DAY_COUNT_ACTUAL_ACTUAL(), d0, d1);
}
AddIn xai_date_dcf_30_360(
	Function(XLL_DOUBLE, "xll_date_dcf_30_360", CATEGORY ".DATE.DCF_30_360")
	.Arguments({
		Arg(XLL_DOUBLE, "d0", "is an Excel date."),
		Arg(XLL_DOUBLE, "d1", "is an Excel date."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return 30/360 day count fraction from d0 to d1.")
);
double WINAPI xll_date_dcf_30_360(double d0, double d1)
{
#pragma XLLEXPORT
	return xll_date_dcf(xll_TMX_DAY_COUNT_30_360(), d0, d1);
}

*/