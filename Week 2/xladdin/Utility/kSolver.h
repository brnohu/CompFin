#pragma once

#pragma once

//	desc:	solver functionality
//	auth:	jesper andreasen
//	date:	nov 2022	

//	includes
#include "kVector.h"
#include "kMatrix.h"
#include <string>

//	using string
using std::string;

//	solver objective
class kSolverObjective
{
public:

	//	value y = value(x)
	virtual double	value(double x) { return 0.0; }

	//	deriv dy/dx
	virtual double	deriv(double x) { return 0.0; }

};

//	solver
class kSolver
{
public:

	//	newton rapson
	static	bool	newtonRapson(
		kSolverObjective&	obj,
		double&				x,
		int&				numIter,
		double&				epsilon,
		string*				error);


};


