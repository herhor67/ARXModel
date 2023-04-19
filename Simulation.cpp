#include "Simulation.h"

#include <string>
#include <iostream>
#include <fstream>

#include "json.hpp"
using json = nlohmann::json;

Simulation::Simulation(const ARX& a, const PID& p) : arx(a), pid(p) {}

Simulation::Simulation() {}


void Simulation::save(std::string file)
{
	std::ofstream out;
	out.open(file);

	json j;

	j["ARX"] = arx;
	j["PID"] = pid;

}