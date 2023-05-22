#pragma once

#include <memory>
#include <vector>

#include <cmath>
#include <numbers>

#include <utility>

/// \class Signal
/// \brief Abstrakcyjna klasa reprezentująca sygnał.
///
/// Klasa Signal definiuje interfejs dla różnych rodzajów sygnałów.
class Signal
{
public:
	/// \brief Konstruktor domyślny klasy Signal.
	Signal() = default;

	/// \brief Wirtualny destruktor klasy Signal.
	virtual ~Signal() = default;

	/// \brief Metoda pobierająca wartość sygnału dla określonego indeksu.
	/// \param index Indeks sygnału.
	/// \return Wartość sygnału dla podanego indeksu.
	virtual double get(size_t) const = 0;
};

/// \typedef SignalPtr
/// \brief Typ wskaźnika na obiekt klasy Signal.
using SignalPtr = std::unique_ptr<Signal>;

/// \class SignalConst
/// \brief Klasa reprezentująca stały sygnał o wartości równiej 1.
class SignalConst : public Signal
{
public:
	/// \brief Konstruktor domyślny klasy SignalConst.
	SignalConst() = default;

	/// \brief Wirtualny destruktor klasy SignalConst.
	~SignalConst() = default;

	double get(size_t)  const override
	{
		return 1;
	}
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

	double get(size_t i)  const override
	{
		return std::sin(i * 2 * std::numbers::pi / T);
	}
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

	double get(size_t i)  const override
	{
		double trash;
		double x = std::modf(i/T, &trash);
		return (x < D) ? 1 : 0;
	}
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

	double get(size_t i) const override
	{
		return i == 0;
	}
};

/// \class SignalDelay
/// \brief Klasa reprezentująca opóźniony sygnał.
class SignalDelay : Signal
{
	size_t D; ///< Opóźnienie sygnału.
	SignalPtr S; ///< Wskaźnik na obiekt sygnału.

public:
	/// \brief Konstruktor klasy SignalDelay.
	/// \param d Opóźnienie sygnału.
	/// \param s Wskaźnik na obiekt sygnału.
	SignalDelay(size_t d, SignalPtr&& s) : D(d), S(std::move(s)) {}

	/// \brief Wirtualny destruktor klasy SignalDelay.
	~SignalDelay() = default;

	SignalDelay(SignalDelay&&) = default;

	double get(size_t i) const override
	{
		if (i < D)
			return 0;
		return S->get(i-D);
	}
};

/// \enum SignalType
/// \brief Typ wyliczeniowy reprezentujący typy sygnałów.
enum class SignalType : uint8_t
{
	Const, ///< Sygnał stały.
	Sine, ///< Sygnał sinusoidalny.
	Square, ///< Sygnał kwadratowy.
	Triangle, ///< Sygnał trójkątny.
	Impulse, ///< Sygnał impulsowy.
};

/// \class Generator
/// \brief Klasa reprezentująca generator sygnałów.
class Generator
{
	using Signals = std::vector<std::pair<double, SignalPtr>>; ///< Typ reprezentujący kolekcję sygnałów.

	Signals signals; ///< Kolekcja sygnałów.

public:
	/// \brief Konstruktor domyślny klasy Generator.
	Generator() = default;

	/// \brief Wirtualny destruktor klasy Generator.
	~Generator() = default;

	/// \brief Konstruktor przenoszący klasy Generator.
	Generator(Generator&&) = default;

	/// \brief Dodaje sygnał do generatora wraz z jego współczynnikiem.
	/// \param a Współczynnik sygnału.
	/// \param s Wskaźnik na obiekt sygnału.
	void add(double a, SignalPtr&& s)
	{
		signals.emplace_back(a, std::move(s));
	}

	/// \brief Pobiera wartość wygenerowanego sygnału dla określonego indeksu.
	/// \param i Indeks sygnału.
	/// \return Wartość wygenerowanego sygnału dla podanego indeksu.
	double get(size_t i)
	{
		double sum = 0;
		for (const auto& s : signals)
			sum += s.first * s.second->get(i);

		return sum;
	}
};
