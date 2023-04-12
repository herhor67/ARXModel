#include "ARX.h"

#include "settings.h"

#include "helpers.h"


//#include <exception>
#include <stdexcept>





ARX::ARX(std::initializer_list<double> A, std::initializer_list<double> B, unsigned dly, double ns) : k(dly), noiseamp(ns)
{
	setNum(B);
	setDen(A);
	k = 0;
}

ARX::~ARX()
{}



double ARX::iteration(double in)
{
	cout << inBuf;
	inBuf = inBuf.shift(1);
	if (inBuf.size())
		inBuf[inBuf.size() - 1] = in;
	cout << inBuf << '\t';


	double out = (B * static_cast<DS>(inBuf[std::slice(0, B.size(), 1)])).sum() - (A * outBuf).sum() + noiseamp * getNoise();


	cout << outBuf;
	outBuf = outBuf.shift(1);
	if (outBuf.size())
		outBuf[outBuf.size() - 1] = out;
	cout << outBuf << endl;

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

