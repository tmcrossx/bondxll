// xll_enum.cpp
#include "xll24/include/xll.h"

#ifndef CATEGORY
#define CATEGORY "XLL"
#endif

using namespace xll;

AddIn xai_enum(
	Function(XLL_LPOPER, "xll_enum", "ENUM")
	.Arguments({
		Arg(XLL_LPOPER, "enum", "is the string representation of an enumeration.")
		})
	.Category(CATEGORY)
	.FunctionHelp("Evaluate a string name of an enumeration.")
);
const XLOPER12* WINAPI xll_enum(const LPOPER e)
{
#pragma XLLEXPORT
	static OPER o, l(L"="), r(L"()");

	try {
		if (isStr(*e)) {
			o = Excel(xlfEvaluate, l & OPER(*e) & r);
		}
		else {
			o = *e;
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return &ErrValue;
	}

	return &o;
}

