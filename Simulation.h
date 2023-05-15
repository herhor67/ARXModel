#pragma once

#include "ARX.h"
#include "PID.h"

#include <string>

/* Deklaracja klasy Simulation, która reprezentuje symulację w programie.
 Klasa składa się z dwóch obiektów klasy ARX i PID, które są częściami symulowanego systemu */
class Simulation
{
public:

	ARX arx; // obiekt klasy ARX reprezentujący model matematyczny systemu regulacji
	PID pid; // obiekt klasy PID reprezentujący regulator PID systemu regulacji

	// Konstruktor klasy Simulation, który inicjuje obiekty ARX i PID 
	Simulation(const ARX&, const PID&);
	
	// Konstruktor klasy Simulation, który inicjuje obiekt ARX na podstawie ścieżki do pliku konfiguracyjnego.
	Simulation(const std::string&);
	
	// Konstruktor domyślny klasy Simulation, który nie przyjmuje żadnych argumentów.
	Simulation();
	
	// Destruktor klasy Simulation, który jest domyślny.
	~Simulation() = default;

	// Metoda run wykonuje symulację przez określoną liczbę iteracji dla podanego wejścia.
	void run(size_t, double = 0);

	/* Metoda save zapisuje symulację do pliku. 
	Wykorzystywana jest biblioteka json.hpp, która umożliwia zapisywanie danych w formacie JSON. */
	void save(const std::string&);
};
