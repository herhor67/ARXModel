/// \file ARX.h
/// \brief Zawiera definicję klasy ARX.

#include "ARX.h"

#include "settings.h"
//#include "helpers.h"

//#include <exception>
#include <stdexcept>
#include <random>

/// \brief Konstruktor klasy ARX.
/// \param A Wektor z danymi typu double reprezentujący mianownik.
/// \param B Wektor z danymi typu double reprezentujący licznik.
/// \param dly Wartość typu unsigned reprezentująca opóźnienie.
/// \param ns Wartość typu double reprezentująca amplitudę szumu.
ARX::ARX(std::initializer_list<double> A, std::initializer_list<double> B, unsigned dly, double ns) : k(dly), ns_var(ns)
{
	setDen(A); ///< ustawia wartości wektora A
	setNum(B); ///< ustawia wartości wektora B
}

/// \brief Destruktor klasy ARX.
ARX::~ARX()
{}

/// \brief Wykonuje jeden krok symulacji.
/// \param in Wartość typu double reprezentująca wejście.
/// \return Wartość typu double reprezentująca wyjście.
double ARX::sim(double in)
{
	/// przesuwa elementy inBuf o jedno miejsce i dopisuje najnowsze wyjście
	inBuf = inBuf.shift(-1);
	if (inBuf.size())
		inBuf[0] = in; ///< Dopisanie najnowszego wejścia na pierwszą pozycję wektora inBuf.

	/**
	* Tworzenie podwektora inBuf
	* Wektor inBuf przechowuje wartości wejściowe wraz z opóźnieniem "k"
	* Przypisanie wartości elementów wektora inBuf o indeksach k, k+1, ..., k+B.size()-1 do zmiennej temp
	*/
	DS temp = inBuf[std::slice(k, B.size(), 1)]; 

	double NUMxIN = (B * temp).sum() ; ///< oblicza iloczyn skalarny wektorów B i temp
	double DENxOUT = (A * outBuf).sum(); ///< oblicza iloczyn skalarny wektorów A i outBuf

	double out = NUMxIN - DENxOUT + ns_var * getNoise(); ///< oblicza wartość wyjścia algorytmu ARX

	/// przesuwa elementy outBuf o jedno miejsce i dopisuje najnowsze wyjście
	outBuf = outBuf.shift(-1); 
	if (outBuf.size())
		outBuf[0] = out; ///< Dopisanie najnowszego wyjścia na pierwszą pozycję wektora outBuf.

	return out; ///< zwraca wartość wyjścia
}

/// \brief Funkcja ustawiająca wartości licznika (B) modelu ARX.
///
/// Ustawia wartości wektora B i przesuwa wartości wektora inBuf o odpowiednią ilość elementów.
/// \param n Lista inicjalizacyjna z wartościami typu double reprezentującymi licznik (B).
void ARX::setNum(std::initializer_list<double> n)
{
	B = n; ///< Przypisanie wartości z listy n do tablicy B.
	inBuf.resize(B.size() + k); ///< zmienia rozmiar wektora inBuf
}

/// \brief Funkcja ustawiająca wartości mianownika (A) modelu ARX.
///
/// Ustawia wartości wektora A i zmienia rozmiar wektora outBuf.
/// \param d Lista inicjalizacyjna z wartościami typu double reprezentującymi mianownik (A).
void ARX::setDen(std::initializer_list<double> d)
{
	A = d; ///< Przypisanie wartości z listy d do wektora A.
	outBuf.resize(A.size()); ///< Zmiana rozmiaru wektora outBuf na rozmiar wektora A
}


/// \brief Funkcja statyczna generująca szum.
///
/// Generuje i zwraca wartość szumu.
/// \return Wygenerowana wartość szumu.
double ARX::getNoise()
{
	//std::default_random_engine generator;

	/// Statyczny generator liczb pseudolosowych - Mersenne Twister z "seed" pobranym z ustawień.
	static std::mt19937 generator; 

	//std::mt19937 generator(std::random_device{}());

	/// Statyczny rozkład normalny (Gaussa). Rozkład normalny o średniej 0 i odchyleniu standardowym 1.
	static std::normal_distribution<double> dist; 

	return dist(generator); ///< Zwraca losową wartość z rozkładu normalnego, będącą wygenerowaną wartością szumu.
}


/// \brief Serializacja obiektu klasy ARX do formatu JSON.
/// \param Obiekt JSON, do którego będą zapisywane dane.
/// \param Obiekt ARX, który będzie serializowany.
void to_json(json& j, const ARX& o)
{
	j["A"] = o.A;
	j["B"] = o.B;
	j["k"] = o.k;
	j["ns_var"] = o.ns_var;
}

/// \brief Deserializacja obiektu klasy ARX z formatu JSON.
/// \param j Obiekt JSON, z którego będą odczytywane dane.
/// \param o Obiekt ARX, do którego będą wczytywane dane.
void from_json(const json& j, ARX& o)
{
	j.at("A").get_to(o.A);
	j.at("B").get_to(o.B);
	j.at("k").get_to(o.k);
	j.at("ns_var").get_to(o.ns_var);

	o.outBuf.resize(o.A.size());
	o.inBuf.resize(o.B.size());
}