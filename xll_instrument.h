// xll_instrument.h - Two row array of floating point time in years and cash flows.
#pragma once
#include "instrument/tmx_instrument.h"
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
		FPX u(time);
		u.resize(1, size(time));

		FPX c(cash);
		c.resize(1, size(cash));

		return u.vstack(c);
	}
	// Return value type.
	template<class IU, class IC>
	inline FPX instrument(const tmx::instrument::iterable<IU, IC>& i)
	{
		FPX u(i.time());
		FPX c(i.cash());

		return u.vstack(c);
	}

} // namespace xll