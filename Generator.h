#pragma once

#include <memory>
#include <vector>

#include <cmath>
#include <numbers>

#include <utility>

#include "json.hpp" ///< Wczytanie biblioteki "json.hpp", która umożliwia serializację i deserializację obiektów JSON.
using json = nlohmann::json;


#include "Signal.h"


/// \class Generator
/// \brief Klasa reprezentująca generator sygnałów.
class Generator
{
	using Signals = std::vector<std::pair<double, SignalHdl>>; ///< Typ reprezentujący kolekcję sygnałów.

	Signals signals; ///< Kolekcja sygnałów.

public:
	/// \brief Konstruktor domyślny klasy Generator.
	Generator() = default;

	/// \brief Wirtualny destruktor klasy Generator.
	~Generator() = default;

	/// \brief Konstruktor przenoszący klasy Generator.
	Generator(Generator&&) = default;
	Generator& operator=(Generator&&) = default;

	/// \brief Dodaje sygnał do generatora wraz z jego współczynnikiem.
	/// \param a Współczynnik sygnału.
	/// \param s Wskaźnik na obiekt sygnału.
	void add(double, SignalHdl&&);

	/// \brief Pobiera wartość wygenerowanego sygnału dla określonego indeksu.
	/// \param i Indeks sygnału.
	/// \return Wartość wygenerowanego sygnału dla podanego indeksu.
	double get(size_t);

	friend void to_json(json&, const Generator&);
	friend void from_json(const json&, Generator&);

};
