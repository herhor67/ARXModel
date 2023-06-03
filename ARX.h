#pragma once

#include "SISO.h"

#include <valarray>
#include <span>
#include <initializer_list>

#include "json.hpp" ///< Biblioteka obsługująca format JSON
using json = nlohmann::json;

/// \class ARX
/// \brief Klasa ARX reprezentuje model ARX.
///
/// Klasa ARX dziedziczy po klasie SISO (Single Input Single Output), która implementuje interfejs dla regulatorów jednokanałowych.
/// Implementuje model ARX z mianownikiem (A) i licznikiem (B).
class ARX : public SISO {
	friend class Simulation; ///< Deklaracja przyjaźni z klasą Simulation

	/// Definicja aliasu typu danych DS, odnosi się do typu std::valarray<double>.
	/// Zamiast używać pełnej nazwy, można użyć skróconej nazwy DS.
	using DS = std::valarray<double>;

	DS A; ///< Wektor przechowujący mianownik modelu ARX.
	DS B; ///< Wektor przechowujący licznik modelu ARX.
	unsigned k = 0;  ///< Opóźnienie modelu ARX. Domyślnie 0.
	double ns_var = 1; ///< Amplituda szumu modelu ARX. Domyślnie 1.

	DS inBuf; ///< Wektor przechowujący wartości wejściowe wraz z opóźnieniem.
	DS outBuf; ///< Wektor przechowujący wartości wyjściowe wraz z opóźnieniem.

public:
	/// \brief Konstruktor klasy ARX.
	///
	/// Tworzy nowy obiekt ARX z danymi mianownika (A), licznika (B), opóźnieniem (k) i amplitudą szumu (ns_var).
	/// \param A Wektor z danymi typu double reprezentujący mianownik. Domyślnie pusty.
	/// \param B Wektor z danymi typu double reprezentujący licznik. Domyślnie pusty.
	/// \param k Wartość typu unsigned reprezentująca opóźnienie. Domyślnie 0.
	/// \param ns_var Wartość typu double reprezentująca amplitudę szumu. Domyślnie 1.
	ARX(std::initializer_list<double> A = {}, std::initializer_list<double> B = {}, unsigned k = 0, double ns_var = 1);

	/// \brief Destruktor klasy ARX.
	~ARX();

	/// \brief Funkcja symulująca jeden krok modelu ARX.
	///
	/// Symuluje jeden krok modelu ARX dla określonej wartości wejściowej.
	/// \param in Wartość typu double reprezentująca wejście.
	/// \return Wartość wyjściowa po zastosowaniu modelu ARX.
	double sim(double in);

	/// \brief Funkcja ustawiająca wartości licznika (B) modelu ARX.
	///
	/// Ustawia wartości wektora B i przesuwa wartości wektora inBuf o odpowiednią ilość elementów.
	/// \param n Lista inicjalizacyjna z wartościami typu double reprezentującymi licznik (B).
	void setNum(std::initializer_list<double> n);

	/// \brief Funkcja ustawiająca wartości mianownika (A) modelu ARX.
	///
	/// Ustawia wartości wektora A i zmienia rozmiar wektora outBuf.
	/// \param d Lista inicjalizacyjna z wartościami typu double reprezentującymi mianownik (A).
	void setDen(std::initializer_list<double> d);

	/// \brief Funkcja statyczna generująca szum.
	///
	/// Generuje i zwraca wartość szumu.
	/// \return Wygenerowana wartość szumu.
	static double getNoise();

	/// \brief Serializacja obiektu klasy ARX do formatu JSON.
	/// \param j Obiekt JSON, do którego będą zapisywane dane.
	/// \param o Obiekt ARX, który będzie serializowany.
	friend void to_json(json& j, const ARX& o);

	/// \brief Deserializacja obiektu klasy ARX z formatu JSON.
	/// \param j Obiekt JSON, z którego będą odczytywane dane.
	/// \param o Obiekt ARX, do którego będą wczytywane dane.
	friend void from_json(const json& j, ARX& o);
};
