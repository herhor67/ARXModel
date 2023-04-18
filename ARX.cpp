#include "ARX.h"

#include "settings.h"

#include "helpers.h"


//#include <exception>
#include <stdexcept>





ARX::ARX(std::initializer_list<double> A, std::initializer_list<double> B, unsigned dly, double ns) : k(dly), noiseamp(ns)
{
	setNum(B);
	setDen(A);
}

ARX::~ARX()
{}



double ARX::iteration(double in)
{
	inBuf = inBuf.shift(-1);
	if (inBuf.size())
		inBuf[0] = in;

	DS temp = inBuf[std::slice(k, B.size(), 1)];

	double NUMxIN = (B * temp).sum() ;
	double DENxOUT = (A * outBuf).sum();

	double out = NUMxIN - DENxOUT + noiseamp * getNoise();

	outBuf = outBuf.shift(-1);
	if (outBuf.size())
		outBuf[0] = out;

	return out;
}



void ARX::setNum(std::initializer_list<double> n)
{
	B = n;

	inBuf.resize(B.size() + k);
}

void ARX::setDen(std::initializer_list<double> d)
{
	//if (d.size() == 0)
	//	throw std::invalid_argument("Denominator (A) must be of length at least one!");

	A = d;
	
	//if (A[0] != 1)
	//	A /= A[0];

	outBuf.resize(A.size());
}

void ARX::setup()
{
	
}

// static S.H.I.T.

double ARX::getNoise()
{
	return 0;
}

