#pragma once

#include "settings.h" 

#include <valarray> // Wczytanie biblioteki "valarray", która zawiera definicje i funkcje do obsługi tablic wartości typu T

#include <iostream> // Wczytanie biblioteki "iostream", która zawiera definicje i funkcje do obsługi strumieni wejścia/wyjścia

// Zdefiniowanie szablonu funkcji dla typu T.
template <typename T>
std::ostream& operator<<(std::ostream& o, const std::valarray<T>& va)
{
	o << '(';
	for (const T& v : va) // Iteracja po wszystkich elementach tablicy va
		o << v << ',';
	o << ')'; 
	return o; // Zwrócenie referencji do strumienia wyjścia o 
}
