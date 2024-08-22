// xll_security_teasury.cpp - Treasury bills, notes, and bonds
#include "curve/tmx_curve_bootstrap.h"
#include "security/tmx_bond_muni.h"
#include "xll_instrument.h"

//using namespace fms::iterable;
using namespace tmx;
using namespace xll;

AddIn xai_security_muni_(
	Function(XLL_HANDLEX, "xll_security_muni_", "\\" CATEGORY ".SECURITY.BOND.MUNI")
	.Arguments({
		Arg(XLL_DOUBLE, "dated", "is the date at which interest begins accruing. Default is today.", "=TODAY()"),
		Arg(XLL_DOUBLE, "maturity", "is the bond maturity as date or in years. Default is 30 years.", 30),
		Arg(XLL_DOUBLE, "coupon", "is the bond coupon. Default is 0.05.", 0.05),
		Arg(XLL_DOUBLE, "face", "is the face amount of the bond. Default is 100.", 100),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a treasury bond security.")
	.HelpTopic(TMX_LIB_URL "/security/tmx_bond_muni.h#:~:text=struct%20muni")
);
HANDLEX WINAPI xll_security_muni_(double dated, double maturity, double coupon, double face)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {
		using std::chrono::years;

		date::ymd dat;
		if (dated == 0) {
			dat = to_ymd(Num(Excel(xlfToday)));
		}
		else {
			dat = to_ymd(dated);
		}

		date::ymd mat;
		if (maturity < 300) {
			mat = dat + years(static_cast<int>(maturity));
		}
		else {
			mat = to_ymd(maturity);
		}

		if (coupon == 0) {
			coupon = 0.05;
		}

		if (face == 0) {
			face = 100;
		}

		handle<security::bond<>> h(new security::muni<>(dat, mat, coupon, face));
		ensure(h);

		result = h.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

AddIn xai_tmx_curve_bootstrap_muni_(
	Function(XLL_HANDLEX, "xll_tmx_curve_bootstrap_muni_", "\\" CATEGORY ".CURVE.BOOTSTRAP.MUNI")
	.Arguments({
		Arg(XLL_FP, "curve", "is municipal bond curve of time in years to maturity and coupon rates."),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a curve repricing instruments.")
);
HANDLEX WINAPI xll_tmx_curve_bootstrap_muni_(FP12* ptf)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {
		using std::chrono::years;

		const date::ymd dated = to_ymd(asNum(Excel(xlfToday)));
		handle<curve::interface<>> f(new curve::pwflat{});
		curve::pwflat<>* pf = f.as<curve::pwflat<>>();
		ensure(pf);

		double _t = 0;
		double _f = index(*ptf, 0, 1); // first rate
		int i = 0;
		while (i < rows(*ptf)) {
			int ti = static_cast<int>(index(*ptf, i, 0));
			double fi = index(*ptf, i, 1); // par coupon
			const auto bi = security::muni{ dated, dated + years(ti) };
			const auto ii = security::instrument(bi, dated);
			const auto y = value::continuous_rate(fi, bi.frequency);
			const auto pi = value::price(ii, y);
			if (pf->size() > 0) {
				std::tie(_t, _f) = pf->back();
			}
			pf->push_back(tmx::curve::bootstrap0(ii, *f, _t, _f, pi));
			++i;
		}
		result = f.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

