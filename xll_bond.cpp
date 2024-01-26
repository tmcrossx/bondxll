// xll_bond.cpp - Bonds
#include "../bondlib/tmx_bond.h"
#include "bondxll.h"

using namespace tmx;
using namespace xll;

AddIn xai_bond_basic_(
	Function(XLL_HANDLEX, "xll_bond_basic_", "\\" CATEGORY ".BOND.BASIC")
	.Arguments({
		Arg(XLL_WORD, "maturity", "is the bond maturity in years."),
		Arg(XLL_DOUBLE, "coupon", "is the bond coupon."),
		Arg(XLL_WORD, "frequency", "is the yearly payment frequency from the TMX_FREQUENCY_* enumeration. Default is semiannually"),
		Arg(XLL_HANDLEX, "day_count", "is the day count basis from the TMX_DAY_COUNT_* enumeration. Default is 30/360."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a basic bond.")
);
HANDLEX WINAPI xll_bond_basic_(WORD maturity, double coupon, date::frequency freq, HANDLEX dcf)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {
		using std::chrono::years;

		if (freq == 0) {
			freq = tmx::date::frequency::semiannually;
		}

		date::day_count_t* _dcf = nullptr;
		if (dcf == 0) {
			_dcf = &date::day_count_isma30360;
		}
		else {
			date::day_count_t* p = safe_pointer<date::day_count_t>(dcf);
			ensure(p);
			_dcf = *p;
		}

		handle<bond::basic<>> h(new bond::basic<>{ maturity, coupon, freq, *_dcf });
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
	.FunctionHelp("Return the maturity, coupon, frequency, and day count of a basic bond.")
);
LPOPER WINAPI xll_bond_basic(HANDLEX h)
{
#pragma XLLEXPORT
	static OPER result;

	try {
		result = ErrNA;
		handle<bond::basic<>> h_(h);
		ensure(h_);

		result.resize(4, 1);
		result[0] = std::chrono::years(h_->maturity).count();
		result[1] = h_->coupon;
		result[2] = h_->frequency;
		result[3] = to_handle(&h_->day_count);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &result;
}

AddIn xai_bond_cash_flow_(
	Function(XLL_HANDLEX, "xll_bond_cash_flow_", "\\" CATEGORY ".BOND.INSTRUMENT")
	.Arguments({
		Arg(XLL_HANDLEX, "bond", "is a handle to a bond."),
		Arg(XLL_DOUBLE, "dated", "is the dated date of the bond."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to bond instrument cash flows.")
);
HANDLEX WINAPI xll_bond_cash_flow_(HANDLEX b, double dated)
{
#pragma XLLEXPORT
	static HANDLEX result;

	try {
		result = INVALID_HANDLEX;

		handle<bond::basic<>> b_(b);
		ensure(b_);

		handle<instrument<>> i_(new instrument_value<>(bond::instrument<>(*b_, as_days(dated))));
		ensure(i_);

		result = i_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}
