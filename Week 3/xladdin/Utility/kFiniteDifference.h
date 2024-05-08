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
	
	// to do : implement

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
	
	// to do: implement

	//	done
	return;
}


