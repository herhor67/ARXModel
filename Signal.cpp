#include "Signal.h"


SignalHdl::SignalHdl(SignalPtr&& p) : ptr(std::move(p)) {}

Signal& SignalHdl::operator*() const
{
	return *ptr.get();
}
Signal* SignalHdl::operator->() const
{
	return ptr.get();
}


void to_json(json& j, const SignalHdl& o)
{
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

void from_json(const json& j, SignalHdl& o)
{
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

