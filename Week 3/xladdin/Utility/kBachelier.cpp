#include "kBachelier.h"
#include "kSolver.h"

class kBachelierObj : public kSolverObjective
{
public:

	//	constructor
	kBachelierObj(
		double	expiry,
		double	strike,
		double	price,
		double	forward)
		: kSolverObjective(),
		myExpiry(expiry),
		myStrike(strike),
		myPrice(price),
		myForward(forward)
	{}

	//	value
	virtual double	value(double x)
	{
		double res = kBachelier::call(myExpiry, myStrike, myForward, x) - myPrice;

		//	done
		return res;
	}

	//	deriv
	virtual double	deriv(double x)
	{
		double res = kBachelier::vega(myExpiry, myStrike, myForward, x);

		//	done
		return res;
	}

	//	private parts
private:

	//	expiry
	double	myExpiry;
	double	myStrike;
	double	myPrice;
	double	myForward;

};

//	implied
double
kBachelier::implied(
	double	expiry,
	double	strike,
	double	price,
	double	forward)
{
	//	calc intrinsic
	double intrinc = max(0.0, forward - strike);
	if (price <= intrinc) return 0.0;

	//	objective
	kBachelierObj obj(expiry, strike, price, forward);

	//	start guess
	double volatility = 0.1;
	int    numIter = 10;
	double epsilon = (price - intrinc) * kConstants::epsilon();

	//	solve
	kSolver::newtonRapson(obj, volatility, numIter, epsilon, nullptr);

	//	bound
	volatility = max(0.0, volatility);

	//	done
	return volatility;
}


