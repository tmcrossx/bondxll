// xll_instrument.h - Two row array of floating point time in years and cash flows.
#pragma once
#include "instrument/tmx_instrument.h"
#include "curve/tmx_curve_pwflat.h"
#include "bondxll.h"

#undef interface

namespace xll {

	// Instruments are two row array of times and cash flows.
	// Assumes lifetime of x.
	inline auto instrument_iterable(FPX& uc)
	{
		ensure(uc.rows() == 2);

		auto u = fms::iterable::counted(fms::iterable::ptr(begin(uc)), columns(uc));
		auto c = fms::iterable::counted(fms::iterable::ptr(begin(uc) + columns(uc)), columns(uc));

		return tmx::instrument::iterable(u, c);
	}
	// Return value type.
	inline FPX instrument(const FP12& time, const FP12& cash)
	{
		ensure(size(time) == size(cash));
		FPX uc(2, size(time));
		std::copy(begin(time), end(time), uc.array());
		std::copy(begin(cash), end(cash), uc.array() + size(time));

		return uc;
	}
	// Return value type.
	template<class IU, class IC>
	inline FPX instrument(const tmx::instrument::iterable<IU, IC>& i)
	{
		int n = static_cast<int>(size(i));
		FPX uc(2, n);	
		copy(i.time(), uc.array());
		copy(i.cash(), uc.array() + n);

		return uc;
	}

} // namespace xll