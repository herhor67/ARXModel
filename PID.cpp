#include "PID.h"


// Konstruktor klasy PID przyjmuje opcjonalne wartości składowych P, I i D. Domyślnie ustawione na 0.
PID::PID(double p = 0, double i = 0, double d = 0) : P(p), I(i), D(d) {}

PID::~PID() = default; // Destruktor klasy PID jest zdefiniowany domyślnie.

/* Metoda iteration reprezentuje jedną iterację regulatora PID.
Przyjmuje jako argument sygnał wejściowy (błąd regulacji) i zwraca sygnał wyjściowy (sygnał sterujący) */
double PID::sim(double err)
{
	sumerr += err;
	double differr = err - lasterr;
	lasterr = err;

	return P * err + I * sumerr + D * differr;
}