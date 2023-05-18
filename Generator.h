#pragma once

#include <memory>
#include <vector>

#include <cmath>
#include <numbers>

#include <utility>


class Signal
{
public:
	Signal() = default;
	virtual ~Signal() = default;
	virtual double get(size_t) const = 0;
};

using SignalPtr = std::unique_ptr<Signal>;

class SignalConst : public Signal
{
public:
	SignalConst() = default;
	~SignalConst() = default;

	double get(size_t)  const override
	{
		return 1;
	}
};

class SignalSine : public Signal
{
	double T;

public:
	SignalSine(double t) : T(t) {}
	~SignalSine() = default;

	double get(size_t i)  const override
	{
		return std::sin(i * 2 * std::numbers::pi / T);
	}
};

class SignalSquare : public Signal
{
	double T;
	double D;

public:
	SignalSquare(double t, double d = 0.5) : T(t), D(d) {}
	~SignalSquare() = default;

	double get(size_t i)  const override
	{
		double trash;
		double x = std::modf(i/T, &trash);
		return (x < D) ? 1 : 0;
	}
};

class SignalTriangle : public Signal
{
	double T;

public:
	SignalTriangle(double t) : T(t) {}
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

class SignalImpulse : public Signal
{
public:
	SignalImpulse() {}
	~SignalImpulse() = default;

	double get(size_t i) const override
	{
		return i == 0;
	}
};

class SignalDelay : Signal
{
	size_t D;
	SignalPtr S;

public:
	SignalDelay(size_t d, SignalPtr&& s) : D(d), S(std::move(s)) {}
	~SignalDelay() = default;
	SignalDelay(SignalDelay&&) = default;

	double get(size_t i) const override
	{
		if (i < D)
			return 0;
		return S->get(i-D);
	}
};



enum class SignalType : uint8_t
{
	Const,
	Sine,
	Square,
	Triangle,
	Impulse,
};


class Generator
{
	using Signals = std::vector<std::pair<double, SignalPtr>>;

	Signals signals;

public:
	Generator() = default;
	~Generator() = default;

	Generator(Generator&&) = default;

	void add(double a, SignalPtr&& s)
	{
		signals.emplace_back(a, std::move(s));
	}

	double get(size_t i)
	{
		double sum = 0;
		for (const auto& s : signals)
			sum += s.first * s.second->get(i);

		return sum;
	}
};

