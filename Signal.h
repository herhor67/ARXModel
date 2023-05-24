#pragma once

#include <memory>
#include <vector>

#include <cmath>
#include <numbers>
#include <utility>

#include "json.hpp" ///< Wczytanie biblioteki "json.hpp", która umo¿liwia serializacjê i deserializacjê obiektów JSON.
using json = nlohmann::json;


using SignalEnumT = int8_t;
/// \enum SignalType
/// \brief Typ wyliczeniowy reprezentuj¹cy typy sygna³ów.
enum SignalType : SignalEnumT
{
	Virtual = 0,
	Const, ///< Sygna³ sta³y.
	Impulse, ///< Sygna³ impulsowy.
	Sine, ///< Sygna³ sinusoidalny.
	Square, ///< Sygna³ kwadratowy.
	Triangle, ///< Sygna³ trójk¹tny.

	Delay = std::numeric_limits<SignalEnumT>::min(),
};

//NLOHMANN_JSON_SERIALIZE_ENUM(SignalType,
//	{
//		{ SignalType::Virtual, "Virtual" },
//		{ SignalType::Const, "Const" },
//		{ SignalType::Impulse, "Impulse" },
//		{ SignalType::Sine, "Sine" },
//		{ SignalType::Square, "SquareSquare" },
//		{ SignalType::Triangle, "Triangle" },
//		{ SignalType::Delay, "Delay" },
//	}
//)

class Signal;

/// \typedef SignalHdl
/// \brief Typ wskaŸnika na obiekt klasy Signal.
class SignalHdl
{
	using SignalPtr = std::unique_ptr<Signal>;
	SignalPtr ptr;
public:

	SignalHdl() = default;
	SignalHdl(const SignalHdl&) = delete;
	SignalHdl& operator=(const SignalHdl&) = delete;
	SignalHdl(SignalHdl&&) = default;
	SignalHdl& operator=(SignalHdl&&) = default;

	SignalHdl(SignalPtr&&);

	Signal& operator*() const;
	Signal* operator->() const;

	friend void to_json(json&, const SignalHdl&);
	friend void from_json(const json&, SignalHdl&);

	template<typename T, typename... Args>
	static SignalHdl make(Args&&... args)
	{
		SignalPtr sp = std::make_unique<T>(args...);
		return SignalHdl(std::move(sp));
	}
};

/// \class Signal
/// \brief Abstrakcyjna klasa reprezentuj¹ca sygna³.
///
/// Klasa Signal definiuje interfejs dla ró¿nych rodzajów sygna³ów.
class Signal
{
	friend SignalHdl;
public:

	/// \brief Konstruktor domyœlny klasy Signal.
	Signal() = default;

	/// \brief Wirtualny destruktor klasy Signal.
	virtual ~Signal() = default;

	/// \brief Metoda pobieraj¹ca wartoœæ sygna³u dla okreœlonego indeksu.
	/// \param index Indeks sygna³u.
	/// \return Wartoœæ sygna³u dla podanego indeksu.
	virtual double get(size_t) const = 0;

	virtual SignalType type() const = 0;
};

/// \class SignalConst
/// \brief Klasa reprezentuj¹ca sta³y sygna³ o wartoœci równiej 1.
class SignalConst : public Signal
{
public:
	/// \brief Konstruktor domyœlny klasy SignalConst.
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

	friend void to_json(json& j, const SignalConst& o) {}
	friend void from_json(const json& j, SignalConst& o) {}
};

/// \class SignalImpulse
/// \brief Klasa reprezentuj¹ca sygna³ impulsowy.
class SignalImpulse : public Signal
{
public:
	/// \brief Konstruktor domyœlny klasy SignalImpulse.
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

/// \class SignalSine
/// \brief Klasa reprezentuj¹ca sygna³ sinusoidalny.
class SignalSine : public Signal
{
	double T; ///< Okres sygna³u.

public:
	/// \brief Konstruktor klasy SignalSine.
	/// \param t Okres sygna³u.
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
/// \brief Klasa reprezentuj¹ca sygna³ kwadratowy.
class SignalSquare : public Signal
{
	double T; ///< Okres sygna³u.
	double D; ///< Wspó³czynnik wype³nienia sygna³u.

public:
	/// \brief Konstruktor klasy SignalSquare.
	/// \param t Okres sygna³u.
	/// \param d Wspó³czynnik wype³nienia sygna³u. Domyœlnie 0.5.
	SignalSquare(double t, double d = 0.5) : T(t), D(d) {}

	/// \brief Wirtualny destruktor klasy SignalSquare.
	~SignalSquare() = default;

	double get(size_t i)  const override
	{
		double trash;
		double x = std::modf(i / T, &trash);
		return (x < D) ? 1 : 0;
	}

	SignalType type() const override
	{
		return SignalType::Square;
	}

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SignalSquare, T, D);
};

/// \class SignalTriangle
/// \brief Klasa reprezentuj¹ca sygna³ trójk¹tny.
class SignalTriangle : public Signal
{
	double T; ///< Okres sygna³u.

public:
	/// \brief Konstruktor klasy SignalTriangle.
	/// \param t Okres sygna³u.
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


/// \class SignalDelay
/// \brief Klasa reprezentuj¹ca opóŸniony sygna³.
class SignalDelay : public Signal
{
	size_t D; ///< OpóŸnienie sygna³u.
	SignalHdl S; ///< WskaŸnik na obiekt sygna³u.

public:
	SignalDelay() = default;
	/// \brief Konstruktor klasy SignalDelay.
	/// \param d OpóŸnienie sygna³u.
	/// \param s WskaŸnik na obiekt sygna³u.
	SignalDelay(size_t d, SignalHdl&& s) : D(d), S(std::move(s)) {}

	/// \brief Wirtualny destruktor klasy SignalDelay.
	~SignalDelay() = default;

	SignalDelay(SignalDelay&&) = default;
	//SignalDelay& operator=(SignalDelay&&) = default;

	double get(size_t i) const override
	{
		if (i < D)
			return 0;
		return S->get(i - D);
	}

	SignalType type() const override
	{
		return SignalType::Delay;
	}

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SignalDelay, D, S);

};




