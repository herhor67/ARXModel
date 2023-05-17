#pragma once

#include "SISO.h"

#include "json.hpp" // Wczytanie biblioteki "json.hpp", która umożliwia serializację i deserializację obiektów JSON.
using json = nlohmann::json;

// Klasa PID dziedziczy po klasie SISO i definiuje regulator PID
class PID : public SISO
{
	// Klasa Simulation zostaje zadeklarowana jako przyjaciel klasy PID, co umożliwia dostęp do prywatnych składowych.
	friend class Simulation;
public:
	// Składowe klasy PID
	double P, I, D; // Współczynniki P, I, D dla regulatora PID.
	double sumerr, lasterr; // Suma błędów i ostatni błąd.

	// Konstruktor klasy PID przyjmuje opcjonalne wartości składowych P, I i D. Domyślnie ustawione na 0.
	PID(double = 0, double = 0, double = 0);
	
	// Destruktor klasy PID
	~PID();
	
	// Funkcja sim() symuluje działanie regulatora PID dla określonej wartości
	double sim(double);
	
	// Szablon generujący serializację i deserializację klasy PID za pomocą biblioteki nlohmann::json.
	friend void to_json(json& j, const PID& o)
	{
		j["P"] = o.P;
		j["I"] = o.I;
		j["D"] = o.D;
	}

	friend void from_json(const json& j, PID& o)
	{
		j.at("P").get_to(o.P);
		j.at("I").get_to(o.I);
		j.at("D").get_to(o.D);
	}

};

