// bondxll.h
#pragma once
#include <chrono>
#include <span>
#include "xll24/xll.h"

#undef interface

#ifndef CATEGORY
#define CATEGORY "TMX"
#endif

namespace xll {

	template<class X>
	constexpr std::span<double> span(const _FP12& x)
	{
		return std::span{ x.array, x.rows * x.columns };
	}
	template<class X>
	constexpr std::span<double> span(const _FP& x)
	{
		return std::span{ x.array, x.rows * x.columns };
	}

	// https://stackoverflow.com/questions/33964461/handling-julian-days-in-c11-14
	struct excel_clock;

	template <class Dur>
	using excel_time = std::chrono::time_point<excel_clock, Dur>;

	// Excel clock represented as days since 1900-01-01.
	struct excel_clock
	{
		using rep = double;
		using period = std::chrono::days::period;
		using duration = std::chrono::duration<rep, period>;
		using time_point = std::chrono::time_point<excel_clock>;

		static constexpr bool is_steady = false;

		static time_point now() noexcept
		{
			return std::chrono::clock_cast<excel_clock>(std::chrono::system_clock::now());
		}

		template <class Dur>
		static auto from_sys(std::chrono::sys_time<Dur> const& tp) noexcept
		{
			auto constexpr epoch = std::chrono::sys_days{ std::chrono::year(1900) / 1 / 1 };
			
			return excel_time{ tp - epoch };
		}

		template <class Dur>
		static auto to_sys(excel_time<Dur> const& tp) noexcept
		{
			return std::chrono::sys_time{ tp - std::chrono::clock_cast<excel_clock>(std::chrono::sys_days{}) };
		}
	};

	inline auto as_time(double d)
	{
		return excel_time{ excel_clock::duration{d} };
	}
	inline std::chrono::sys_days to_days(double d)
	{
		return std::chrono::time_point_cast<std::chrono::days>(excel_clock::to_sys(as_time(d)));
	}
	inline std::chrono::year_month_day to_ymd(double d)
	{
		return std::chrono::year_month_day(to_days(d));
	}
}