#pragma once

//	desc:	various tools for finite difference

//	includes
#include "kVector.h"
#include "kMatrix.h"

//	class declaration
class kFiniteDifference
{
public:

	//	1st order diff operator
	template <class V>
	static void	dx(
		const int			wind,
		const kVector<V>&	x,
		kMatrix<V>&			dx);

	//	2nd order diff operator
	template <class V>
	static void	dxx(
		const kVector<V>&	x,
		kMatrix<V>&			dxx);
	
};

//	dx operator
template <class V>
void 
kFiniteDifference::dx(
	const int			wind,
	const kVector<V>&	x,
	kMatrix<V>&			dx)
{
	//	dim
	int n = x.size();
	int m = n - 1;

	//	resize
	dx.resize(n,3);
	if(!n) return;

	//	helpers
	int i, j;
	V dxl, dxu, dxm;

	//	down
	if(wind<0)
	{
		for(j=0;j<3;++j) dx(0, j) = 0.0;
		for(i=1;i<=m;++i)
		{
			dxl = x(i) - x(i - 1);
			dx(i, 0) = -1.0/dxl;
			dx(i, 1) =  1.0/dxl;
			dx(i, 2) =  0.0;
		}
	}
	else if(wind==0)	//	central
	{
		dxu = x(1) - x(0);
		dx(0, 0) = 0.0;
		dx(0, 1) = -1.0 / dxu;
		dx(0, 2) = 1.0 / dxu;
		for (i = 1; i < m; ++i)
		{
			dxl = x(i) - x(i - 1);
			dxu = x(i + 1) - x(i);
			dxm = dxl + dxu;
			dx(i, 0) = -dxu / dxl/dxm;
			dx(i, 1) = (dxu / dxl - dxl / dxu) / dxm;
			dx(i, 2) =  dxl / dxu / dxm;
		}
		dxl = x(m) - x(m - 1);
		dx(m, 0) = -1.0 / dxl;
		dx(m, 1) = 1.0 / dxl;
		dx(m, 2) = 0.0;
	}
	else if (wind > 0)	//	up
	{
		for (i = 0; i < m; ++i)
		{
			dxu = x(i + 1) - x(i);
			dx(i, 0) = 0.0;
			dx(i, 1) = -1.0 / dxu;
			dx(i, 2) = 1.0 / dxu;
		}
		for (j = 0; j < 3; ++j) dx(m, j) = 0.0;
	}

	//	done
	return;
}

//	2nd order diff operator
template <class V>
void	
kFiniteDifference::dxx(
	const kVector<V>&	x,
	kMatrix<V>&			dxx)
{
	//	dims
	int n = x.size();
	int m = n - 1;

	//	resize
	dxx.resize(n, 3);
	if(!n) return;

	//	helps
	int i, j;
	V dxl, dxu, dxm;

	//	loop
	for (j = 0; j < 3; ++j) dxx(0, j) = 0.0;
	for (i = 1; i < m; ++i)
	{
		dxl = x(i) - x(i - 1);
		dxu = x(i + 1) - x(i);
		dxm = 0.5*(dxl + dxu);
		dxx(i, 0) = 1.0/dxl/dxm;
		dxx(i, 1) = -(1.0 / dxl + 1.0 / dxu) / dxm;
		dxx(i, 2) = 1.0 / dxu / dxm;
	}
	for (j = 0; j < 3; ++j) dxx(m, j) = 0.0;

	//	done
	return;
}


