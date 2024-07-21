// xll_instrument.h - Two row array of floating point times and cash flows.
#pragma once
#include "instrument/tmx_instrument.h"
#include "bondxll.h"

#undef interface

namespace xll {

	// Instruments are two row array of times and cash flows.
	inline auto instrument_iterable(FPX& x)
	{
		ensure(x.rows() == 2);

		auto time = fms::iterable::counted(fms::iterable::ptr(begin(x)), columns(x));
		auto cash = fms::iterable::counted(fms::iterable::ptr(begin(x) + columns(x)), columns(x));

		return tmx::instrument::iterable(time, cash);
	}
	inline FPX instrument(const _FP12& time, const _FP12& cash)
	{
		FPX u(time);
		u.resize(1, size(time));

		FPX c(cash);
		c.resize(1, size(cash));

		return u.vstack(c);
	}
	template<class IU, class IC>
	inline FPX instrument(const tmx::instrument::iterable<IU, IC>& i)
	{
		FPX u(i.time());
		FPX c(i.cash());

		return u.vstack(c);
	}

} // namespace xll