#pragma once

#include "ARX.h"
#include "PID.h"

#include <string>

class Simulation
{
public:

	ARX arx;
	PID pid;

	Simulation(const ARX&, const PID&);
	Simulation();
	~Simulation() = default;

	void save(std::string);
};



