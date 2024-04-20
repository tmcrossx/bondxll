// xll_black.cpp - Black forward model
#include "../bondlib/tmx_option.h"
#include "bondxll.h"

using namespace xll;
using namespace tmx;

AddIn xai_black_moneyness(
	Function(XLL_DOUBLE, L"xll_black_moneyness", CATEGORY L".BLACK.MONEYNESS")
	.Arguments({
	Arg(XLL_DOUBLE, L"f", L"is the forward price."),
	Arg(XLL_DOUBLE, L"s", L"is the vol."),
	Arg(XLL_DOUBLE, L"k", L"is the strike."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the Black moneyness log(k/f)/s + s/2.")
);
double WINAPI xll_black_moneyness(double f, double s, double k)
{
#pragma XLLEXPORT
	return option::moneyness(f, s, k);
}

AddIn xai_black_put_value(
	Function(XLL_DOUBLE, L"xll_black_put_value", CATEGORY L".BLACK.PUT.VALUE")
	.Arguments({
	Arg(XLL_DOUBLE, L"f", L"is the forward price."),
	Arg(XLL_DOUBLE, L"s", L"is the vol."),
	Arg(XLL_DOUBLE, L"k", L"is the strike."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return the Black put value.")
);
double WINAPI xll_black_put_value(double f, double s, double k)
{
#pragma XLLEXPORT
	return option::put::value(f, s, k);
}

AddIn xai_black_put_delta(
	Function(XLL_DOUBLE, L"xll_black_put_delta", CATEGORY L".BLACK.PUT.DELTA")
	.Arguments({
	Arg(XLL_DOUBLE, L"f", L"is the forward price."),
	Arg(XLL_DOUBLE, L"s", L"is the vol."),
	Arg(XLL_DOUBLE, L"k", L"is the strike."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return the Black put delta.")
);
double WINAPI xll_black_put_delta(double f, double s, double k)
{
#pragma XLLEXPORT
	return option::put::delta(f, s, k);
}

AddIn xai_black_put_gamma(
	Function(XLL_DOUBLE, L"xll_black_put_gamma", CATEGORY L".BLACK.GAMMA")
	.Arguments({
	Arg(XLL_DOUBLE, L"f", L"is the forward price."),
	Arg(XLL_DOUBLE, L"s", L"is the vol."),
	Arg(XLL_DOUBLE, L"k", L"is the strike."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return the Black put gamma.")
);
double WINAPI xll_black_put_gamma(double f, double s, double k)
{
#pragma XLLEXPORT
	return option::put::gamma(f, s, k);
}

AddIn xai_black_put_vega(
	Function(XLL_DOUBLE, L"xll_black_put_vega", CATEGORY L".BLACK.VEGA")
	.Arguments({
	Arg(XLL_DOUBLE, L"f", L"is the forward price."),
	Arg(XLL_DOUBLE, L"s", L"is the vol."),
	Arg(XLL_DOUBLE, L"k", L"is the strike."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return the Black put vega.")
);
double WINAPI xll_black_put_vega(double f, double s, double k)
{
#pragma XLLEXPORT
	return option::put::vega(f, s, k);
}

AddIn xai_black_call_value(
	Function(XLL_DOUBLE, L"xll_black_call_value", CATEGORY L".BLACK.CALL.VALUE")
	.Arguments({
	Arg(XLL_DOUBLE, L"f", L"is the forward price."),
	Arg(XLL_DOUBLE, L"s", L"is the vol."),
	Arg(XLL_DOUBLE, L"k", L"is the strike."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return the Black call value.")
);
double WINAPI xll_black_call_value(double f, double s, double k)
{
#pragma XLLEXPORT
	return option::call::value(f, s, k);
}

AddIn xai_black_call_delta(
	Function(XLL_DOUBLE, L"xll_black_call_delta", CATEGORY L".BLACK.CALL.DELTA")
	.Arguments({
	Arg(XLL_DOUBLE, L"f", L"is the forward price."),
	Arg(XLL_DOUBLE, L"s", L"is the vol."),
	Arg(XLL_DOUBLE, L"k", L"is the strike."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return the Black call delta.")
);
double WINAPI xll_black_call_delta(double f, double s, double k)
{
#pragma XLLEXPORT
	return option::call::delta(f, s, k);
}

AddIn xai_black_put_implied(
	Function(XLL_DOUBLE, L"xll_black_put_implied", CATEGORY L".BLACK.PUT.IMPLIED")
	.Arguments({
	Arg(XLL_DOUBLE, L"f", L"is the forward price."),
	Arg(XLL_DOUBLE, L"p", L"is the put price."),
	Arg(XLL_DOUBLE, L"k", L"is the strike."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return the Black implied volatility repricing a put.")
);
double WINAPI xll_black_put_implied(double f, double p, double k)
{
#pragma XLLEXPORT
	return option::put::implied(f, p, k);
}
