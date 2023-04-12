#pragma once

#include "settings.h"

#include <valarray>
#include <span>

template <typename T>
std::valarray<T> span_to_valarray(const std::span<T>& sp)
{
	std::valarray<T> temp(sp.size());
	for (size_t i = 0; i < sp.size(); ++i)
		temp[i] = sp[i];
	
	return temp;
}