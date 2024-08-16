// xll_security_bond.cpp - Bonds
#include "security/tmx_bond.h"
#include "xll_instrument.h"

//using namespace fms::iterable;
using namespace tmx;
using namespace xll;

AddIn xai_security_bond_(
	Function(XLL_HANDLEX, "xll_security_bond_", "\\" CATEGORY ".SECURITY.BOND")
	.Arguments({
		Arg(XLL_DOUBLE, "dated", "is the date at which interest begins accruing. Default is today.", "=TODAY()"),
		Arg(XLL_DOUBLE, "maturity", "is the bond maturity as date or in years.", 10),
		Arg(XLL_DOUBLE, "coupon", "is the bond coupon. Default is 5%.", 0.05),
		Arg(XLL_LPOPER, "frequency", "is the yearly payment frequency from the TMX_FREQUENCY_* enumeration. Default is semiannually"),
		Arg(XLL_LPOPER, "day_count", "is the day count basis from the TMX_DAY_COUNT_* enumeration. Default is 30/360."),
		Arg(XLL_LPOPER, "roll", "is business day rolling convention from the TMX_BUSINESS_DAY_* enumeration. Default is modified following."),
		Arg(XLL_LPOPER, "calendar", "is the holiday calendar from the TMX_CALENDAR_* enumeration. Default is 30/360."),
		Arg(XLL_DOUBLE, "face", "is the face amount of the bond. Default is 100."),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a bond security.")
);
HANDLEX WINAPI xll_security_bond_(double dated, double maturity, double coupon, const LPOPER pfreq,
	LPOPER pdcb, LPOPER proll, LPOPER pcal, double face)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {
		using std::chrono::years;

		date::ymd dat;
		if (dated == 0) {
			dat = to_days(Num(Excel(xlfToday)));
		}
		else {
			dat = to_days(dated);
		}

		date::ymd mat;
		if (maturity < 300) {
			mat = dat + years(static_cast<int>(maturity));
		}
		else {
			mat = to_days(to_time_t(maturity));
		}

		// default to semiannual
		auto freq = EnumVal(*pfreq, date::frequency::semiannually);
		if (!freq) return INVALID_HANDLEX;

		// default to ISMA 30/360
		const auto dcf = EnumPtr(*pdcb, date::day_count_isma30360);
		if (!dcf) return INVALID_HANDLEX;

		// default to no roll convention
		const auto roll = EnumVal(*proll, date::business_day::roll::none);
		if (!roll) return INVALID_HANDLEX;

		const auto cal = EnumPtr(*pcal, date::holiday::calendar::none);
		if (!cal) return INVALID_HANDLEX;

		if (face == 0) {
			face = 100;
		}

		handle<security::bond<>> h(new security::bond<>{ dat, mat, coupon, *freq, *dcf, *roll, *cal, face });
		ensure(h);

		result = h.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

AddIn xai_security_bond(
	Function(XLL_LPOPER, "xll_security_bond", CATEGORY ".SECURITY.BOND")
	.Arguments({
		Arg(XLL_HANDLEX, "handle", "is a handle to a basic bond."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return the dated date, maturity, coupon, frequency, day count, roll convention, holiday calendar, and face of a bond.")
);
LPOPER WINAPI xll_security_bond(HANDLEX h)
{
#pragma XLLEXPORT
	static OPER result(8, 1, nullptr);

	try {
		if (h == 0) {
			result = ArgumentNames(OPER("\\" CATEGORY ".SECURITY.BOND"));
			std::swap(result.val.array.rows, result.val.array.columns);
		}
		else {
			handle<security::bond<>> h_(h);
			ensure(h_);

			result[0] = from_days(h_->dated);
			result[1] = from_days(h_->maturity);
			result[2] = h_->coupon;
			result[3] = frequency_string(h_->frequency);
			result[4] = day_count_string(to_handle(h_->day_count));
			result[5] = business_day_string(h_->roll);
			result[6] = holiday_calendar_string(h_->cal); // TODO: preserve type or lookup fails
			result[7] = h_->face;
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &result;
}
