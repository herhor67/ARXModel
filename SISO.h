#pragma once

class SISO
{
public:
	SISO() = default;
	virtual ~SISO() = default;
	
	virtual double iteration(double) = 0;

};