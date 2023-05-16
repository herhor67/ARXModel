#pragma once

#include <memory>

#include <cmath>
#include <numbers>


class Signal
{
public:
	Signal() = default;
	virtual ~Signal() = 0;

	virtual double get(size_t) = 0;
};


class SignalConst : public Signal
{
public:
	SignalConst() = default;
	~SignalConst() = default;
	double get(size_t)
	{
		return 1;
	}
};

class SignalSine : public Signal
{
	double F;

public:
	SignalSine(double f, double a) : F(f) {}
	~SignalSine() = default;
	double get(size_t i)
	{
		return std::sin(i * 2 * std::numbers::pi * F);
	}
};

class SignalSquare : public Signal
{
	double F;

public:
	SignalSquare(double f) : F(f) {}
	~SignalSquare() = default;
};



using SignalPtr = std::unique_ptr<Signal>;



class Generator
{
	using Signals = std::vector<std::pair<double, SignalPtr>>;

	Signals signals;

public:
	Generator() = default;
	Generator(const GenConstr& c) : constructors(c) {}
	~Generator() = default;

	void Add()

	double get()
	{
		if (generators.empty())
	}
}