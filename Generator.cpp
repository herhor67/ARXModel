#include "Generator.h"

#include <memory>
#include <vector>

#include <cmath>
#include <numbers>

#include <utility>


#include "json.hpp" ///< Wczytanie biblioteki "json.hpp", kt�ra umo�liwia serializacj� i deserializacj� obiekt�w JSON.
using json = nlohmann::json;


void Generator::add(double a, SignalHdl&& s)
{
	signals.emplace_back(a, std::move(s));
}

/// \brief Pobiera warto�� wygenerowanego sygna�u dla okre�lonego indeksu.
/// \param i Indeks sygna�u.
/// \return Warto�� wygenerowanego sygna�u dla podanego indeksu.
double Generator::get(size_t i)
{
	double sum = 0;
	for (const auto& s : signals)
		sum += s.first * s.second->get(i);

	return sum;
}


void to_json(json& j, const Generator& o)
{
	j = json::array();

	for (const auto& s : o.signals)
	{
		json obj = json::object();
		obj["A"] = s.first;
		obj["S"] = s.second;
		j.push_back(obj);
	}
}

void from_json(const json& j, Generator& o)
{
	for (const auto& it : j.items())
	{
		const auto obj = it.value();

		double a = obj["A"];
		SignalHdl s = obj["S"];

		o.add(a, std::move(s));
	}
}