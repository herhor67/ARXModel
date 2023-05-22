#pragma once

/// \class SISO
/// \brief Klasa abstrakcyjna reprezentująca system jednowejściowy-jednowyjściowy (SISO).
///
/// Klasa SISO definiuje interfejs dla systemów SISO. System SISO to system, który posiada
/// tylko jedno wejście i jedno wyjście.
class SISO
{
	/// \brief Deklaracja klasy Simulation jako przyjaciela klasy SISO.
	friend class Simulation;

public:
	/// \brief Konstruktor domyślny klasy SISO.
	SISO() = default;

	/// \brief Wirtualny destruktor klasy SISO.
	virtual ~SISO() = default;
	
	/// \brief Wirtualna metoda symulująca jedną iterację systemu SISO.
    /// \param input Sygnał wejściowy.
    /// \return Sygnał wyjściowy.
	/// 
	/// Metoda sim przyjmuje sygnał wejściowy jako argument i zwraca sygnał wyjściowy. 
	/// Jest to czysto wirtualna metoda, co oznacza, że nie jest zdefiniowana w tej klasie 
	/// i musi być zaimplementowana w klasie dziedziczącej.
	virtual double sim(double) = 0;
};
