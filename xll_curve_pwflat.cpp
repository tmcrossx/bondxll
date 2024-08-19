// xll_pwflat.cpp - Piecewise flat curve view
#include "curve/tmx_curve_pwflat.h"
#include "bondxll.h"

using namespace tmx;
using namespace xll;

// 2 row array of times and rates.
inline FPX pwflat(const tmx::curve::pwflat<>& c)
{
	int n = static_cast<int>(c.size());
	FPX tf(2, n);
	if (n) {
		copy(c.time(), tf.array());
		copy(c.rate(), tf.array() + n);
	}

	return tf;
}

AddIn xai_curve_constant_(
	Function(XLL_HANDLEX, "xll_curve_constant_", "\\" CATEGORY ".CURVE.CONSTANT")
	.Arguments({
		Arg(XLL_DOUBLE, "rate", "is the constant forward rate.", 0),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a constant forward curve.")
);
HANDLEX WINAPI xll_curve_constant_(double f)
{
#pragma XLLEXPORT

	HANDLEX h = INVALID_HANDLEX;

	try {
		handle<curve::interface<>> h_(new curve::constant<>(f));
		ensure(h_);
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

AddIn xai_curve_bump_(
	Function(XLL_HANDLEX, "xll_curve_bump_", "\\" CATEGORY ".CURVE.BUMP")
	.Arguments({
		Arg(XLL_DOUBLE, "bump", "is the amount to bump the forward rate.", 0),
		Arg(XLL_DOUBLE, "begin", "is the time to start the bump.", 0),
		Arg(XLL_DOUBLE, "end", "is the time to end the bump.", 0)
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a bump forward curve.")
);
HANDLEX WINAPI xll_curve_bump_(double s, double t0, double t1)
{
#pragma XLLEXPORT

	HANDLEX h = INVALID_HANDLEX;

	try {
		handle<curve::interface<>> h_(new curve::bump<>(s, t0, t1));
		ensure(h_);
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

AddIn xai_curve_pwflat_(
	Function(XLL_HANDLEX, "xll_curve_pwflat_", "\\" CATEGORY ".CURVE.PWFLAT")
	.Arguments({
		Arg(XLL_FP, "time", "is an array of positive non-decreasing times.", "={1, 2, 3}"),
		Arg(XLL_FP, "rate", "is an array of corresponding forward rates.", "={.01, .02, .03}"),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a piecewise flat forward curve.")
);
HANDLEX WINAPI xll_curve_pwflat_(const FP12* pt, const FP12* pf)
{
#pragma XLLEXPORT

	HANDLEX h = INVALID_HANDLEX;

	try {
		if (size(*pt) == 1 and pt->array[0] == 0 and pf->array[0] == 0) {
			handle<curve::interface<>> h_(new curve::pwflat{});
			ensure(h_);
			h = h_.get();
		}
		else if (rows(*pt) == 2) {
			ensure(size(*pf) == 1 and pf->array[0] == 0);
			int n = columns(*pt);
			handle<curve::interface<>> h_(new curve::pwflat(n, pt->array, pt->array + n));
			ensure(h_);
			h = h_.get();
		}
		else {
			ensure(size(*pt) == size(*pf) || !"times and rates must be the same size");

			handle<curve::interface<>> h_(new curve::pwflat(size(*pt), pt->array, pf->array));
			ensure(h_);
			h = h_.get();
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

AddIn xai_curve_pwflat_push_back(
	Function(XLL_HANDLEX, "xll_curve_pwflat_push_back", CATEGORY ".CURVE.PWFLAT.PUSH_BACK")
	.Arguments({
		Arg(XLL_HANDLEX, "curve", "is handle to a piecewise flat curve."),
		Arg(XLL_FP, "time", "time value to add."),
		Arg(XLL_DOUBLE, "rate", "forward rate add.")
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return curve with extrapolated point at end of curve.")
);
HANDLEX WINAPI xll_curve_pwflat_push_back(HANDLEX c, const FP12* pt, double f)
{
#pragma XLLEXPORT
	try {
		handle<curve::interface<>> c_(c);
		ensure(c_);
		curve::pwflat<>* pc = c_.as<curve::pwflat<>>();
		ensure(pc);

		double t = pt->array[0];
		if (size(*pt) > 1) {
			f = pt->array[1];
		}
		pc->push_back(t, f);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return INVALID_HANDLEX;
	}

	return c;
}

AddIn xai_curve_pwflat(
	Function(XLL_FP, "xll_curve_pwflat", CATEGORY ".CURVE.PWFLAT")
	.Arguments({
		Arg(XLL_HANDLEX, "curve", "is handle to a curve."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return a two row array of times and rates.")
);
FP12* WINAPI xll_curve_pwflat(HANDLEX c)
{
#pragma XLLEXPORT
	static FPX result;

	try {
		handle<curve::interface<>> _c(c);
		ensure(_c);
		curve::pwflat<>* c_ = _c.as<curve::pwflat<>>();
		ensure(c_);

		result = ::pwflat(*c_);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return nullptr;
	}

	return result.get();
}

AddIn xai_curve_pwflat_value(
	Function(XLL_FP, "xll_curve_pwflat_value", CATEGORY ".CURVE.FORWARD")
	.Arguments({
		Arg(XLL_HANDLEX, "curve", "is a handle to a curve."),
		Arg(XLL_FP, "time", "is the forward time."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the forward value of a piecewise flat curve.")
);
FP12* WINAPI xll_curve_pwflat_value(HANDLEX c, FP12* pt)
{
#pragma XLLEXPORT

	try {
		handle<curve::interface<>> c_(c);
		ensure(c_);

		std::transform(begin(*pt), end(*pt), begin(*pt), [&c_](double t) { return c_->value(t); });
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return pt;
}

AddIn xai_curve_pwflat_spot(
	Function(XLL_FP, "xll_curve_pwflat_spot", CATEGORY ".CURVE.SPOT")
	.Arguments({
		Arg(XLL_HANDLEX, "curve", "is a handle to a piecewise flat curve."),
		Arg(XLL_FP, "time", "is the yield time."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the spot value of a piecewise flat curve.")
);
FP12* WINAPI xll_curve_pwflat_spot(HANDLEX c, FP12* pt)
{
#pragma XLLEXPORT

	try {
		handle<curve::interface<>> c_(c);
		ensure(c_);

		std::transform(begin(*pt), end(*pt), begin(*pt), [&c_](double t) { return c_->spot(t); });
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return pt;
}

AddIn xai_curve_pwflat_discount(
	Function(XLL_FP, "xll_curve_pwflat_discount", CATEGORY ".CURVE.DISCOUNT")
	.Arguments({
		Arg(XLL_HANDLEX, "curve", "is a handle to a piecewise flat curve."),
		Arg(XLL_FP, "time", "is the discount time."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the discount value of a piecewise flat curve.")
);
FP12* WINAPI xll_curve_pwflat_discount(HANDLEX c, FP12* pt)
{
#pragma XLLEXPORT

	try {
		handle<curve::interface<>> c_(c);
		ensure(c_);

		std::transform(begin(*pt), end(*pt), begin(*pt), [&c_](double t) { return c_->discount(t); });
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return pt;
}


AddIn xai_curve_pwflat_spread_(
	Function(XLL_HANDLEX, "xll_curve_pwflat_spread_", "\\" CATEGORY ".CURVE.SPREAD")
	.Arguments({
		Arg(XLL_HANDLEX, "curve", "is handle to a curve."),
		Arg(XLL_DOUBLE, "spread", "is the spread.")
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a curve incremented by spread.")
);
HANDLEX WINAPI xll_curve_pwflat_spread_(HANDLEX c, double s)
{
#pragma XLLEXPORT
	double result = INVALID_HANDLEX;

	try {
		handle<curve::interface<>> c_(c);
		ensure(c_);
		handle<curve::interface<>> _c(new curve::spread(*c_, s));
		ensure(_c);

		result = _c.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result; 
}

AddIn xai_curve_translate_(
	Function(XLL_HANDLEX, "xll_curve_translate_", "\\" CATEGORY ".CURVE.TRANSLATE")
	.Arguments({
		Arg(XLL_HANDLEX, "curve", "is handle to a curve."),
		Arg(XLL_DOUBLE, "time", "is the time in years to translate the curve.")
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a two row array of times and rates.")
);
HANDLEX WINAPI xll_curve_translate_(HANDLEX c, double u)
{
#pragma XLLEXPORT
	double result = std::numeric_limits<double>::quiet_NaN();

	try {
		handle<curve::interface<>> c_(c);
		ensure(c_);
		handle<curve::interface<>> _c(new curve::translate(*c_, u));
		ensure(_c);

		result = _c.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}
