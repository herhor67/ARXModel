#include "ARX.h"

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



double ARX::iteration(double)
{
	return 1;
}



void ARX::setNum(std::span<double> n)
{
	//B.assign(n.begin(), n.end());
	B = span_to_valarray(n);
}

void ARX::setDen(std::span<double> d)
{
	if (d.size() == 0)
		throw std::invalid_argument("Denominator (A) must be of length at least one!");

	//A.assign(d.begin(), d.end());
	A = span_to_valarray(d);
	
	if (A[0] != 1)
		A /= A[0];

}