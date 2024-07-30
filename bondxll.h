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
