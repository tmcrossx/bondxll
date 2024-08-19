// xll_value.cpp - Valuation functions
#include "fms_iterable/fms_iterable.h"
#include "instrument/tmx_instrument.h"
#include "curve/tmx_curve_pwflat.h"
#include "value/tmx_valuation.h"
#include "xll_instrument.h"

using namespace fms;
using namespace tmx;
using namespace xll;

AddIn xai_value_compound_yield(
	Function(XLL_DOUBLE, "xll_value_compound_yield", CATEGORY ".VALUE.COMPOUND_YIELD")
	.Arguments({
		Arg(XLL_DOUBLE, "r", "is a continuously compounded rate."),
		Arg(XLL_WORD, "n", "is the number of times to compound per year."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Convert rate to compounded yield. (1 + y/n)^n = exp(r)")
);
double WINAPI xll_value_compound_yield(double r, WORD n)
{
#pragma XLLEXPORT

	return value::compound_yield(r, n);
}

AddIn xai_value_continuous_rate(
	Function(XLL_DOUBLE, "xll_value_continuous_rate", CATEGORY ".VALUE.CONTINUOUS_RATE")
	.Arguments({
		Arg(XLL_DOUBLE, "y", "is compounded yield."),
		Arg(XLL_WORD, "n", "is the number of times the yield is compounded per year."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Convert compounded yield to rate. exp(r) = (1 + y/n)^n")
);
double WINAPI xll_value_continuous_rate(double y, WORD n)
{
#pragma XLLEXPORT

	return value::continuous_rate(y, n);
}

AddIn xai_value_present(
	Function(XLL_DOUBLE, "xll_value_present", CATEGORY ".VALUE.PRESENT")
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

		curve::constant cc(c);
		curve::interface<>* _c = &cc;
		handle<curve::interface<>> c_(c);
		if (c_) {
			_c = c_.ptr();
		}	

		result = value::present(instrument_iterable(*i_), *_c);
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

		curve::constant cc(c);
		curve::interface<>* _c = &cc;
		handle<curve::interface<>> c_(c);
		if (c_) {
			_c = c_.ptr();
		}

		result = value::duration(instrument_iterable(*i_), *_c);
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

		curve::constant cc(c);
		curve::interface<>* _c = &cc;
		handle<curve::interface<>> c_(c);
		if (c_) {
			_c = c_.ptr();
		}

		result = value::convexity(instrument_iterable(*i_), *_c);
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
	.FunctionHelp("Return the continuously compounded yield repricing the instrument.")
);
double WINAPI xll_value_yield(HANDLEX i, double p)
{
#pragma XLLEXPORT
	double y = math::NaN<double>;

	try {
		handle<FPX> i_(i);
		ensure(i_);

		y = value::yield(instrument_iterable(*i_), p);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return y;
}

AddIn xai_value_price(
	Function(XLL_DOUBLE, "xll_value_price", CATEGORY ".VALUE.PRICE")
	.Arguments({
		Arg(XLL_HANDLEX, "instrument", "is a handle to an instrument."),
		Arg(XLL_DOUBLE, "yield", "is the yield."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return the price of instrument given the continuously compounded yield.")
);
double WINAPI xll_value_price(HANDLEX i, double y)
{
#pragma XLLEXPORT
	double result = math::NaN<double>;

	try {
		handle<FPX> i_(i);
		ensure(i_);

		tmx::curve::constant c(y);

		result = value::present(instrument_iterable(*i_), c);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
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

		curve::constant cc(c);
		curve::interface<>* _c = &cc;
		handle<curve::interface<>> c_(c);
		if (c_) {
			_c = c_.ptr();
		}

		y = value::oas(instrument_iterable(*i_), *_c, p);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return y;
}
