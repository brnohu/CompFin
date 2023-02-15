#pragma once

#include <vector>
#include <set>
#include<span>
#include <algorithm>
#include <stdexcept>

using std::vector;
using std::set;
using std::span;
using std::enable_if_t;
using std::conditional;
using std::min;
using std::max;

template<typename T> class kMatrixView;
template<typename T> class kVectorView;

template<typename T=double> 
class kVector 
{
public:
	using Container = vector<T>;

	//	declarations
	using value_type = T;

	//	trivi c'tors
	kVector()=default;
	explicit kVector(size_t size) : myData(size){}
	kVector(size_t size, T t0) : myData(size,t0){}
	kVector(const kVector& rhs)=default;
	kVector(kVector&& rhs)noexcept=default;
	~kVector()noexcept=default;

	//	trivi assign
	kVector& operator=(const kVector& rhs)=default;
	kVector& operator=(kVector&&) noexcept=default;

	//	assign from single value
	kVector& operator=(const T& t)
	{
		const int s = (int)myData.size();
		for(int i=0;i<s;++i) myData[i]=t;
		return *this;
	}

	//	element access
	const T& operator[](int i) const
	{
#ifdef _DEBUG
		if(i<0 || i>=size())
		{
			throw std::runtime_error("kVector subscript out of range");
		}
#endif
		return myData[i];
	}
	
	T& operator[](int i)				
	{
#ifdef _DEBUG
		if(i<0 || i>=size())
		{
			throw std::runtime_error("kVector subscript out of range");
		}
#endif
		return myData[i];
	}
	
	const T& operator()(int i)				const{return operator[](i);}
	T&		operator()(int i)				{return operator[](i);}

	//	funcs
	int		size()							const{return (int)myData.size();}
	bool	empty()							const{return myData.empty();}
	void	reserve(size_t s)				{myData.reserve(s);}
	void	resize( size_t s)				{myData.resize(s);}
	void	resize( size_t s, const T& t0)	{myData.resize(s,t0);}
	void	assign( size_t s, const T& t0)	{myData.assign(s,t0);}
	void	clear()							{myData.clear();}
	void	push_back(const T& val)			{myData.push_back(val);}
	void	push_back(T&& val)				{myData.push_back(val);}

	const Container&	data() const{return myData;}
	Container&			data() {return myData;}

	//	to view
	explicit operator       kVectorView<T>()	          { return kVectorView<T>(*this); } 
	explicit operator const kVectorView<T>()        const { return kVectorView<T>(*this); } 

	const kVectorView<T> operator()()               const { return kVectorView<T>(*this); }
	kVectorView<T>       operator()()                     { return kVectorView<T>(*this); }
	const kVectorView<T> operator()(int i,int size) const { return kVectorView<T>(&(*this)(i),size); }
	kVectorView<T>		 operator()(int i,int size)       { return kVectorView<T>(&(*this)(i),size); }

	const kMatrixView<T> asRowMatrix() const { return kMatrixView<T>(const_cast<kVector<T>*>(this)->myData,1,size());}
	const kMatrixView<T> asColMatrix() const { return kMatrixView<T>(const_cast<kVector<T>*>(this)->myData,size(),1);}
	kMatrixView<T> asRowMatrix()			 { return kMatrixView<T>(data(),1,size()); }
	kMatrixView<T> asColMatrix()             { return kMatrixView<T>(data(),size(),1); }

private:
	Container myData;
};

template<typename T> class kMatrixView;

template<typename T> 
class kVectorView
{
public:
	//	declarations
	using view = span<T>;
	using value_type = T;

	//	trivi c'tors
	kVectorView()noexcept=default;
	kVectorView(const kVectorView&)noexcept=default;
	kVectorView(kVectorView&&) noexcept=default;
	~kVectorView()noexcept=default;

	//	c'tors will make a view on the rhs (i.e. updating values will update the rhs)
	kVectorView(kVector<T>& rhs): myView(rhs.data()){}
	kVectorView(const kVector<T>& rhs) : myView(const_cast<kVector<T>&>(rhs).data()){}
	kVectorView(vector<T>& rhs) : myView(rhs){}
	kVectorView(const vector<T>& rhs) : myView(const_cast<vector<T>&>(rhs)){}
	kVectorView(T& rhs) : myView(&rhs,1){}
	kVectorView(const T& rhs) : myView(&const_cast<T&>(rhs),1){}
	explicit kVectorView(view& rhs) : myView(rhs){}
	kVectorView(T* t, size_t count) : myView(t,count){}
	kVectorView(const T* t, size_t count) : myView(const_cast<T*>(t),count){}

	//	trivi assign
	kVectorView& operator=(const kVectorView&)noexcept=default;
	kVectorView& operator=(kVectorView&&) noexcept=default;

	//	assign from single value
	kVectorView& operator=(const T& t)
	{
		const int s = (int)myView.size();
		for(int i=0;i<s;++i) myView[i]=t;
		return *this;
	}

	//	to matrix view
	const kMatrixView<T> asRowMatrix() const { return kMatrixView<T>(const_cast<kVectorView<T>*>(this)->myView,1,size());}
	const kMatrixView<T> asColMatrix() const { return kMatrixView<T>(const_cast<kVectorView<T>*>(this)->myView,size(),1);}
	kMatrixView<T> asRowMatrix(){return kMatrixView<T>(data(),1,size());}
	kMatrixView<T> asColMatrix(){return kMatrixView<T>(data(),size(),1);}

	//	to view
	const kVectorView<T> operator()(int i,int size)	const{return kVectorView<T>(&(*this)(i),size);}
	kVectorView<T>		 operator()(int i,int size)	{return kVectorView<T>(&(*this)(i),size);}

	//	element access
	const T& operator[](int i) const
	{
#ifdef _DEBUG
		if(i<0 || i>=size()) throw std::runtime_error("kVectorView subscript out of range");
#endif
		return myView[i];
	}

	T& operator[](int i) 
	{
#ifdef _DEBUG
		if(i<0 || i>=size()) throw std::runtime_error("kVectorView subscript out of range");
#endif
		return myView[i];
	}

	const T&	operator()(int i)	const{return operator[](i);}
	T&			operator()(int i)	{return operator[](i);}

	//	funcs
	int			size()				const{return (int)myView.size();}
	bool		empty()				const{return size()==0;}

	const view& data()	const{return myView;}
	view& data()		{return myView;}

private:
	view myView;
};
