/// \file Simulation.h
/// \brief Zawiera implementację klasy Simulation. 
/// 
/// Klasa ta odpowiada za przeprowadzanie symulacji oraz zapisywanie parametrów symulacji do pliku w formacie JSON.

#include "Simulation.h"
#include <string>
#include <iostream> 
#include <fstream>

/// Dołączenie biblioteki json.hpp i nadanie jej aliasu json
#include "json.hpp"
using json = nlohmann::json;

/**
 * @brief Konstruktor klasy Simulation.
 *
 * Konstruktor przyjmuje referencje do obiektów klasy ARX, PID i Generator
 * i inicjalizuje odpowiednie pola klasy.
 *
 * @param a Obiekt klasy ARX.
 * @param p Obiekt klasy PID.
 * @param g Obiekt klasy Generator.
 */
Simulation::Simulation(ARX&& a, PID&& p, Generator&& g, size_t l) : arx(std::move(a)), pid(std::move(p)), gen(std::move(g)), len(l) {}

/**
 * @brief Konstruktor klasy Simulation.
 *
 * Konstruktor wczytuje parametry symulacji z pliku o podanej nazwie.
 * Parametry ARX i PID są odczytywane z pliku w formacie JSON.
 *
 * @param file Nazwa pliku, z którego zostaną wczytane parametry symulacji.
 */
Simulation::Simulation(const std::string& file)
{
	try
	{
		std::ifstream ifs(file); ///< Otwarcie pliku o podanej nazwie za pomocą strumienia wejściowego.
		json j = json::parse(ifs); ///< Parsowanie zawartości pliku jako obiekt JSON.

		arx = j["ARX"];
		pid = j["PID"];
		gen = j["gen"];
		len = j["len"];

	}
	/// \brief Obsługa wyjątków typu std::exception.
	/// \param e Referencja, w której jest przechowywany przechwytywany wyjątek.
	catch (const std::exception& e)
	{
		std::cerr << "Nie udało się wczytać parametrów symulacji z pliku!" << std::endl;
		std::cerr << e.what() << std::endl; ///< Wypisanie komunikatu o wyjątku (informacje szczegółowe o błędzie).
	}
}

/**
 * @brief Konstruktor domyślny klasy Simulation.
 */
Simulation::Simulation() : len(0) {}

/**
 * @brief Metoda wykonująca symulację.
 *
 * Metoda run wykonuje symulację o zadanej liczbie iteracji.
 * Dla każdej iteracji, obliczane są wartości punktu zadanej, błędu regulacji, sygnału sterującego i wyjścia obiektu ARX.
 * Informacje dotyczące iteracji i obliczonych wartości są wypisywane na standardowe wyjście.
 *
 * @param iter Liczba iteracji symulacji.
 * @param arxout Aktualna wartość wyjścia modelu ARX.
 * @param setp Wartość zadana.
 * @param err Błąd regulacji.
 * @param ster Sygnał sterujący.
 */
void Simulation::run()
{
	double arxout = 0;
	double setp = 0, err = 0, ster = 0;

	for (size_t i = 0; i <= len; ++i)
	{
		setp = gen.get(i);

		err = setp - arxout;
		ster = pid.sim(err);
		//ster = setp;

		arxout = arx.sim(ster);

		std::cout << "It: " << i << "\tSetp: " << setp << "\t\tErr: " << err << "\t\tSter: " << ster << "\t\tARX: " << arxout << std::endl;
	}
}

/**
 * @brief Metoda zapisująca parametry symulacji do pliku.
 *
 * Metoda save zapisuje parametry symulacji do pliku JSON o podanej nazwie.
 * Parametry obiektów ARX i PID oraz argumenty szablonu, pola A, B, k, ns_varsą są zapisywane do obiektu JSON,
 * a następnie zapisywane do pliku za pomocą strumienia wyjściowego.
 *
 * @param file Nazwa pliku, do którego zostaną zapisane parametry symulacji.
 * @param j Zmienna używana do tworzenia obiektu JSON, w którym będą przechowywane parametry symulacji przed zapisem do pliku.
 */
void Simulation::save(const std::string& file)
{
	try
	{
		json j; ///< Tworzenie obiektu json
	
		j["ARX"] = arx;
		j["PID"] = pid;
		j["gen"] = gen;
		j["len"] = len;

		std::ofstream out(file); ///< Otwarcie pliku
		out << std::setw(4) << j << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Nie udało się zapisać parametrów symulacji do pliku!" << std::endl;
		std::cerr << e.what() << std::endl;
	}
}
