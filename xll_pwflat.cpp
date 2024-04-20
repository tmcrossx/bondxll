// xll_pwflat.cpp - Piecewise flat curve view
#include "../bondlib/tmx_curve_pwflat.h"
#include "bondxll.h"

using namespace tmx;
using namespace xll;

AddIn xai_curve_pwflat_(
	Function(XLL_HANDLEX, "xll_curve_pwflat_", "\\" CATEGORY ".CURVE.PWFLAT")
	.Arguments({
		Arg(XLL_FP, "t", "is an array of positive increasing times."),
		Arg(XLL_FP, "f", "is an array of corresponding rates."),
		Arg(XLL_LPOPER, "_f", "is an optional extrapolation rate."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a piecewise flat forward curve.")
);
HANDLEX WINAPI xll_curve_pwflat_(const _FP12* pt, const _FP12* pf, LPOPER p_f)
{
#pragma XLLEXPORT

	HANDLEX h = INVALID_HANDLEX;

	try {
		auto m = size(*pt);
		ensure(m == size(*pf));
		double _f = std::numeric_limits<double>::quiet_NaN();
		if (type(*p_f) != xltypeMissing) {
			ensure(type(*p_f) == xltypeNum);
			_f = p_f->val.num;
		}
		// lasts 2 times equal use last f to extrapolate
		if (m >= 2 and pt->array[m-1] == pt->array[m-2]) {
			--m;
			_f = pf->array[m];
		}
		handle<curve::pwflat<>> h_(new curve::pwflat(m, pt->array, pf->array, _f));
		ensure(h_);
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
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
_FP12* WINAPI xll_curve_pwflat(HANDLEX c)
{
#pragma XLLEXPORT
	static FPX result;

	try {
		result.resize(0, 0);
		handle<curve::pwflat<>> c_(c);
		ensure(c_);

		int m = (int)c_->size();
		result.resize(2, m + 1);
		const auto t = c_->time();
		const auto f = c_->rate();
		std::copy(t.begin(), t.end(), result.array());
		std::copy(f.begin(), f.end(), result.array() + m + 1);

		result(0, m) = result(0, m - 1);
		result(1, m) = c_->extrapolate();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result.get();
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

AddIn xai_curve_pwflat_value(
	Function(XLL_DOUBLE, "xll_curve_pwflat_value", CATEGORY ".CURVE.PWFLAT.FORWARD")
	.Arguments({
		Arg(XLL_HANDLEX, "c", "is a handle to a curve."),
		Arg(XLL_DOUBLE, "t", "is the forward time."),
		Arg(XLL_DOUBLE, "_t", "is the time at which to evaluate the forward. Default is 0."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the forward value of a piecewise flat curve.")
);
HANDLEX WINAPI xll_curve_pwflat_value(HANDLEX c, double t, double _t)
{
#pragma XLLEXPORT

	double v = math::NaN<double>;

	try {
		handle<curve::pwflat<>> c_(c);
		ensure(c_);

		v = c_->forward(t, _t);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return v;
}

AddIn xai_curve_pwflat_spot(
	Function(XLL_DOUBLE, "xll_curve_pwflat_yield", CATEGORY ".CURVE.PWFLAT.SPOT")
	.Arguments({
		Arg(XLL_HANDLEX, "c", "is a handle to a piecewise flat curve."),
		Arg(XLL_DOUBLE, "t", "is the yield time."),
		Arg(XLL_DOUBLE, "_t", "is the time at which to evaluate the yield. Default is 0."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the spot value of a piecewise flat curve.")
);
HANDLEX WINAPI xll_curve_pwflat_spot(HANDLEX c, double t, double _t)
{
#pragma XLLEXPORT

	double v = math::NaN<double>;

	try {
		handle<curve::pwflat<>> c_(c);
		ensure(c_);

		v = c_->spot(t, _t);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return v;
}

AddIn xai_curve_pwflat_discount(
	Function(XLL_DOUBLE, "xll_curve_pwflat_discount", CATEGORY ".CURVE.PWFLAT.DISCOUNT")
	.Arguments({
		Arg(XLL_HANDLEX, "c", "is a handle to a piecewise flat curve."),
		Arg(XLL_DOUBLE, "t", "is the discount time."),
		Arg(XLL_DOUBLE, "_t", "is the time at which to evaluate the discount. Default is 0."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the discount value of a piecewise flat curve.")
);
HANDLEX WINAPI xll_curve_pwflat_discount(HANDLEX c, double t, double _t)
{
#pragma XLLEXPORT

	double v = math::NaN<double>;

	try {
		handle<curve::pwflat<>> c_(c);
		ensure(c_);

		v = c_->discount(t + _t, _t);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return v;
}
