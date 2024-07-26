// bondxll.h
#pragma once
#include <chrono>
#include "xll24/xll.h"

#undef interface

#ifndef CATEGORY
#define CATEGORY "TMX"
#endif

namespace xll {

	// Assumes e in an enumerated constant.
	template<typename T>
	inline T Enum(const OPER& e, T init)
	{
		static OPER l(L"="), r(L"()");

		if (isMissing(e) || isNil(e) || (isNum(e) && Num(e) == 0)) {
			return init;
		}

		ensure(isStr(e) || isNum(e));

		HANDLEX h = INVALID_HANDLEX;
		if (isStr(e)) {
			h = asNum(Excel(xlfEvaluate, l & e & r));
		}
		else if (isNum(e)) {
			h = Num(e);
		}
		T* p = safe_pointer<T>(h);

		return p ? reinterpret_cast<T>(*p) : init;
	}

	inline std::chrono::sys_days to_days(time_t t)
	{
		using namespace std::chrono;

		return ceil<days>(system_clock::from_time_t(t));
	}

	// Convert Excel date to sys_days
	inline std::chrono::sys_days to_days(double d)
	{
		return to_days(to_time_t(d));
	}

	inline int from_days(const std::chrono::sys_days& d)
	{
		using namespace std::chrono;

		return static_cast<int>(from_time_t(system_clock::to_time_t(d)));
	}

} // namespace xll
