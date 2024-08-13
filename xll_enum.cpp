// xll_enum.cpp
#include "xll24/include/xll.h"

#ifndef CATEGORY
#define CATEGORY "XLL"
#endif

using namespace xll;

AddIn xai_enum(
	Function(XLL_LPOPER, "xll_enum", "ENUM")
	.Arguments({
		Arg(XLL_CSTRING, "enum", "is the string representation of an enumeration.")
		})
	.Category(CATEGORY)
	.FunctionHelp("Evaluate a string name of an enumeration.")
);
const XLOPER12* WINAPI xll_enum(const XCHAR* e)
{
#pragma XLLEXPORT
	static OPER o, l(L"="), r(L"()");

	try {
		o = Excel(xlfEvaluate, l & OPER(e) & r);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return &ErrValue;
	}

	return &o;
}

