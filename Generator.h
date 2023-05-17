#pragma once

#include <memory>
#include <vector>

#include <cmath>
#include <numbers>


class Signal
{
public:
	Signal() = default;
	virtual ~Signal() = default;
	virtual double get(size_t) const = 0;
};


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


using SignalPtr = std::unique_ptr<Signal>;

enum class SignalType : uint8_t
{
	Const,
	Sine,
	Square,
	Triangle
};


class Generator
{
	using Signals = std::vector<std::pair<double, SignalPtr>>;

	Signals signals;

public:
	Generator() = default;
	~Generator() = default;

	void Add(double a, SignalPtr s)
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

