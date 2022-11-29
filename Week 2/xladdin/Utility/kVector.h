#pragma once

#include <vector>
#include <set>
#include <algorithm>
#include <stdexcept>

using std::vector;
using std::set;
using std::enable_if_t;
using std::conditional;
using std::min;
using std::max;

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

private:
	Container myData;
};
