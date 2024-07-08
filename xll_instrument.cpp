// xll_instrument.cpp - Instrument times and cash flows.
#include "instrument/tmx_instrument.h"
#include "bondxll.h"

using namespace tmx;
using namespace xll;

AddIn xai_instrument_(
	Function(XLL_HANDLEX, "xll_instrument_", "\\" CATEGORY ".INSTRUMENT")
	.Arguments({
		Arg(XLL_FP, "time", "is an array of times."),
		Arg(XLL_FP, "cash", "is an array of cash flow amounts."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to instrument times and cash flows.")
);
HANDLEX WINAPI xll_instrument_(const _FP12* pu, const _FP12* pc)
{
#pragma XLLEXPORT
	HANDLEX h_ = INVALID_HANDLEX;

	try {
		if (size(*pu) != size(*pc)) {
			XLL_ERROR(__FUNCTION__ ": time and cash must have same size");

			return INVALID_HANDLEX;
		}

		handle<FPX> i(new FPX{});
		ensure(i);
		auto n = size(*pu);
		i->resize(2, n);
		std::copy_n(begin(*pu), n, i->array());
		std::copy_n(begin(*pc), n, i->array() + n);

		h_ = i.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return INVALID_HANDLEX;
	}

	return h_;
}

AddIn xai_instrument(
	Function(XLL_FP, "xll_instrument", CATEGORY ".INSTRUMENT")
	.Arguments({
		Arg(XLL_HANDLEX, "instrument", "is a handle to an instrument."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return two row array of instrument times and cash flows.")
);
const _FP12* WINAPI xll_instrument(HANDLEX i)
{
#pragma XLLEXPORT
	_FP12* pi;

	try {
		handle<FPX> i_(i);
		ensure(i_);
		pi = static_cast<_FP12*>(i_.ptr()->get());
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return nullptr;
	}

	return pi;
}
