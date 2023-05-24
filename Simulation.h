#pragma once

#include "ARX.h"
#include "PID.h"
#include "Generator.h"

#include <string>

/// \class Simulation
/// \brief Klasa reprezentująca symulację systemu regulacji.
///
/// Klasa Simulation składa się z dwóch obiektów: ARX i PID.
class Simulation
{
public:

	ARX arx; ///< Obiekt klasy ARX reprezentujący model matematyczny systemu regulacji.
	PID pid; ///< Obiekt klasy PID reprezentujący regulator PID systemu regulacji.
	Generator gen; ///< Obiekt klasy Generator odpowiedzialny za generowanie danych wejściowych.
	size_t len;

	/// \brief Konstruktor klasy Simulation.
	/// \param arx Obiekt klasy ARX, model matematyczny systemu.
    /// \param pid Obiekt klasy PID, regulator PID systemu.
    /// \param gen Obiekt klasy Generator, generator danych wejściowych. 
	Simulation(ARX&&, PID&&, Generator&&, size_t = 0);
	
	/// \brief Konstruktor klasy Simulation.
	Simulation(const std::string&);
	
	/// \brief Konstruktor domyślny klasy Simulation.
	Simulation();
	
	/// \brief Destruktor klasy Simulation.
	~Simulation() = default;

	/// \brief Metoda run wykonuje symulację przez określoną liczbę iteracji dla podanego wejścia.
	void run(const std::string& = "");

	/// \brief Metoda save zapisuje symulację do pliku w formacie JSON. 
	///Wykorzystywana jest biblioteka json.hpp, która umożliwia zapisywanie danych w formacie JSON.
	void save(const std::string&);
};
