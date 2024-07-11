// bondxll.h
#pragma once
#include <algorithm>
#include "fms_iterable/fms_iterable.h"
#include "instrument/tmx_instrument.h"
#include "xll24/xll.h"

#undef interface

#ifndef CATEGORY
#define CATEGORY "TMX"
#endif

namespace xll {

	// TODO: move to xll24???
	template<class T>
	using counted_iterable = fms::iterable::counted<fms::iterable::ptr<T>>;

	// Used in tmx::instrument::iterable
	class iterable_value : public counted_iterable<double> {
		FPX i;
		static auto make_iterable(_FP12* pi)
		{
			using namespace fms::iterable;

			return take(ptr(xll::array(*pi)), xll::size(*pi));
		}
	public:
		iterable_value()
			: counted_iterable<double>(), i()
		{ }
		iterable_value(const double* p, int r, int c = 1)
			: counted_iterable<double>(), i(r, c, p)
		{
			counted_iterable<double>::operator=(make_iterable(i.get()));
		}
		iterable_value(const _FP12* p)
			: iterable_value(p->array, p->rows, p->columns)
		{ }
		template<class I>
		iterable_value(I i_)
		{
			while (i_) {
				i.push_back(*i_);
				++i_;
			}
		}
		iterable_value(const iterable_value& i_)
			: iterable_value(i_.i.array(), i_.i.rows(), i_.i.columns())
		{ }
		iterable_value& operator=(const iterable_value& i_)
		{
			if (this != &i_) {
				i = i_.i;
				counted_iterable<double>::operator=(make_iterable(i.get()));
			}

			return *this;
		}
		~iterable_value() = default;

		FPX get()
		{
			return i;
		}
	};

	template<class IU, class IC>
	inline tmx::instrument::iterable<iterable_value, iterable_value> make_instrument(tmx::instrument::iterable<IU, IC> i)
	{

	}

	// Two row array of times and cash flows.
	template<class IU, class IC>
	inline FPX make_fpx(const tmx::instrument::iterable<IU, IC>& i)
	{
		FPX uc;

		auto u = i.time();
		while (u) {
			uc.push_back(*u);
			++u;
		}
		auto c = i.cash();
		while (c) {
			uc.push_back(*c);
			++c;
		}
		if (uc.size()) {
			uc.resize(2, uc.size() / 2);
		}

		return uc;
	}

} // namespace xll