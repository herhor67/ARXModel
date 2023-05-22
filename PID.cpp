/// \file PID.h
/// \brief Zawiera implementację klasy PID, która reprezentuje regulator PID.

#include "PID.h"

/**
 * @brief Konstruktor klasy PID.
 *
 * Tworzy obiekt PID z opcjonalnymi wartościami składowych P, I i D.
 * Domyślne wartości ustawione na 0.
 *
 * @param p Wartość składowej proporcjonalnej.
 * @param i Wartość składowej całkującej.
 * @param d Wartość składowej różniczkującej.
 */
PID::PID(double p, double i, double d) : P(p), I(i), D(d), sumerr(0), lasterr(0) {}

/**
 * @brief Destruktor klasy PID.
 *
 * Destruktor klasy PID jest zdefiniowany domyślnie.
 */
PID::~PID() = default; 

/**
 * @brief Metoda wykonująca jedną iterację regulatora PID.
 *
 * Metoda iteration wykonuje jedną iterację regulatora PID.
 * Przyjmuje błąd regulacji jako argument i zwraca sygnał sterujący.
 *
 * @param err Sygnał wejściowy (błąd regulacji).
 * @return Sygnał wyjściowy (sygnał sterujący).
 */
double PID::sim(double err)
{
	/// Akumulacja błędu regulacji
	/// backward euler
	sumerr += err; 
	/// forward euler
	// sumerr += lasterr;
	// trapezoidal
	// sumerr += (err + lasterr) / 2;

	double differr = err - lasterr; ///< Obliczenie różnicy błędu regulacji
	lasterr = err; ///< Przypisanie bieżącego błędu regulacji jako poprzedni błąd

	return P * err + I * sumerr + D * differr; ///< Obliczenie sygnału sterującego z wykorzystaniem składowych P, I i D
}
