#pragma once

#include "SISO.h"

#include <valarray>
#include <span>
#include <initializer_list>


class ARX : public SISO
{
	std::valarray<double> A; // den
	std::valarray<double> B; // num
	unsigned k = 0;
	double noiseamp = 1;

public:
	ARX(std::initializer_list<double>, std::initializer_list<double>, unsigned = 0, double = 0);
	~ARX();

	double iteration(double);

	void setNum(std::span<double>);

	void setDen(std::span<double>);

};

