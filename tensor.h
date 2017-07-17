
/***************************************************************************
 *   tensor.h							   *
 *   Copyright (C) 2005 by Monika Kalinowska                               *
 *   mkalinow@igf.edu.pl                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef TENSOR_H
#define TENSOR_H

#include <cstdlib>
#include <string>
#include <iostream>

#include "point.h"

using namespace std;

//***********************************************DTensor******************************************************
//!  Klasa podstawowa tensora dyspersji. 
/*!
  Zawiera metody potrzebane do wyznaczenia wratosci wspolczynnikow tensora dyspersji,
	w zaleznosci od wybranego sposobu obliczen jest uzywna ona lub jedna z klas pochodnych.
*/
class DTensor {
public:

	//! Konstruktor.
    /*!
      Tworzy obiekt Tensora Dyspersji
	*/
	DTensor() {/*cout<<"Dtensor"<<endl;*/};
	
	void test(double Dl, double Dt, Point &p) {cout<<"test"<<endl;};

	/*! Oblicza wspolczyniki tensora Dyspersji uzywjac odpowieniej metody w zaleznosci od opcji podanej w pliku wejsciowym 
		\param Dl - wspolczyniki dyspersji podluznej
		\param Dt - wspolczynnik dyspersji porzecznej
		\param p - punkt dla ktorego obiczany jest tensor
	*/
	void virtual ComputeTensor (double Dl, double Dt, Point &p);
};


//***********************************************DFischer*****************************************************
//!  Klasa tensora dyspersji. 
/*!
  Zawiera metody potrzebane do obliczenia wratosci wspolczynnikow tensora dyspersji, 
	wg Fischer et al 1979 "Mixing in inland and coastal waters".
*/
class DFischer : public DTensor{
public:

	//! Konstruktor.
    /*!
      Tworzy obiekt Tensora Dyspersji wyznaczanego metoda Fischera
	*/
	DFischer() {/*cout<<"DFischer"<<endl;*/};
	
	void ComputeTensor (double Dl, double Dt, Point &p);
};

//***********************************************DFischer2*****************************************************
//!  Klasa tensora dyspersji. 
/*!
  Zawiera metody potrzebane do obliczenia wratosci wspolczynnikow tensora dyspersji, 
	wg Fischer et al 1979 "Mixing in inland and coastal waters", ale zaniedbujaca pozadiagonalne wspolczynniki.
*/
class DFischer2 : public DTensor{
public:

	//! Konstruktor.
    /*!
      Tworzy obiekt Tensora Dyspersji wyznaczanego metoda Fischera z zaniedbaniem wspolczynnikow pozadiagonalnych
	*/
	DFischer2() {/*cout<<"DFischer2"<<endl;*/};
	
	void ComputeTensor (double Dl, double Dt, Point &p);
};


//***********************************************DVector*****************************************************
//!  Klasa tensora dyspersji. 
/*!
  Zawiera metody potrzebane do obliczenia wratosci wspolczynnikow tensora dyspersji, 
	traktujac tensor jako wektor w ukaladzie wspolrzednych [Dl,Dt] i obracajac go do wektora [Dxx, Dyy].
*/
class DVector : public DTensor{
public:

	//! Konstruktor.
    /*!
      Tworzy obiekt Tensora Dyspersji traktowanego jako wektor
	*/
	DVector() {/*cout<<"DVector"<<endl;*/};
	
	void ComputeTensor (double Dl, double Dt, Point &p);
};


#endif
