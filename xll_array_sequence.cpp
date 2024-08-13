// xll_array_sequence.cpp - Arithmetic sequence.
#include "xll24/include/xll.h"

using namespace xll;

#ifndef CATEGORY
#define CATEGORY "XLL"
#endif

AddIn xai_array_sequence(
	Function(XLL_FP, "xll_array_sequence", "ARRAY.SEQUENCE")
	.Arguments({
		Arg(XLL_DOUBLE, "start", "is the first value in the sequence.", 0),
		Arg(XLL_DOUBLE, "stop", "is the last value in the sequence.", 1),
		Arg(XLL_DOUBLE, "_incr", "is an optional value to increment by (<10) or the number of array items. Default is 1.", 0.1),
		Arg(XLL_BOOL, "_transpose", "is an optional boolean to return a columns vector. Default is false.", false),
		})
	.FunctionHelp("Return a one column array from start to stop with specified optional increment.")
	.Category(CATEGORY)
	/*
	.Documentation(LR"(
Return a one columns array <code>{start; start + incr; ...; stop}<code>.
If <code>_incr</code> is greater than 1 return <code>_incr</code> values
from <code>start</code> to <code>stop</code>
)")
*/
);
const FP12* WINAPI xll_array_sequence(double start, double stop, double incr, BOOL trans)
{
#pragma XLLEXPORT
	static xll::FPX a;

	try {
		if (incr == 0) {
			incr = 1;
			if (start > stop) {
				incr = -1;
			}
		}

		unsigned n;
		if (incr > 1) {
			n = static_cast<unsigned>(incr);
			incr = (stop - start) / (n - 1);
		}
		else {
			n = 1u + static_cast<unsigned>(fabs((stop - start) / incr));
		}

		a.resize(1, n);
		for (unsigned i = 0; i < n; ++i) {
			a[i] = start + i * incr;
		}
		if (trans) {
			a.resize(n, 1);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
	}

	return a.get();
}

