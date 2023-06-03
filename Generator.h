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
	/// \param other Obiekt Generator, który ma zostać przeniesiony.
	Generator(Generator&&) = default;

	/// \brief Operator przypisania przenoszącego dla klasy Generator.
	/// \param other Obiekt Generator, który ma zostać przypisany.
	/// \return Referencja do obiektu Generator po przypisaniu.
	Generator& operator=(Generator&&) = default;

	/// \brief Dodaje sygnał do generatora wraz z jego współczynnikiem.
	/// \param a Współczynnik sygnału.
	/// \param s Wskaźnik na obiekt sygnału.
	void add(double, SignalHdl&&);

	/// \brief Pobiera wartość wygenerowanego sygnału dla określonego indeksu.
	/// \param i Indeks sygnału.
	/// \return Wartość wygenerowanego sygnału dla podanego indeksu.
	double get(size_t);

	/// \brief Serializacja obiektu klasy Generator do formatu JSON.
	/// \param j Obiekt JSON, do którego będą zapisywane dane.
	/// \param g Obiekt Generator, który będzie serializowany.
	friend void to_json(json&, const Generator&);

	/// \brief Deserializacja obiektu klasy Generator z formatu JSON.
	/// \param j Obiekt JSON, z którego będą odczytywane dane.
	/// \param g Obiekt Generator, do którego będą wczytywane dane.
	friend void from_json(const json&, Generator&);
};
