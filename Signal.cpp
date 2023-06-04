#include "Signal.h"

/**
 * @brief Konstruktor klasy SignalHdl.
 *
 * Konstruktor tworzy obiekt SignalHdl przyjmując wskaźnik przenoszony na obiekt sygnału.
 * @param p Wskaźnik przenoszony na obiekt sygnału.
 */
SignalHdl::SignalHdl(SignalPtr&& p) : ptr(std::move(p)) {}

/**
 * @brief Operator dereferencji.
 *
 * Operator dereferencji (*) zwraca referencję do obiektu sygnału przechowywanego przez SignalHdl.
 * @return Referencja do obiektu sygnału.
 */
Signal& SignalHdl::operator*() const
{
	return *ptr.get();
}

/**
 * @brief Operator dostępu do składowych.
 *
 * Operator dostępu do składowych (->) zwraca wskaźnik do obiektu sygnału przechowywanego przez SignalHdl.
 * @return Wskaźnik do obiektu sygnału.
 */
Signal* SignalHdl::operator->() const
{
	return ptr.get();
}

/**
 * @brief Funkcja to_json.
 *
 * Funkcja to_json służy do serializacji obiektu SignalHdl do formatu JSON.
 * Konwertuje obiekt SignalHdl na obiekt JSON, zapisując typ sygnału i jego parametry.
 *
 * @param j Obiekt JSON, do którego zostanie zapisana serializacja.
 * @param o Obiekt SignalHdl, który ma zostać zserializowany.
 */
void to_json(json& j, const SignalHdl& o)
{
	// Pobranie typu sygnału przechowywanego przez SignalHdl
	SignalType t = o.ptr->type();
	j["t"] = static_cast<SignalEnumT>(t);

	json obj;
	switch (t)
	{
	case SignalType::Virtual:
		throw std::invalid_argument("Cannot serialize pure virtual class!");
		break;
	case SignalType::Const:
		obj = *static_cast<SignalConst*>(o.ptr.get());
		break;
	case SignalType::Impulse:
		obj = *static_cast<SignalImpulse*>(o.ptr.get());
		break;
	case SignalType::Sine:
		obj = *static_cast<SignalSine*>(o.ptr.get());
		break;
	case SignalType::Square:
		obj = *static_cast<SignalSquare*>(o.ptr.get());
		break;
	case SignalType::Triangle:
		obj = *static_cast<SignalTriangle*>(o.ptr.get());
		break;
	case SignalType::Delay:
		obj = *static_cast<SignalDelay*>(o.ptr.get());
		break;
	default:
		throw std::invalid_argument("Such generator does not exist!");
		break;
	}

	j["p"] = obj;
}

/**
 * @brief Funkcja from_json.
 *
 * Funkcja from_json służy do deserializacji obiektu SignalHdl z formatu JSON.
 * Konwertuje obiekt JSON na obiekt SignalHdl, odtwarzając typ sygnału i jego parametry.
 *
 * @param j Obiekt JSON, który ma zostać zdeserializowany.
 * @param o Obiekt SignalHdl, do którego zostanie zapisana deserializacja.
 */
void from_json(const json& j, SignalHdl& o)
{
	// Pobranie typu sygnału z obiektu JSON
	SignalType t = static_cast<SignalType>(j["t"]);

	switch (t)
	{
	case SignalType::Virtual:
		throw std::invalid_argument("Cannot create pure virtual class!");
		break;
	case SignalType::Const:
		o = SignalHdl::make<SignalConst>(j["p"]);
		break;
	case SignalType::Impulse:
		o = SignalHdl::make<SignalImpulse>(j["p"]);
		break;
	case SignalType::Sine:
		o = SignalHdl::make<SignalSine>(j["p"]);
		break;
	case SignalType::Square:
		o = SignalHdl::make<SignalSquare>(j["p"]);
		break;
	case SignalType::Triangle:
		o = SignalHdl::make<SignalTriangle>(j["p"]);
		break;
	case SignalType::Delay:
		o = SignalHdl::make<SignalDelay>(j["p"]);
		break;
	default:
		throw std::invalid_argument("Such generator does not exist!");
		break;
	}
}
