#include "ARX.h"

#include "settings.h"

#include "helpers.h"


//#include <exception>
#include <stdexcept>




/* konstruktor klasy ARX z argumentami:
 - A - wektor z danymi typu double reprezentujący mianownik
 - B - wektor z danymi typu double reprezentujący licznik
 - dly - wartość typu unsigned reprezentująca opóźnienie
 - ns - wartość typu double reprezentująca amplitudę szumu */
ARX::ARX(std::initializer_list<double> A, std::initializer_list<double> B, unsigned dly, double ns) : k(dly), noiseamp(ns)
{
	setNum(B);// ustawia wartości wektora B
	setDen(A);// ustawia wartości wektora A
}
// destruktor klasy ARX
ARX::~ARX()
{}


/* Funkcja klasy ARX wykonująca jeden krok symulacji, z argumentem:
 - in - wartość typu double reprezentująca wejście */
double ARX::iteration(double in)
{
	inBuf = inBuf.shift(-1); // przesuwa wektor inBuf o jedno miejsce/element w lewo
	if (inBuf.size()) // Jeżeli rozmiar inBuf jest różny od zera
		inBuf[0] = in; // Ustawienie pierwszego elementu wektora inBuf na wartość in

	/* tworzy podwektor inBuf 
	Wektor inBuf przechowuje wartości wejściowe wraz z opóźnieniem
	 Przypisanie wartości elementów wektora inBuf o indeksach k, k+1, ..., k+B.size()-1 do zmiennej temp 
	 - k - opóżnienie */
	DS temp = inBuf[std::slice(k, B.size(), 1)]; 

	double NUMxIN = (B * temp).sum() ; // oblicza iloczyn skalarny wektorów B i temp, i przypisanie wyniku do zmiennej NUMxIN
	double DENxOUT = (A * outBuf).sum(); // oblicza iloczyn skalarny wektorów A i outBuf, i przypisanie wyniku do zmiennej DENxOUT

	double out = NUMxIN - DENxOUT + noiseamp * getNoise(); // oblicza wartość wyjścia algorytmu ARX i przypisanie wyniku do zmiennej out

	// wektor outBuf przechowuje wartości wyjściowe wraz z opóźnieniem
	outBuf = outBuf.shift(-1); // przesuwa wektor outBuf o jedno miejsce/element w lewo
	if (outBuf.size()) // Jeżeli rozmiar outBuf jest różny od zera
		outBuf[0] = out; // Ustawienie pierwszego elementu wektora outBuf na wartość out

	return out; // zwraca wartość wyjścia "out"
}


/* Funkcja klasy ARX ustawiająca wartości wektora B oraz przesuwająca 
wartości wektora inBuf o odpowiednią ilość elementów, z argumentem:
 - n - wektor z danymi typu double reprezentujący licznik */
void ARX::setNum(std::initializer_list<double> n)
{
	B = n; // przypisuje wartości z n do wektora B

	inBuf.resize(B.size() + k); // zmienia rozmiar wektora inBuf
}
// funkcja ustawiająca wartości wektora A
void ARX::setDen(std::initializer_list<double> d)
{
	//if (d.size() == 0)
	//	throw std::invalid_argument("Denominator (A) must be of length at least one!");

	A = d; // Przypisanie wektora d do A
	
	//if (A[0] != 1)
	//	A /= A[0];

	outBuf.resize(A.size()); // Zmiana rozmiaru wektora outBuf na rozmiar wektora A
}

void ARX::setup() //Funkcję można wykorzystać do inicjalizacji różnych wartości, jeśli zajdzie taka potrzeba.
{
	
}

// static S.H.I.T.
//Funkcja mogąca generować szum. W tym przypadku szum nie jest generowany
double ARX::getNoise()
{
	return 0;
}

