#include <windows.h>
#include <cmath>

#include "xlcall.h"
#include "framework.h"
#include "xlOper.h"

#include "../Utility/kMatrixAlgebra.h"
#include "../Utility/kBachelier.h"
#include "../Utility/kBlack.h"
#include "../Utility/xlUtils.h"
#include "../Utility/kFd1d.h"

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
		return TempStr12("input 2 is not a matrix");

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

extern "C" __declspec(dllexport)
LPXLOPER12
xBlackCall(
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
	double call = kBlack::call(expiry, strike, forward, volatility);

	//	set output
	LPXLOPER12 out = kXlUtils::getOper(1, 1);
	kXlUtils::setDbl(0, 0, call, out);

	//	done
	return out;
}

extern "C" __declspec(dllexport)
LPXLOPER12
xBlackImplied(
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
	double volatility = kBlack::implied(expiry, strike, price, forward);

	//	set output
	LPXLOPER12 out = kXlUtils::getOper(1, 1);
	kXlUtils::setDbl(0, 0, volatility, out);

	//	done
	return out;
}

extern "C" __declspec(dllexport)
LPXLOPER12 xTridag(LPXLOPER12 A_in, LPXLOPER12 b_in)
{
	FreeAllTempMemory();

	kMatrix<double> A;
	if(!kXlUtils::getMatrix(A_in, A))
		return TempStr12("input 1 is not a matrix");

	kVector<double> b;
	if(!kXlUtils::getVector(b_in, b))
		return TempStr12("input 2 is not a vector");

	if(b.size()!=A.rows())
		return TempStr12("input 1 must have same number of rows as input 2");

	kVector<double> gam, res;
	kMatrixAlgebra::tridag(A, b, res, gam);

	LPXLOPER12 out = TempXLOPER12();
	kXlUtils::setVector(res, out);
	return out;
}

extern "C" __declspec(dllexport)
LPXLOPER12 xBanmul(LPXLOPER12 A_in, LPXLOPER12 x_in, double m1_in, double m2_in)
{
	FreeAllTempMemory();

	kMatrix<double> A;
	if(!kXlUtils::getMatrix(A_in, A))
		return TempStr12("input 1 is not a matrix");

	kVector<double> x;
	if(!kXlUtils::getVector(x_in, x))
		return TempStr12("input 2 is not a vector");

	if(x.size()!=A.rows())
		return TempStr12("input 1 must have same number of rows as input 2");

	kVector<double> res;
	int m1=(int)(m1_in+0.5), m2=(int)(m2_in+0.5);
	kMatrixAlgebra::banmul(A, m1, m2, x, res);

	LPXLOPER12 out = TempXLOPER12();
	kXlUtils::setVector(res, out);
	return out;
}

extern "C" __declspec(dllexport)
LPXLOPER12 xFd1d(LPXLOPER12 t_in, LPXLOPER12 x_in, LPXLOPER12 r_in, LPXLOPER12 mu_in, LPXLOPER12 sigma_in, LPXLOPER12 v0_in, LPXLOPER12 tech_in)
{
	FreeAllTempMemory();

	//	help
	string err;

	//	get t 
	double t;
	if(!kXlUtils::getDbl(t_in, 0, 0, t, &err)) return kXlUtils::setError(err);

	//	get tech
	kVector<double> tech;
	if(!kXlUtils::getVector(tech_in, tech))
		return kXlUtils::setError("input 1 is not a vector");

	//	standard data
	int    numt  = tech.size()>0 ? (int)std::lround(tech(0)) : 1;
	double theta = tech.size()>1 ? tech(1) : 0.5;
	int    fb    = tech.size()>2 ? (int)std::lround(tech(2)) : -1;
	int    log   = tech.size()>3 ? (int)std::lround(tech(3)) :  0;
	int    wind  = tech.size()>4 ? (int)std::lround(tech(4)) :  0;

	//	fd grid
	kFd1d<double> fd;
	if(!kXlUtils::getVector(x_in, fd.x()))
		return kXlUtils::setError("input 1 is not a vector");

	int n = fd.x().size();

	//	init fd
	fd.init(1,fd.x(),log>0);

	if(!kXlUtils::getVector(r_in, fd.r()))
		return kXlUtils::setError("r is not a vector");

	if(n!=fd.r().size())
		return kXlUtils::setError("r must have same size as x");

	if(!kXlUtils::getVector(mu_in, fd.mu()))
		return kXlUtils::setError("mu is not a vector");

	if(n!=fd.mu().size())
		return kXlUtils::setError("mu must have same size as x");

	if(!kXlUtils::getVector(sigma_in, fd.var()))
		return kXlUtils::setError("sigma is not a vector");

	if(n!=fd.var().size())
		return kXlUtils::setError("sigma must have same size as x");

	auto& fd_var = fd.var();
	for(int i=0;i<fd_var.size();++i)
		fd_var(i) *= fd_var(i);

	fd.res().resize(1);
	if(!kXlUtils::getVector(v0_in, fd.res()[0]))
		return kXlUtils::setError("input 2 is not a vector");

	if(n!=fd.res()[0].size())
		return kXlUtils::setError("v0 must have same size as x");

	if(theta<0.0) theta=0.0;
	if(theta>1.0) theta=1.0;

	double dt = t/numt;
	for(int n=0;n<numt;++n)
	{
		if(fb<=0)
		{
			fd.rollBwd(dt,theta,wind,fd.res());
		}
		else
		{
			fd.rollFwd(dt,theta,wind,fd.res());
		}
	}

	LPXLOPER12 out = TempXLOPER12();
	kXlUtils::setVector(fd.res()[0], out);
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

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xBlackCall"),
		(LPXLOPER12)TempStr12(L"QQQQQ"),
		(LPXLOPER12)TempStr12(L"xBlackCall"),
		(LPXLOPER12)TempStr12(L"expiry, strike, forward, volatility"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Price option in Black model"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xBlackImplied"),
		(LPXLOPER12)TempStr12(L"QQQQQ"),
		(LPXLOPER12)TempStr12(L"xBlackImplied"),
		(LPXLOPER12)TempStr12(L"expiry, strike, price, forward"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Compute implied volatility in Black model"),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xTridag"),
		(LPXLOPER12)TempStr12(L"QQQ"),
		(LPXLOPER12)TempStr12(L"xTridag"),
		(LPXLOPER12)TempStr12(L"A, b"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Solving tri-diagonal system."),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xBanmul"),
		(LPXLOPER12)TempStr12(L"QQQBB"),
		(LPXLOPER12)TempStr12(L"xBanmul"),
		(LPXLOPER12)TempStr12(L"A, x, m1, m2"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Multiplying band-matrix with vector."),
		(LPXLOPER12)TempStr12(L""));

	Excel12f(xlfRegister, 0, 11, (LPXLOPER12)&xDLL,
		(LPXLOPER12)TempStr12(L"xFd1d"),
		(LPXLOPER12)TempStr12(L"QQQQQQQQ"),
		(LPXLOPER12)TempStr12(L"xFd1d"),
		(LPXLOPER12)TempStr12(L"t, x, r, mu, sigma, v0, tech"),
		(LPXLOPER12)TempStr12(L"1"),
		(LPXLOPER12)TempStr12(L"myOwnCppFunctions"),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L""),
		(LPXLOPER12)TempStr12(L"Solve 1d fd."),
		(LPXLOPER12)TempStr12(L""));
	
	/* Free the XLL filename */
	Excel12f(xlFree, 0, 1, (LPXLOPER12)&xDLL);

	return 1;
}
