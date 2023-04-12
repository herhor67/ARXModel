#pragma once

#include "SISO.h"

#include <valarray>
#include <span>
#include <initializer_list>


class ARX : public SISO
{
	using DS = std::valarray<double>;

	DS A; // den
	DS B; // num
	unsigned k = 0;
	double noiseamp = 1;

	DS inBuf;
	DS outBuf;


public:
	ARX(std::initializer_list<double>, std::initializer_list<double>, unsigned = 0, double = 0);
	~ARX();

	double iteration(double);

	void setNum(std::initializer_list<double>);

	void setDen(std::initializer_list<double>);

	void setup();

	static double getNoise();

};

