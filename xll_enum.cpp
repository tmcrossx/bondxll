// xll_enum.cpp
#include "xll.h"

using namespace xll;

AddIn xai_enum(
	Function(XLL_LPOPER, "xll_enum", "ENUM")
	.Arguments({
		Arg(XLL_CSTRING, "enum", "is the string representation of an enumeration.")
		})
	.Category("XLL")
	.FunctionHelp("Evaluate a string name of an enumeration.")
);
LPXLOPER12 WINAPI xll_enum(const XCHAR* e)
{
#pragma XLLEXPORT
	static OPER o;

	try {
		auto x = OPER("=") & OPER(e) & OPER("()");
		o = Excel(xlfEvaluate, x);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return const_cast<LPXLOPER12>(&ErrValue);
	}

	return &o;
}

