#pragma once

#include "kVector.h"

template<typename T=double> 
class kMatrix
{
public:
	//	declarations
	using Container=vector<T>;
	using value_type = T;

	//	trivi c'tors
	kMatrix()=default;
	kMatrix(size_t rows, size_t cols) : myData(rows*cols),myRows((int)rows),myCols((int)cols){}
	kMatrix(size_t rows, size_t cols, T t0) : myData(rows*cols,t0),myRows((int)rows),myCols((int)cols){}
	kMatrix(const kMatrix& rhs)=default;
	kMatrix(kMatrix&& rhs)noexcept=default;
	~kMatrix()noexcept=default;

	//	trivi assign
	kMatrix& operator=(const kMatrix& rhs)=default;
	kMatrix& operator=(kMatrix&& rhs)=default;

	//	assign from single value
	kMatrix& operator=(const T& t)
	{
		const int s = (int)myData.size();
		for(int i=0;i<s;++i) myData[i]=t;
		return *this;
	}

	//	funcs
	int rows()		const{return myRows;}
	int cols()		const{return myCols;}
	int size()		const{return (int)myData.size();}
	bool empty()	const{return size()==0;}

	//	row,col to idx
	int rcToIdx(int i, int j) const{return i*myCols+j;}
	int rToIdx (int i)		  const{return i*myCols;}

	//	get element
	const T& operator()(int i, int j)const
	{
#ifdef _DEBUG
		if(i<0 || i>=myRows) throw std::runtime_error("kMatrix row subscript out of range");
		if(j<0 || j>=myCols) throw std::runtime_error("kMatrix col subscript out of range");
#endif
		return myData[rcToIdx(i,j)];
	}
	T& operator()(int i, int j)
	{
#ifdef _DEBUG
		if(i<0 || i>=myRows) throw std::runtime_error("kMatrix row subscript out of range");
		if(j<0 || j>=myCols) throw std::runtime_error("kMatrix col subscript out of range");
#endif
		return myData[rcToIdx(i,j)];
	}

	const T& operator[](int i) const
	{
#ifdef _DEBUG
		if(i<0 || i>=size()) throw std::runtime_error("kMatrix subscript out of range");
#endif
		return myData[i];
	}
	
	T& operator[](int i)
	{
#ifdef _DEBUG
		if(i<0 || i>=size()) throw std::runtime_error("kMatrix subscript out of range");
#endif
		return myData[i];
	}

	//	resize
	void resize(size_t rows, size_t cols)				
	{
		resize(rows,cols,T{});
	}
	void resize(size_t rows, size_t cols, const T& t0)
	{
		if(cols==myCols)
		{
			myData.resize(rows*cols,t0);
			myRows = (int)rows;
			return;
		}

		Container tmp;
		swap(tmp,myData);
		myData.resize(rows*cols,t0);

		int minR = min<int>((int)rows,myRows);
		int minC = min<int>((int)cols,myCols);
		for(int i=0;i<minR;++i)
		{
			for(int j=0;j<minC;++j)
			{
				myData[i*cols+j]=tmp[i*myCols+j];
			}
		}

		myRows = (int)rows;
		myCols = (int)cols;
	}
	void clear(){ resize(0,0); }

	//	data
	const Container&	data() const{return myData;}
	Container&			data() {return myData;}


private:
	Container	myData;
	int			myRows{0};
	int			myCols{0};
};

