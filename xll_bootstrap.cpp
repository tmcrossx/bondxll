// xll_bootstrap.cpp - Bootstrap a curve from bonds
#include "curve/tmx_bootstrap.h"
#include "curve/tmx_curve_pwflat.h"
#include "bondxll.h"

using namespace tmx;
using namespace xll;

AddIn xai_tmx_bootstrap_(
	Function(XLL_HANDLEX, "xll_tmx_bootstrap_", "\\" CATEGORY ".BOOTSTRAP.CURVE")
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

		curve::pwflat<> f;
		for (size_t i = 0; i < size(*pi); ++i) {
			handle<instrument::interface<>> ii(pi->array[i]);
			ensure(ii);
			//auto [t, f] = bootstrap::instrument<>(*ii, f, pp->array[i]);
		}

		handle<curve::interface<>> result(&f);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}