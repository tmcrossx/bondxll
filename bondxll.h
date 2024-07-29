// bondxll.h
#pragma once
#include <chrono>
#include <expected>
#include "xll24/xll.h"

#undef interface

#ifndef CATEGORY
#define CATEGORY "TMX"
#endif

namespace xll {

	inline OPER Eval(const OPER& o)
	{
		return Excel(xlfEvaluate, OPER(L"=") & o & OPER(L"()"));
	}

	// Return asNum(o) as T. If o is a string, evaluate it first.
	template<class T>
	inline std::expected<T, OPER> EnumVal(const OPER& o, T init)
	{
		if (o) {
			double x;
			if (isStr(o)) {
				OPER e = Eval(o);
				if (!isNum(e)) {
					return std::unexpected<OPER>(e);
				}
				x = asNum(e);
				if (std::isnan(x)) {
					return std::unexpected<OPER>(ErrValue);
				}
			}
			else {
				x = asNum(o);
				if (std::isnan(x)) {
					return std::unexpected<OPER>(ErrValue);
				}
			}
			init = static_cast<T>(x);

		}

		return init;
	}
	template<class T>
	inline std::expected<T*,OPER> EnumPtr(const OPER& o, T* init)
	{
		if (o) {
			HANDLEX h = INVALID_HANDLEX;
			if (isStr(o)) {
				OPER e = Eval(o);
				if (!isNum(e)) {
					return std::unexpected(e);
				}
				h = asNum(e);
			}
			else {
				h = asNum(o);
				if (std::isnan(h)) {
					return std::unexpected(ErrValue);
				}
			}

			init = safe_pointer<T>(h);
		}

		return init;
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
