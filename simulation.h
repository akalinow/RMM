
/***************************************************************************
 *   simulation.h							   *
 *   Copyright (C) 2005 by Monika Kalinowska                               *
 *   mkalinow@igf.edu.pl                                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <set>

#include "comments.h"
#include "param.h"
#include "point.h"
#include "grid.h"
#include "scheme.h"

using namespace std;

//!  Klasa odpowiedzialna za symulacje. 
/*!
  Odpowiedizlna za przygotowanie danych do symulacji start i przebieg symulacji
*/

class Simulation {
public:

	//! Konstruktor.
	/*!
	Tworzy obiekt simuation
			\param paramFile - nazwa pliku z parametrami symulacji
			\param saveFile - nazwa pliku z parametrami zapisu wyników symulacji
	*/
	Simulation (const string &paramFile, const string &saveFile);
	
	//______________________________________________________________________________________________________________
	/*! Przebieg symulacji od ustawienia parametrow przez uruchominie obiczen po zapis danych i zakonczenie*/
	void MySimulation (const string &param_file, const string &save_file);
	
	//______________________________________________________________________________________________________________	
	/*! Pobiera i ustawia parametry symulacji
		\param param - obiekt w ktorym beda zapisane parmetry symulacji
		\param file - nazwa pliku z param symulacji
	*/
	int SetParam(SimulationParameters *param, const string &file);
	
	//_______________________________________________________________________________________________________________
	/*! Przygotowuje Mape Punktow (siatke) na ktorej bedize przeprowadona symulacja
		\param PointMap - pusta mapa
		\param param - obiekt z parametrami symulacji 
	*/
	int PrepareMap(map<int, Point> &PointMap, SimulationParameters &param);

	//______________________________________________________________________________________________________________	
	/*! \brief Odczytuje z pliku i ustawia parametry zapisu wyników symulacji
	 * 
	 * 	\param file - plik w który zapsano parametry symulacji
		\param n - liczba kroków czasowych symylacji
		\return 1 gdy wszystko poszło OK
	
	
	\code format pliku wejściowego "save.txt" (domyślnie): \endcode	
	 \code{.unparsed}
	    range	pattern 
	    step0	step1	...	stepN 
	 \endcode
	
	 \c range - definiuje czy maja zapisywane być wszystkie kroki symulacji czy tylko kroki wybrane
	 możliwe wartości:
	 <ul>
			<li> \em steps – zapisuje tylko wybrane kroki symulacji, </li>
			<li> \em all – zapisuje wszystkie kroki symulacji. </li>
	</ul>
	\c pattern - definiuje, co ile kroków czasowych ma się odbywać zapis do pliku, 
	możliwe wartości:
	<ul>
			<li> \em last (wartość domyślna) – zapisuje tylko ostatni krok symulacji,</li>
			<li> \em int (zmienna typu int) – dodatnia liczba naturalna określająca, co ile kroków
	czasowych ma być dokonywany zapis do pliku np. 1, 10, 100, 1000 ... - oznacza
	odpowiednio zapis, co 1, 10, 100 lub 1000 kroków czasowych.</li>
	</ul>
	\c step0, \c step1, ... - opcjonalnie istnieje możliwość zdefiniowania w 2 linii pliku save.txt dodatkowych kroków
	czasowych, w których ma być dokonywany zapis. Możliwe tylko dodatnie liczby naturalne oddzielone od siebie
	tabulatorami. Przykład: oprócz zapisu ostatniego kroku (co zdefiniowano w pierwszej
	linijce pliku save.txt, podając wartości „steps” i „last”), chcemy dokonać zapisu
	pierwszych 10 kroków czasowych; druga linijka pliku save.txt powinna wyglądać
	następująco: 
	\code
	1	2	3	4	5	6	7	8	9	10
	\endcode
	*/
	int SetSaveParameters(const string &file, int n);
	
	//_______________________________________________________________________________________________________________
	/*! zapisueje Mape Punktow do podanego pliku
		\param PointMap - mapa pkt
		\param file - nazwa pliku do ktorego ma byc zapisana mapa punktow
		\param opt - opcja wskazujaca co mam byc zapisane
	*	<ul> 
	*		<li> all - zapisuje wszystkie dostepne wartosci dla kazdego pkt</li>
	* 		<li> grid - zapisuje tylko siatke pkt (wpolrzedne x, y pktow i ich flagi) </li>
	*	</ul>
	*/
	void SaveMap(map<int, Point> &PointMap, const string &file, const string &opt);
	
	
	//_______________________________________________________________________________________________________________
	/*! ustawia w miejscu zrodla odpowiednia wartosc koncentracji 
		\param PointMap - mapa pkt
		\param val - wartosc koncentracji zadawanej w punkcie zrodla
	*/
	void SetSourceConcentration (map<int, Point> &PointMap, double &val);


	//_______________________________________________________________________________________________________________
	/*! ustawia w miejscu zrodla odpowiednia wartosc koncentracji dla temeratury 
		\param PointMap - mapa pkt
		\param param - obiekt z parametrami symulacji 	
		\param val - wartosc koncentracji zadawanej w punkcie zrodla
	*/
	void SetSourceConcentrationTemp (map<int, Point> &PointMap, SimulationParameters &param, double &val);

	double ComputeEffectiveTemp(Point &p, double dx, double val, double hz = -9999.99, double Qz = -9999.99);
	
	//_______________________________________________________________________________________________________________
	/*! Wczytuje dane z pliku \ca "ini_cond.txt" dotyczące wartości początkowej koncentracji/temperatury i wywołuje 
	 *  właściwą funckcję zająca wartosci koncentracji/temperatur początkowej na siatce
		\param PointMap - pusta mapa
		\param param - obiekt z parametrami symulacji \n

	
	 format pliku \c "ini_cond.txt" :\n
	 \----------------------------------------- \n
	    \ref SimulationParameters::set_pollutant() "pollutant" \n
	    \ref SimulationParameters::set_dump() "dump" \n
	    \ref SimulationParameters::set_area() "area" \n 
	    \ref SimulationParameters::set_mod() "mod"	\f$x\f$	\f$y\f$	\f$val\f$	\f$h_{z}\f$	\f$Q_{z}\f$ \n
	    \ref SimulationParameters::set_mod() "mod"	\f$x\f$	\f$y\f$	\f$val\f$	\f$h_{z}\f$	\f$Q_{z}\f$ \n
	    ...\n
	    \ref SimulationParameters::set_mod() "mod"	\f$x\f$	\f$y\f$	\f$val\f$	\f$h_{z}\f$	\f$Q_{z}\f$ \n
	    \----------------------------------------- \n	
	Kliknij na zmienną aby zoabczyć możliwe wartości\n
	\f$x\f$	\f$y\f$	- współrzędne punktu lokalizacji źródła \n 
	jeśli mod == val \f$h_{z}\f$	\f$Q_{z}\f$ - wartości głebokości oraz natężenia przepływu w miejscu źródła \n	
	\f$val\f$ - wartość koncentracji/temperatury w miejscu źródła\
	Można podać wiele punktów w kolejnych liniach 
	*/
	int SetInitialConcentration (map<int, Point> &PointMap, SimulationParameters &param);

	
	void SetInitialConcentrationPoint (map<int, Point> &PointMap, SimulationParameters &param, double x0, double y0, double val);
	void SetInitialConcentrationPlaneWave (map<int, Point> &PointMap);
	
	void SetInitialConcentrationGauss (map<int, Point> &PointMap, SimulationParameters &param, double x0, double y0, double val, int krok);
	void SetInitialConcentrationGaussNonDiag (map<int, Point> &PointMap, SimulationParameters &param, double x0, double y0, double val, int krok);

	double Gaus(double x, double mean, double sigma, bool norm);
	
	int SetInitialConcentrationFile (map<int, Point> &PointMap);
	void SetInOutPointValue (map<int, Point> &PointMap);
	
	double My_Gaus(double x, double y, double meanX, double meanY, double sigma);
	
	
	void set_file (string file){_file = file;};
	string get_file () const {return _file;};
	
	double ComputeEffectiveTemp(Point pkt, double arg2, double arg3, double val1, double hz, double Qz);
	
private:
  //const string _file_param; //param.txt
  string _file_grid;// = "grid.txt";
  string _file_v;// = "v.txt";
  string _file_data;// = "data.txt";
  string _file_d;// = "d.txt";
  string _file;
 // const string _file_save; //save.txt
  set<int> _save; //zbior z lista kroków w ktorym ma sie odbyc zapis do symulacji
	
};
  
#endif
