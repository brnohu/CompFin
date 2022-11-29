#pragma once

//	desc:	special function catalog
//	auth:	jesper andreasen
//	date:	nov 2022	

//	includes
#include "kConstants.h"
#include <cmath>

//	class
class kSpecialFunction
{
public:

	//	normal pdf
	template <class V>
	static V	normalPdf(V	x);

	//	normal pol
	template <class V>
	static V	normalPol(V x);

	//	normal cdf
	template <class V>
	static V	normalCdf(V x, V& pdf);


};

//	normal pdf
template <class V>
V
kSpecialFunction::normalPdf(
	V	x)
{
	V res = exp(-0.5 * x * x) * kConstants::oneOverSqrt2Pi();

	//	done
	return res;
}

//	normal pol
template <class V>
V
kSpecialFunction::normalPol(
	V	x)
{
	//	constants
	const double p = 0.23164'1900;
	const double b1 = 0.31938'1530;
	const double b2 = -0.35656'3782;
	const double b3 = 1.78147'7937;
	const double b4 = -1.82125'5978;
	const double b5 = 1.33027'4429;

	V t = 1.0 / (1.0 + p * x);
	V res = t * (b1 + t * (b2 + t * (b3 + t * (b4 + t * b5))));

	//	done
	return res;
}

//	normal cdf
template <class V>
V
kSpecialFunction::normalCdf(
	V	x,
	V& pdf)
{
	//	calc cdf(|x|)
	pdf = normalPdf(x);
	V res = pdf * normalPol(fabs(x));

	//	tjek
	if (x > 0.0) res = 1.0 - res;

	//	done
	return res;
}





