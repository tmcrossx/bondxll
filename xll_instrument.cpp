// xll_instrument.cpp - Instrument times and cash flows.
#include "bondxll.h"
#include "../bondlib/tmx_instrument.h"

using namespace tmx;
//using namespace tmx::instrument;
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
	if (size(*pu) != size(*pc)) {
		XLL_ERROR(__FUNCTION__ ": time and cash must have same size");

		return INVALID_HANDLEX;
	}

	handle<instrument::interface<>> h_(new instrument::value<>(size(*pu), pu->array, pc->array));

	return h_ ? h_.get() : INVALID_HANDLEX;
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
	static xll::FPX result;

	try {
		handle<instrument::interface<>> i_(i);
		ensure(i_);

		result.resize(0, 0);
		while (*i_) {
			const auto uc = **i_;
			result.push_back(uc.u);
			result.push_back(uc.c);
			++*i_;
		}

		result.resize(result.size() / 2, 2);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return nullptr;
	}

	return result.get();
}
