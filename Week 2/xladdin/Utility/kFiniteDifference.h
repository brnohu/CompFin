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
		int					wind,
		const kVector<V>&	x,
		kMatrix<V>&			dx)
	{
		// todo: implement
	}

	//	2nd order diff operator
	template <class V>
	static void	dxx(
		const kVector<V>&	x,
		kMatrix<V>&			dxx)
	{
		// todo: implement
	}

};
