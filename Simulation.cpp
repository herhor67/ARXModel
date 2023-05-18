#include "Simulation.h"

#include <string>
#include <iostream>
#include <fstream>

// Dołączenie biblioteki json.hpp i nadanie jej aliasu json
#include "json.hpp"
using json = nlohmann::json;

// Definicja konstruktora z parametrami klasy Simulation, przyjmującego obiekty klasy ARX i PID
Simulation::Simulation(ARX&& a, PID&& p, Generator&& g) : arx(std::move(a)), pid(std::move(p)), gen(std::move(g)) {}

// Definicja konstruktora wczytującego dane z pliku
Simulation::Simulation(const std::string& file)
{
	try
	{
		std::ifstream ifs(file);
		json j = json::parse(ifs);

		arx = j["ARX"];
		pid = j["PID"];
		//gen = j["gen"];

	}
	catch (const std::exception& e)
	{
		std::cerr << "Nie udało się wczytać parametrów symulacji z pliku!" << std::endl;
		std::cerr << e.what() << std::endl;
	}

}

// Konstruktor domyślny klasy Simulation
Simulation::Simulation() = default;

// Metoda run klasy Simulation, wykonująca symulację o zadanej liczbie iteracji i wartości punktu zadanej
void Simulation::run(size_t iter)
{
	double arxout = 0;
	double setp, err, ster;

	for (size_t i = 0; i < iter; ++i)
	{
		setp = gen.get(i);
		err = setp - arxout;

		ster = pid.sim(err);

		arxout = arx.sim(ster);

		std::cout << "Setp: " << setp << "\t\tErr: " << err << "\t\tSter: " << ster << "\t\tARX: " << arxout << std::endl;
	}

}


// Metoda save klasy Simulation, zapisująca parametry symulacji do pliku JSON o nadanej nazwie
// Argumenty szablonu, pola A, B, k, ns_var oraz nastawy regulatora PID klasy ARX i PID będące zapisane/deserializowane do/z formatu JSON.
void Simulation::save(const std::string& file)
{
	try
	{
		json j; // Tworzenie obiektu json
	
		// Zapisanie parametrów obiektów ARX i PID do obiektu json
		j["ARX"] = arx;
		j["PID"] = pid;

		// Otwarcie pliku
		std::ofstream out(file);
		// Zapisanie danych w formacie JSON do pliku za pomocą strumienia wyjściowego
		out << j;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Nie udało się zapisać parametrów symulacji do pliku!" << std::endl;
		std::cerr << e.what() << std::endl;
	}
}
