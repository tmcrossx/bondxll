// bondxll.h
#pragma once
#include <chrono>
#include <expected>
#include "xll24/include/xll.h"

#undef interface

#ifndef CATEGORY
#define CATEGORY "TMX"
#endif

namespace xll {

	// Array of argument names given add-in name. 
	inline OPER ArgumentNames(const OPER& o)
	{
		const Args* pargs = AddIn::find(o);

		return pargs ? pargs->argumentName : ErrValue;
	}

} // namespace xll
