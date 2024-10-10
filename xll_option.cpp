// xll_option.cpp - Generic option forward pricing
#include "value/tmx_option.h"
#include "bondxll.h"

using namespace xll;
using namespace tmx;
/*
AddIn xai_normal_cdf(
	Function(XLL_DOUBLE, "xll_normal_cdf", CATEGORY ".NORMAL.CDF")
	.Arguments({
		Arg(XLL_DOUBLE, "x", "is a number."),
		Arg(XLL_DOUBLE, "s", "is the vol."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return the normal cumulative distribution function.")
);
double WINAPI xll_normal_cdf(double x, double s)
{
#pragma XLLEXPORT
	return black::normal::cdf(x, s);
}
*/
AddIn xai_option_moneyness(
	Function(XLL_DOUBLE, "xll_option_moneyness", CATEGORY ".OPTION.MONEYNESS")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward price.", 100),
		Arg(XLL_DOUBLE, "s", "is the vol.", .1),
		Arg(XLL_DOUBLE, "k", "is the strike.", 100),
	})
	.Category(CATEGORY)
	.FunctionHelp("Return the option moneyness log(k/f)/s + s/2.")
);
double WINAPI xll_option_moneyness(double f, double s, double k)
{
#pragma XLLEXPORT
	return option::moneyness(f, s, k);
}

AddIn xai_option_put_value(
	Function(XLL_DOUBLE, "xll_option_put_value", CATEGORY ".OPTION.PUT.VALUE")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward price."),
		Arg(XLL_DOUBLE, "s", "is the vol."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
	})
	.Category(CATEGORY)
	.FunctionHelp("Return the option put value.")
);
double WINAPI xll_option_put_value(double f, double s, double k)
{
#pragma XLLEXPORT
	return option::put::value(f, s, k);
}

AddIn xai_option_put_delta(
	Function(XLL_DOUBLE, "xll_option_put_delta", CATEGORY ".OPTION.PUT.DELTA")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward price."),
		Arg(XLL_DOUBLE, "s", "is the vol."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
	})
	.Category(CATEGORY)
	.FunctionHelp("Return the option put delta.")
);
double WINAPI xll_option_put_delta(double f, double s, double k)
{
#pragma XLLEXPORT
	return option::put::delta(f, s, k);
}

AddIn xai_option_put_gamma(
	Function(XLL_DOUBLE, "xll_option_put_gamma", CATEGORY ".OPTION.GAMMA")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward price."),
		Arg(XLL_DOUBLE, "s", "is the vol."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
	})
	.Category(CATEGORY)
	.FunctionHelp("Return the option put gamma.")
);
double WINAPI xll_option_put_gamma(double f, double s, double k)
{
#pragma XLLEXPORT
	return option::put::gamma(f, s, k);
}

AddIn xai_option_put_vega(
	Function(XLL_DOUBLE, "xll_option_put_vega", CATEGORY ".OPTION.VEGA")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward price."),
		Arg(XLL_DOUBLE, "s", "is the vol."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the option put vega.")
);
double WINAPI xll_option_put_vega(double f, double s, double k)
{
#pragma XLLEXPORT
	return option::put::vega(f, s, k);
}

AddIn xai_option_call_value(
	Function(XLL_DOUBLE, "xll_option_call_value", CATEGORY ".OPTION.CALL.VALUE")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward price."),
		Arg(XLL_DOUBLE, "s", "is the vol."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
	})
	.Category(CATEGORY)
	.FunctionHelp("Return the option call value.")
);
double WINAPI xll_option_call_value(double f, double s, double k)
{
#pragma XLLEXPORT
	return option::call::value(f, s, k);
}

AddIn xai_option_call_delta(
	Function(XLL_DOUBLE, "xll_option_call_delta", CATEGORY ".OPTION.CALL.DELTA")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward price."),
		Arg(XLL_DOUBLE, "s", "is the vol."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
	})
	.Category(CATEGORY)
	.FunctionHelp("Return the option call delta.")
);
double WINAPI xll_option_call_delta(double f, double s, double k)
{
#pragma XLLEXPORT
	return option::call::delta(f, s, k);
}

AddIn xai_option_put_implied(
	Function(XLL_DOUBLE, "xll_option_put_implied", CATEGORY ".OPTION.PUT.IMPLIED")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward price."),
		Arg(XLL_DOUBLE, "p", "is the put price."),
		Arg(XLL_DOUBLE, "k", "is the strike."), 
	})
	.Category(CATEGORY)
	.FunctionHelp("Return the option implied volatility repricing a put.")
);
double WINAPI xll_option_put_implied(double f, double p, double k)
{
#pragma XLLEXPORT
	return option::put::implied(f, p, k);
}
