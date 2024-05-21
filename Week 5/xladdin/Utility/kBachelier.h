#pragma once

//	desc:	bachelier model closed forms
//	auth:	jesper andreasen
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
		V				volatility);

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
	V res;
	res =  (forward - strike) * kSpecialFunction::normalCdf(x);
	res += std * kSpecialFunction::normalPdf(x);

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

