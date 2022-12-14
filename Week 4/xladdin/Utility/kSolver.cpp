#include "kSolver.h"

//	newton rapson
bool
kSolver::newtonRapson(
	kSolverObjective& obj,
	double& x,
	int& numIter,
	double& epsilon,
	string* error)
{
	//	calc value
	int		i;
	double	y, dydx;

	//	loop
	for(i=0;i<numIter;++i)
	{
		y = obj.value(x);
		dydx = obj.deriv(x);
		x -= y / dydx;

		if(fabs(y)<=epsilon) break;
	}

	//	set output
	numIter = min(i + 1, numIter);
	epsilon = obj.value(x);

	//	done
	return true;
}

