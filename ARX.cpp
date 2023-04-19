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
	// przesuwa elementy inBuf o jedno miejsce i dopisuje najnowsze wyjście
	inBuf = inBuf.shift(-1);
	if (inBuf.size())
		inBuf[0] = in;

	/* tworzy podwektor inBuf 
	Wektor inBuf przechowuje wartości wejściowe wraz z opóźnieniem "k"
	 Przypisanie wartości elementów wektora inBuf o indeksach k, k+1, ..., k+B.size()-1 do zmiennej temp 
	*/
	DS temp = inBuf[std::slice(k, B.size(), 1)]; 

	double NUMxIN = (B * temp).sum() ; // oblicza iloczyn skalarny wektorów B i temp
	double DENxOUT = (A * outBuf).sum(); // oblicza iloczyn skalarny wektorów A i outBuf

	double out = NUMxIN - DENxOUT + noiseamp * getNoise(); // oblicza wartość wyjścia algorytmu ARX

	// przesuwa elementy outBuf o jedno miejsce i dopisuje najnowsze wyjście
	outBuf = outBuf.shift(-1); 
	if (outBuf.size())
		outBuf[0] = out;

	return out; // zwraca wartość wyjścia
}


/* Funkcja klasy ARX ustawiająca wartości tablicy B oraz zmieniająca rozmiar
wektora inBuf na odpowiednią ilość elementów, z argumentem:
 - n - lista z danymi typu double reprezentujący licznik */
void ARX::setNum(std::initializer_list<double> n)
{
	B = n; // przypisuje wartości z n do wektora B
	inBuf.resize(B.size() + k); // zmienia rozmiar wektora inBuf
}
// funkcja ustawiająca wartości wektora A
void ARX::setDen(std::initializer_list<double> d)
{
	A = d; // Przypisanie wektora d do A
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

