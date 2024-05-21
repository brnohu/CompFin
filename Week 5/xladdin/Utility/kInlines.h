#pragma once

//	desc:	a few inlines help functions
//	auth:	jesper andreasen
//	date:	nov 2022	

class kInlines
{
public:

	//	bound
	template <class V>
	static V	bound(V a, V x, V b);

	//	swap
	template <class V>
	static void	swap(V& a, V& b);

	//	sign
	template <class V>
	static int	sign(V x);

	//	sqr
	template <class V>
	static V	sqr(V x);

};

//	bound
template <class V>
V
kInlines::bound(
	V	a,
	V	x,
	V	b)
{
	return max(a, min(x, b));
}

//	swap
template <class V>
void
kInlines::swap(
	V& a,
	V& b)
{
	V c = a; 
	a = b;
	b = c;

	//	done
	return;
}

//	sign
template <class V>
int	
kInlines::sign(
	V x)
{
	return x<0.0 ? -1 : (x>0.0 ? 1 : 0);
}

//	sqr
template <class V>
V
kInlines::sqr(
	V x)
{
	return x * x;
}




