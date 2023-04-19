#pragma once

#include "SISO.h"

#include "json.hpp"

class PID : public SISO
{
	friend class Simulation;
public:
	double P, I, D;

	PID(double p = 0, double i = 0, double d = 0) : P(p), I(i), D(d) {}
	~PID() = default;

	

	double iteration(double err)
	{
		return 0;
	}

	// szablon generujêcy serializacjê i deserializacjê za pomoc¹ JSON
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(PID, P, I, D)

};

