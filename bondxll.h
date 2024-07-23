// bondxll.h
#pragma once
#include <chrono>
#include "xll24/xll.h"

#undef interface

#ifndef CATEGORY
#define CATEGORY "TMX"
#endif

inline std::chrono::sys_days to_days(time_t t)
{
	using namespace std::chrono;

	return floor<days>(system_clock::from_time_t(t));
}
