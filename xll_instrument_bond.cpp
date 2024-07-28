// xll_instrument_bond.cpp - Bonds
#include "instrument/tmx_instrument_bond.h"
#include "xll_instrument.h"

//using namespace fms::iterable;
using namespace tmx;
using namespace xll;

#define XLL_HASH_(x) L#x
#define XLL_STRZ_(x) XLL_HASH_(x)

// Enum string from frequency enum
#define TMX_DATE_FREQUENCY_STRING(a, b, c, d) \
	if (h == date::frequency::##b) return CATEGORY "_FREQUENCY_" #a;
constexpr const char* frequency_string(date::frequency h)
{
	TMX_DATE_FREQUENCY(TMX_DATE_FREQUENCY_STRING)
	else return nullptr;
}
#undef TMX_FREQUENCY_STRING
#define TMX_DATE_FREQUENCY_ENUM(a, b, c, d) if (str.ends_with(XLL_STRZ_(a))) return date::frequency::##b;
constexpr date::frequency frequency_enum(std::wstring_view str)
{
	TMX_DATE_FREQUENCY(TMX_DATE_FREQUENCY_ENUM)
	else return static_cast<date::frequency>(0);
}
#undef TMX_DATE_FREQUENCY_ENUM

constexpr date::frequency frequency_enum(const OPER& freq, date::frequency init)
{
	if (isMissing(freq)) {
		; // return init
	}
	else if (isStr(freq)) {
		init = frequency_enum(view(freq));
		ensure(date::is_frequency((int)init));
	}
	else if (isNum(freq)) {
		int e = static_cast<int>(Num(freq));
		ensure(date::is_frequency(e));
		init = static_cast<date::frequency>(e);
	}
	
	return init;
}

// Enum string from handle
#define TMX_DAY_COUNT_STRING(a, b, c) if (h == to_handle(date::day_count_##b)) return CATEGORY "_DAY_COUNT_" #a;
inline const char* day_count_string(HANDLEX h)
{
	TMX_DAY_COUNT(TMX_DAY_COUNT_STRING)
		return nullptr;
}
#undef TMX_DAY_COUNT_STRING
#define TMX_DAY_COUNT_ENUM(a, b, c) if (dcb.ends_with(XLL_STRZ_(a))) return date::day_count_##b;
inline date::day_count_t day_count_enum(std::wstring_view dcb)
{
	TMX_DAY_COUNT(TMX_DAY_COUNT_ENUM)
	return nullptr;
}
#undef TMX_DAY_COUNT_ENUM
inline date::day_count_t day_count_enum(const OPER& dcb, date::day_count_t init)
{
	if (isMissing(dcb)) {
		; // return init
	}
	else if (isStr(dcb)) {
		init = day_count_enum(view(dcb));
		ensure(init);
	}
	else if (isNum(dcb)) {
		init = reinterpret_cast<date::day_count_t>(safe_pointer<date::day_count_t>(Num(dcb)));
		ensure(init);
	}

	return init;
}

// Enum string from business day roll convention.
#define TMX_DATE_BUSINESS_DAY_STRING(a, b, c) if (h == date::business_day::roll::##b) return CATEGORY "_BUSINESS_DAY_" #a;
inline const char* business_day_string(date::business_day::roll h)
{
	TMX_DATE_BUSINESS_DAY(TMX_DATE_BUSINESS_DAY_STRING)
		return nullptr;
}
#undef TMX_DATE_BUSINESS_DAY_STRING
#define TMX_DATE_BUSINESS_DAY_ENUM(a, b, c) if (roll.ends_with(XLL_STRZ_(a))) return date::business_day::roll::##b;
inline date::business_day::roll business_day_enum(std::wstring_view roll)
{
	TMX_DATE_BUSINESS_DAY(TMX_DATE_BUSINESS_DAY_ENUM)
	return static_cast<date::business_day::roll>(-1);
}
#undef TMX_DATE_BUSINESS_DAY_ENUM
inline date::business_day::roll business_day_enum(const OPER& roll, date::business_day::roll init)
{
	if (isMissing(roll)) {
		; // return init
	}
	else if (isStr(roll)) {
		init = business_day_enum(view(roll));
		ensure(date::business_day::is_business_day_roll(static_cast<int>(init)));
	}
	else if (isNum(roll)) {
		int e = static_cast<int>(Num(roll));
		ensure(date::business_day::is_business_day_roll(e));
		init = static_cast<date::business_day::roll>(e);
	}

	return init;
}

// Enum string from calendar.
#define TMX_DATE_HOLIDAY_CALENDAR_STRING(a, b, c) if (h == date::holiday::calendar::##b) return CATEGORY "_HOLIDAY_CALENDAR_" #a;
inline const char* holiday_calendar_string(date::holiday::calendar_t h)
{
	TMX_DATE_HOLIDAY_CALENDAR(TMX_DATE_HOLIDAY_CALENDAR_STRING)
	return nullptr;
}
#undef TMX_DATE_HOLIDAY_CALENDAR_STRING

AddIn xai_instrument_bond_basic_(
	Function(XLL_HANDLEX, "xll_instrument_bond_basic_", "\\" CATEGORY ".INSTRUMENT.BOND")
	.Arguments({
		Arg(XLL_DOUBLE, "dated", "is the date at which interest begins accruing. Default is today.", "=TODAY()"),
		Arg(XLL_DOUBLE, "maturity", "is the bond maturity as date or in years.", 10),
		Arg(XLL_DOUBLE, "coupon", "is the bond coupon. Default is 5%.", 0.05),
		Arg(XLL_LPOPER, "frequency", "is the yearly payment frequency from the TMX_FREQUENCY_* enumeration. Default is semiannually"),
		Arg(XLL_LPOPER, "day_count", "is the day count basis from the TMX_DAY_COUNT_* enumeration. Default is 30/360."),
		Arg(XLL_LPOPER, "roll", "is business day rolling convention from the TMX_BUSINESS_DAY_* enumeration. Default is modified following."),
		Arg(XLL_LPOPER, "calendar", "is the holiday calendar from the TMX_CALENDAR_* enumeration. Default is 30/360."),
		Arg(XLL_DOUBLE, "face", "is the face amount of the bond. Default is 100."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a basic bond.")
);
HANDLEX WINAPI xll_instrument_bond_basic_(double dated, double maturity, double coupon, const LPOPER pfreq, 
	LPOPER pdcb, LPOPER proll, LPOPER pcal, double face)
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
			mat = to_days(to_time_t(maturity));
		}

		// default to semiannual
		date::frequency freq = frequency_enum(*pfreq, date::frequency::semiannually);
		// default to ISMA 30/360
		date::day_count_t _dcf = day_count_enum(*pdcb, date::day_count_isma30360);
		// default to no roll convention
		date::business_day::roll roll = /*business_day_enum(view(*proll),*/ date::business_day::roll::none/*)*/;
		proll = proll;
		date::holiday::calendar_t cal = Enum(*pcal, date::holiday::calendar::none);	

		if (face == 0) {
			face = 100;
		}

		handle<instrument::bond::basic<>> h(new instrument::bond::basic<>{ dat, mat, coupon, freq, _dcf, roll, cal, face });
		ensure(h);

		result = h.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}

AddIn xai_instrument_bond_basic(
	Function(XLL_LPOPER, "xll_instrument_bond_basic", CATEGORY ".INSTRUMENT.BOND")
	.Arguments({
		Arg(XLL_HANDLEX, "handle", "is a handle to a basic bond."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the dated date, maturity, coupon, frequency, and day count of a basic bond.")
);
LPOPER WINAPI xll_instrument_bond_basic(HANDLEX h)
{
#pragma XLLEXPORT
	static OPER result(8,1,nullptr);

	try {
		handle<instrument::bond::basic<>> h_(h);
		ensure(h_);

		result[0] = from_days(h_->dated);
		result[1] = from_days(h_->maturity);
		result[2] = h_->coupon;
		result[3] = frequency_string(h_->frequency);
		result[4] = day_count_string(to_handle(h_->day_count));
		result[5] = business_day_string(h_->roll);
		result[6] = holiday_calendar_string(h_->cal); // TODO: preserve type or lookup fails
		result[7] = h_->face;
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return &result;
}

AddIn xai_bond_instrument_(
	Function(XLL_HANDLEX, "xll_bond_instrument_", "\\" CATEGORY ".BOND.INSTRUMENT")
	.Arguments({
		Arg(XLL_HANDLEX, "bond", "is a handle to a bond."),
		Arg(XLL_DOUBLE, "dated", "is the dated date of the bond."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to bond instrument cash flows.")
);
HANDLEX WINAPI xll_bond_instrument_(HANDLEX b, double dated)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		handle<instrument::bond::basic<>> b_(b);
		ensure(b_);

		auto i = instrument::bond::instrument(*b_, to_days(dated));

		handle h_(new FPX(xll::instrument(i)));
		ensure(h);

		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}
/*
AddIn xai_bond_treasury_(
	Function(XLL_HANDLEX, "xll_bond_treasury_", "\\" CATEGORY ".INSTRUMENT.BOND.TREASURY")
	.Arguments({
		Arg(XLL_DOUBLE, "dated", "is the date at which interest begins accruing. Default is today."),
		Arg(XLL_DOUBLE, "maturity", "is the bond maturity as date or in years."),
		Arg(XLL_DOUBLE, "coupon", "is the bond coupon. Default is 5%."),
		Arg(XLL_DOUBLE, "face", "is the face amount of the bond. Default is 100."),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a treasury bond.")
);
HANDLEX WINAPI xll_bond_treasury_(double dated, double maturity, double coupon, double face)
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

		handle<instrument::bond::basic<>> h(new instrument::bond::treasury<>(dat, mat, coupon, face));
		ensure(h);

		result = h.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result;
}
*/