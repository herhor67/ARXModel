#include "Simulation.h"

#include <string>
#include <iostream>
#include <fstream>

// Dołączenie biblioteki json.hpp i nadanie jej aliasu json
#include "json.hpp"
using json = nlohmann::json;

// Definicja konstruktora z parametrami klasy Simulation, przyjmującego obiekty klasy ARX i PID
Simulation::Simulation(const ARX& a, const PID& p) : arx(a), pid(p) {}

// Konstruktor domyślny klasy Simulation
Simulation::Simulation() {}

// Metoda save klasy Simulation, zapisująca parametry symulacji do pliku JSON o nadanej nazwie
void Simulation::save(std::string file)
{
	json j; // Tworzenie obiektu json
	
	// Zapisanie parametrów obiektów ARX i PID do obiektu json
	j["ARX"] = arx;
	j["PID"] = pid;

	// Następuje otwarcie pliku i zapisuje się obiektu json
	std::ofstream out(file);
	
	// Zapisanie danych w formacie JSON do pliku za pomocą strumienia wyjściowego
	out << j;

}
