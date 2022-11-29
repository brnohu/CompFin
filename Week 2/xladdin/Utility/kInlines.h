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



