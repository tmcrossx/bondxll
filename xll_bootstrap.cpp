// xll_bootstrap.cpp - Bootstrap a curve from bonds
#include "curve/tmx_curve_bootstrap.h"
#include "curve/tmx_curve_pwflat.h"
#include "bondxll.h"

using namespace tmx;
using namespace xll;

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
HANDLEX WINAPI xll_tmx_bootstrap_(_FP12* pi, _FP12* pp)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {
		ensure(size(*pi) == size(*pp));

		handle<curve::interface<>> f(new curve::pwflat{});
		curve::pwflat<>* pf = f.as<curve::pwflat<>>();
		ensure(pf);

		for (size_t i = 0; i < size(*pi); ++i) {
			handle<instrument::iterable<iterable_value, iterable_value>> ii(pi->array[i]);
			ensure(ii);
			auto [_t, _f] = pf->back();
			pf->push_back(tmx::curve::bootstrap(*ii, *f, _t, _f, pp->array[i]));
		}

		result = f.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}