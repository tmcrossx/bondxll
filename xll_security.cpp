// xll_security.cpp - Bonds
#include "security/tmx_treasury.h"
#include "security/tmx_bond.h"
#include "xll_instrument.h"

//using namespace fms::iterable;
using namespace tmx;
using namespace xll;

AddIn xai_instrument_security_(
	Function(XLL_HANDLEX, "xll_instrument_security_", "\\" CATEGORY ".INSTRUMENT.SECURITY")
	.Arguments({
		Arg(XLL_HANDLEX, "bond", "is a handle to a bond."),
		Arg(XLL_DOUBLE, "pvdate", "is the calculation date of the bond."),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to instrument cash flows determined by present value date.")
);
HANDLEX WINAPI xll_instrument_security_(HANDLEX b, double dated)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		handle<security::bond<>> b_(b);
		ensure(b_);

		auto i = security::instrument(*b_, to_days(dated));

		handle h_(new FPX(xll::instrument(i)));
		ensure(h);

		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}
