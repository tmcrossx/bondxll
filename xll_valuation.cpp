// xll_value.h - Value functions
#include "../bondlib/tmx_instrument_value.h"
#include "../bondlib/tmx_curve_pwflat.h"
#include "../bondlib/tmx_valuation.h"
#include "bondxll.h"

using namespace tmx;
using namespace xll;

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
	double r = math::NaN<double>;

	try {
		r = valuation::compound_rate(y, n);
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
	double r = math::NaN<double>;

	try {
		r = valuation::continuous_rate(y, n);
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
		})
	.Category(CATEGORY)
	.FunctionHelp("Return present value of instrument cash flows using curve at time t.")
);
double WINAPI xll_value_present(HANDLEX i, HANDLEX c, double t)
{
#pragma XLLEXPORT
	double result = math::NaN<double>;

	try {
		handle<instrument::interface<>> i_(i);
		ensure(i_);
		const auto _i = i_.as<instrument::value<>>(); 
		ensure(_i);

		handle<curve::interface<>> c_(c);
		ensure(c_);

		result = valuation::present(instrument::view(*_i), *c_);
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
		handle<instrument::interface<>> i_(i);
		ensure(i_);
		const auto _i = i_.as<instrument::value<>>(); 
		ensure(_i);

		handle<curve::interface<>> c_(c);
		ensure(c_);

		result = 0;// valuation::duration(instrument::view(*_i), *c_);
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
		handle<instrument::interface<>> i_(i);
		ensure(i_);
		const auto _i = i_.as<instrument::value<>>(); 
		ensure(_i);

		handle<curve::interface<>> c_(c);
		ensure(c_);

		result = 0;//!!! valuation::convexity(instrument::view(*_i), *c_, t);
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
		handle<instrument::interface<>> i_(i);
		ensure(i_);
		const auto _i = i_.as<instrument::value<>>();

		handle<curve::interface<>> c_(c);
		ensure(c_);
		const auto _c = c_.as<curve::pwflat<>>();
		ensure(_c);

		y = valuation::oas(*_i, *_c, p);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return y;
}
