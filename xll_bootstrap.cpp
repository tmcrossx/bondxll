// xll_bootstrap.cpp - Bootstrap a curve from bonds
#include "../bondlib/tmx_bootstrap.h"
#include "../bondlib/tmx_curve_pwflat.h"
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

		handle<curve::interface<>> f_(new curve::pwflat<>{});
		ensure(f_);
		for (size_t i = 0; i < size(*pi); ++i) {
			handle<instrument::interface<>> ii(pi->array[i]);
			ensure(ii);
			f_.as<curve::pwflat<>>()->push_back(bootstrap::instrument<>(*ii, *f_, pp->array[i]));
		}

		result = f_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}