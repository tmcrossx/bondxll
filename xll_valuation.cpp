// xll_valuation.h - Valuation functions
#include "fms_iterable/fms_iterable.h"
#include "instrument/tmx_instrument.h"
#include "curve/tmx_curve_pwflat.h"
#include "valuation/tmx_valuation.h"
#include "bondxll.h"

using namespace fms;
using namespace tmx;
using namespace xll;

//using iterable_vector = fms::iterable::vector<double>;
//using instrument_iterable = tmx::instrument::iterable<iterable_vector, iterable_vector>;


AddIn xai_value_compound_yield(
	Function(XLL_DOUBLE, "xll_value_compound_yield", CATEGORY ".VALUATION.COMPOUND_YIELD")
	.Arguments({
		Arg(XLL_DOUBLE, "rate", "is a continuously compounded rate."),
		Arg(XLL_WORD, "n", "is the number of times to compound per year."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Convert rate to compounded yield.")
);
double WINAPI xll_value_compound_yield(double r, WORD n)
{
#pragma XLLEXPORT
	double y = math::NaN<double>;

	try {
		y = valuation::compound_yield(r, n);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return y;
}

AddIn xai_value_continuous_rate(
	Function(XLL_DOUBLE, "xll_value_continuous_rate", CATEGORY ".VALUATION.CONTINUOUS_RATE")
	.Arguments({
		Arg(XLL_DOUBLE, "yield", "is compounded yield."),
		Arg(XLL_WORD, "n", "is the number of times the yield is compounded per year."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Convert compounded yield to rate.")
);
double WINAPI xll_value_continuous_rate(double y, WORD n)
{
#pragma XLLEXPORT
	double r = math::NaN<double>;

	try {
		r = valuation::continuous_rate(y, n);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return r;
}

// 2 row array of times and cash flows.
inline auto time(FPX& i)
{
	auto n = i.columns();

	return iterable::counted(iterable::ptr(i.array()), n);
}
inline auto cash(FPX& i)
{
	auto n = i.columns();

	return iterable::counted(iterable::ptr(i.array() + n), n);
}
inline auto make_iterable(FPX& i)
{
	return instrument::iterable(time(i), cash(i));
}

AddIn xai_value_present(
	Function(XLL_DOUBLE, "xll_value_present", CATEGORY ".VALUATION.PRESENT")
	.Arguments({
		Arg(XLL_HANDLEX, "instrument", "is a handle to an instrument."),
		Arg(XLL_HANDLEX, "curve", "is a handle to a curve."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return present value of instrument cash flows using curve at time t.")
);
double WINAPI xll_value_present(HANDLEX i, HANDLEX c)
{
#pragma XLLEXPORT
	double result = math::NaN<double>;

	try {
		handle<FPX> i_(i);
		ensure(i_);

		handle<curve::interface<>> c_(c);
		ensure(c_);

		result = valuation::present(make_iterable(*i_), *c_);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

AddIn xai_value_duration(
	Function(XLL_DOUBLE, "xll_value_duration", CATEGORY ".VALUE.DURATION")
	.Arguments({
		Arg(XLL_HANDLEX, "instrument", "is a handle to an instrument."),
		Arg(XLL_HANDLEX, "curve", "is a handle to a curve."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return duration of cash flows using curve.")
);
double WINAPI xll_value_duration(HANDLEX i, HANDLEX c)
{
#pragma XLLEXPORT
	double result = math::NaN<double>;

	try {
		handle<FPX> i_(i);
		ensure(i_);

		handle<curve::interface<>> c_(c);
		ensure(c_);

		result = valuation::duration(make_iterable(*i_), *c_);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

AddIn xai_value_convexity(
	Function(XLL_DOUBLE, "xll_value_convexity", CATEGORY ".VALUE.CONVEXITY")
	.Arguments({
		Arg(XLL_HANDLEX, "instrument", "is a handle to an instrument."),
		Arg(XLL_HANDLEX, "curve", "is a handle to a curve."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return convexity of cash flows using curve.")
);
double WINAPI xll_value_convexity(HANDLEX i, HANDLEX c)
{
#pragma XLLEXPORT
	double result = math::NaN<double>;

	try {
		handle<FPX> i_(i);
		ensure(i_);

		handle<curve::interface<>> c_(c);
		ensure(c_);

		result = valuation::convexity(make_iterable(*i_), *c_);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

AddIn xai_value_yield(
	Function(XLL_DOUBLE, "xll_value_yield", CATEGORY ".VALUE.YIELD")
	.Arguments({
		Arg(XLL_HANDLEX, "instrument", "is a handle to an instrument."),
		Arg(XLL_DOUBLE, "price", "is the price of the instrument."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return constant yield repricing the instrument.")
);
double WINAPI xll_value_yield(HANDLEX i, double p)
{
#pragma XLLEXPORT
	double y = math::NaN<double>;

	try {
		handle<FPX> i_(i);
		ensure(i_);

		y = valuation::yield(make_iterable(*i_), p);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return y;
}

AddIn xai_value_oas(
	Function(XLL_DOUBLE, "xll_value_oas", CATEGORY ".VALUE.OAS")
	.Arguments({
		Arg(XLL_HANDLEX, "instrument", "is a handle to an instrument."),
		Arg(XLL_HANDLEX, "curve", "is a handle to a curve."),
		Arg(XLL_DOUBLE, "price", "is the price of the instrument."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return constant oas repricing the instrument.")
);
double WINAPI xll_value_oas(HANDLEX i, HANDLEX c, double p)
{
#pragma XLLEXPORT
	double y = math::NaN<double>;

	try {
		handle<FPX> i_(i);
		ensure(i_);

		handle<curve::interface<>> c_(c);
		ensure(c_);

		y = valuation::oas(make_iterable(*i_), *c_, p);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return y;
}
