// xll_bond.cpp - Bonds
#include "../bondlib/tmx_bond.h"
#include "../bondlib/tmx_instrument_value.h"
#include "bondxll.h"

using namespace tmx;
using namespace xll;

AddIn xai_bond_basic_(
	Function(XLL_HANDLEX, "xll_bond_basic_", "\\" CATEGORY ".BOND.BASIC")
	.Arguments({
		Arg(XLL_DOUBLE, "dated", "is the date at which interest begins accruing. Default is today."),
		Arg(XLL_DOUBLE, "maturity", "is the bond maturity as date or in years."),
		Arg(XLL_DOUBLE, "coupon", "is the bond coupon."),
		Arg(XLL_WORD, "frequency", "is the yearly payment frequency from the TMX_FREQUENCY_* enumeration. Default is semiannually"),
		Arg(XLL_HANDLEX, "day_count", "is the day count basis from the TMX_DAY_COUNT_* enumeration. Default is 30/360."),
		Arg(XLL_DOUBLE, "face", "is the face amount of the bond. Default is 100."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a basic bond.")
);
HANDLEX WINAPI xll_bond_basic_(double dated, double maturity, double coupon, date::frequency freq, HANDLEX dcf, double face)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {
		using std::chrono::years;

		date::ymd dat, mat;

		if (dated == 0) {
			dat = to_ymd(Num(Excel(xlfToday)));
		}
		else {
			dat = to_ymd(dated);
		}

		if (maturity < 1000) {
			mat = dat + years(static_cast<int>(maturity));
		}
		else {
			mat = to_ymd(maturity);
		}

		if (freq == date::frequency::null) {
			freq = tmx::date::frequency::semiannually;
		}
	
		if (!dcf) {
			dcf = safe_handle(&date::day_count_isma30360);
		}
		date::day_count_t _dcf = reinterpret_cast<date::day_count_t>(safe_pointer<date::day_count_t>(dcf));
		ensure(_dcf);

		if (face == 0) {
			face = 100;
		}

		handle<bond::basic<>> h(new bond::basic<>{ dat, mat, coupon, freq, _dcf, face });
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
	static OPER result;

	try {
		result = ErrNA;
		handle<bond::basic<>> h_(h);
		ensure(h_);

		result.reshape(5, 1);
		auto xxx = std::chrono::sys_days(h_->dated);
		result[0] = 0;
		result[1] = 0.;
		result[2] = h_->coupon;
		result[3] = static_cast<double>(h_->frequency);
		result[4] = to_handle(&h_->day_count);
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

		auto i = bond::fix(*b_, to_days(dated));
		handle<instrument::interface<>> h(new instrument::value(i));
		ensure(h);

		result = h.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}
