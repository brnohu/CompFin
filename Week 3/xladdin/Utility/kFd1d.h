#pragma once

//	desc:	1d finite difference solution for pdes of the form
//
//		0 = dV/dt + A V
//
//		A = -r + mu d/dx + 1/2 var d^2/dx^2
//
//	using the theta scheme
//
//		[1-theta dt A] V(t) = [1 + (1-theta) dt A] V(t+dt)
//

//	includes
#include "kFiniteDifference.h"
#include "kMatrixAlgebra.h"

//	class declaration
template <class V>
class kFd1d
{
public:

	//	init 
	void	init(
		int						numV,
		const kVector<V>&		x,
		bool					log);

	const kVector<V>& r()	const { return myR; }
	const kVector<V>& mu()	const { return myMu; }
	const kVector<V>& var()	const { return myVar; }
	const kVector<V>& x()	const { return myX; }
	const kVector<kVector<V>>& res() const { return myRes; }

	kVector<V>& r()		{ return myR; }
	kVector<V>& mu()	{ return myMu; }
	kVector<V>& var()	{ return myVar; }
	kVector<V>& x()		{ return myX; }
	kVector<kVector<V>>& res() { return myRes; }

	//	operator
	void	calcAx(
		const V&				one,
		const V&				dtTheta,
		int						wind,
		bool					tr,
		kMatrix<V>&				A) const;

	//	roll bwd
	void	rollBwd(
		const V&				dt,
		const V&				theta,
		int						wind,
		kVector<kVector<V>>&	res);

	//	roll fwd
	void	rollFwd(
		const V&				dt,
		const V&				theta,
		int						wind,
		kVector<kVector<V>>&	res);

private:
	//	log ?
	bool		myLog{ false };

	//	r, mu, var
	kVector<V>	myX, myR, myMu, myVar;

	//	diff operators
	kMatrix<V>	myDxd, myDxu, myDx, myDxx;

	//	operator matrix
	kMatrix<V>	myA;

	//	helper
	kVector<V>	myVs, myWs;

	//	vector of results
	kVector<kVector<V>> myRes;
};

//	init
template <class V>
void
kFd1d<V>::init(
	int					numV,
	const kVector<V>&	x,
	bool				log)
{
	myX = x;
	myRes.resize(numV);

	//	resize params
	myR.resize(myX.size(), 0.0);
	myMu.resize(myX.size(), 0.0);
	myVar.resize(myX.size(), 0.0);

	kFiniteDifference::dx(-1,myX,myDxd);
	kFiniteDifference::dx( 0,myX,myDx);
	kFiniteDifference::dx( 1,myX,myDxu);
	kFiniteDifference::dxx(  myX,myDxx);

	int numC = myDxx.cols();

	//	log transform case
	myLog = log;
	if(myLog)
	{
		int n = myX.size()-1;
		for(int i = 1;i<n;++i)
		{
			for(int j=0;j<numC;++j)
				myDxx(i,j) -= myDx(i,j);
		}
	}

	myA.resize(myX.size(),numC);
	myVs.resize(myX.size());
	myWs.resize(myX.size());

	//	done
	return;
}

//	construct operator
template <class V>
void
kFd1d<V>::calcAx(
	const V&		one,
	const V&		dtTheta,
	int				wind,
	bool			tr,
	kMatrix<V>&		A) const
{
	
	// to do: implement
	// 
	//	done
	return;
}

//	roll bwd
template <class V>
void
kFd1d<V>::rollBwd(
	const V&				dt,
	const V&				theta,
	int						wind,
	kVector<kVector<V>>&	res)
{
	//	helps
	int k;

	//	dims
	int n = myX.size();
	int mm = myDxx.cols()/2;
	int numV = (int)res.size();

	//	explicit
	if (theta != 1.0)
	{
		calcAx(1.0, dt * (1.0 - theta), wind, false, myA);
		for (k = 0; k < numV; ++k)
		{
			myVs = res[k];
			kMatrixAlgebra::banmul(myA, mm, mm, myVs, res[k]);
		}
	}

	//	implicit
	if (theta != 0.0)
	{
		calcAx(1.0, -dt * theta, wind, false, myA);
		for (k = 0; k < numV; ++k)
		{
			myVs = res[k];
			kMatrixAlgebra::tridag(myA, myVs, res[k], myWs);
		}
	}

	//	done
	return;
}

//	roll fwd
template <class V>
void
kFd1d<V>::rollFwd(
	const V&				dt,
	const V&				theta,
	int						wind,
	kVector<kVector<V>>&	res)
{
	//	dims
	int numV = res.size();
	int mm = myDxx.cols() / 2;

	//	help
	int k;

	//	implicit
	if (theta != 0.0)
	{
		calcAx(1.0, -dt * theta, wind, true, myA);
		for (k = 0; k < numV; ++k)
		{
			myVs = res[k];
			kMatrixAlgebra::tridag(myA, myVs, res[k], myWs);
		}
	}

	//	explicit
	if (theta != 1.0)
	{
		calcAx(1.0, dt * (1.0 - theta), wind, true, myA);
		for (k = 0; k < numV; ++k)
		{
			myVs = res[k];
			kMatrixAlgebra::banmul(myA, mm, mm, myVs, res[k]);
		}
	}

	//	done
	return;
}

