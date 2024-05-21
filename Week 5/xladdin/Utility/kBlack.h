#pragma once

//	desc:	black model closed forms
//	auth:	frederik kryger
//	date:	nov 2022	

//	includes
#include "kSpecialFunction.h"
#include "kInlines.h"
#include "kVector.h"
#include <cmath>
#include <algorithm>
#include <string>

using std::max;
using std::string;

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

	//	fd runner
	static bool	fdRunner(
		const double		s0,
		const double		r,
		const double		mu,
		const double		sigma,
		const double		expiry,
		const double		strike,
		const bool			dig,
		const int			pc,			//	put (-1) call (1)
		const int			ea,			//	european (0), american (1)
		const int			smooth,		//	smoothing
		const double		theta,
		const int			wind,
		const double		numStd,
		const int			numt,
		const int			numx,
		const bool			update,
		const int			numPr,
		double&				res0,
		kVector<double>&	s,
		kVector<double>&	res,
		string&				error);

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

	V std	 = volatility*sqrt(expiry);
	V xPlus	 = log(forward/strike)/std + 0.5*std;
	V xMinus = xPlus - std;
	V res	 = forward*kSpecialFunction::normalCdf(xPlus) - strike*kSpecialFunction::normalCdf(xMinus);

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