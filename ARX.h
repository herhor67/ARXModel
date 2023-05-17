#pragma once

#include "SISO.h" // Dołączenie pliku nagłówkowego "SISO.h"

#include <valarray>
#include <span>
#include <initializer_list>

#include "json.hpp" // Biblioteka obsługująca format JSON
using json = nlohmann::json;

// deklaracja klasy ARX dziedziczącej publicznie po klasie SISO
class ARX : public SISO 
{
	friend class Simulation; // Deklaracja przyjaźni z klasą Simulation

	/* definiuje alias typu danych DS, odnosi się do typu std::valarray<double>, 
	zamiast używać pełnej nazwy, można użyć skróconej nazwy DS */
	using DS = std::valarray<double>; 

	DS A; // den // obiekt klasy valarray przechowujący mianownik
	DS B; // num  // obiekt klasy valarray przechowujący licznik
	unsigned k = 0;  // wartość typu unsigned reprezentująca opóźnienie, domyślnie 0
	double ns_var = 1; // wartość typu double reprezentująca amplitudę szumu, domyślnie o wartości 1

	DS inBuf; // wektor przechowujący wartości wejściowe wraz z opóźnieniem
	DS outBuf; // wektor przechowujący wartości wyjściowe wraz z opóźnieniem

	

public:
	/* Konstruktor klasy ARX z argumentami:
	- A - wektor z danymi typu double reprezentujący mianownik
	- B - wektor z danymi typu double reprezentujący licznik
	- dly - wartość typu unsigned reprezentująca opóźnienie, domyślnie 0
	- ns - wartość typu double reprezentująca amplitudę szumu, domyślnie 1 */
	ARX(std::initializer_list<double> = {}, std::initializer_list<double> = {}, unsigned = 0, double = 0);
	~ARX(); // deklaracja destruktora klasy ARX

	double sim(double); // deklaracja funkcji wykonującej jeden krok symulacji

	/* deklaracja funkcji ustawiającej wartości wektora B oraz przesuwającej 
	wartości wektora inBuf o odpowiednią ilość elementów */
	void setNum(std::initializer_list<double>); 

	void setDen(std::initializer_list<double>); // deklaracja funkcji ustawiającej wartości wektora A

	void setup(); // deklaracja funkcji pomocniczej, której można użyć do inicjalizacji różnych wartości

	static double getNoise(); // deklaracja funkcji statycznej, która może generować szum

	/* serializacja i deserializacja obiektu klasy ARX za pomocą biblioteki JSON nlohmann
	Szablon zapisuje obiektu klasy ARX do formatu JSON i odczytuje obiektu klasy ARX z formatu JSON 
	Argumenty szablonu określają, które pola klasy ARX mają być zserializowane/deserializowane i w jakiej kolejności*/
	friend void to_json(json& j, const ARX& o)
	{
		j["A"] = o.A;
		j["B"] = o.B;
		j["k"] = o.k;
		j["ns_var"] = o.ns_var;
	}

	friend void from_json(const json& j, ARX& o)
	{
		j.at("A").get_to(o.A);
		j.at("B").get_to(o.B);
		j.at("k").get_to(o.k);
		j.at("ns_var").get_to(o.ns_var);

		o.outBuf.resize(o.A.size());
		o.inBuf.resize(o.B.size());
	}

};

