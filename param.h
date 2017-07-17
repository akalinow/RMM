
/***************************************************************************
 *   param.h
 *   Copyright (C) 2005 by Monika Kalinowska                               *
 *   mkalinow@igf.edu.pl                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef PARAM_H
#define PARAM_H

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>

#include "tensor.h"
#include "scheme.h"

using namespace std;

//!  Klasa z parametrami symulacji. 
/*!
  Zawiera i pozwala modyfikowac informacje na temat parametrow symulacji.
*/

class SimulationParameters {
public:

	//! Konstruktor.
    /*!
      Tworzy obiekt simuationparameters i ustawaia parametry symulacji.
			\param param - nazwa pliku z parametrami symulacji
		*/
	SimulationParameters(string param);
//_________________________________________________________________________________
		
	
	//! Destruktor.
  /*! Usuwa obiekt Simulation.*/
	~SimulationParameters(void){};
	//____________________________________________________________________________________
	

	/*! Zwaraca flage z jaka zostala uruchomiona symulacja.
	*	\return 
	*	<ul> 
	*		<li> 0 - stale wartosci predkosci i wspolczynnikow dyspersji poprzecznej i podlyznej; siatka pkt w pkiku <i>grid.txt</i></li>
	* 		<li> 1 - stale wartosci wspolczynikow dysersji poprzecznej i podluznej; pole predkosci i siatka pkt w pliku v.txt </li>
	*		<li> 2 - pole predkosci, tensor dyspersji i siatka w pliku data.txt </li>
	*		<li> 3 - stale wartosci predkosci; siatka i tensor dyspersji w pliku d.txt </li>
	*	</ul>
	*/
	int get_flag() const {return _flag;};
	
	/*! Zwaraca liczbe krokow symulacji*/
	int get_n() const {return _n;};
	
	/*! Zwaraca ktok przestrzenny symulacji*/
	double get_dx() const {return _dx;};
  
	/*! Zwaraca ktok przestrzenny symulacji*/
	double get_dy() const {return _dy;};
	
	/*! Zwaraca ktok czasowy symulacji*/
	double get_dt() const {return _dt;};
  
	/*! Zwaraca maksymalny mozliwy krok czasowy symulacji symulacji*/
	//double get_dtmax() const {return _dtmax;};

		
	/*! Zwaraca zadana skaldowa x predkosci*/
	double get_vx() const {return _vx;};
  
	/*! Zwaraca zadana skaldowa y predkosci*/
	double get_vy() const {return _vy;};
	
	/*! Zwaraca maksymalna skaldowa x predkosci*/
	double get_vxmax() const {return _vxmax;};
  
	/*! Zwaraca maksymalna skaldowa y predkosci*/
	double get_vymax() const {return _vymax;};
	
	/*! Zwaraca minimalna skaldowa x predkosci*/
	double get_vxmin() const {return _vxmin;};
  
	/*! Zwaraca minimalna skaldowa y predkosci*/
	double get_vymin() const {return _vymin;};
	
	
	/*! Zwaraca wspolczynnik dyspersji podluznej*/
	double get_Dlong() const {return _Dlong;};
  
	/*! Zwaraca wspolczynnik dyspersji poprzecznej*/
	double get_Dtrans() const {return _Dtrans;};
	
	/*! Zwraca maksymalalna wartosc wspolczynnika dyspersji w kierunku x*/
	double get_Dxxmax() const {return _Dxxmax;};

	/*! Zwraca maksymalalna wartosc wspolczynnika dyspersji w kierunku y*/	
	double get_Dyymax() const {return _Dyymax;};

	/*! Zwraca maksymalalna wartosc wspolczynnika dyspersji w kierunku xy*/	
	double get_Dxymax() const {return _Dxymax;};

	/*! Zwraca minimalna wartosc wspolczynnika dyspersji w kierunku x*/
	double get_Dxxmin() const {return _Dxxmin;};

	/*! Zwraca  minimalna wartosc wspolczynnika dyspersji w kierunku y*/	
	double get_Dyymin() const {return _Dyymin;};

	/*! Zwraca  minimalna wartosc wspolczynnika dyspersji w kierunku xy*/	
	double get_Dxymin() const {return _Dxymin;};

	
	/*! Zwraca semat uzwany do symulacji */
	string get_scheme() const {return _scheme;};
	
	/*! Zwraca sposob obliczania dyspersji uzwany do symulacji */
	string get_dispersion() const {return _dispersion;};
	
	/*! Zwraca sposob nazwę pliku z danymi wejsciowymi - siatką */
	string get_gridFile() const {return _gridFile;};
	
	/*! Zwraca rodzaj zenieszcyszenia dla ktorego wykonywane są obliczenia 
	\see set_pollutant (string pollutant)
	*/
	string get_pollutant() const {return _pollutant;};
	
	/*! Zwraca sposób zrzutu zanieczyszczeń (ciągły lub chwilowy) 
	\see set_dump (string dump)
	*/
	string get_dump() const {return _dump;};

	/*! Zwraca definicje obszaru zrzutu zanieczyszczeń 
	\see set_area (string area)
	*/
	string get_area() const {return _area;};

	/*! Zwraca  spsosób w jaki podawana jest początkowa wartość temperatury/koncentracji w danym punkcie 
	\see set_mod (string mod)
	*/
	string get_mod() const {return _mod;};
	
	
	/*! Wskaznik do obiektu tensor, odpowiedzialnego za wlasciwe obilczania tensora dyspersji */
	DTensor *get_tensor() const {/*cout<<"biore tensor"<<endl;*/ return _tensor;};

	/*! Wskaznik do obiektu scheme, zawierajacego informacje o aktulnie wybranym schemacie numerycznym do obliczen */
	SScheme *get_s_scheme() const {/*cout<<"biore schemat"<<endl*/; return _s_scheme;};
	
	
	//____________________________________________________________________________________

	/*!ustawia flaga z jaka uruchomiona zostala symulacja
	*	\sa get_flag()
	*/
	void set_flag(int flag) {_flag = flag;};
	
	/*! Ustawia liczbe krokow symulacju*/
	void set_n(int n) {_n = n;};
	
	/*! Ustwia wielkosc kroku przestezennego wzdluz osi x*/
	void set_dx(double dx) {_dx = dx;};
  
	/*! Ustwia wielkosc kroku przestezennego wzdluz osi y*/
	void set_dy(double dy) {_dy = dy;};
	
	/*! Ustwia wartosc skladowej x predkosci */
	void set_vx(double vx) {_vx = vx;};
  
	/*! Ustwia wartosc skladowej y predkosci */
	void set_vy(double vy) {_vy = vy;};
	
	/*! Ustwia maksymalna wartosc skladowej x predkosci */
	void set_vxmax(double vxmax) {_vxmax = vxmax;};
  
	/*! Ustwia maksymalna wartosc skladowej y predkosci */
	void set_vymax(double vymax) {_vymax = vymax;};
	
	/*! Ustwia minimalna wartosc skladowej x predkosci */
	void set_vxmin(double vxmin) {_vxmin = vxmin;};
  
	/*! Ustwia minimalna wartosc skladowej y predkosci */
	void set_vymin(double vymin) {_vymin = vymin;};
	
	/*! Ustwia wartosc wspolczynnika dyspersji podluzenej */
	void set_Dlong(double Dlong) {_Dlong = Dlong;};
  
	/*! Ustwia wartosc wspolczynnika dyspersji poprzecznej */
	void set_Dtrans(double Dtrans) {_Dtrans = Dtrans;};
	
	/*! Ustwia wielkosc kroku czsowego symulacji */
	void set_dt(double dt) {_dt = dt;};
	
	/*! Ustawia maxymalna mozliwo wartosc kroku czasowego symulacji*/
	//void set_dtmax(double dtmax) {_dtmax = dtmax;};

	/*! Ustawia maksymalalna wartosc wspolczynnika dyspersji w kierunku x*/
	void set_Dxxmax(double Dxxmax) {_Dxxmax = Dxxmax;};

	/*! Ustawia maksymalalna wartosc wspolczynnika dyspersji w kierunku y*/	
	void set_Dyymax(double Dyymax) {_Dyymax = Dyymax;};

	/*! Ustwaia maksymalalna wartosc wspolczynnika dyspersji w kierunku xy*/	
	void set_Dxymax(double Dxymax) {_Dxymax = Dxymax;};

	/*! Ustawia minimalna wartosc wspolczynnika dyspersji w kierunku x*/
	void set_Dxxmin(double Dxxmin) {_Dxxmin = Dxxmin;};

	/*! Ustawia minimalna wartosc wspolczynnika dyspersji w kierunku y*/	
	void set_Dyymin(double Dyymin) {_Dyymin = Dyymin;};

	/*! Ustwaia minimalna wartosc wspolczynnika dyspersji w kierunku xy*/	
	void set_Dxymin(double Dxymin) {_Dxymin = Dxymin;};


	//______________________________________________________________________________
	/*! Ustawia schemat ktory ma byc uzywany do symulacji*/
	void set_scheme(string scheme);
	
	//_____________________________________________________________________________
	/*! Ustawia sposob obliczania dyspersji ktory ma byc uzywany do symulacji*/
	void set_dispersion(string dispersion);
	
	//_____________________________________________________________________________
	/*! Ustawia nazwe pliku wejsciowego zawierającego dane z siatką ktora ma byc uzywana do symulacji*/
	void set_gridFile(string gridFile) {_gridFile = gridFile;};
	
	//_____________________________________________________________________________
	/*! Ustawia rodzaj zenieszcyszenia dla ktorego wykonywane beda obliczenia 
		\param pollutant - rodzaj zanieczyszczenia
	*	<ul> 
	*		<li> conc - zanieczyszczenie chemiczne - sybstancja pasywana</li>
	* 		<li> temp - zanieczyszczenie teramiczne </li>
	*	</ul> 
	*/
	void set_pollutant (string pollutant) {_pollutant = pollutant;};

	//_____________________________________________________________________________
	/*! Ustawia sposób zrzutu zanieczyszczeń 
	*	\param dump - sposób zrzutu
	*		\li \c inst - zrzut chwilowy (tylko w chwili t = 0)
	* 		\li \c cont - zrzut ciągły (w każdym kroku czasowym)
	*/
	void set_dump (string dump) {_dump = dump;};
	

	//_____________________________________________________________________________
	/*! Definiuje obszar zrzutu zanieczyszczeń 
	*	\param area - obszar zrzutu
	*		\li \c point - w jednym zdefiniownaym pukcie, 
			\li \c points - w kilku zdefiniowanych punktach, 
			\li \c area - w zdefiniownym obszare
			\li \c gauss - rokład gauss-a wokół daneg punktu (diagonalny)
			\li \c gaussnondiag - rozkład gauss-a niediagonalny
	*/
	void set_area (string area) {_area = area;};

	//_____________________________________________________________________________
	/*! Definiuje spsosób w jaki podawana jest początkowa wartość temperatury/koncentracji w danym punkcie  
	*	\param mod - sposób definiowania wrtości temperatury/koncentracji
	*		\li \c val - wartosc nominalan ("oczekiwna") koncentracji/temperatury w danym punkcie 
				   -nie uwzglednia kroku przestrzennego siatki obliczeniowej i glebokosci w danym punkcie. 
				   Na jej podstawie i na podstawie danych o głebokosci w danym pukcie i wartosci natęzenia przepływu zanieczyszcznie wyznaczana jest wartosc efektywna koncentracji/temperatury
			\li \c val_e - wartosc efektywna koncentracji/temperatury w danym punkcie 
				(powinna uwzgledniac już krok przestrzenny siatki obliczenbiowej, glebokość w danym punkcie zrzutu oraz natezenie zrzutu i natężenie przepywu wody w danym puncie siatki) 
	*/
	void set_mod (string mod) {_mod = mod;};
	
	
	//_____________________________________________________________________________
	/*! Oblicza i zwraca Liczbe Pekleta*/
	double ComputePecletNumber(map<int, Point> &PointMap);
	
	//_____________________________________________________________________________
	/*! \brief Oblicza liczby Cournata
	 *
	 * Oblicza liczby Cournata dla ekstremalnych wartości zmiennych na siatce:  
	 *      \li CrAx - adwekcyjna liczba Couranta w kieruynku x
		\li CrAy - adwekcyjna liczba Couranta w kieruynku y
		\li CrDx - dyfuzyjna liczba Couranta w kieruynku x
		\li Crxy - dyfuzyjna liczba Couranta w kieruynku xy
		\li CrDy - dyfuzyjna liczba Couranta w kieruynku y 
	 * \return maksymalna z obliczonych liczb Couranta
	 */
	double ComputeCourantNumber();
	
	
	//_____________________________________________________________________________
	/*! Wyswietla informacje na temat parametrow symulacji*/
	void print() const;

	
private:
	int _flag, _n;
	double _dx, _dy, _dt, _vx, _vy, _Dlong, _Dtrans;//, _Dxx, _Dxy, _Dyy,
		//_Dlmax, _Dtmax, 
	double _Dxxmax, _Dyymax, _Dxymax, _vxmax, _vymax; //maxsmlane wartosci zmiennych na siatce
	double _Dxxmin, _Dyymin, _Dxymin, _vxmin, _vymin; //minimalne wartosci zmiennych na siatce
	string _param, _scheme, _dispersion, _gridFile;

	string _pollutant;// rodzaj zenieszcyszenia dra ktorego wykonywane beda obliczenia
			//"conc" - zanieczyszczenie chemiczne - sybstancja pasywana 
			 //"temp" - zanieczyszczenie teramiczne 
	string _dump; // sposób zrzutu zanieczyszczen - chwilowy - "inst",  lub ciągły - "cont"
	
	string _area; 	//obszar zrzutu: 
				//"point" - w jednym pukcie, 
				//"points" - w kilku zdefiniowanych punktach, 
				//"area" - na konkretnym obszare
				//"gaus" - rokład gausa wokół daneg punktu (diagonalny)
				//"gausnondiag" - rozkład gaussa niediagonalny

	string _mod; //czy podawana wartosc temperatury/concentracji jest okreslona w danym punkcie jako wartosc zakladana czy efektywana
			    //"val" - wartosc nominalan koncentracji/temperatury jaka chcemy miec w danym punkcie 
				//- nie uwzglednia kroku przestrzennego na siatce i glebokosci w danym punkcie siatki 
				// na jej podstawie i na podstawie danych o głebokosci w danym pukcie i wartosci natęzenia przepływu zanieczyszcznie
				// wyznaczana jest wartosc efektywna koncentracji/temperatury
			    //"val_e" - wartosc efektywna koncentracji/temperatury w danym punkcie (powinna uwzgledniac juz  juz krok przestrzenny, glebokosc i natezenie zrzutu) 
			    
	DTensor *_tensor; //wskanik do obiektu pod ktorym bedzie odpowiedni sposob liczenia tensora dyspersji
	DTensor _t_simple;
	DFischer _t_fischer;
	DVector _t_vector;
	DFischer2 _t_fischer2;
	
	SScheme * _s_scheme; //wskaznik do obiektu pod ktorym bedzie odpowiedni schemat
	SCrankNicolson _s_CN;
	SUpWind _s_UP;
	SADI _s_ADI;
	SADI2 _s_ADI2;
};
  
#endif
