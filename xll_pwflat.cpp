// xll_pwflat.cpp - Piecewise flat curve view
#include "curve/tmx_curve_pwflat.h"
#include "bondxll.h"

using namespace tmx;
using namespace xll;

AddIn xai_curve_constant_(
	Function(XLL_HANDLEX, "xll_curve_constant_", "\\" CATEGORY ".CURVE.CONSTANT")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the constant forward rate.", 0),
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

AddIn xai_curve_pwflat_(
	Function(XLL_HANDLEX, "xll_curve_pwflat_", "\\" CATEGORY ".CURVE.PWFLAT")
	.Arguments({
		Arg(XLL_FP, "t", "is an array of positive increasing times.", "{1, 2, 3}"),
		Arg(XLL_FP, "f", "is an array of corresponding rates.", "{.01, .02, .03}"),
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
		ensure(size(*pt) == size(*pf));

		handle<curve::interface<>> h_(new curve::pwflat(size(*pt), pt->array, pf->array));
		ensure(h_);
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

AddIn xai_curve_pwflat_push_back(
	Function(XLL_HANDLEX, "xll_curve_pwflat_push_back", CATEGORY ".CURVE.PWFLAT.PUSH_BACK")
	.Arguments({
		Arg(XLL_HANDLEX, "curve", "is handle to a curve."),
		Arg(XLL_FP, "time", "time value to push back."),
		Arg(XLL_DOUBLE, "rate", "forward rate to push back.")
		})
	.Category(CATEGORY)
	.FunctionHelp("Add point to end of curve.")
);
HANDLEX WINAPI xll_curve_pwflat_push_back(HANDLEX c, const FP12* pt, double f)
{
#pragma XLLEXPORT
	try {
		handle<curve::interface<>> _c(c);
		ensure(_c);
		curve::pwflat<>* c_ = _c.as<curve::pwflat<>>();
		ensure(c_);
		
		double t = pt->array[0];
		if (size(*pt) > 1) {
			f = pt->array[1];
		}

		c_->push_back(t, f);
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
	.FunctionHelp("Return a two row array of times and rates. "
	"The last time is duplicated and the last rate is the extrapolation.")
);
FP12* WINAPI xll_curve_pwflat(HANDLEX c)
{
#pragma XLLEXPORT
	static FPX result;

	try {
		handle<curve::interface<>> _c(c);
		ensure(_c);
		const curve::pwflat<>* c_ = _c.as<curve::pwflat<>>();
		ensure(c_);

		int m = static_cast<int>(c_->size());
		result.resize(2, m);
		const auto t = c_->time();
		const auto f = c_->rate();
		std::copy(t.begin(), t.end(), result.array());
		std::copy(f.begin(), f.end(), result.array() + m);

		//result.transpose();
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
		Arg(XLL_HANDLEX, "c", "is a handle to a curve."),
		Arg(XLL_FP, "t", "is the forward time."),
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

		std::transform(begin(*pt), end(*pt), begin(*pt), [&c_](double t) { return c_->forward(t); });
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return pt;
}

AddIn xai_curve_pwflat_spot(
	Function(XLL_FP, "xll_curve_pwflat_spot", CATEGORY ".CURVE.SPOT")
	.Arguments({
		Arg(XLL_HANDLEX, "c", "is a handle to a piecewise flat curve."),
		Arg(XLL_FP, "t", "is the yield time."),
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

		for (int i = 0; i < size(*pt); ++i) {
			pt->array[i] = c_->spot(pt->array[i]);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return pt;
}

AddIn xai_curve_pwflat_discount(
	Function(XLL_FP, "xll_curve_pwflat_discount", CATEGORY ".CURVE.DISCOUNT")
	.Arguments({
		Arg(XLL_HANDLEX, "c", "is a handle to a piecewise flat curve."),
		Arg(XLL_FP, "t", "is the discount time."),
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

		for (int i = 0; i < size(*pt); ++i) {
			pt->array[i] = c_->discount(pt->array[i]);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return pt;
}


#if 0

AddIn xai_curve_pwflat_shift_(
	Function(XLL_HANDLEX, "xll_curve_pwflat_shift_", "\\" CATEGORY ".CURVE.SHIFT")
	.Arguments({
		Arg(XLL_HANDLEX, "curve", "is handle to a curve."),
		Arg(XLL_DOUBLE, "spread", "is the spread.")
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a curve shifted by spread.")
);
HANDLEX WINAPI xll_curve_pwflat_shift_(HANDLEX c, double s)
{
#pragma XLLEXPORT
	double result = INVALID_HANDLEX;

	try {
		handle<curve::pwflat<>> c_(c);
		ensure(c_);
		handle<curve::pwflat<>> _c(new pwflat::curve(*c_ + s));
		ensure(_c);

		result = _c.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result; 
}

AddIn xai_curve_pwflat_translate_(
	Function(XLL_HANDLEX, "xll_curve_pwflat_translate_", "\\" CATEGORY ".PWFLAT.CURVE.TRANSLATE")
	.Arguments({
		Arg(XLL_HANDLEX, "curve", "is handle to a curve."),
		Arg(XLL_DOUBLE, "time", "is the time in years to translate the curve.")
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a two row array of times and rates.")
);
HANDLEX WINAPI xll_curve_pwflat_translate_(HANDLEX c, double u)
{
#pragma XLLEXPORT
	double result = std::numeric_limits<double>::quiet_NaN();

	try {
		handle<pwflat::curve_view<>> c_(c);
		ensure(c_);
		handle<pwflat::curve_view<>> _c(new pwflat::curve_value(*c_));
		ensure(_c);
		_c->translate(u);

		result = _c.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}
#endif // 0