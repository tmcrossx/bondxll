// xll_value.h - Value functions
#include "../bondlib/tmx_instrument_value.h"
#include "../bondlib/tmx_valuation.h"
#include "bondxll.h"

using namespace tmx;
using namespace xll;

// TODO: fix this
inline instrument::value<> instrument_value(instrument::interface<> *i_)
{
	ensure(i_);

	instrument::value<> i;
	while (*i_) {
		i.push_back(**i_);
		++*i_;
	}

	return i;
}


AddIn xai_value_compound_yield(
	Function(XLL_DOUBLE, "xll_value_compound_yield", CATEGORY ".VALUE.COMPOUND_YIELD")
	.Arguments({
		Arg(XLL_DOUBLE, "yield", "is a continuously compounded yield."),
		Arg(XLL_WORD, "n", "is the number of times to compound per year."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return the compounded yield.")
);
double WINAPI xll_value_compound_yield(double y, WORD n)
{
#pragma XLLEXPORT
	double r = std::numeric_limits<double>::quiet_NaN();

	try {
		r = valuation::compound_yield(y, n);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return r;
}

AddIn xai_value_continuous_yield(
	Function(XLL_DOUBLE, "xll_value_continuous_yield", CATEGORY ".VALUE.CONTINUOUS_YIELD")
	.Arguments({
		Arg(XLL_DOUBLE, "yield", "is compounded yield."),
		Arg(XLL_WORD, "n", "is the number of times the yield is compounded per year."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return constant continuous_yield repricing the instrument.")
);
double WINAPI xll_value_continuous_yield(double y, WORD n)
{
#pragma XLLEXPORT
	double r = std::numeric_limits<double>::quiet_NaN();

	try {
		r = valuation::continuous_yield(y, n);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return r;
}
AddIn xai_value_present(
	Function(XLL_DOUBLE, "xll_value_present", CATEGORY ".VALUE.PRESENT")
	.Arguments({
		Arg(XLL_HANDLEX, "instrument", "is a handle to an instrument."),
		Arg(XLL_HANDLEX, "curve", "is a handle to a curve."),
		Arg(XLL_DOUBLE, "t", "is an optional time in years at which to compute the present value. Default is 0."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return present value of instrument cash flows using curve at time t.")
);
double WINAPI xll_value_present(HANDLEX i, HANDLEX c, double t)
{
#pragma XLLEXPORT
	double result = std::numeric_limits<double>::quiet_NaN();

	try {
		handle<instrument::interface<>> i_(i);
		ensure(i_);
		const auto _i = i_.as<instrument::value<>>(); 

		handle<curve::interface<>> c_(c);
		ensure(c_);

		result = valuation::present(*_i, *c_, t);
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
		Arg(XLL_DOUBLE, "t", "is an optional time in years at which to compute the duration. Default is 0."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return duration of cash flows using curve.")
);
double WINAPI xll_value_duration(HANDLEX i, HANDLEX c, double t)
{
#pragma XLLEXPORT
	double result = std::numeric_limits<double>::quiet_NaN();

	try {
		handle<instrument::interface<>> i_(i);
		ensure(i_);
		const auto _i = i_.as<instrument::value<>>(); 

		handle<curve::interface<>> c_(c);
		ensure(c_);

		result = valuation::duration(*_i, *c_, t);
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
		Arg(XLL_DOUBLE, "t", "is an optional time in years at which to compute the convexity. Default is 0."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return convexity of cash flows using curve.")
);
double WINAPI xll_value_convexity(HANDLEX i, HANDLEX c, double t)
{
#pragma XLLEXPORT
	double result = std::numeric_limits<double>::quiet_NaN();

	try {
		handle<instrument::interface<>> i_(i);
		ensure(i_);
		const auto _i = i_.as<instrument::value<>>(); 

		handle<curve::interface<>> c_(c);
		ensure(c_);

		result = valuation::convexity(*_i, *c_, t);
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
	double y = std::numeric_limits<double>::quiet_NaN();

	try {
		handle<instrument::interface<>> i_(i);
		ensure(i_);
		const auto _i = i_.as<instrument::value<>>(); 

		y = valuation::yield(*_i, p);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return y;
}
