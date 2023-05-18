#include "settings.h"

#include "ARX.h"
#include "PID.h"
#include "Generator.h"
#include "Simulation.h"



#include <iomanip>

#include <vector>

#ifndef NDEBUG

/**
 * Funkcja raportuje o błędach w sekwencji, porównując oczekiwane wartości z faktycznymi.
 * @param spodz - wektor zawierający spodziewane wartości sekwencji
 * @param fakt - wektor zawierający faktyczne wartości sekwencji
 */
void raportBleduSekwencji(const std::vector<double>& spodz, const std::vector<double>& fakt)
{
	constexpr size_t PREC = 3;
	std::cerr << std::fixed << std::setprecision(PREC);
	std::cerr << "  Spodziewany:\t";
	for (const auto& el : spodz)
		std::cerr << el << ", ";
	std::cerr << "\n  Faktyczny:\t";
	for (const auto& el : fakt)
		std::cerr << el << ", ";
	std::cerr << std::endl << std::endl;
}
/**
 * Funkcja porównująca dwie sekwencje liczb zmiennoprzecinkowych
 * @param spodz - wektor zawierający spodziewane wartości sekwencji
 * @param fakt - wektor zawierający faktyczne wartości sekwencji
 * @return true, jeśli sekwencje są takie same; false, odwrotne zdarzenie
 */
bool porownanieSekwencji(const std::vector<double>& spodz, const std::vector<double>& fakt)
{
	constexpr double TOL = 1e-3;	// tolerancja dla porównań zmiennoprzecinkowych
	bool result = fakt.size() == spodz.size();
	for (int i = 0; result && i < fakt.size(); i++)
		result = fabs(fakt[i] - spodz[i]) < TOL;
	return result;
}
/**
 * Testuje ARX dla braku pobudzenia.
 */
void test_ARX_brakPobudzenia()
{
	//Sygnatura testu:
	std::cerr << "ARX (-0.4 | 0.6 | 1 | 0 ) -> test zerowego pobudzenia: ";
	try
	{
		// Przygotowanie danych:
		ARX instancjaTestowa({ -0.4 }, { 0.6 }, 1, 0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu (tu same 0)
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja modelu:
		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.sim(sygWe[i]);

		// Walidacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy))
			std::cerr << "OK!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy);
		}
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}
// Test - skok jednostkowy nr 1
void test_ARX_skokJednostkowy_1()
{
	//Sygnatura testu:
	std::cerr << "ARX (-0.4 | 0.6 | 1 | 0 ) -> test skoku jednostkowego nr 1: ";

	try
	{
		// Przygotowanie danych:
		ARX instancjaTestowa({ -0.4 }, { 0.6 }, 1, 0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu 
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy 
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0, 0, 0.6, 0.84, 0.936, 0.9744, 0.98976, 0.995904, 0.998362, 0.999345, 0.999738, 0.999895, 0.999958, 0.999983, 0.999993, 0.999997, 0.999999, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

		// Symulacja modelu:
		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.sim(sygWe[i]);

		// Walidacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy))
			std::cerr << "OK!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy);
		}
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}
// Test - skok jednostkowy nr 2
void test_ARX_skokJednostkowy_2()
{
	//Sygnatura testu:
	std::cerr << "ARX (-0.4 | 0.6 | 2 | 0 ) -> test skoku jednostkowego nr 2: ";

	try
	{
		// Przygotowanie danych:
		ARX instancjaTestowa({ -0.4 }, { 0.6 }, 2, 0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu, 
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0, 0, 0, 0.6, 0.84, 0.936, 0.9744, 0.98976, 0.995904, 0.998362, 0.999345, 0.999738, 0.999895, 0.999958, 0.999983, 0.999993, 0.999997, 0.999999, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

		// Symulacja modelu:
		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.sim(sygWe[i]);

		// Walidacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy))
			std::cerr << "OK!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy);
		}
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}
// Test - skok jednostkowy nr 3
void test_ARX_skokJednostkowy_3()
{
	//Sygnatura testu:
	std::cerr << "ARX (-0.4, 0.2 | 0.6, 0.3 | 2 | 0 ) -> test skoku jednostkowego nr 3: ";
	try
	{
		// Przygotowanie danych:
		ARX instancjaTestowa({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu, 
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0, 0, 0, 0.6, 1.14, 1.236, 1.1664, 1.11936, 1.11446, 1.12191, 1.12587, 1.12597, 1.12521, 1.12489, 1.12491, 1.12499, 1.12501, 1.12501, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125 };

		// Symulacja modelu:
		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.sim(sygWe[i]);

		// Weryfikacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy))
			std::cerr << "OK!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy);
		}
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

int main()
{
	// Testy dla modelu ARX
	test_ARX_brakPobudzenia(); // Wywołanie testu dla ARX bez pobudzenia
	test_ARX_skokJednostkowy_1(); // Wywołanie testu dla ARX z skokiem jednostkowym nr 1
	test_ARX_skokJednostkowy_2(); // Wywołanie testu dla ARX z skokiem jednostkowym nr 2
	test_ARX_skokJednostkowy_3(); // Wywołanie testu dla ARX z skokiem jednostkowym nr 3

	system("PAUSE"); // Oczekiwanie na wciśnięcie dowolnego klawisza przez użytkownika
}

#else

int main()
{
	cout << "Hello there!" << endl;

	ARX arx({ 0.5, -0.2, 0.3 } , { 1.2, -0.6, 0.8, 0.2 }, 0, 0); // Inicjalizacja obiektu ARX
	//PID pid(1.32, 0.7, 0.175); // Inicjalizacja obiektu PID
	PID pid(1, 0.1, 0.01); // Inicjalizacja obiektu PID

	Generator gen;
	gen.add(1, std::make_unique<SignalConst>());

	Simulation sim(std::move(arx), std::move(pid), std::move(gen)); // Inicjalizacja obiektu symulacji z ARX i PID
	//sim.save("save.json"); // Zapisanie symulacji do pliku JSON

	//Simulation sim("save.json"); // Wczytanie symulacji z pliku JSON

	sim.run(10); // Uruchomienie symulacji przez 10 jednostek czasu z krokiem równym 1

	system("PAUSE"); // Oczekiwanie na wciśnięcie dowolnego klawisza przez użytkownika
}
#endif //ndebug // Warunek kompilacji w trybie Release