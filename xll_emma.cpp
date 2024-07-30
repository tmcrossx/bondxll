// xll_emma.cpp - Get EMMA data.
// https://emma.msrb.org/TreasuryData/GetTreasuryDailyYieldCurve?curveDate=mm/dd/yyyy
// https://emma.msrb.org/ICEData/GetICEDailyYieldCurve?curveDate=
// https://emma.msrb.org/TradeData/MostRecentTrades
// https://emma.msrb.org/TradeData/GetMostRecentTrades?
// https://home.treasury.gov/resource-center/data-chart-center/interest-rates/daily-treasury-rates.csv/2023/all?type=daily_treasury_yield_curve&field_tdr_date_value=2023&page&_format=csv

#include "fms_sqlite/fms_sqlite.h"
#include "bondxll.h"

using namespace xll;

const OPER TreasuryDailyYieldCurve("https://emma.msrb.org/TreasuryData/GetTreasuryDailyYieldCurve?curveDate=");
const OPER ICEDailyYieldCurve("https://emma.msrb.org/ICEData/GetICEDailyYieldCurve?curveDate=");

// in memory database
sqlite::db db("");

Auto<Open> xao_emma_db([] {
	try {
		sqlite::stmt stmt(::db);
		stmt.exec("DROP TABLE IF EXISTS emma");
		// curve name, Excel date, JSON data
		stmt.exec("CREATE TABLE emma (curve TEXT, date FLOAT, data TEXT)");
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return 1;
});

int insert_curve_row(const char* curve, double date, std::wstring_view data)
{
	try {
		sqlite::stmt stmt(::db);
		stmt.prepare("INSERT INTO emma (curve, date, data) VALUES (?, ?, ?)");
		stmt.bind(1, curve);
		stmt.bind(2, date);
		stmt.bind(3, data);
		ensure(SQLITE_DONE == stmt.step());
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}

int insert_curve_date(const OPER& curve, double date)
{
	try {
		OPER c(curve);
		OPER d(date);
		c &= Excel(xlfText, date, L"mm/dd/yyyy");
		OPER data = Excel(xlfWebservice, c);
		// {"Series":[{"Points":[{"X":"1","Y":"2.903"},...]]

		insert_curve_row("ICE", date, view(data));
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}

AddIn xai_emma_curve(
	Function(XLL_FP, "xll_emma_curve", "XLL.EMMA.CURVE")
	.Arguments({
		Arg(XLL_DOUBLE, "date", "is the date of the curve."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to the curve data.")
);
_FP12* WINAPI xll_emma_curve(double date)
{
#pragma XLLEXPORT
	static FPX result;
	try {
		sqlite::stmt stmt(::db);
		stmt.prepare("SELECT data FROM emma WHERE date = ?");
		stmt.bind(1, date);
		// if not in database, insert and return xll_emma_curve(date)
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return nullptr;
	}

	return result.get();
}