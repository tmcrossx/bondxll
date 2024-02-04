// xll_instrument.cpp - Instrument times and cash flows.
#include "../bondlib/tmx_instrument_value.h"
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
	if (size(*pu) != size(*pc)) {
		XLL_ERROR(__FUNCTION__ ": time and cash must have same size");

		return INVALID_HANDLEX;
	}

	handle<instrument::base<>> h_(new instrument::value(size(*pu), pu->array, pc->array));

	return h_ ? h_.get() : INVALID_HANDLEX;
}

AddIn xai_instrument(
	Function(XLL_FP, "xll_instrument", CATEGORY ".INSTRUMENT")
	.Arguments({
		Arg(XLL_HANDLEX, "instrument", "is a handle to an instrument."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return two row array of times and cash flows of instrument.")
);
const _FP12* WINAPI xll_instrument(HANDLEX i)
{
#pragma XLLEXPORT
	static FPX result;

	try {
		result.resize(0, 0);
		handle<instrument::base<>> i_(i);
		ensure(i_);

		int m = (int)i_->size();
		result.resize(2, m);
		auto u = i_->time();
		auto c = i_->cash();
		std::copy(u.begin(), u.end(), array(result));
		std::copy(c.begin(), c.end(), array(result) + m);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result.get();
}
