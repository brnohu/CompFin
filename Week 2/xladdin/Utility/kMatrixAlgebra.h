#pragma once

#include "kMatrix.h"

//	class
namespace kMatrixAlgebra
{
	//	mat mult res = A * B
	template <class U, class V, class W>
	void mmult(
		const kMatrix<U>&	a,
		const kMatrix<V>&	b,
		kMatrix<W>&			ab)
	{
		//	dims
		int m1 = a.rows();
		int m2 = a.cols();
		int m3 = b.cols();

		ab.resize(m1, m3, 0.0);

		//	calc
		for(int i=0;i<m1;++i)
		{
			for(int k=0;k<m2;++k)
			{
				for(int j=0;j<m3;++j)
					ab(i,j) += a(i,k)*b(k,j);
			}
		}

		//	done
		return;
	}

	//	tridag: solves A u = r when A is tridag
	template <class V>
	void	tridag(
		const kMatrix<V>&	A,		//	n x 3
		const kVector<V>&	r,
		kVector<V>&			u,
		kVector<V>&			gam)
	{
		// todo: Implement
	}

	template <class V>
	void banmul(
		const kMatrix<V>&	A,		//	n x 3
		int					m1,
		int					m2,
		const kVector<V>&	b,
		kVector<V>&			x)
	{
		// todo: Implement
	}
}
