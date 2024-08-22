// xll_instrument.cpp - Two row array of floating point instrument times and cash flows.
#include "xll_instrument.h"

using namespace tmx;
using namespace xll;

AddIn xai_instrument_(
	Function(XLL_HANDLEX, "xll_instrument_", "\\" CATEGORY ".INSTRUMENT")
	.Arguments({
		Arg(XLL_FP, "time", "is an array of times.", "={1, 2, 3}"),
		Arg(XLL_FP, "cash", "is an array of cash flow amounts.", "={0.1, 0.2, 0.3}"),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to an instrument with cash flows at specified times.")
	.HelpTopic(TMX_LIB_URL "/instrument/tmx_instrument.h")
);
HANDLEX WINAPI xll_instrument_(const FP12* pu, const FP12* pc)
{
#pragma XLLEXPORT
	HANDLEX h_ = INVALID_HANDLEX;

	try {
		ensure(size(*pu) == size(*pc));
		ensure(std::is_sorted(begin(*pu), end(*pu), std::less<double>{}));

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
	.FunctionHelp("Return a two row array of instrument times and cash flows.")
);
const FP12* WINAPI xll_instrument(HANDLEX i)
{
#pragma XLLEXPORT
	const FP12* uc = nullptr;

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
