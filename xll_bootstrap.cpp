// xll_bootstrap.cpp - Bootstrap a curve from bonds
#include "curve/tmx_curve_bootstrap.h"
#include "curve/tmx_curve_pwflat.h"
#include "xll_instrument.h"

using namespace tmx;
using namespace xll;
#if 0
#ifdef _DEBUG
AddIn xai_tmx_bootstrap0_(
	Function(XLL_HANDLEX, "xll_tmx_bootstrap0_", "\\" CATEGORY ".CURVE.BOOTSTRAP0")
	.Arguments({
		Arg(XLL_HANDLEX, "instrument", "is a handle to a fixed income instrument."),
		Arg(XLL_HANDLEX, "curve", "is a handle to a piecewise constant curve."),
		Arg(XLL_FP, "prices", "is an array of instrument prices."),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a curve repricing instruments.")
);
HANDLEX WINAPI xll_tmx_bootstrap0_(FP12* pi, FP12* pp)
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
			pf->push_back(tmx::curve::bootstrap(instrument_iterable(*ii), *f, _t, _f, pp->array[i]));
		}

		result = f.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}
#endif // _DEBUG
#endif // 0

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
			pf->push_back(tmx::curve::bootstrap(instrument_iterable(*ii), *f, _t, _f, pp->array[i]));
		}

		result = f.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}