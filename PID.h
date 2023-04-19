#pragma once

#include "SISO.h"

class PID : public SISO
{
	friend class Simulation;
public:
	double P, I, D;

	PID(double p, double i, double d) : P(p), I(i), D(d) {}
	~PID() = default;

	

	double iteration(double in)
	{
		return 0;
	}

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(P, I, D)
};

