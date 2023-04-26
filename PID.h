#pragma once

#include "SISO.h"

#include "json.hpp"

// Klasa PID dziedziczy po klasie SISO i definiuje regulator PID
class PID : public SISO
{
	// Klasa Simulation zostaje zadeklarowana jako przyjaciel klasy PID, co umożliwia dostęp do prywatnych składowych.
	friend class Simulation;
public:
	// Składowe klasy PID
	double P, I, D;
	double sumerr, lasterr;

	// Konstruktor klasy PID przyjmuje opcjonalne wartości składowych P, I i D. Domyślnie ustawione na 0.
	PID(double = 0, double = 0, double = 0);
	
	~PID();

	double sim(double);
	
	// Szablon generujący serializację i deserializację klasy PID za pomocą biblioteki nlohmann::json.
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(PID, P, I, D)

};

