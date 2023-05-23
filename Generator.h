#pragma once

#include <memory>
#include <vector>

#include <cmath>
#include <numbers>

#include <utility>


#include "json.hpp" ///< Wczytanie biblioteki "json.hpp", która umożliwia serializację i deserializację obiektów JSON.
using json = nlohmann::json;


/// \enum SignalType
/// \brief Typ wyliczeniowy reprezentujący typy sygnałów.
enum class SignalType : uint8_t
{
	Virtual,
	Const, ///< Sygnał stały.
	Sine, ///< Sygnał sinusoidalny.
	Square, ///< Sygnał kwadratowy.
	Triangle, ///< Sygnał trójkątny.
	Impulse, ///< Sygnał impulsowy.
	Delay = -1,
};


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

	virtual SignalType type() const = 0;

	friend void to_json(json&, const Signal&) {}
	friend void from_json(const json&, Signal&) {}
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

	SignalType type() const override
	{
		return SignalType::Const;
	}

	friend void to_json(nlohmann::json& j, const SignalConst& o) {}
	friend void from_json(const nlohmann::json& j, SignalConst& o) {}
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

	SignalType type() const override
	{
		return SignalType::Sine;
	}

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

	double get(size_t i)  const override
	{
		double trash;
		double x = std::modf(i/T, &trash);
		return (x < D) ? 1 : 0;
	}

	SignalType type() const override
	{
		return SignalType::Square;
	}

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

	SignalType type() const override
	{
		return SignalType::Triangle;
	}

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SignalTriangle, T);
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

	SignalType type() const override
	{
		return SignalType::Impulse;
	}

	friend void to_json(json& j, const SignalImpulse& o) {}
	friend void from_json(const json& j, SignalImpulse& o) {}
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

	SignalType type() const override
	{
		return SignalType::Delay;
	}

	friend void to_json(nlohmann::json& nlohmann_json_j, const SignalDelay& nlohmann_json_t)
	{
		nlohmann_json_j["D"] = nlohmann_json_t.D;
		nlohmann_json_j["S"] = nlohmann_json_t.S;
	}
	friend void from_json(const nlohmann::json& nlohmann_json_j, SignalDelay& nlohmann_json_t)
	{
		nlohmann_json_j.at("D").get_to(nlohmann_json_t.D);
		nlohmann_json_j.at("S").get_to(nlohmann_json_t.S);
	}
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

	friend void to_json(json& j, const Generator& o)
	{
		j = json::array();

		for (const auto& s : o.signals)
		{
			json sj = json::object();
			sj["type"] = static_cast<uint8_t>(s.second->type());
			sj["A"] = s.first;

			to_json(sj["params"], *s.second);

			j.push_back(sj);
		}
	}
	friend void from_json(const json& j, Generator& o)
	{
		for (const auto& sj : j)
		{

			SignalType t = static_cast<SignalType>(sj["type"]);

			SignalPtr s;

			//o.add();
		}
	}

};
