// xll_array_sequence.cpp - Arithmetic sequence.
#include "bondxll.h"

using namespace xll;

AddIn xai_array_sequence(
	Function(XLL_FP, L"xll_array_sequence", L"ARRAY.SEQUENCE")
	.Arguments({
		Arg(XLL_DOUBLE, L"start", L"is the first value in the sequence."),
		Arg(XLL_DOUBLE, L"stop", L"is the last value in the sequence."),
		Arg(XLL_DOUBLE, L"_incr", L"is an optional value to increment by. Default is 1.")
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
const _FP12* WINAPI xll_array_sequence(double start, double stop, double incr)
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

		a.resize(n, 1);
		for (unsigned i = 0; i < n; ++i) {
			a[i] = start + i * incr;
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR("ARRAY.SEQUENCE: unknown exception");
	}

	return a.get();
}

