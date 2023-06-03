#include "Generator.h"

#include <memory>
#include <vector>

#include <cmath>
#include <numbers>

#include <utility>

#include "json.hpp" ///< Wczytanie biblioteki "json.hpp", która umożliwia serializację i deserializację obiektów JSON.
using json = nlohmann::json;

/**
 * @brief Dodaje sygnał do generatora wraz z jego współczynnikiem.
 *
 * @param a Współczynnik sygnału.
 * @param s R-wartościowy wskaźnik na obiekt sygnału.
 */
void Generator::add(double a, SignalHdl&& s)
{
    signals.emplace_back(a, std::move(s)); ///< Dodaje sygnał wraz z jego współczynnikiem do wektora sygnałów.
}

/**
 * @brief Pobiera wartość wygenerowanego sygnału dla określonego indeksu.
 *
 * @param i Indeks sygnału.
 * @return Wartość wygenerowanego sygnału dla podanego indeksu.
 */
double Generator::get(size_t i)
{
    double sum = 0;
    for (const auto& s : signals) ///< Przechodzi przez każdy sygnał w wektorze sygnałów.
        sum += s.first * s.second->get(i); ///< Oblicza sumę mnożenia współczynnika sygnału przez jego wartość dla danego indeksu.

    return sum;
}

/**
 * @brief Serializacja obiektu klasy Generator do formatu JSON.
 *
 * @param j Obiekt JSON, do którego będą zapisywane dane.
 * @param o Obiekt Generator, który będzie serializowany.
 */
void to_json(json& j, const Generator& o)
{
    j = json::array(); ///< Inicjalizuje obiekt JSON jako tablicę.

    for (const auto& s : o.signals) ///< Przechodzi przez każdy sygnał w wektorze sygnałów.
    {
        json obj = json::object();
        obj["A"] = s.first; ///< Dodaje współczynnik sygnału do obiektu JSON.
        obj["S"] = s.second; ///< Dodaje obiekt sygnału do obiektu JSON.
        j.push_back(obj); ///< Dodaje obiekt JSON do tablicy JSON.
    }
}

/**
 * @brief Deserializacja obiektu klasy Generator z formatu JSON.
 *
 * @param j Obiekt JSON, z którego będą odczytywane dane.
 * @param o Obiekt Generator, do którego będą wczytywane dane.
 */
void from_json(const json& j, Generator& o)
{
    for (const auto& it : j.items()) ///< Przechodzi przez elementy obiektu JSON.
    {
        const auto obj = it.value(); ///< Pobiera wartość bieżącego elementu.

        double a = obj["A"]; ///< Pobiera współczynnik z obiektu JSON.
        SignalHdl s = obj["S"]; ///< Pobiera obiekt sygnału z obiektu JSON.

        o.add(a, std::move(s)); ///< Dodaje sygnał wraz z jego współczynnikiem do obiektu Generator.
    }
}
