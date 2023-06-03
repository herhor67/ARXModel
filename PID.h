#pragma once

#include "SISO.h"

#include "json.hpp" ///< Wczytanie biblioteki "json.hpp", która umożliwia serializację i deserializację obiektów JSON.
using json = nlohmann::json;

/// \class PID
/// \brief Klasa PID reprezentuje regulator PID.
///
/// Dziedziczy po klasie SISO (Single Input Single Output), która implementuje interfejs dla regulatorów jednokanałowych.
/// Definiuje regulator PID.
class PID : public SISO
{
	/// Klasa Simulation zostaje zadeklarowana jako przyjaciel klasy PID. 
	/// Umożliwia dostęp do prywatnych składowych.
	friend class Simulation;
public:
	/// Składowe klasy PID
	/// \param P Współczynnik proporcjonalny P
	/// \param I Współczynnik całkujący I
	/// \param D Współczynnik różniczkujący D
	/// \param sumerr Suma błędów
	/// \param lasterr Ostatni błąd
	double P, I, D; ///< Współczynniki P, I, D dla regulatora PID.
	double sumerr, lasterr; ///< Suma błędów i ostatni błąd.

	/// \brief Konstruktor klasy PID.
	///
	/// Tworzy nowy obiekt PID z opcjonalnymi współczynnikami P, I i D.
	/// \param p Współczynnik proporcjonalny (P). Domyślnie 0.
	/// \param i Współczynnik całkujący (I). Domyślnie 0.
	/// \param d Współczynnik różniczkujący (D). Domyślnie 0.
	PID(double = 0, double = 0, double = 0);
	
	/// \brief Destruktor klasy PID.
	~PID();
	
	/// \brief Funkcja symulująca działanie regulatora PID.
	///
	/// Symuluje działanie regulatora PID dla określonej wartości.
	/// \return Wartość wyjściowa po zastosowaniu regulatora PID.
	double sim(double);
	
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(PID, P, I, D); ///< Makro z biblioteki JSON dla serializacji/deserializacji obiektu klasy PID do formatu JSON.

	///// \brief Serializacja obiektu klasy PID do formatu JSON.
	///// \param j Obiekt JSON, do którego będą zapisywane dane.
	///// \param o Obiekt PID, który będzie serializowany.
	//friend void to_json(json& j, const PID& o)
	//{
	//	j["P"] = o.P; ///< Zapisuje współczynnik proporcjonalny P do obiektu JSON.
	//	j["I"] = o.I;
	//	j["D"] = o.D;
	//}

	///// \brief Deserializacja obiektu klasy PID z formatu JSON.
	///// \param j Obiekt JSON, z którego będą odczytywane dane.
	///// \param o Obiekt PID, do którego będą wczytywane dane.
	//friend void from_json(const json& j, PID& o)
	//{
	//	j.at("P").get_to(o.P); ///< Odczytuje wartość współczynnika proporcjonalnego P z obiektu JSON i przypisuje go do obiektu PID.
	//	j.at("I").get_to(o.I);
	//	j.at("D").get_to(o.D);
	//}

};
