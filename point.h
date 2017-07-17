/***************************************************************************
 *   point.h							   *
 *   Copyright (C) 2005 by Monika Kalinowska                               *
 *   mkalinow@igf.edu.pl                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef PUNKT_H
#define PUNKT_H
//#define MY_DEBUG

//C++ headers
#include <cstdlib>
#include <fstream>
#include <string>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

#include <cmath>
#include <vector>
#include <map>

//#include "grid.h"

using namespace std;

//***********************************************DTensor******************************************************
//!  Klasa opisujaca Punkt
/*!
  Zawiera wszystkie informacje na temat konkretnego Punktu siatki, metody do ich ustawiaenia i dobrania sie do nich 
*/
class Point {

public:
 
	//! Konstruktor.
    /*!
		Tworzy Punkt
		\param - nazwa pliku z parametrami symulacji
	*/
	Point(double x, double y, int type, int type1):
		_x(x), _y(y), _vx(0), _vy(0), _Dxx(0), _Dyy(0), _Dxy(0),
		_type(type), _type1(type1),
		_valCurr(0), _valPrev(0), _valNext(0), _flag(0),
		_CurAx(0), _CurAy(0), _CurDx(0), _CurDxy(0), _CurDy(0) { //cout<<"Tworze pkt 1"<<endl;
		this->set_near_neighbours(0);};

	Point(double x, double y, int flag):
		_x(x), _y(y), _flag(flag), _vx(0), _vy(0), _Dxx(0), _Dyy(0), _Dxy(0),
		_type(0), _type1(0),
		_valCurr(0), _valPrev(0), _valNext(0),
		_CurAx(0), _CurAy(0), _CurDx(0), _CurDxy(0), _CurDy(0) { //cout<<"Tworze pkt 2"<<endl; 
		this->set_near_neighbours(0);};

	Point(double x, double y, double vx, double vy, int flag):
		_x(x), _y(y), _flag(flag), _vx(vx), _vy(vy), _Dxx(0), _Dyy(0), _Dxy(0),
		_type(0), _type1(0),
		_valCurr(0), _valPrev(0), _valNext(0),
		_CurAx(0), _CurAy(0), _CurDx(0), _CurDxy(0), _CurDy(0) { //cout<<"Tworze pkt 3"<<endl; 
		this->set_near_neighbours(0);};

	Point(double x, double y, double vx, double vy, double Dxx, double Dyy, double Dxy, int flag):
		_x(x), _y(y), _flag(flag), _vx(vx), _vy(vy), _Dxx(Dxx), _Dyy(Dyy), _Dxy(Dxy),
		_type(0), _type1(0),
		_valCurr(0), _valPrev(0), _valNext(0),
		_CurAx(0), _CurAy(0), _CurDx(0), _CurDxy(0), _CurDy(0) { //cout<<"Tworze pkt 4"<<endl; 
		this->set_near_neighbours(0);};
	
	Point(double x, double y, double Dxx, double Dyy, double Dxy, int flag):
		_x(x), _y(y), _flag(flag), _vx(0), _vy(0), _Dxx(Dxx), _Dyy(Dyy), _Dxy(Dxy),
		_type(0), _type1(0),
		_valCurr(0), _valPrev(0), _valNext(0),
		_CurAx(0), _CurAy(0), _CurDx(0), _CurDxy(0), _CurDy(0) { //cout<<"Tworze pkt 5"<<endl; 
		this->set_near_neighbours(0);};	
				
	Point(): _x(-9999), _y(-9999), _vx(0), _vy(0),
		_Dxx(0), _Dyy(0), _Dxy(0),
		_type(0), _type1(0), _valCurr(0),
		_valPrev(0), _valNext(0), _flag(-9999) { //cout<<"Tworze pkt 6"<<endl; 
		this->set_near_neighbours(0); };

	//___________________________________________________________________________________________________
	/*! zwraca flage pkt*/
	int get_flag() const {return _flag;};

	/*! zwraca wspolrzednza x pkt */
	double get_x() const {return _x;}; 
	
	/*! zwraca wspolrzednza y pkt */
  	double get_y() const {return _y;};
	
	/*! zwraca wartosc nastepna */
  	long double get_Next() const {return _valNext;};
	
	/*! zwraca wartosc obecna */
  	long double get_Curr() const {return _valCurr;};
	
	/*! zwraca wartosc poprzednia  */
  	long double get_Prev() const {return _valPrev;};
	
	/*! zwraca wartosc wspolrzednej Vx predkosci w danym pkt */
  	double get_vx() const {return _vx;};
  
	/*!  zwraca wartosc wspolrzednej Vy predkosci w danym pkt */	
	double get_vy() const {return _vy;}; 
  
	/*! zwraca wartosc wspolczynnika tensors dyspersji Dxx w danym pkt */
	double get_Dxx() const {return _Dxx;}; 
  
	/*!  zwraca wartosc wspolczynnika tensora dyspersji Dxy w danym pkt */	
	double get_Dxy() const {return _Dxy;};
	
	/*! zwraca wartosc wspolczynnika dyspersji Dy w danym pkt */	
	double get_Dyy() const {return _Dyy;};

	/*! zwaraca wartosc liczby Couranta dla danego pkt */
	double get_CurAx() const {return _CurAx;};
	double get_CurAy() const {return _CurAy;};
	double get_CurDx() const {return _CurDx;};
	double get_CurDxy() const {return _CurDxy;};
	double get_CurDy() const {return _CurDy;};
	
	/*! zwaraca wartosc liczb Peclet-a dla danego pkt */
	/*double get_Pex() const {return _Pex;};
	double get_Pey() const {return _Pey;};
	double get_Pexy() const {return _Pexy;};
	double get_PeRes() const {return _PeRes;};
	*/

	double get_factor_f(string f) const {return _factors_fij.find(f)->second;};
	double get_factor(string f) const {return _factors.find(f)->second;};
			
	//_________________________________________________________________________________________________________
	
	//____________________________________
	/*! zwraca mape do ktorej pkt nalezy */
  	map<int, Point> * get_map() const {return _PointMap;}; // tutaj pkt naleza (musza) tylko do jednej mapy
  
	/*! zwraca index odpowieniengo sasiada z najblizszych sasiadow */
	int get_near_neighbour(int nr) { return _near_neighbours[nr]; };
	
	/*! zwraca index odpowieniengo sasiada z dalszych sasiadow */
	int get_distant_neighbours(int number) {return _distant_neighbours[number];};
	
	
	//__________________________________________________________________________________________________________
	/*! ustawia flage pkt*/	
	void set_flag(int f) {_flag = f;}; 
  
	/*!  ustwaia wspolrzedna x pkt.*/	
	void set_x(double x) {_x = x;}; 
  
	/*! ustwaia wspolrzedna y pkt */	
	void set_y(double y) {_y = y;}; 

	/*! ustawia wartosc skladowej x predkosci*/  
	void set_vx(double vX) {_vx = vX;}; 
  
	/*! ustawia wartosc skaldowej y predkosci*/
	void set_vy(double vY) {_vy = vY;};
  
	/*! ustwaia wartosc wspolczynnika tensora dyspersji Dxx w danym pkt */
	void set_Dxx(double Dx) {_Dxx = Dx;};
  
	/*! ustwaia wartosc wspolczynnika tensora dyspersji Dxx w danym pkt*/
	void set_Dxy(double Dxy) {_Dxy = Dxy;}; 
	
	/*! ustwaia wartosc wspolczynnika tensora dyspersji Dyy w danym pkt*/	
	void set_Dyy(double Dy) {_Dyy = Dy;};
  
	/*! ustwaia wartosc poprzednia dla pkt*/	
	void set_Prev(long double val) {_valPrev = val;};
  
	/*! ustwaia wartosc w pkt*/	
	void set_Curr(long double val) {_valCurr = val;};
 
	/*! ustwaia wartosc nastepna dla pkt*/
	void set_Next(long double val) {_valNext = val;};

	void set_factor_fij(string f, double val) {_factors_fij[f] = val;};
	
	void set_factor(string f, double val) {_factors[f] = val;};
	
	/*! ustawia mape dla pkt*/	
	void set_map(map<int, Point> * Map) {_PointMap = Map;}; 

	//_____________________________________________________________________________________________________________
	/*! Ustawia najblizszych sasiadow danego pkt */
	void set_near_neighbours(int opt = 1);
	
	//_____________________________________________________________________________________________________________
	/*! Ustawia dalszych sasiadow danego pkt */
	void set_distant_neighbours (int opt = 1);
	
	//_____________________________________________________________________________________________________________
	/*! Oblicza liczby adwekcyjne i dyfuzyjne Couranta  dla danego pkt*/
	void set_Courant_numbers (double dx, double dy, double dt);
	
	//_____________________________________________________________________________________________________________
	/*! Oblicza liczby Pecleta  dla danego pkt*/
	//void set_Peclet_numbers (double dx, double dy, double dt);
	
  	//_____________________________________________________________________________________________________________
	/*! wypisuje na ekranie informacje o danym pkt */
  	void print() const;

	//_____________________________________________________________________________________________________________
  	/*! wypisuje informacje o danym pkt */
  	friend ostream& operator<<(ostream& out, const Point& obj){
    	out<<"Flaga: "<<obj.get_flag()<<"  x= "<<obj.get_x()<<" y= "<<obj.get_y()
       		<<" valPrev = "<<obj.get_Prev()<<" valCurr = "<<obj.get_Curr()
       		<<" vx = "<<obj.get_vx()<<" vy = "<<obj.get_vy();
    	return out;
  	};
	
	//_____________________________________________________________________________________________________________
  	/*! porownuje inny pkt z aktualnym */
  	bool operator< (const Point& obj) const {
   		if(this->get_x()<obj.get_x()) return true;
   		if(this->get_y()<obj.get_y()) return true;
   		return false;
  	};
	
	//_____________________________________________________________________________________________________________
	/// tylko jedna mapa moze byc
	static map<int, Point>  * _PointMap; 
 //static map<int, Point>  * Point::_PointMap = 0;
 
 //	const static int empty_index;// = -99999999;

	 
private:

	double _x, _y; //wspolrzedne pkt
	double _vx, _vy; // wartosci skaldowych predkosci
	double _Dxx, _Dyy, _Dxy; // wartosci wspolczynnikow dyspersji
	int _type, _type1, _flag; // okresla ktory bzreg - lewy, prawy
                    // okresla ktory kawalek bzregu
	long double _valCurr, _valPrev, _valNext; // wartosci obliczanych funkcji w roznych krokach

	double _CurAx, _CurAy; //Adwekcyjne liczby kuranta
	double _CurDx, _CurDxy, _CurDy; //Dyfuzyjne liczby kuranta
	//double _Pex, _Pey, _Pexy, _PeRes; //Liczby Pecleta (Pex= vx*dx/Dxx, Pey= vy*dy/Dyy, 
					  //Pxy= (sqrt(pow((vx/dx),2) + pow((vy/dy),2))) / ((_Dxx/pow(dx,2)) + (Dyy/pow(dy,2)) - (Dxy/(dx*dy)))
					 
					  
	vector<Point *> _near_neighboursPtr;
	vector<int> _near_neighbours;	// indeksy najblizszych sasiadow danego pktu :
  									//	* * * * *
  									//	* 0 1 2 *		0->(i-1,j+1)	1->(i,j+1)	2->(i+1,j+1)
  									//	* 3 4 5 *		3->(i-1,j)		4->(i,j)	5->(i+1,j) 
									//	* 6 7 8 *		6->(i-1,j-1)	7->(i,j-1)	8->(i+1,j-1)
									//	* * * * *		
			
	vector<Point *> _distant_neighboursPtr; 						
	vector<int> _distant_neighbours; 	// indexy dalszych sasiadow potrzebne np w schemacie upwind o dokladnosci 2
  										//	* * 1 * * 		1->(i,j+2)
  										//	* * * * *
  										//	4 * 0 * 2		4->(i-2,j)		0->(i,j)	2->(i+2,j) 
										//	* * * * *
  										//	* * 3 * *		3->(i,j-2)
  										//
	map <string, double> _factors; // wektor wspolczynnikow do obliczania koncentracji; inne dla roznych schematow
							// kolejnosc: a (przy i,j), b (i+1,j), c (i-1,j), d (i,j+1), e (i,j-1),
							// f (i+1,j+1), g (i+1,j-1), h (i-1,j+1), i (i-1,j-1),
							// j (i+2,j), k (i-2,j), l (i,j+2), m (i,j-2);
	map <string, double> _factors_fij; // wektor wspolczynikow do obliczania
};

#endif
