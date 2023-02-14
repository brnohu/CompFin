#pragma once

//	includes
#include "kMatrix.h"
#include "kInlines.h"

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
		const kMatrixView<V>	A,		//	n x 3
		const kVectorView<V>	r,
		kVectorView<V>			u,
		kVectorView<V>			gam)
	{
		//	helps
		V bet;
		int j;

		//	dim
		int n = A.rows();

		//	go
		u(0) = r(0)/(bet=A(0,1));
		for(j=1;j<n;++j)
		{
			gam(j) = A(j-1,2)/bet;
			bet    = A(j,1)-A(j,0)*gam(j);
			u(j)   = (r(j)-A(j,0)*u(j-1))/bet;
		}
		for(j=n-2;j>=0;--j)
		{
			u(j) -= gam(j+1)*u(j+1);
		}

		//	done
		return;
	}

	//	tridag: solves A u = r when A is tridag
	template <class V>
	void	tridag(
		const kMatrix<V>&	    A,		//	n x 3
		const kVector<V>&	    r,
		kVector<V>&				u,
		kVector<V>&				gam)
	{
		//	dim
		int n = A.rows();

		//	check dim
		if(u.size()<n) u.resize(n);
		if(gam.size()<n) gam.resize(n);

		tridag(A(), r(), u(), gam());
		//	done
		return;
	}

	//	band diagonal matrix vector multiplication
	template <class V>
	void banmul(
		const kMatrixView<V> A,		//	n x 3
		int					 m1,
		int					 m2,
		const kVectorView<V> b,
		kVectorView<V>		 x)
	{
		int n = A.rows()-1;
		V xi;
		for(int i = 0;i<=n;++i)
		{
			int jl = max<int>(0, i - m1);
			int ju = min<int>(i + m2, n);
			xi = 0.0;
			for(int j = jl;j<=ju;++j)
			{
				int k = j - i + m1;
				xi += A(i,k)*b(j);
			}
			x(i) = xi;
		}

		//	done
		return;
	}

	//	band diagonal matrix vector multiplication
	template <class V>
	void banmul(
		const kMatrix<V>&	A,		//	n x 3
		int					m1,
		int					m2,	 
		const kVector<V>&	b,
		kVector<V>&			x)
	{
		int n = A.rows()-1;
		x.resize(n+1);

		banmul(A(), m1, m2, b(), x());

		//	done
		return;
	}

	//	transposing a banddiagonal matrix
	template <class V>
	void	transpose(
		const int			mm,
		kMatrix<V>&			A)
	{
		int n = A.rows()-1;
		int i, j, k, l;
		int jl;
		for(i=0;i<=n;++i)
		{
			jl = max(0, i - mm);
			for(j=jl;j<i;++j)
			{
				k = j - i + mm;
				l = i - j + mm;
				kInlines::swap(A(i, k), A(j, l));
			}
		}

		//	done
		return;
	}

}
