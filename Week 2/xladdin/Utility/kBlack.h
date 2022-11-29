#pragma once

//	desc:	black model closed forms
//	auth:	frederik kryger
//	date:	nov 2022	

//	includes
#include "kSpecialFunction.h"
#include "kInlines.h"
#include <cmath>
#include <algorithm>

using std::max;

class kBlack
{
public:

	//	call 
	template <class V>
	static V	call(
		V		expiry,		//	in years
		V		strike,
		V		forward,
		V		volatility);

	//	vega
	template <class V>
	static V	vega(
		V		expiry,
		V		strike,
		V		forward,
		V		volatility);

	//	implied
	static double implied(
		double	expiry,
		double	strike,
		double	price,
		double	forward);
};

template <class V>
static V
kBlack::call(
	V	expiry,
	V	strike,
	V	forward,
	V	volatility)
{
	// expiry ok?
	if(expiry<=0) return max(forward-strike,0.0);

	V std		= volatility*sqrt(expiry);
	V xPlus		= log(forward/strike)/std + 0.5*std;
	V xMinus	= xPlus - std;
	V pdf;
	V res		= forward*kSpecialFunction::normalCdf(xPlus,pdf) - strike*kSpecialFunction::normalCdf(xMinus,pdf);

	// return
	return res;
}

template <class V>
static V
kBlack::vega(
	V	expiry,
	V	strike,
	V	forward,
	V	volatility)
{
	// expiry ok?
	if(expiry<=0) return 0.0;

	V sqrtT = sqrt(expiry);
	V std	= volatility*sqrtT;
	V xPlus	= log(forward/strike)/std + 0.5*std;
	V res	= forward*kSpecialFunction::normalPdf(xPlus)*sqrtT;

	// return
	return res;
}