#pragma once

//	desc:	bachelier model closed forms
//	auth:	jesper andreasen
//	date:	nov 2022	

//	includes
#include "kSpecialFunction.h"
#include "kInlines.h"
#include <cmath>
#include <algorithm>

using std::max;

//	class
class kBachelier
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

//	call 
template <class V>
static V
kBachelier::call(
	V	expiry,
	V	strike,
	V	forward,
	V	volatility)
{
	if (expiry <= 0.0) return max(0.0, forward - strike);

	V std = volatility * sqrt(expiry);
	V x = (forward - strike) / std;
	V pdf, res;
	res = (forward - strike) * kSpecialFunction::normalCdf(x, pdf);
	res += std * pdf;

	//	done
	return res;
}

//	vega
template <class V>
static V
kBachelier::vega(
	V	expiry,
	V	strike,
	V	forward,
	V	volatility)
{
	if (expiry <= 0.0) return 0.0;

	V st = sqrt(expiry);
	V x = (forward - strike) / (volatility * st);
	V res = st * kSpecialFunction::normalPdf(x);

	//	done
	return res;
}

