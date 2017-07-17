/*
 * ***************************************************************************		
 *  *   Copyright (C) by Monika Kalinowska                               *
 *  *   mkalinow@igf.edu.pl                                                   *
 *  *                                                                         *
 *  ***************************************************************************
 */

#ifndef NETHEATFLUX_H
#define NETHEATFLUX_H

#include <math.h>  
#include <iostream>

using namespace std;

//***********************************************NetHeatFlux******************************************************
//!  Klasa do obliczen członu źródłowego wyminay ciepla z atmosfera
/*!
  Zawiera metody potrzebane do wyznaczenia wratosci wypadkowego strumienia ciepła na granicy woda powietrze
*/

class NetHeatFlux
{
public:
  
   //! Konstruktor.
    /*!
      Tworzy obiekt NetHeatFlux
    */
    NetHeatFlux() {
        /*cout<<"NetHeatFlux"<<endl;*/
    };
    
    NetHeatFlux(double Ta, double Rh, double pa, double Js, double u):_Ta(Ta), _Rh(Rh), _pa(pa), _Js(Js), _u(u) {
      this->set_aw();
      this->set_ea();
      this->set_emis_w();
      this->set_cb();
    };

    
    //__________________________________________________________________________________________________________
    /*!
    	\brief oblicza wartosc czlonu zródłowego

    	Oblicza wartosc czlonu zrodlowego dla danej temp wody 
    	na podstawie podsnaych danych meteorologicznych 

     	\param Tw - wartość temperatury wody dla ktorej ma być obliczony czlon zrodlowy
     	\return wartosc czlonu zrodlowego dla danej temp
    */
    long double ComputeNetHeatFlux (long double Tw);
    

    //__________________________________________________________________________________________________________
    /*!
    	\brief oblicza wartosc promieniowania słonecznego krotkofalowe

    	Oblicza wartosc promieniowania słonecznego krotkofalowego 
    	na podstawie pomiarów promieniowania słonecznego padającego  oraz albedo wody 

     	\return wartosc promieniowania słonecznego krotkofalowe
    */
    long double ComputeNetSolar ();

    //__________________________________________________________________________________________________________
    /*!
    	\brief oblicza wartosc promieniowania atmosferycznego długofalowego

     	\param  - 
     	\return wartosc promieniowania atmosferycznego długofalowego
     */
    long double ComputeLongWaveAtmosf ();
    
   //__________________________________________________________________________________________________________
    /*!
    	\brief oblicza wartosc emisyjnosci atmosfery wg formuły podanej przez Brunt (1932)
    	\param a1 - coefficient []
    	\param a2 - coefficient [hPa^(-1/2)]
     	\return wartosc emisyjnosci atmosfery
    */    
    long double Emissivity_Brunt1932 (double a1 = 0.55, double a2 = 0.065);
    
    //__________________________________________________________________________________________________________
    /*!
    	\brief oblicza wartosc promieniowania długofalowego WODY

    	Oblicza wartosc promieniowania długofalowego WODY 
    	kożystając z prawa Stefana-Boltzmana 
	\param Tw - temp wody
     	\return wartosc promieniowania długofalowego wody
     */
    long double ComputeLongWaveWater (long double &Tw);

    
    //__________________________________________________________________________________________________________
    /*!
    	\brief oblicza wartosc strat ciepła zwiazanego z praowaniem

    	Oblicza wartosc strat ciepła zwiazanego z parowaniem 
    	lub dostarcenia ciepła związnego z procesem kondensacji
	\param Tw - temp wody
     	\return wartosc strumienia energii w wyniku parowania/kondenscji pary wodnej
     */
    long double ComputeEvapCondens (long double &Tw);
    
        //__________________________________________________________________________________________________________
    /*!
    	\brief oblicza wartosc funkcji zależnej od predkosci waitru
	oblicza wartosc funkcji zależnej od predkosci waitru 
	wg zależnosci f(u) = c1 + c2 u^2 - domyślnie c1 = 19.0, c2 = 0.95 - Chapra 2008, Edinger et al 1974
     	\return wartosc strumienia energii w wyniku parowania/kondenscji pary wodnej
     */
    long double WindFunction_Chapra2008 (float c1 = 19.0, float c2 = 0.95);
    
    //__________________________________________________________________________________________________________
    /*!
    	\brief oblicza wartosc strumienia  ciepła w wyniku przewodzenia i konwekscji

    	Oblicza wartosc strumienia  ciepła w wyniku przewodzenia i konwekscji
	\param Tw - temp wody
     	\return wartosc strumienia  ciepła w wyniku przewodzenia i konwekscji
     */
    long double ComputeCondConv(long double &Tw);
    
    //___________________________________________________________________________________________________
    /*! zwraca wartość temp powietrza*/
    double get_Ta() const {
        return _Ta;
    };

    /*! zwraca wartość wilgotnosci powietrza */
    double get_Rh() const {
        return _Rh;
    };

    /*! zwraca wartość ciśnienie atmosferycznego */
    double get_pa() const {
        return _pa;
    };

    /*! zwraca wartosc promieniowania krotkofalowego */
    double get_Js() const {
        return _Js;
    };

    /*! zwraca wartosc predkosci wiatru */
    double get_u() const {
        return _u;
    };

    /*! zwraca wartosc albedo wody */
    double get_aw() const {
        return _aw;
    };
     
    /*! zwraca wartosc aktualnej prężności (ciśnienia) pary wodnej w powietrzu */
    double get_ea() const {
        return _ea;
    };

    /*! zwraca wartosc ustawionej zdolności emisyjnej wody */
    double get_emis_w() const {
        return _emis_w;
    };


    //___________________________________________________________________________________________________
    /*! ustawia wartość temp powietrza*/
    void set_Ta(double Ta) {
        _Ta = Ta;
    };

    /*! ustawia wartość wilgotnosci powietrza */
    void set_Rh(double Rh) {
        _Rh = Rh;
    };
//_____________________________________________________________________________________________________________

    /*! ustawia wartość ciśnienie atmosferycznego */
    void set_pa(double pa) {
        _pa = pa;
    };

    /*! ustawia wartosc promieniowania krotkofalowego */
    void set_Js(double Js) {
       _Js = Js;
    };

    /*! ustawia wartosc predkosci wiatru */
    void set_u(double u) {
        _u = u;
    };

    /*! ustawia wartosc albedo dla wody */
    void set_aw(double aw = 0.06) {
        _aw = aw;
    };
    
    /*! oblicza i ustawia wartosc aktualnej prężności (ciśnienia) pary wodnej w powietrzu */
    void set_ea();

    /*! oblicza wartosc  prężności (ciśnienia) pary wodnej 
     (defoltowo wg formuly  Raudkivi 1979, Chapra 1997: 6.12 exp(17.27*Ta/(Ta+237.3)) */
    long double e_sat(long double T, double b1 = 6.12, double b2 = 17.27, double b3 = 237.3);
   
    /*! ustawia wartosc dla zdolności emisyjnej wody */
    void set_emis_w(double emis_w = 0.97) {
        _emis_w = emis_w;
    };
    
    /*! ustawia wartosc wspolczynnika Bowena - domyślnie )0.62 mb/stC */
    void set_cb(double cb = 0.62) {
        _cb = cb;
    };

    
private:
  //wczytywane - podstawowe
  double _Ta; //temperatura powietrza [st C]
  double _Rh; //Wilgotność powietrza [%]
  double _pa; //ciśnienie atmosferyczne [mb = hPa]
  double _Js; //promieniowanie krotkofalowe [W/m2]
  double _u; //predkosc wiatru 
  
  //wartosci wspolczynnikow ustawiane raz
  double _aw; //albedo wody
  static long double _sigma; // stała stefana Boltzmana [W/(m2 K)
  static double _kelvin; 
  double _emis_w; //zdolność emisyjna wody (0.97 wg Chapra 1997, Zhen and Gang 2008)
  float _cb; //wspolczynnik Bowena (=0.62 mb/st C)
  static float _p0; //referencyjne ciśnienie atmosferyczne na wysokosci morza (=1013 mb) 
  
  //obliczane
  long double _ea;
  
};

#endif // NETHEATFLUX_H
