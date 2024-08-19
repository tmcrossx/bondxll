// xll_security_teasury.cpp - Treasury bills, notes, and bonds
#include "curve/tmx_curve_bootstrap.h"
#include "security/tmx_treasury.h"
#include "xll_instrument.h"

//using namespace fms::iterable;
using namespace tmx;
using namespace xll;

AddIn xai_security_treasury_bill_(
	Function(XLL_HANDLEX, "xll_security_treasury_bill_", "\\" CATEGORY ".SECURITY.BOND.TREASURY.BILL")
	.Arguments({
		Arg(XLL_DOUBLE, "dated", "is the date at which interest begins accruing.", "=TODAY()"),
		Arg(XLL_UINT, "weeks", "is the bill maturity in weeks.", 53),
		Arg(XLL_DOUBLE, "coupon", "is the bill coupon.", 0.05),
		Arg(XLL_DOUBLE, "face", "is the face amount of the bill.", 100),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a treasury bill security.")
	.HelpTopic(TMX_LIB_URL "/security/tmx_treasury.h#:~:text=struct%20bill")
);
HANDLEX WINAPI xll_security_treasury_bill_(double dated, unsigned weeks, double coupon, double face)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {
		using std::chrono::years;

		date::ymd dat;
		if (dated == 0) {
			dat = to_days(Num(Excel(xlfToday)));
		}
		else {
			dat = to_days(dated);
		}

		if (face == 0) {
			face = 100;
		}

		handle<security::bond<>> h(new security::treasury::bill<>(dat, weeks, coupon, face));
		ensure(h);

		result = h.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

AddIn xai_security_treasury_note_(
	Function(XLL_HANDLEX, "xll_security_treasury_note_", "\\" CATEGORY ".SECURITY.BOND.TREASURY.NOTE")
	.Arguments({
		Arg(XLL_DOUBLE, "dated", "is the date at which interest begins accruing.", "=TODAY()"),
		Arg(XLL_DOUBLE, "maturity", "is the note maturity as date or in years.", 10),
		Arg(XLL_DOUBLE, "coupon", "is the note coupon.", 0.05),
		Arg(XLL_DOUBLE, "face", "is the face amount of the note.", 100),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a treasury note security.")
	.HelpTopic(TMX_LIB_URL "/security/tmx_treasury.h#:~:text=struct%20note")
);
HANDLEX WINAPI xll_security_treasury_note_(double dated, double maturity, double coupon, double face)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {
		using std::chrono::years;

		date::ymd dat;
		if (dated == 0) {
			dat = to_days(Num(Excel(xlfToday)));
		}
		else {
			dat = to_days(dated);
		}

		date::ymd mat;
		if (maturity < 300) {
			mat = dat + years(static_cast<int>(maturity));
		}
		else {
			mat = to_days(maturity);
		}

		if (face == 0) {
			face = 100;
		}

		handle<security::bond<>> h(new security::treasury::note<>(dat, mat, coupon, face));
		ensure(h);

		result = h.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

AddIn xai_security_treasury_bond_(
	Function(XLL_HANDLEX, "xll_security_treasury_bond_", "\\" CATEGORY ".SECURITY.BOND.TREASURY.BOND")
	.Arguments({
		Arg(XLL_DOUBLE, "dated", "is the date at which interest begins accruing.", "=TODAY()"),
		Arg(XLL_DOUBLE, "maturity", "is the bond maturity as date or in years.", 30),
		Arg(XLL_DOUBLE, "coupon", "is the bond coupon.", 0.05),
		Arg(XLL_DOUBLE, "face", "is the face amount of the bond.", 100),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a treasury bond security.")
	.HelpTopic(TMX_LIB_URL "/security/tmx_treasury.h#:~:text=struct%20bond")
);
HANDLEX WINAPI xll_security_treasury_bond_(double dated, double maturity, double coupon, double face)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {
		using std::chrono::years;

		date::ymd dat;
		if (dated == 0) {
			dat = to_days(Num(Excel(xlfToday)));
		}
		else {
			dat = to_days(dated);
		}

		date::ymd mat;
		if (maturity < 300) {
			mat = dat + years(static_cast<int>(maturity));
		}
		else {
			mat = to_days(maturity);
		}

		if (face == 0) {
			face = 100;
		}

		handle<security::bond<>> h(new security::treasury::bond<>(dat, mat, coupon, face));
		ensure(h);

		result = h.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

AddIn xai_tmx_curve_bootstrap_treasury_(
	Function(XLL_HANDLEX, "xll_tmx_curve_bootstrap_treasury_", "\\" CATEGORY ".CURVE.BOOTSTRAP.TREASURY")
	.Arguments({
		Arg(XLL_FP, "curve", "is two column array of time in years to maturity and rates."),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a curve repricing instruments.")
);
HANDLEX WINAPI xll_tmx_curve_bootstrap_treasury_(FP12* ptf)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {
		using namespace std::chrono;
		using namespace tmx::security::treasury;

		const date::ymd dated = to_days(asNum(Excel(xlfToday)));
		handle<curve::interface<>> f(new curve::pwflat{});
		curve::pwflat<>* pf = f.as<curve::pwflat<>>();
		ensure(pf);

		// bills
		double _t = 0;
		double _f = index(*ptf, 0, 1); // first rate
		int i = 0;
		while (i < rows(*ptf)) {
			double ti = index(*ptf, i, 0);
			if (ti > 1) {
				break;
			}
			const auto ii = instrument::zero_coupon_bond(index(*ptf, i, 1));
			if (pf->size() > 0) {
				std::tie(_t, _f) = pf->back();
			}
			double fi = index(*ptf, i, 1);
			pf->push_back(tmx::curve::bootstrap0(ii, *f, _t, _f, 1 - fi * ti));
			++i;
		}
		// notes
		while (i < rows(*ptf)) {
			int ti = static_cast<int>(index(*ptf, i, 0));
			if (ti > 10) {
				break;
			}
			double fi = index(*ptf, i, 1); // coupon
			const auto ni = security::treasury::note(dated, dated + years(ti), fi, 1.);
			const auto ii = security::instrument(ni, dated);
			if (pf->size() > 0) {
				std::tie(_t, _f) = pf->back();
			}
			pf->push_back(tmx::curve::bootstrap0(ii, *f, _t, _f, ni.face));
			++i;
		}
		// bonds
		while (i < rows(*ptf)) {
			int ti = static_cast<int>(index(*ptf, i, 0));
			double fi = index(*ptf, i, 1);
			const auto bi = security::treasury::bond(dated, dated + years(ti), fi, 1.);
			const auto ii = security::instrument(bi, dated);
			if (pf->size() > 0) {
				std::tie(_t, _f) = pf->back();
			}
			pf->push_back(tmx::curve::bootstrap0(ii, *f, _t, _f, bi.face));
			++i;
		}

		result = f.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}
