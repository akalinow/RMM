
/***************************************************************************
 *   grid.h																   *
 *   Copyright (C) 2005 by Monika Kalinowska                               *
 *   mkalinow@igf.edu.pl                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef GRID_H
#define GRID_H
 
#include <cstdlib>
#include <string>
#include <map>
#include <vector>
#include "point.h"
#include "comments.h"
#include "tensor.h"

using namespace std;

//***********************************************GRID******************************************************
//!  Klasa Przygotowujaca siatke do symulacji. 
/*!
  Zawiera funkcje potrzebne do przgotowania siatki, wspolne dla roznych sposobow wczytywania danych.
*/
class Grid {
public:
		
	/*! Wczytywania dane z pliku wejsciowego do mapy punktow 
		\param PointMap - referencja do Mapy Punktow
		\param file - nazwa pliku z danymi 
		\param sym_param - obiekt z parametrami symulacji
	*/
	int virtual ReadData (map<int, Point> &PointMap, const string & file, SimulationParameters & sym_param) { return 0; }
	
	//_____________________________________________________________________________________________________
	/*! Ustawianie max wartosci zmiennych w parametrcha symulacji
		\param vx - refer. do maksymalnej skladowej x wektora predkosci
		\param vy - refer. do maksymalnej skladowej y wektora predkosci
		\param Dxx - refer. do maksymalnej skladowej xx tensora Dyspersji
		\param Dxy - refer. do maksymalnej skladowej xy tensora Dyspersji
		\param Dyy - refer. do maksymalnej skladowej tensora Dyspersji
		\param param - referencje do obiektu z parametrami symulacji
	*/
	void SetMaxValues(double &vx, double &vy, 
		double &Dxx, double &Dxy, double Dyy, SimulationParameters &param);

	//_____________________________________________________________________________________________________
	/*! Ustawianie min wartosci zmiennych w parametrcha symulacji
		\param vx - refer. do minimalnej skladowej x wektora predkosci
		\param vy - refer. do minimalnej skladowej y wektora predkosci
		\param Dxx - refer. do minimalnej skladowej xx tensora Dyspersji
		\param Dxy - refer. do minimalnej skladowej xy tensora Dyspersji
		\param Dyy - refer. do minimalnej skladowej tensora Dyspersji
		\param param - referencje do obiektu z parametrami symulacji
	*/	
	void SetMinValues(double &vx, double &vy, 
		double &Dxx, double &Dxy, double Dyy, SimulationParameters &param);	
	
	//_____________________________________________________________________________________________________
	/*! Ustawia wartosci max i min wspolrzednych x i y na siatce
		\param xmax - refer. do najwiekszej wartosc wspolz. x na siatce
		\param xmin - refer. do najmniejszej wartosc wspolz. x na siatce
		\param xmax - refer. do najwiekszej wartosc wspolz. y na siatce
		\param xmax - refer. do najmniejszej wartosc wspolz. y na siatce
	*/	
	void SetExtremeValues (double &xmax, double &xmin, double &ymax, double &ymin);
		
	//_____________________________________________________________________________________________________
	/*! Ustawia punkty graniczne siatki
		\param pxmax - refer. do punktu psosiadajacego najwieksza wartosc wspolz. x na siatce
		\param pxmin - refer. do punktu psosiadajacego najmniejsza wartosc wspolz. x na siatce
		\param pxmax - refer. do punktu psosiadajacego najwieksza wartosc wspolz. y na siatce
		\param pxmax - refer. do punktu psosiadajacego najmniejsza wartosc wspolz. y na siatce
	*/	
	void SetExtremePoints (Point &pxmax, Point &pxmin, Point &pymax, Point &pymin);
	
	//_____________________________________________________________________________________________________
	/*! Korzystajac z wyznaczony wartosci namniejszych i najwiekszych wspolrzednych na siatce,
		uzuplnia siatke punktami, ktore leza poza przeplywem nadajac im flage 0
		\param PointMap - referencja do Mapy Punktow
	*/
	void CompleteOutsideFlow (map<int, Point> &PointMap);
	
	//_____________________________________________________________________________________________________
	/*! Po oznaczeniu najblizszych sasiadow kazdego pktu 
		koryguje siatkę (korzysta z flag pkt onaczonych jako  przeplyw i poz przeplywem);
		Ustawia flagi 0 (poza przepływem) dla "dziwnych" punktow dla których punkty obok są poza przepływem
		(nie ma sensu liczyc w tych punktach - generowaloby to tylko bledy nyumeryczne): 
		\param PointMap - referencja do Mapy Punktow
	*/	
	int CorrectGrid (map<int, Point> &PointMap);
	
	
	//_____________________________________________________________________________________________________
	/*! Po oznaczeniu najblizszych sasiadow kazdego pktu i ewentulanie korekcji siatki 
		wyznacza brzeg obszaru przeplywu (korzysta z flag pkt onaczonych jako  przeplyw i poz przeplywem);
		Ustawia odpowiednie flagi punktow w zaleznosci od rodzaju brzegu do ktorego pktr nalezy: 
	*	<ul> 
	*		<li> brzeg gorny -> flaga <b>2</b></li>
	* 		<li> brzeg dolny -> flaga <b>3</b></li>
	*		<li> brzeg lewy -> flaga <b>4</b></li>
	*		<li> brzeg prawy -> flaga <b>5</b></li>
	*		<li> rog gorny-lewy -> flaga <b>24</b></li>
	*		<li> rog gorny-prawy -> flaga <b>25</b></li>
	*		<li> rog dolny-lewy -> flaga <b>34</b></li>
	*		<li> rog dolny-prawy -> flaga <b>35</b></li>
	*		<li> kanty -> flaga <b>1</b></li>
	*		<li> zrodla -> flaga <b>101</b></li>
	*	</ul>
		\param PointMap - referencja do Mapy Punktow
	*/	
	void MarkBorderPoints (map<int, Point> &PointMap);
	
	
	//_____________________________________________________________________________________________________
	/*! Po oznaczeniu dalszych sasiadow kazdego pktu,
		Ustawia odpowiednie flagi punktow lezacych przy brzegach;
	    w zaleznosci od rodzaju brzegu do ktorym lezy pkt : 
	*	<ul> 
	*		<li> przy brzegu gornym -> flaga <b>22</b></li>
	* 		<li> przy brzegu dolnym -> flaga <b>33</b></li>
	*		<li> przy brzegu lewym -> flaga <b>44</b></li>
	*		<li> przy brzegu prawym -> flaga <b>55</b></li>
	*		<li> przy rogu gornym-lewym -> flaga <b>2244</b></li>
	*		<li> przy rogu gornym-prawym -> flaga <b>2255</b></li>
	*		<li> przy rogu dolnym-lewym -> flaga <b>3344</b></li>
	*		<li> przy rogu dolnym-prawym -> flaga <b>3355</b></li>
	*	</ul>
		\param PointMap - referencja do Mapy Punktow
	*/	
	void MarkNearBorderPoints (map<int, Point> &PointMap);
	
	//_____________________________________________________________________________________________________
	/*! Ustawia odpowiednie flagi punktow lezacych na wejsciach i wyjsciach; 
		\param PointMap - referencja do Mapy Punktow
		
		<b>DO NAPISANIA </b>
		na razie prowizorycznie w konkretnym przypadku!!!
	*/
	int MarkInOutPoints (map<int, Point> &PointMap);
	
	
	/*! Ustawia pkt z maxymalana wartoscia wspolrzednej x */	
	void set_pxmax(Point pkt) {_pxmax = pkt;}; 
  
	/*! Ustawia pkt z maxymalana wartoscia wspolrzednej y */
	void set_pymax(Point pkt) {_pymax = pkt;}; 
	
	/*! Ustawia pkt z minimalan wartoscia wspolrzednej x */	
	void set_pxmin(Point pkt) {_pxmin = pkt;}; 
  
	/*! Ustawia pkt z minimalna wartoscia wspolrzednej y */
	void set_pymin(Point pkt) {_pymin = pkt;}; 
	
	//__________________________________________________________________
	
	/*! Zwaraca pkt majacy maksymalna wartosc wspolrzednej x na siatce*/
	Point get_pxmax() const {return _pxmax;};
  
	/*! Zwaraca pkt majacy maksymalna wartosc wspolrzednej y na siatce*/
	Point get_pymax() const {return _pymax;};
	
	/*! Zwaraca pkt majacy minimalna wartosc wspolzrednej x na siatce*/
	Point get_pxmin() const {return _pxmin;};
  
	/*! Zwaraca pkt majacy minimalna wartosc wpolrzednej y na siatce */
	Point get_pymin() const {return _pymin;};
	
	
	/*! Zwaraca maksymalna wartosc wspolrzednej x na siatce*/
	double get_xmax() const {return _xmax;};
  
	/*! Zwaraca maksymalna wartosc wspolrzednej y na siatce*/
	double get_ymax() const {return _ymax;};
	
	/*! Zwaraca minimalna wartosc wspolzrednej x na siatce*/
	double get_xmin() const {return _xmin;};
  
	/*! Zwaraca minimalna wartosc wpolrzednej y na siatce */
	double get_ymin() const {return _ymin;};
	
	const static int empty_index;// = Point::empty_index;
	static int _xmax, _xmin, _ymax, _ymin;
private:
	Point _pxmax, _pymax, _pxmin, _pymin; 
	//double _xmax, _xmin, _ymax;//, _ymin;
};


//*******************************************GridOnly******************************************************
//!  Klasa Przygotowujaca siatke do symulacji. 
/*!
  Zawiera funkcje potrzebne do przgotowania siatki, gdy z pliku wczytywane sa tylko
	wspolrzedne pktow przeplywu, a wartosci predkosci i wspolczynniki dyspersji sa stale na calej siatce.
*/
class GridOnly : public Grid {
public:
	
	//! Konstruktor.
    /*!
      Tworzy obiekt Grid Only
		*/
	GridOnly () {};
	
	int ReadData(map<int, Point> &PointMap, const string & file, SimulationParameters & sym_param);
	
private:
	map<int, Point> _PointMap;
	string _file;
};


//****************************************GridVelocity******************************************************
//!  Klasa Przygotowujaca siatke do symulacji. 
/*!
  Zawiera funkcje potrzebne do przgotowania siatki, gdy z pliku wczytywane sa 
	wspolrzedne pktow przeplywu i pole predkosci a wartosci wspolczynnikow dyspersji sa stale na calej siatce.
*/
class GridVelocity : public Grid {
public:
	//! Konstruktor.
    /*!
      Tworzy obiekt Grid Velocity
		*/
	GridVelocity () {};
	
	int ReadData(map<int, Point> &PointMap, const string & file, SimulationParameters & sym_param);
	
};


//*********************************************GridAll*****************************************************
//!  Klasa Przygotowujaca siatke do symulacji. 
/*!
  Zawiera funkcje potrzebne do przgotowania siatki, gdy z pliku wczytywane sa 
	wspolrzedne pktow przeplywu, pole predkosci i tensor dyspersji dla wszytskich pkt przeplywu.
*/
class GridAll : public Grid {
public:
	//! Konstruktor.
    /*!
      Tworzy obiekt Grid All
		*/
	GridAll () {};
	
	int ReadData(map<int, Point> &PointMap, const string & file, SimulationParameters & sym_param);

};


//****************************************GridDispoersion***************************************************
//!  Klasa Przygotowujaca siatke do symulacji. 
/*!
  Zawiera funkcje potrzebne do przgotowania siatki, gdy z pliku wczytywane sa 
	wspolrzedne pktow przeplywu i wartosci wspolczynnikow dyspersji, a skladowe predkosci sa stale na calej siatce.
*/
class GridDispersion : public Grid {
public:
	//! Konstruktor.
    /*!
      Tworzy obiekt Grid Dispersion
		*/
	GridDispersion () {};
	
	int ReadData(map<int, Point> &PointMap, const string & file, SimulationParameters & sym_param);

};

#endif
