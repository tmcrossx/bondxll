// xll_instrument.cpp - Two row array of floating point instrument times and cash flows.
#include "xll_instrument.h"

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
		ensure(size(*pu) == size(*pc));

		handle i(new FPX(xll::instrument(*pu, *pc)));
		ensure(i);

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
	const _FP12* uc = nullptr;

	try {
		handle<FPX> i_(i);
		ensure(i_);
		
		uc = i_.ptr()->get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return nullptr;
	}

	return uc;
}
