#pragma once

#include "settings.h"

#include <valarray>

#include <iostream>

template <typename T>
std::ostream& operator<<(std::ostream& o, const std::valarray<T>& va)
{
	o << '(';
	for (const T& v : va)
		o << v << ',';
	o << ')';
	return o;
}