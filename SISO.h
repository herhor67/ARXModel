#pragma once

class SISO
{
	friend class Simulation;

public:
	// Konstruktor domyślny klasy SISO. Oznaczenie "= default" wskazuje na to,
	// że konstruktor ten ma zostać wygenerowany automatycznie przez kompilator
	SISO() = default;
	// Destruktor wirtualny klasy SISO
	virtual ~SISO() = default;
	
	// Wirtualna metoda iteration reprezentuje jedną iterację systemu SISO.
	// Metoda ta przyjmuje jako argument sygnał wejściowy i zwraca sygnał wyjściowy,
	// jest czysto wirtualna, co oznacza, że nie jest ona zdefiniowana 
	// w tej klasie i musi zostać zaimplementowana w klasie dziedziczącej.
	virtual double iteration(double) = 0;

};
