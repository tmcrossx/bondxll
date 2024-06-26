// xll_bond.cpp - Bonds
#include "instrument/tmx_instrument_bond.h"
#include "bondxll.h"
#include "xll24/excel_clock.h"

//using namespace fms::iterable;
using namespace tmx;
using namespace xll;

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


AddIn xai_bond_basic_(
	Function(XLL_HANDLEX, "xll_bond_basic_", "\\" CATEGORY ".BOND.BASIC")
	.Arguments({
		Arg(XLL_DOUBLE, "dated", "is the date at which interest begins accruing. Default is today."),
		Arg(XLL_DOUBLE, "maturity", "is the bond maturity as date or in years."),
		Arg(XLL_DOUBLE, "coupon", "is the bond coupon. Default is 5%."),
		Arg(XLL_WORD, "frequency", "is the yearly payment frequency from the TMX_FREQUENCY_* enumeration. Default is semiannually"),
		Arg(XLL_HANDLEX, "day_count", "is the day count basis from the TMX_DAY_COUNT_* enumeration. Default is 30/360."),
		Arg(XLL_WORD, "roll", "is business day rolling convention from the TMX_ROLL_* enumeration. Default is modified following."),
		Arg(XLL_HANDLEX, "calendar", "is the holiday calendar from the TMX_CALENDAR_* enumeration. Default is 30/360."),
		Arg(XLL_DOUBLE, "face", "is the face amount of the bond. Default is 100."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a basic bond.")
);
HANDLEX WINAPI xll_bond_basic_(double dated, double maturity, double coupon, date::frequency freq, 
	HANDLEX dcf, date::business_day::roll roll, HANDLEX cal, double face)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {
		using std::chrono::years;


		date::ymd dat;
		if (dated == 0) {
			dat = to_ymd(Num(Excel(xlfToday)));
		}
		else {
			dat = to_ymd(dated);
		}

		date::ymd mat;
		if (maturity < 300) {
			mat = dat + years(static_cast<int>(maturity));
		}
		else {
			mat = to_days(maturity);
		}

		if (freq == date::frequency::missing) {
			freq = tmx::date::frequency::semiannually;
		}
	
		if (!dcf) {
			dcf = safe_handle(&date::day_count_isma30360);
		}
		date::day_count_t _dcf 
			= reinterpret_cast<date::day_count_t>(safe_pointer<date::day_count_t>(dcf));
		ensure(_dcf);

		if (roll == date::business_day::roll::missing) {
			roll = date::business_day::roll::none;
		}

		if (!cal) {
			cal = safe_handle(&date::holiday::calendar::none);
		}
		date::holiday::calendar::calendar_t _cal 
			= reinterpret_cast<date::holiday::calendar::calendar_t>(safe_pointer<date::holiday::calendar::calendar_t>(cal));
		ensure(_cal);

		if (face == 0) {
			face = 100;
		}

		handle<bond::basic<>> h(new bond::basic<>{ dat, mat, coupon, freq, _dcf, roll, _cal, face });
		ensure(h);

		result = h.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

AddIn xai_bond_basic(
	Function(XLL_LPOPER, "xll_bond_basic", CATEGORY ".BOND.BASIC")
	.Arguments({
		Arg(XLL_HANDLEX, "handle", "is a handle to a basic bond."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the dated date, maturity, coupon, frequency, and day count of a basic bond.")
);
LPOPER WINAPI xll_bond_basic(HANDLEX h)
{
#pragma XLLEXPORT
	static OPER result(8,1,nullptr);

	try {
		handle<bond::basic<>> h_(h);
		ensure(h_);

		result[0] = to_excel(h_->dated);
		result[1] = to_excel(h_->maturity);
		result[2] = h_->coupon;
		result[3] = frequency_string(h_->frequency);
		result[4] = day_count_string(to_handle(h_->day_count));
		result[5] = business_day_roll_string(h_->roll);
		result[6] = holiday_calendar_string(to_handle(h_->cal));
		result[7] = h_->face;
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &result;
}

AddIn xai_bond_basic_fix_(
	Function(XLL_HANDLEX, "xll_bond_basic_fix_", "\\" CATEGORY ".BOND.BASIC.INSTRUMENT")
	.Arguments({
		Arg(XLL_HANDLEX, "bond", "is a handle to a bond."),
		Arg(XLL_DOUBLE, "dated", "is the dated date of the bond."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to bond instrument cash flows.")
);
HANDLEX WINAPI xll_bond_basic_fix_(HANDLEX b, double dated)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {
		handle<bond::basic<>> b_(b);
		ensure(b_);

		handle<instrument::value<>> h(new instrument::value(bond::instrument(*b_, to_days(dated))));
		ensure(h);

		result = h.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}
