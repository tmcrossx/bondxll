// xll_enum.h - Convert string to enumeration value
#pragma once
#include "xll.h"

using namespace xll;

AddIn xai_enum(
	Function(XLL_LPOPER, "xll_enum", "ENUM")
	.Arguments({
		Arg(XLL_CSTRING, "enum", "is the string representation of an enumeration.")
		})
	//.Category(CATEGORY)
	.FunctionHelp("Evaluate a string representation of an enumeration.")
);
LPOPER WINAPI xll_enum(const XCHAR* e)
{
#pragma XLLEXPORT
	static OPER o;

	try {
		o = Excel(xlfEvaluate, OPER(L"=") & OPER(e) & OPER(L"()"));
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return (LPOPER)&ErrValue;
	}

	return &o;
}

