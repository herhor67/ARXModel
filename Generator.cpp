#include "Generator.h"

#include <memory>
#include <vector>

#include <cmath>
#include <numbers>

#include <utility>


#include "json.hpp" ///< Wczytanie biblioteki "json.hpp", która umo¿liwia serializacjê i deserializacjê obiektów JSON.
using json = nlohmann::json;


void Generator::add(double a, SignalHdl&& s)
{
	signals.emplace_back(a, std::move(s));
}

/// \brief Pobiera wartoœæ wygenerowanego sygna³u dla okreœlonego indeksu.
/// \param i Indeks sygna³u.
/// \return Wartoœæ wygenerowanego sygna³u dla podanego indeksu.
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