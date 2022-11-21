#include <windows.h>

#include "xlcall.h"
#include "framework.h"
#include "xlOper.h"

#include "../Utility/kMatrixAlgebra.h"
#include "../Utility/kBachelier.h"
#include "../Utility/xlUtils.h"

//	Wrappers

extern "C" __declspec(dllexport)
double xMultiply2Numbers(double x, double y)
{
	return x * y;
}

extern "C" __declspec(dllexport)
LPXLOPER12 xMatrixMul(LPXLOPER12 A_in, LPXLOPER12 B_in)
{
	FreeAllTempMemory();

	kMatrix<double> A;
	if(!kXlUtils::getMatrix(A_in, A))
		return TempStr12("input 1 is not a matrix");

	kMatrix<double> B;
	if(!kXlUtils::getMatrix(B_in, B))
		return TempStr12("input 2 is not a vector");

	if(B.rows()!=A.cols())
		return TempStr12("input 2 must have number of rows same as input 1 number of cols");

	kMatrix<double> res;
	kMatrixAlgebra::mmult(A, B, res);

	LPXLOPER12 out = TempXLOPER12();
	kXlUtils::setMatrix(res, out);
	return out;
}

extern "C" __declspec(dllexport)
LPXLOPER12
xBachelierCall(
	LPXLOPER12	expiry_,
	LPXLOPER12	strike_,
	LPXLOPER12	forward_,
	LPXLOPER12	volatility_)
{
	FreeAllTempMemory();

	//	helps
	string err;

	//	get expiry
	double expiry;
	if (!kXlUtils::getDbl(expiry_, 0, 0, expiry, &err)) return kXlUtils::setError(err);

	//	get strike
	double strike;
	if (!kXlUtils::getDbl(strike_, 0, 0, strike, &err)) return kXlUtils::setError(err);

	//	get forwad
	double forward;
	if (!kXlUtils::getDbl(forward_, 0, 0, forward, &err)) return kXlUtils::setError(err);

	//	get volatility
	double volatility;
	if (!kXlUtils::getDbl(volatility_, 0, 0, volatility, &err)) return kXlUtils::setError(err);

	//	calc
	double call = kBachelier::call(expiry, strike, forward, volatility);

	//	set output
	LPXLOPER12 out = kXlUtils::getOper(1, 1);
	kXlUtils::setDbl(0, 0, call, out);

	//	done
	return out;
}

extern "C" __declspec(dllexport)
LPXLOPER12
xBachelierImplied(
	LPXLOPER12	expiry_,
	LPXLOPER12	strike_,
	LPXLOPER12	price_,
	LPXLOPER12	forward_)
{
	FreeAllTempMemory();

	//	helps
	string err;

	//	get expiry
	double expiry;
	if (!kXlUtils::getDbl(expiry_, 0, 0, expiry, &err)) return kXlUtils::setError(err);

	//	get strike
	double strike;
	if (!kXlUtils::getDbl(strike_, 0, 0, strike, &err)) return kXlUtils::setError(err);

	//	get volatility
	double price;
	if (!kXlUtils::getDbl(price_, 0, 0, price, &err)) return kXlUtils::setError(err);

	//	get forward
	double forward;
	if (!kXlUtils::getDbl(forward_, 0, 0, forward, &err)) return kXlUtils::setError(err);

	//	calc
	double volatility = kBachelier::implied(expiry, strike, price, forward);

	//	set output
	LPXLOPER12 out = kXlUtils::getOper(1, 1);
	kXlUtils::setDbl(0, 0, volatility, out);

	//	done
	return out;
}


//	Registers

extern "C" __declspec(dllexport) int xlAutoOpen(void)
{
	XLOPER12 xDLL;	

	Excel12f(xlGetName, &xDLL, 0);

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xMultiply2Numbers"),
		(LPXLOPER12)TempStr12(L"BBB"),
		(LPXLOPER12)TempStr12(L"xMultiply2Numbers"),
		(LPXLOPER12)TempStr12(L"x, y"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Multiplies 2 numbers"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xMatrixMul"),
		(LPXLOPER12)TempStr12(L"QQQ"),
		(LPXLOPER12)TempStr12(L"xMatrixMul"),
		(LPXLOPER12)TempStr12(L"A, B"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Multiplying 2 matrices."),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xBachelierCall"),
		(LPXLOPER12)TempStr12(L"QQQQQ"),
		(LPXLOPER12)TempStr12(L"xBachelierCall"),
		(LPXLOPER12)TempStr12(L"expiry, strike, forward, volatility"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Price option in Bachelier model"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xBachelierImplied"),
		(LPXLOPER12)TempStr12(L"QQQQQ"),
		(LPXLOPER12)TempStr12(L"xBachelierImplied"),
		(LPXLOPER12)TempStr12(L"expiry, strike, price, forward"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Compute implied volatility in Bachelier model"),
		(LPXLOPER12)TempStr12(L""));
	
	/* Free the XLL filename */
	Excel12f(xlFree, 0, 1, (LPXLOPER12)&xDLL);

	return 1;
}
