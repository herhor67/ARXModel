#pragma once

#include <memory>
#include <vector>

#include <cmath>
#include <numbers>
#include <utility>

#include "json.hpp" ///< Wczytanie biblioteki "json.hpp", która umożliwia serializację i deserializację obiektów JSON.
using json = nlohmann::json;


using SignalEnumT = int8_t;
/// \enum SignalType
/// \brief Typ wyliczeniowy reprezentujący typy sygnałów.
enum SignalType : SignalEnumT
{
	Virtual = 0,
	Const, ///< Sygnał stały.
	Impulse, ///< Sygnał impulsowy.
	Sine, ///< Sygnał sinusoidalny.
	Square, ///< Sygnał kwadratowy.
	Triangle, ///< Sygnał trójkątny.

	Delay = std::numeric_limits<SignalEnumT>::min(),
};

//NLOHMANN_JSON_SERIALIZE_ENUM(SignalType, ///< Wykonuje serializację i deserializację typu SignalType przy pomocy biblioteki JSON. 
//	{
//		{ SignalType::Virtual, "Virtual" }, ///< Typ sygnału wirtualnego
//		{ SignalType::Const, "Const" }, ///< Typ sygnału stałego
//		{ SignalType::Impulse, "Impulse" }, ///< Typ sygnału impulsowego
//		{ SignalType::Sine, "Sine" }, ///< Typ sygnału sinusoidalnego
//		{ SignalType::Square, "Square" }, ///< Typ sygnału kwadratowego
//		{ SignalType::Triangle, "Triangle" }, ///< Typ sygnału trójkątnego
//		{ SignalType::Delay, "Delay" }, ///< Typ sygnału opóźnionego
//	}
//)

/// Klasa reprezentuje sygnał.
class Signal;

/// \typedef SignalHdl
/// \brief Typ wskaźnika na obiekt klasy Signal.
class SignalHdl
{
	using SignalPtr = std::unique_ptr<Signal>; 
	SignalPtr ptr; 
public:
	SignalHdl() = default; ///< Konstruktor domyślny klasy SignalHdl.
	SignalHdl(const SignalHdl&) = delete; ///< Konstruktor kopiujący klasy SignalHdl.
	SignalHdl& operator=(const SignalHdl&) = delete; ///< Przypisanie kopii klasy SignalHdl.
	SignalHdl(SignalHdl&&) = default; ///< Konstruktor przenoszący klasy SignalHdl.
	SignalHdl& operator=(SignalHdl&&) = default; ///< Przypisanie przeniesienia klasy SignalHdl.

	SignalHdl(SignalPtr&&); ///< Konstruktor przenoszący klasy SignalHdl.

	Signal& operator*() const; 
	Signal* operator->() const; 

	friend void to_json(json&, const SignalHdl&); ///< Funkcja serializująca obiekt SignalHdl do formatu JSON.
	friend void from_json(const json&, SignalHdl&); ///< Funkcja deserializująca obiekt SignalHdl z formatu JSON.

	/**
	* @brief Tworzy obiekt SignalHdl z dynamicznie utworzonym obiektem klasy sygnału.
	*
	* Ta funkcja statyczna jest używana do tworzenia obiektu SignalHdl, który jest inteligentnym
	* wskaźnikiem na obiekt klasy sygnału (Signal). Umożliwia tworzenie obiektów sygnałów różnych typów
	* na podstawie podanych argumentów konstruktora.
	*
	* @tparam T Typ klasy sygnału, która ma być utworzona.
	* @tparam Args Typy argumentów konstruktora klasy sygnału.
	* @param args Argumenty konstruktora klasy sygnału.
	* @return SignalHdl Inteligentny wskaźnik na utworzony obiekt sygnału.
	*/
	template<typename T, typename... Args>
	static SignalHdl make(Args&&... args)
	{
		//SignalPtr sp = std::unique_ptr<T>(new T(std::forward<Args>(args)...));
		SignalPtr sp = std::make_unique<T>(std::forward<Args>(args)...);
		return SignalHdl(std::move(sp));
	}
};

/// \class Signal
/// \brief Abstrakcyjna klasa reprezentująca sygnał.
///
/// Klasa Signal definiuje interfejs dla różnych rodzajów sygnałów.
class Signal
{
	friend SignalHdl; 
public:

	/// \brief Konstruktor domyślny klasy Signal.
	Signal() = default;

	/// \brief Wirtualny destruktor klasy Signal.
	virtual ~Signal() = default;

	/**
	* @brief Metoda pobierająca wartość sygnału dla określonego indeksu.
	* @param index Indeks sygnału.
	* @return Wartość sygnału dla podanego indeksu.
	*/
	virtual double get(size_t) const = 0;

	/**
	* @brief Metoda zwracająca typ sygnału. 
	* @return Typ sygnału.
	*/
	virtual SignalType type() const = 0;
};

/// \class SignalConst
/// \brief Klasa reprezentująca stały sygnał o wartości równiej 1.
class SignalConst : public Signal
{
public:
	/// \brief Konstruktor domyślny klasy SignalConst.
	SignalConst() = default;

	/// \brief Wirtualny destruktor klasy SignalConst.
	~SignalConst() = default;

	/**
	* @brief Metoda pobierająca wartość sygnału dla określonego indeksu.
	*
	* Ta metoda zwraca zawsze wartość 1. Klasa reprezentuje stały sygnał o wartości 1 dla wszystkich indeksów.
	*
	* @param index Indeks sygnału.
	* @return Wartość stałego sygnału, która zawsze wynosi 1.
	*/
	double get(size_t) const override
	{
		return 1;
	}

	/**
	 * @brief Metoda zwracająca typ sygnału.
	 * @return Typ sygnału, który jest `SignalType::Const`.
	 */
	SignalType type() const override
	{
		return SignalType::Const;
	}

	friend void to_json(json& j, const SignalConst& o) {} ///< Funkcja serializująca obiekt SignalConst do formatu JSON.
	friend void from_json(const json& j, SignalConst& o) {} ///< Funkcja deserializująca obiekt SignalConst z formatu JSON.
};

/// \class SignalImpulse
/// \brief Klasa reprezentująca sygnał impulsowy.
class SignalImpulse : public Signal
{
public:
	/// \brief Konstruktor domyślny klasy SignalImpulse.
	SignalImpulse() {}

	/// \brief Wirtualny destruktor klasy SignalImpulse.
	~SignalImpulse() = default;

	/**
	* @brief Metoda pobierająca wartość sygnału dla określonego indeksu.
	*
	* Ta metoda zwraca wartość sygnału impulsowego w zależności od podanego indeksu.
	* Jeśli indeks jest równy 0, metoda zwraca wartość true (1), w przeciwnym razie zwraca wartość false (0).
	*
	* @param i Indeks sygnału.
	* @return Wartość sygnału impulsowego dla podanego indeksu.
	*/
	double get(size_t i) const override
	{
		return i == 0;
	}

	/**
	 * @brief Metoda zwracająca typ sygnału.
	 * @return Typ sygnału, który jest `SignalType::Impulse`.
	 */
	SignalType type() const override
	{
		return SignalType::Impulse;
	}

	friend void to_json(json& j, const SignalImpulse& o) {} ///< Funkcja serializująca obiekt SignalImpulse do formatu JSON.
	friend void from_json(const json& j, SignalImpulse& o) {} ///< Funkcja deserializująca obiekt SignalImpulse z formatu JSON.
};

/// \class SignalSine
/// \brief Klasa reprezentująca sygnał sinusoidalny.
class SignalSine : public Signal
{
	double T; ///< Okres sygnału.

public:
	/// \brief Konstruktor klasy SignalSine.
	/// \param t Okres sygnału.
	SignalSine(double t) : T(t) {}

	/// \brief Wirtualny destruktor klasy SignalSine.
	~SignalSine() = default;

	/**
	* @brief Metoda pobierająca wartość sygnału dla określonego indeksu.
	*
	* Ta metoda zwraca wartość sygnału sinusoidalnego dla podanego indeksu. Wartość jest obliczana
	* na podstawie wzoru: sin(i * 2 * pi / T), gdzie i to indeks, a T to okres sygnału.
	*
	* @param i Indeks sygnału.
	* @return Wartość sygnału sinusoidalnego dla podanego indeksu.
	*/
	double get(size_t i) const override
	{
		return std::sin(i * 2 * std::numbers::pi / T);
	}

	/**
	 * @brief Metoda zwracająca typ sygnału.
	 * @return Typ sygnału, który jest `SignalType::Sine`.
	 */
	SignalType type() const override
	{
		return SignalType::Sine;
	}

	// Makro definiujące informacje o typie SignalSine niezbędne do serializacji/deserializacji z/do formatu JSON.
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SignalSine, T); 
};

/// \class SignalSquare
/// \brief Klasa reprezentująca sygnał kwadratowy.
class SignalSquare : public Signal
{
	double T; ///< Okres sygnału.
	double D; ///< Współczynnik wypełnienia sygnału.

public:
	/// \brief Konstruktor klasy SignalSquare.
	/// \param t Okres sygnału.
	/// \param d Współczynnik wypełnienia sygnału. Domyślnie 0.5.
	SignalSquare(double t, double d = 0.5) : T(t), D(d) {}

	/// \brief Wirtualny destruktor klasy SignalSquare.
	~SignalSquare() = default;

	/**
	* @brief Metoda pobierająca wartość sygnału dla określonego indeksu.
	*
	* Ta metoda zwraca wartość sygnału kwadratowego dla podanego indeksu. Wartość jest obliczana
	* na podstawie wzoru: (x < D) ? 1 : 0, gdzie x to część ułamkowa wyniku dzielenia indeksu przez okres sygnału.
	* Jeśli wartość x jest mniejsza niż wartość D, to zwracana jest wartość 1, w przeciwnym przypadku zwracana jest wartość 0.
	*
	* @param i Indeks sygnału.
	 * @return Wartość sygnału kwadratowego dla podanego indeksu.
	 */
	double get(size_t i) const override
	{
		double trash;
		double x = std::modf(i / T, &trash);
		return (x < D) ? 1 : 0;
	}

	/**
	 * @brief Metoda zwracająca typ sygnału.
	 * @return Typ sygnału, który jest `SignalType::Square`.
	 */
	SignalType type() const override
	{
		return SignalType::Square;
	}

	// Makro definiujące informacje o typie SignalSquare niezbędne do serializacji/deserializacji z/do formatu JSON.
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SignalSquare, T, D); 
};

/// \class SignalTriangle
/// \brief Klasa reprezentująca sygnał trójkątny.
class SignalTriangle : public Signal
{
	double T; ///< Okres sygnału.

public:
	/// \brief Konstruktor klasy SignalTriangle.
	/// \param t Okres sygnału.
	SignalTriangle(double t) : T(t) {}

	/// \brief Wirtualny destruktor klasy SignalTriangle.
	~SignalTriangle() = default;

	/**
	* @brief Metoda pobierająca wartość sygnału dla określonego indeksu.
	*
	* Ta metoda zwraca wartość sygnału trójkątnego dla podanego indeksu. Wartość jest obliczana
	* na podstawie wzoru warunkowego, który dzieli zakres wartości x na trzy części.
	* Jeśli wartość x jest mniejsza niż 0.25, to zwracana jest wartość x * 4.
	* Jeśli wartość x jest większa niż 0.75, to zwracana jest wartość (x - 1) * 4.
	* W przeciwnym przypadku, czyli gdy wartość x mieści się między 0.25 a 0.75, zwracana jest wartość (0.5 - x) * 4.
	*
	* @param i Indeks sygnału.
	* @return Wartość sygnału trójkątnego dla podanego indeksu.
	*/
	double get(size_t i) const override
	{
		double trash;
		double x = std::modf(i / T, &trash);
		if (x < 0.25)
			return x * 4;
		if (x > 0.75)
			return (x - 1) * 4;
		return (0.5 - x) * 4;
	}

	/**
	 * @brief Metoda zwracająca typ sygnału.
	 * @return Typ sygnału, który jest `SignalType::Triangle`.
	 */
	SignalType type() const override
	{
		return SignalType::Triangle;
	}

	// Makro definiujące informacje o typie SignalTriangle niezbędne do serializacji/deserializacji z/do formatu JSON.
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SignalTriangle, T); 
};


/// \class SignalDelay
/// \brief Klasa reprezentująca opóźniony sygnał.
class SignalDelay : public Signal
{
	size_t D = 0; ///< Opóźnienie sygnału.
	SignalHdl S; ///< Wskaźnik na obiekt sygnału.

public:
	SignalDelay() = default;
	/// \brief Konstruktor klasy SignalDelay.
	/// \param d Opóźnienie sygnału.
	/// \param s Wskaźnik na obiekt sygnału.
	SignalDelay(size_t d, SignalHdl&& s) : D(d), S(std::move(s)) {}

	/// \brief Wirtualny destruktor klasy SignalDelay.
	~SignalDelay() = default;

	/**
	* @brief Konstruktor przenoszący.
	*
	* Konstruktor przenoszący klasy SignalDelay. Przenosi zasoby z innego obiektu SignalDelay do nowo utworzonego obiektu.
	* @param other Obiekt SignalDelay, z którego mają być przeniesione zasoby.
	*/
	SignalDelay(SignalDelay&&) = default;

	/**
	 * @brief Operator przypisania przenoszącego.
	 *
	 * Operator przypisania przenoszącego dla klasy SignalDelay. Przenosi zasoby z innego obiektu SignalDelay do bieżącego obiektu.
	 * @param other Obiekt SignalDelay, z którego mają być przeniesione zasoby.
	 * @return Referencja do bieżącego obiektu SignalDelay.
	 */
	SignalDelay& operator=(SignalDelay&&) = default;

	/**
	 * @brief Metoda pobierająca wartość sygnału dla określonego indeksu.
	 *
	 * Ta metoda zwraca wartość sygnału opóźnionego dla podanego indeksu. Sprawdza w pierwszej kolejności, czy indeks jest mniejszy 
	 * od wartości opóźnienia D. Jeśli tak, zwraca 0, ponieważ sygnał nie jest jeszcze opóźniony. W przeciwnym razie pobiera wartość 
	 * sygnału ze źródła S, używając indeksu pomniejszonego o wartość opóźnienia D.
	 *
	 * @param i Indeks sygnału.
	 * @return Wartość sygnału opóźnionego dla podanego indeksu.
	 */
	double get(size_t i) const override
	{
		if (i < D)
			return 0;
		return S->get(i - D);
	}

	/**
	 * @brief Metoda zwracająca typ sygnału.
	 * @return Typ sygnału, który jest `SignalType::Delay`.
	 */
	SignalType type() const override
	{
		return SignalType::Delay;
	}

	// Makro definiujące informacje o typie SignalDelay niezbędne do serializacji/deserializacji z/do formatu JSON.
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SignalDelay, D, S); 
};
