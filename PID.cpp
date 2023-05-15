#include "PID.h"

// Konstruktor klasy PID przyjmuje opcjonalne wartości składowych P, I i D. Domyślnie ustawione na 0.
PID::PID(double p, double i, double d) : P(p), I(i), D(d) {}

PID::~PID() = default; // Destruktor klasy PID jest zdefiniowany domyślnie.

/* Metoda iteration reprezentuje jedną iterację regulatora PID.
Przyjmuje jako argument sygnał wejściowy (błąd regulacji) i zwraca sygnał wyjściowy (sygnał sterujący) */
double PID::sim(double err)
{
	sumerr += err; // Akumulacja błędu regulacji
	double differr = err - lasterr; // Obliczenie różnicy błędu regulacji
	lasterr = err; // Przypisanie bieżącego błędu regulacji jako poprzedni błąd

	return P * err + I * sumerr + D * differr; // Obliczenie sygnału sterującego z wykorzystaniem składowych P, I i D
}
