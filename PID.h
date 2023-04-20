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

	// Konstruktor klasy PID przyjmuje opcjonalne wartości składowych P, I i D. Domyślnie ustawione na 0.
	PID(double p = 0, double i = 0, double d = 0) : P(p), I(i), D(d) {}
	
	~PID() = default; // Destruktor klasy PID jest zdefiniowany domyślnie.

	/* Metoda iteration reprezentuje jedną iterację regulatora PID.
	Przyjmuje jako argument sygnał wejściowy (błąd regulacji) i zwraca sygnał wyjściowy (sygnał sterujący) */
	double iteration(double err)
	{
		return 0;
	}
	
	// Szablon generujący serializację i deserializację klasy PID za pomocą biblioteki nlohmann::json.
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(PID, P, I, D)

};

