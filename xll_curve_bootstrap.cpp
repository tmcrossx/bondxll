// xll_bootstrap.cpp - Bootstrap a curve from bonds
#include "curve/tmx_curve_bootstrap.h"
#include "curve/tmx_curve_pwflat.h"
#include "xll_instrument.h"

using namespace tmx;
using namespace xll;

#ifdef _DEBUG
AddIn xai_tmx_bootstrap0_(
	Function(XLL_FP, "xll_tmx_bootstrap0_", CATEGORY ".CURVE.BOOTSTRAP0")
	.Arguments({
		Arg(XLL_HANDLEX, "instrument", "is a handle to a fixed income instrument."),
		Arg(XLL_HANDLEX, "curve", "is a handle to a piecewise constant curve."),
		Arg(XLL_DOUBLE, "time", "is the extrapolation time."),
		Arg(XLL_DOUBLE, "rate", "is the extrapolation rate initial guess."),
		Arg(XLL_DOUBLE, "price", "is the price of the instrument."),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return the last instrument cash flow time and the constant forward rate repricing the instrument.")
	.HelpTopic(TMX_LIB_URL "/curve/tmx_curve_bootstrap.h#:~:text=bootstrap0")
);
FP12* WINAPI xll_tmx_bootstrap0_(HANDLEX i, HANDLEX c, double t, double f, double p)
{
#pragma XLLEXPORT
	static FPX result(1, 2);

	try {
		handle<FPX> i_(i);
		ensure(i_);

		handle<curve::interface<>> c_(c);
		ensure(c_);

		std::tie(result[0], result[1]) = tmx::curve::bootstrap0(instrument_iterable(*i_), *c_, t, f, p);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result.get();
}
#endif // _DEBUG

AddIn xai_tmx_bootstrap_(
	Function(XLL_HANDLEX, "xll_tmx_bootstrap_", "\\" CATEGORY ".CURVE.BOOTSTRAP")
	.Arguments({
		Arg(XLL_FP, "instruments", "is an array of fixed income instrument handles."),
		Arg(XLL_FP, "prices", "is an array of instrument prices."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a curve repricing instruments.")
);
HANDLEX WINAPI xll_tmx_bootstrap_(FP12* pi, FP12* pp)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {
		ensure(size(*pi) == size(*pp));

		handle<curve::interface<>> f(new curve::pwflat{});
		curve::pwflat<>* pf = f.as<curve::pwflat<>>();
		ensure(pf);

		double _t = 0, _f = 0.025;
		for (size_t i = 0; i < size(*pi); ++i) {
			handle<FPX> ii(pi->array[i]);
			ensure(ii);
			if (pf->size() > 0) {
				std::tie(_t, _f) = pf->back();
			}
			pf->push_back(tmx::curve::bootstrap0(instrument_iterable(*ii), *f, _t, _f, pp->array[i]));
		}

		result = f.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}