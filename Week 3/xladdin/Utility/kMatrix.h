#pragma once

#include "kVector.h"

template<typename T> class kMatrixView;

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

	//	get matrix as vector view
	explicit operator const kVectorView<T>()	const{return kVectorView<T>(myData);} 
	explicit operator kVectorView<T>()			{return kVectorView<T>(myData);} 
	const kVectorView<T>	asVector()			const{return kVectorView<T>(myData);}
	kVectorView<T>			asVector()			{return kVectorView<T>(myData);}

	//	get row view (if deep copy is needed, cast to kVector)
	const kMatrixView<T>	operator()() const {return kMatrixView<T >(*this);}
	kMatrixView<T>			operator()() {return kMatrixView<T >(*this);}
	const kVectorView<T>	operator()(int i) const
	{
		//	since a span is read/write it takes non-const pointer arg in c'tor and we have to use a const_cast for this const method (but returns const span)
		T* t = &(const_cast<kMatrix*>(this)->myData[rToIdx(i)]);
		return kVectorView<T >(t,myCols);
	}
	kVectorView<T> operator()(int i) 
	{
		return kVectorView<T>(&myData[rToIdx(i)],myCols);
	}


private:
	Container	myData;
	int			myRows{0};
	int			myCols{0};
};

template<typename T> 
class kMatrixView
{
public:
	//	declarations
	using view = span<T>;
	using value_type = T;

	//	trivi c'tors
	kMatrixView()noexcept=default;
	kMatrixView(const kMatrixView&)noexcept=default;
	kMatrixView(kMatrixView&&) noexcept=default;
	~kMatrixView()noexcept=default;

	//	c'tors will make a view on the rhs (i.e. updating values will update the rhs)
	kMatrixView(kMatrix<T>& rhs) : myView(rhs.data()),myRows(rhs.rows()),myCols(rhs.cols()){}
	kMatrixView(const kMatrix<T>& rhs) : myView(const_cast<kMatrix<T>&>(rhs).data()),myRows(rhs.rows()),myCols(rhs.cols()){}
	kMatrixView(T& rhs) : myView(&rhs,1),myRows(1),myCols(1){}
	kMatrixView(const T& rhs) : myView(&const_cast<T&>(rhs),1),myRows(1),myCols(1){}

	kMatrixView(vector<T>& rhs, size_t rows, size_t cols) : myView(rhs),myRows((int)rows),myCols((int)cols)
	{
#ifdef _DEBUG
		if(myRows*myCols!=myView.size()) throw std::runtime_error("kMatrixView::kMatrixView(vector): Row/col vs view size mismatch");
#endif
	}

	kMatrixView(const vector<T>& rhs, size_t rows, size_t cols) : myView(const_cast<vector<T>&>(rhs)),myRows((int)rows),myCols((int)cols)
	{
#ifdef _DEBUG
		if(myRows*myCols!=myView.size()) throw std::runtime_error("kMatrixView::kMatrixView(const vector): Row/col vs view size mismatch");
#endif
	}

	kMatrixView(view& rhs, size_t rows, size_t cols) : myView(rhs),myRows((int)rows),myCols((int)cols)
	{
#ifdef _DEBUG
		if(myRows*myCols!=myView.size()) throw std::runtime_error("kMatrixView::kMatrixView(view): Row/col vs view size mismatch");
#endif
	}

	kMatrixView(T* t, size_t rows, size_t cols) : myView(t,rows*cols),myRows((int)rows),myCols((int)cols)
	{
#ifdef _DEBUG
		if(myRows*myCols!=myView.size()) throw std::runtime_error("kMatrixView::kMatrixView(T*): Row/col vs view size mismatch");
#endif
	}
	
	kMatrixView(const T* t, size_t rows, size_t cols) : myView(const_cast<T*>(t),rows*cols),myRows((int)rows),myCols((int)cols)
	{
#ifdef _DEBUG
		if(myRows*myCols!=myView.size()) throw std::runtime_error("kMatrixView::kMatrixView(const T*): Row/col vs view size mismatch");
#endif
	}

	//	trivi assign
	kMatrixView& operator=(const kMatrixView&)noexcept=default;
	kMatrixView& operator=(kMatrixView&&) noexcept=default;

	//	assign from single value
	kMatrixView& operator=(const T& t)
	{
		const int s = (int)myView.size();
		for(int i=0;i<s;++i) myView[i]=t;
		return *this;
	}

	//	funcs
	int rows()		const{return myRows;}
	int cols()		const{return myCols;}
	int size()		const{return (int)myView.size();}
	bool empty()	const{return size()==0;}

	//	row,col to idx
	int rcToIdx(int i, int j) const{return i*myCols+j;}
	int rToIdx (int i)		  const{return i*myCols;}

	//	get element
	const T& operator()(int i, int j)const
	{
#ifdef _DEBUG
		if(i<0 || i>=myRows) throw std::runtime_error("kMatrixView row subscript out of range");
		if(j<0 || j>=myCols) throw std::runtime_error("kMatrixView col subscript out of range");
#endif
		return myView[rcToIdx(i,j)];
	}
	T& operator()(int i, int j)
	{
#ifdef _DEBUG
		if(i<0 || i>=myRows) throw std::runtime_error("kMatrixView row subscript out of range");
		if(j<0 || j>=myCols) throw std::runtime_error("kMatrixView col subscript out of range");
#endif
		return myView[rcToIdx(i,j)];
	}

	const T& operator[](int i) const
	{
#ifdef _DEBUG
		if(i<0 || i>=size()) throw std::runtime_error("kMatrixView subscript out of range");
#endif
		return myView[i];
	}

	T& operator[](int i) 
	{
#ifdef _DEBUG
		if(i<0 || i>=size()) throw std::runtime_error("kMatrixView subscript out of range");
#endif
		return myView[i];
	}

	//	get matrix as vector view
	explicit operator const kVectorView<T>()	const{return kVectorView<T>(myView);} 
	explicit operator kVectorView<T>()			{return kVectorView<T>(myView);} 
	const kVectorView<T>	asVector()			const{return kVectorView<T>(myView);}
	kVectorView<T>			asVector()			{return kVectorView<T>(myView);}

	//	get row view (if deep copy is needed, cast to kVector)
	const kVectorView<T>	operator()(int i) const
	{
		//	since a span is read/write it takes non-const pointer arg in c'tor and we have to use a const_cast for this const method (but returns const span)
		T* t = &(const_cast<kMatrixView*>(this)->myView[rToIdx(i)]);
		return kVectorView<T>(t,myCols);
	}
	kVectorView<T> operator()(int i) 
	{
		return kVectorView<T>(&myView[rToIdx(i)],myCols);
	}

	const view& data()	const{return myView;}
	view& data()		{return myView;}

private:
	view myView;
	int  myRows{0};
	int  myCols{0};
};
