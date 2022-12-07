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



