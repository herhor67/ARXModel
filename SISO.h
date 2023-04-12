#pragma once

class SISO
{
public:
	SISO();
	virtual ~SISO() = 0;
	
	virtual double iteration(double) = 0;

};