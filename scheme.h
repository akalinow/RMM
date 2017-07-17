
/***************************************************************************
 *   scheme.h							   								   *
 *   Copyright (C) 2005 by Monika Kalinowska                               *
 *   mkalinow@igf.edu.pl                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef SCHEME_H
#define SCHEME_H

#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "point.h"

/*! \file scheme.h
 * \brief Zawiera definicje kals SScheme, SCrankNicolson, SUpWind, SADI, SADI2
 * 
 * Plik zawiera defincje klas SScheme - podstawowej  i pochodnych SCrankNicolson, SUpWind, SADI, SADI2
 * Obiekt jednej z klas pochodnych jest odpowiednikiem wybranego schematu ronicowego uzywanego do 
 * rozwiazania rownania trnsportu.
 * 
 * \author Monika Kalinowska
 * \date lipiec 2005
 * \version 1.00.02
*/
   
using namespace std;


//***********************************************SCHEME******************************************************
//!  Klasa podstawowa Schematow.
/*!
  Zawiera metody potrzebane do rozwiazania rownania transportu
	w zaleznosci od wybranego schematu numrycznego uzywna sa metody z tej lub jednej z klas pochodnych.
*/
class SScheme {
public:

	//! Konstruktor.
    /*!
      Tworzy schemat do rozwiazywania rownania transportu.
	*/
	SScheme(): _sym_step(0), _omega(1.0) { };
	
	
	//__________________________________________________________________________________________________________
	/*! 
		\brief Oblicza koncentracje.
		
		Oblicza koncentracje w daneym kroku uzywajac odpwoiedniej metody w zaeleznosci
		od wybranego schamtu do rozwiazywanie row transportu
		\param PointMap - mapa punktow
	*/
	void virtual ComputeConcentration (map <int, Point> & PointMap);

	//__________________________________________________________________________________________________________
	/*! 
		\brief Ustawia czynniki w rownaniu transportu dla calej siatki.
		
 		Ustawia czynniki (a, b, c, d, e, f, g, h, i) i (fa, fb, fc, fd, fe, ff, fg, fh, fi) w rownaniu transportu 
		dla wszytskich pktow
		
		\f[
    		a~c_{i,j}^{n+1} + b~c_{i+1,j}^{n+1} + c~c_{i-1,j}^{n+1} 
							+ d~c_{i,j+1}^{n+1} + e~c_{i,j-1}^{n+1}
			+ f~c_{i+1,j+1}^{n+1} + g~c_{i+1,j-1}^{n+1}	+ h~c_{i-1,j+1}^{n+1} + i~c_{i-1,j-1}^{n+1} 		
		\f]
		\f[
    		= fa~c_{i,j}^{n} + fb~c_{i+1,j}^{n} + fc~c_{i-1,j}^{n} 
							+ fd~c_{i,j+1}^{n} + fe~c_{i,j-1}^{n}
			+ ff~c_{i+1,j+1}^{n} + fg~c_{i+1,j-1}^{n} + fh~c_{i-1,j+1}^{n} + fi~c_{i-1,j-1}^{n} 	
  		\f]
	 
		\param PointMap - mapa punktow
	*/
	void SetFactors(map <int, Point> & PointMap);
	
	//__________________________________________________________________________________________________________
	/*! 
		\brief Odpowiedzialna za wyznaczenie czynnikow w row transportu dla danego pkt
		
		Odpowiedzilan za wyznaczenie czynnikow (a, b, c, d, e, f, g, h, i) i (fa, fb, fc, fd, fe, ff, fg, fh, fi)
		w rowenanie transportu dal zadanego pkt w ktorym obliczana jest koncentracja
		w zaleznosci od schematu jaki wykorzystywany jest do 
		rozwiazania rownania
    	\param *p - pkt dla ktorego wyznaczane sa czynniki
		\param CAx - adwekcyjna liczba Couranta w kieruynku x
		\param CAy - adwekcyjna liczba Couranta w kieruynku y
		\param CDx - dyfuzyjna liczba Couranta w kieruynku x
		\param CDxy - dyfuzyjna liczba Couranta w kieruynku xy
		\param CDy - dyfuzyjna liczba Couranta w kieruynku y		
	*/
	void virtual GetFactors(Point *p, const double &CAx, const double &CAy, const double &CDx, const double &CDxy, const double &CDy, const vector<int> &flag);
		
	//__________________________________________________________________________________________________________
	/*!
		\brief metoda iteracyjna Jakobiego
	
		Wyznacza koncentracje w danym punkcie rozwiazywanego ukladu rownan,
		uzywajac formuly iteracyjne Jakobiego do rozwiazywanai ukladow rownan linowych 	
		
	 	\param *p - wskaznik do pktu dla ktorego obliczana jest wartosc koncentracji
		\param PointMap - referencja do mapy punktow siatki
		\param fij - wartosc wyrazu wolnego odpowiedniego dla danego pkt
		\param a, b, c, d, e, f, g, h, i - wspolczynniki ukl rownnan odpowiednie dla danego pkt  
		\return koncentracje dla danego pkt 
	*/
	long double Jakobi (Point * p, map<int, Point> &PointMap, double fij,
 		double a, double b, double c, double d, double e, double f, double g, double h, double i);
	
	long double Jakobi (map<int, Point> &PointMap, map<int,long double> &f, vector<string> &a);
		
	//__________________________________________________________________________________________________________
	/*!
		\brief metoda iteracyjna Gaussa Seidela
	
		Wyznacza koncentracje w danym punkcie rozwiazywanego ukladu rownan,
		uzywajac formuly iteracyjne Gaussa Seidela do rozwiazywanai ukladow rownan linowych 	
		
	 	\param *p - wskaznik do pktu dla ktorego obliczana jest wartosc koncentracji
		\param PointMap - referencja do mapy punktow siatki
		\param fij - wartosc wyrazu wolnego odpowiedniego dla danego pkt
		\param a, b, c, d, e, f, g, h, i - wspolczynniki ukl rownnan odpowiednie dla danego pkt  
		\return koncentracje dla danego pkt 
	*/
	long double Gauss_Seidel(Point * p, map<int, Point> &PointMap, double fij,
 		double a, double b, double c, double d, double e, double f, double g, double h, double i);
	
	long double Gauss_Seidel(map<int, Point> &PointMap, map<int,long double> &f, vector<string> &a);	
				
	//__________________________________________________________________________________________________________
	/*!
		\brief metoda iteracyjna SOR
	
		Wyznacza koncentracje w danym punkcie rozwiazywanego ukladu rownan,
		uzywajac formuly iteracyjne SOR (Saccessive Over Relaksation) do rozwiazywanai ukladow rownan linowych 	
		
	 	\param *p - wskaznik do pktu dla ktorego obliczana jest wartosc koncentracji
		\param PointMap - referencja do mapy punktow siatki
		\param fij - wartosc wyrazu wolnego odpowiedniego dla danego pkt
		\param a, b, c, d, e, f, g, h, i - wspolczynniki ukl rownnan odpowiednie dla danego pkt  
		\param omega - parametr relaksacji
		\return koncentracje dla danego pkt 
	*/
	long double SOR (Point * p, map<int, Point> &PointMap, double fij,
 		double a, double b, double c, double d, double e, double f, double g, double h, double i, long double omega);
		
	long double SOR (map<int, Point> &PointMap, map<int,long double> &f, vector<string> &a, long double omega = 1.0);	

	/*! ustawia wartosc rowna aktualnemu krokowi symulacji (potrzebny do SORa)*/	
	void set_sym_step(int step) {_sym_step = step;}; 
	
	/*! zwraca wrtosc rowna aktualnemu krokowi symulacji (potrzebny do SORa)*/	
	int get_sym_step() const {return _sym_step;};
	
	/*! ustawia wartosc rowna aktualnemu krokowi symulacji (potrzebny do SORa)*/	
	void set_omega(long double omega) {_omega = omega;}; 
	
	/*! zwraca wrtosc rowna aktualnemu krokowi symulacji (potrzebny do SORa)*/	
	long double get_omega() const {return _omega;};
	
	/*! ustawia metode ktora ma byc uzyta do rozwiazywania ukladow rownan*/	
	void set_method(string method) {_method = method;}; 
	
	/*! zwraca wrtosc metody ktora ma byc uzyta do rozwiazywania ukladow rownan*/	
	string get_method() const {return _method;};
	
	
	float FindBestOmega (map<int, Point> &PointMap, map<int,long double> &f);
		
	void ReadMethod (string scheme);
	
	vector<int> _i;
	map<int,double> _w;
	int powrot; 
private:
	
	int _sym_step;
	long double _omega;
	string _method;
};


//***********************************************CRANK NICOLSON*****************************************************
//!  Schemat Cranka Nicolsona.
/*!
  Zawiera metody potrzebane do rozwiazania rownania transportu metoda Crnka Nicolsona.
*/
class SCrankNicolson : public SScheme{

public:

	//! Konstruktor.
    /*!
      Tworzy schemat do rozwiazywania rownania transporu metoda Cranka Nicolsona
	*/
	SCrankNicolson(){};
	
	//__________________________________________________________________________________________________________
	/*! 
		\brief Oblicza koncentracje uzywajac schamatu Cranka Nicolsona.
		
		Oblicza koncentracje w daneym kroku uzywajac metody Cranka Nicolsona
		\param PointMap - mapa punktow
	*/
	void ComputeConcentration (map <int, Point> & PointMap);

	//__________________________________________________________________________________________________________
	/*! 
		Odpowiedzilan za wyznaczenie czynnikow (a, b, c, d, e, f, g, h, i) i (fa, fb, fc, fd, fe, ff, fg, fh, fi)
		w rowenanie transportu dal zadanego pkt w ktorym obliczana jest koncentracja
		dla schemtau Cranka Nicolsona	
		
    	\param *p - pkt dla ktorego wyznaczane sa czynniki
		\param CAx - adwekcyjna liczba Couranta w kieruynku x
		\param CAy - adwekcyjna liczba Couranta w kieruynku y
		\param CDx - dyfuzyjna liczba Couranta w kieruynku x
		\param CDxy - dyfuzyjna liczba Couranta w kieruynku xy
		\param CDy - dyfuzyjna liczba Couranta w kieruynku y		
	*/		
	void GetFactors(Point *p, const double &CAx, const double &CAy, const double &CDx, const double &CDxy, const double &CDy, const vector<int> &flag);
	
private:

	void IterativeSolveSetOfEquations (map <int, Point> & PointMap, map<int,long double> &f);

	
};


//***********************************************UPWIND*****************************************************
//!  Schemat Pod Prad (UpWind).
/*!
  Zawiera metody potrzebane do rozwiazania rownania transportu metoda Pod Prad.
*/
class SUpWind : public SScheme{

public:

	//! Konstruktor.
    /*!
      Tworzy schemat do rozwiazywania rownania transporu metoda UpWind
	*/
	SUpWind() {};

	
	//__________________________________________________________________________________________________________
	/*! 
		\brief Oblicza koncentracje uzywajac schamatu UpWind
		
		Oblicza koncentracje w daneym kroku uzywajac metody UpWind
		\param PointMap - mapa punktow
	*/	
	void ComputeConcentration (map <int, Point> & PointMap);
	
	//__________________________________________________________________________________________________________
	/*! 
		Odpowiedzilan za wyznaczenie czynnikow (a, b, c, d, e, f, g, h, i) i (fa, fb, fc, fd, fe, ff, fg, fh, fi)
		w rowenanie transportu dal zadanego pkt w ktorym obliczana jest koncentracja
		dla schemtau UpWind	
		
    	\param *p - pkt dla ktorego wyznaczane sa czynniki
		\param CAx - adwekcyjna liczba Couranta w kieruynku x
		\param CAy - adwekcyjna liczba Couranta w kieruynku y
		\param CDx - dyfuzyjna liczba Couranta w kieruynku x
		\param CDxy - dyfuzyjna liczba Couranta w kieruynku xy
		\param CDy - dyfuzyjna liczba Couranta w kieruynku y		
	*/
	void GetFactors(Point *p, const double &CAx, const double &CAy, const double &CDx, const double &CDxy, const double &CDy, const vector<int> &flag);

};


//***********************************************ADI*****************************************************
//!  Schemat naprzemiennych kierunkow ADI .
/*!
  Zawiera metody potrzebane do rozwiazania rownania transportu metoda ADI.
*/
class SADI : public SScheme{

public:

	//! Konstruktor.
    /*!
      Tworzy schemat do rozwiazywania rownania transporu metoda ADI
	*/
	SADI() {};

	
	//__________________________________________________________________________________________________________
	/*! 
		\brief Oblicza koncentracje uzywajac schamatu ADI.
		
		Oblicza koncentracje w daneym kroku uzywajac metody ADI
		\param PointMap - mapa punktow
	*/
	void ComputeConcentration (map <int, Point> & PointMap);

	//__________________________________________________________________________________________________________
	/*! 
		Odpowiedzilan za wyznaczenie czynnikow (a, b, c, d, e, f, g, h, i) i (fa, fb, fc, fd, fe, ff, fg, fh, fi)
		w rowenanie transportu dal zadanego pkt w ktorym obliczana jest koncentracja
		dla schemtu ADI
		
    	\param *p - pkt dla ktorego wyznaczane sa czynniki
		\param CAx - adwekcyjna liczba Couranta w kieruynku x
		\param CAy - adwekcyjna liczba Couranta w kieruynku y
		\param CDx - dyfuzyjna liczba Couranta w kieruynku x
		\param CDxy - dyfuzyjna liczba Couranta w kieruynku xy
		\param CDy - dyfuzyjna liczba Couranta w kieruynku y		
	*/	
	void GetFactors(Point *p,  const double &CAx, const double &CAy, const double &CDx, const double &CDxy, const double &CDy, const vector<int> &flag);

private:
	
	void IterativeSolveSetOfEquations (map <int, Point> & PointMap, map<int,long double> &f, vector<string> &a);
	void DirectSolveSetOfEquation1 (map <int, Point> & PointMap, map<int,long double> &f);
	void DirectSolveSetOfEquation2 (map <int, Point> & PointMap, map<int,long double> &f1);
	
};

//***********************************************ADI*****************************************************
//!  Schemat naprzemiennych kierunkow ADI .
/*!
  Zawiera metody potrzebane do rozwiazania rownania transportu metoda ADI.
*/
class SADI2 : public SScheme{

public:

	//! Konstruktor.
    /*!
      Tworzy schemat do rozwiazywania rownania transporu metoda ADI
	*/
	SADI2() { };

	
	//__________________________________________________________________________________________________________
	/*! 
		\brief Oblicza koncentracje uzywajac schamatu ADI2.
		
		Oblicza koncentracje w daneym kroku uzywajac metody ADI2 
		(rozni sie tym od ADI1 ze ma inaczej jest podzielona na dwa "pdkroki" czasowe)
		\param PointMap - mapa punktow
	*/
	void ComputeConcentration (map <int, Point> & PointMap);
	
	//__________________________________________________________________________________________________________
	/*! 
		Odpowiedzilan za wyznaczenie czynnikow (a, b, c, d, e, f, g, h, i) i (fa, fb, fc, fd, fe, ff, fg, fh, fi)
		w rowenanie transportu dal zadanego pkt w ktorym obliczana jest koncentracja
		dla schemtu ADI2
		
    	\param *p - pkt dla ktorego wyznaczane sa czynniki
		\param CAx - adwekcyjna liczba Couranta w kieruynku x
		\param CAy - adwekcyjna liczba Couranta w kieruynku y
		\param CDx - dyfuzyjna liczba Couranta w kieruynku x
		\param CDxy - dyfuzyjna liczba Couranta w kieruynku xy
		\param CDy - dyfuzyjna liczba Couranta w kieruynku y		
	*/
	void GetFactors(Point *p,  const double &CAx, const double &CAy, const double &CDx, const double &CDxy, const double &CDy, const vector<int> &flag);

};

#endif
