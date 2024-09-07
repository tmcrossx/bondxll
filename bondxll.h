// bondxll.h
#pragma once
#include <chrono>
#include <expected>
#include "xll24/include/xll.h"

#undef interface

#ifndef CATEGORY
#define CATEGORY "TMX"
#endif
#define TMX_LIB_URL "https://github.com/tmcrossx/bondlib/blob/main/"
#define TMX_XLL_URL "https://github.com/tmcrossx/bondxll/blob/main/"

namespace xll {

	// Array of argument names given add-in name. 
	inline OPER ArgumentNames(const OPER& o)
	{
		const Args* pargs = AddIn::find(o);

		return pargs ? pargs->argumentName : ErrValue;
	}

	// Convert OPER(s) to 2 row FPX.
	inline FPX tofp(const OPER& x, const OPER& y)
	{
		FPX xy;

		if (isMissing(y) || isNil(y)) {
			if (columns(x) == 2) {
				xy.resize(2, rows(x));
				for (int i = 0; i < rows(x); ++i) {
					xy(0, i) = Num(x(i, 0));
					xy(1, i) = Num(x(i, 1));
				}
			}
			else if (rows(x) == 2) {
				xy.resize(2, columns(x));
				for (int j = 0; j < columns(x); ++j) {
					xy(0, j) = Num(x(0, j));
					xy(1, j) = Num(x(1, j));
				}
			}
			else {
				ensure(size(x) == 0 || !"first argument must be either 2 rows or 2 columns");
			}
		}
		else {
			ensure(size(x) == size(y));
			xy.resize(2, size(x));
			for (int i = 0; i < size(x); ++i) {
				xy(0, i) = Num(x[i]);
				xy(1, i) = Num(y[i]);
			}
		}

		return xy;
	}

} // namespace xll
