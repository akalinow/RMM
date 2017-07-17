
/***************************************************************************
 *   param.cpp
 *   Copyright (C) 2005 by Monika Kalinowska                               *
 *   mkalinow@igf.edu.pl                                                   *
 *                                                                         *
 ***************************************************************************/   
 
#include "param.h"
#include "comments.h"
//#include <algo.h>
#include <algorithm>
 

SimulationParameters::SimulationParameters(string param):
	_param(param), _flag(-1), _n(-1), _dx(-1.0), _dy(-1.0), _dt(-1.0), 
	_vx(-9999.0), _vy(-9999.0), _Dlong(-9999.0), _Dtrans(-9999.0), _scheme ("nieokreslony"), _gridFile(" ") {
//	  cout<<"<< SimulationParameters object >> "<<endl;
};
 //_______________________________________________________________________________
 
 

void SimulationParameters::print() const {
    cout<<"flafa: "<<_flag<<" n= "<<_n<<" dx= "<<_dx<<" dy= "<<_dy<<" dt= "<<_dt
				<<" Dlong= "<<_Dlong<<" Dtrans= "<<_Dtrans
				<<" vx= "<<_vx<<" vy= "<<_vy<<" schemat= "<<_scheme<<" plik wejsciowy z siatka"<<_gridFile;
};

		
void SimulationParameters::set_dispersion(string dispersion) {
	
	_dispersion = dispersion;
	
	if (dispersion == "simple") _tensor = &_t_simple;
	else if (dispersion == "fischer") _tensor = &_t_fischer;
	else if (dispersion == "fischer2") _tensor = &_t_fischer2;
	else if (dispersion == "vector") _tensor = &_t_vector;
	else _tensor = &_t_simple;
	
	//_tensor->ComputeTensor(1,1,p);
	
};


void SimulationParameters::set_scheme(string scheme) {
	_scheme = scheme;
	if (scheme == "cn" ) _s_scheme = &_s_CN;
	else if (scheme == "up" ) _s_scheme = &_s_UP;
	else if (scheme == "adi" ) _s_scheme = &_s_ADI;
	else if (scheme == "adi2") _s_scheme = &_s_ADI2;
	else _s_scheme = &_s_CN;
};


double SimulationParameters::ComputePecletNumber(map<int, Point> &PointMap){
	
	double vx = 0, vy = 0, Dxx = 0, Dxy = 0, Dyy = 0;
	if (abs(_vxmax) > abs(_vxmin)) vx = _vxmax;
	else vx = _vxmin;
	if (abs(_vymax) > abs(_vymin)) vy = _vymax;
	else vy = _vymin;
	
	//INFO
	ofstream out("Info.dat", ios::app);
	out<<endl<<"... Obliczam liczby Pecleta dla ekstemalnych wartosci ..."<<endl<<endl; 
	out<<"------------------------     LICZBY PECLETA      ------------------------"<<endl;
 	out<<"- vx = "<<vx<<"  vy = "<<vy<<"  ";
	out<<"Dxxmax = "<<_Dxxmax<<"  Dyymax = "<<_Dyymax<<"  Dxymax = "<<_Dxymax<<endl;
	cout<<endl<<"... Obliczam liczby Pecleta dla ekstemalnych wartosci ..."<<endl<<endl;  
	cout<<"-----------------------     LICZBY PECLETA      ------------------------"<<endl;
	cout<<"- vx = "<<vx<<"  vy = "<<vy<<"  ";
	cout<<"Dxxmax = "<<_Dxxmax<<"  Dyymax = "<<_Dyymax<<"  Dxymax = "<<_Dxymax<<endl;
	
	//liczba pekleta ze wzory na 2D peclet numeber
	double Co = sqrt(pow((vx/_dx),2) + pow((vy/_dy),2));
	double nom = (_Dxxmax/pow(_dx,2)) + (_Dyymax/pow(_dy,2)) - (_Dxymax/(_dx*_dy));
	
	double Pex = (vx * _dx) / _Dxxmax;
	double Pey = (vy * _dy) / _Dyymax;
	//double Pex = (vx * _dx) / _Dxxmin;
	//double Pey = (vy * _dy) / _Dyymin;

	double Pe_wypadk = sqrt(Pex*Pex + Pey*Pey);
	
	out<<"- Dwuwymiarowa Pe = "<<Co/nom<<"  ";
	out<<"Pex = "<<Pex<<"  Pey = "<<Pey<<"  wypadkowa = "<<Pe_wypadk<<endl;
	out<<"------------------------------------------------------------------------"<<endl<<endl;
	cout<<"- Dwuwymiarowa Pe = "<<Co/nom<<"  ";
	cout<<"Pex = "<<Pex<<"  Pey = "<<Pey<<"  wypadkowa = "<<Pe_wypadk<<endl;
	cout<<"------------------------------------------------------------------------"<<endl<<endl;
	
	if(max((Co/nom),max(Pex,Pey)) > 2){
	  cout<<">>>>>>>>>>>>>>>  UWAGA!  <<<<<<<<<<<<<<<"<<endl;
	  cout<<"Liczba Pecleta < 2    -- Schemat CN i ADI moga generowac niefizyczne oscylcaje"<<endl;
	  out<<">>>>>>>>>>>>>>>  UWAGA!  <<<<<<<<<<<<<<<"<<endl;
	  out<<"Liczba Pecleta < 2    -- Schemat CN i ADI moga generowac niefizyczne oscylcaje"<<endl;
	}
	
	
	double Pexy = -9999;
	double Pex_max = -9999.99, Pey_max = -9999.00, Pexy_max = -9999.99, PeRes_max = -9999.99;
	map<int,Point>::iterator IT;
	for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
	  if (IT->second.get_flag() != -9999 & IT->second.get_flag() != 0){
	    vx = IT->second.get_vx();
	    vy = IT->second.get_vy();
	    Dxx = IT->second.get_Dxx();
	    Dyy = IT->second.get_Dyy();
	    Dxy = IT->second.get_Dxy();
	    
	    if (Dxx != 0.0) Pex = (vx * _dx) / Dxx;
	    else {
	      cout<<"Dxx rowne 0 - sprawdz"<<endl;
	      cout<<IT->second.get_x()*_dx<<","<<IT->second.get_y()*_dy<<endl;
	      cout<<"vx="<<vx<<" vy="<<vy<<" Dxx="<<Dxx<<" Dxy="<<Dxy<<" Dyy="<<Dyy<<endl;
	      //IT->second.set_flag(0);
	    }
	    if (Dyy != 0.0) Pey = (vy * _dy) / Dyy;
	    else {
	      cout<<"Dyy rowne 0 - sprawdz"<<endl;
	      cout<<IT->second.get_x()*_dx<<","<<IT->second.get_y()*_dy<<endl;
	     // IT->second.set_flag(0);
	    }
	    
	    Pe_wypadk = sqrt(Pex*Pex + Pey*Pey);
	  
	    //liczba pekleta ze wzory na 2D peclet numeber
	    Co = sqrt(pow((vx/_dx),2) + pow((vy/_dy),2));
	    nom = (Dxx/pow(_dx,2)) + (Dyy/pow(_dy,2)) - (Dxy/(_dx*_dy));
	    if (nom != 0.0 )Pexy = Co/nom;
	    else cout<<"nom rowne 0 - sprawdz"<<endl;
	    //cout<<"- Dwuwymiarowa Pe="<<Co/nom<<"  ";
	    //cout<<"Pex="<<Pex<<"  Pey="<<Pey<<"  wypadkowa="<<Pe_wypadk<<endl;
	    // cout<<"--------------------------------------------------------------------"<<endl<<endl;  
	    
	    if (Pex > Pex_max) { Pex_max = Pex; 
	      //cout<<IT->second.get_x()*_dx<<","<<IT->second.get_y()*_dy<<"  Pex_max:"<<Pex_max<<endl; 
	      //cout<<"vx="<<vx<<" vy="<<vy<<" Dxx="<<Dxx<<" Dxy="<<Dxy<<" Dyy="<<Dyy<<endl; 
	    }
    	    if (Pey > Pey_max) {Pey_max = Pey;  
	      //cout<<IT->second.get_x()*_dx<<","<<IT->second.get_y()*_dy<<"  Pey_max:"<<Pey_max<<endl;
	      //cout<<"vx="<<vx<<" vy="<<vy<<" Dxx="<<Dxx<<" Dxy="<<Dxy<<" Dyy="<<Dyy<<endl; 
	    }
	    if (Pexy > Pexy_max) {Pexy_max = Pexy;  
	      //cout<<IT->second.get_x()*_dx<<","<<IT->second.get_y()*_dy<<"  Pexy_max:"<<Pexy_max<<endl;
	      //cout<<"vx="<<vx<<" vy="<<vy<<" Dxx="<<Dxx<<" Dxy="<<Dxy<<" Dyy="<<Dyy<<endl; 
	    }
	    if (Pe_wypadk > PeRes_max) {PeRes_max = Pe_wypadk;  
	      //cout<<IT->second.get_x()*_dx<<","<<IT->second.get_y()*_dy<<"  PeRes_max:"<<PeRes_max<<endl;
	      //cout<<"vx="<<vx<<" vy="<<vy<<" Dxx="<<Dxx<<" Dxy="<<Dxy<<" Dyy="<<Dyy<<endl; 
	    }
	    if (Pex > 2 || Pey > 2 || Pexy > 2 || Pe_wypadk > 2){
	      cout<<"UWAGA! "<<IT->second.get_x()*_dx<<","<<IT->second.get_y()*_dy<<" ->";
	      cout<<"Pex:"<<Pex<<"  Pey:"<<Pey<<"  Pexy:"<<Pexy<<"  Pe_wypadk:"<<Pe_wypadk<<endl;
	      cout<<"    "<<"vx="<<vx<<" vy="<<vy<<" Dxx="<<Dxx<<" Dxy="<<Dxy<<" Dyy="<<Dyy<<endl; 
	    }
	  }
	}

	    cout<<"Max Dwuwymiarowa Pe = "<<Co/nom<<"  ";
	    cout<<"Pex_max = "<<Pex_max<<"  Pey_max = "<<Pey_max<<"Pexy_max = "<<Pexy_max<<"  wypadkowa = "<<PeRes_max<<endl;
	    cout<<"------------------------------------------------------------------------"<<endl<<endl;  
	
	
	
	
	out.close();
	return max((Co/nom),max(Pex,Pey));
};

	
double SimulationParameters::ComputeCourantNumber(){

	ofstream out("Info.dat", ios::app);
	cout<<endl<<"... Obliczam liczby Couranta dla ekstemalnych wartosci ..."<<endl;  
	out<<endl<<"... Obliczam liczby Couranta dla ekstemalnych wartosci ..."<<endl;  
	
	double vx = 0.0, vy = 0.0, Dxy = 0.0;
	if (abs(_vxmax) > abs(_vxmin)) vx = _vxmax;
	else vx = _vxmin;
	if (abs(_vymax) > abs(_vymin)) vy = _vymax;
	else vy = _vymin;

	if (abs(_Dxymax) < abs(_Dxymin)) {
		Dxy = _Dxymax;
		cout<<"max:"<<_Dxymax<<"  min:"<<_Dxymin<<"  Dxy:"<<Dxy<<endl;
	}
	else Dxy = _Dxymin;

	//cout<<"vx: "<<vx<<"  vy: "<<vy<<endl;
	
	double Co = _dt * sqrt(pow((vx/_dx),2) + pow((vy/_dy),2));
	double Cox = (vx*_dt)/_dx;
	double Coy = (vy*_dt)/_dy;
	double Co_wypadk = sqrt(Cox*Cox + Coy*Coy);
	double CDox = (_Dxxmax * _dt) / (_dx *_dx);
	double CDoy = (_Dyymax * _dt) / (_dy *_dy);
	double CDxy = (Dxy * _dt) / (_dx *_dy);
	
	//INFO
	out<<endl;
	out<<"------------------------- Adwekcyjne LICZBY COURANTA ----------------------------"<<endl;
 	out<<"- Dwuwymiarowa CrA = "<<Co<<"   ";
	out<<"- CrAx = "<<Cox<<"  CrAy = "<<Coy<<"  wypadkowa = "<<Co_wypadk<<endl;
	out<<endl;
	out<<"------------------------  Dyfuzyjne LICZBY COURANTA  ----------------------------"<<endl;
	out<<"- CrDx = "<<CDox<<"   CrDy = "<<CDoy<<"   CrDxy = "<<CDxy<<endl;
	out<<"------------------------------------------------------------------"<<endl;
	
	cout<<endl;
	cout<<"------------------------- Adwekcyjne LICZBY COURANTA ----------------------------"<<endl;
	cout<<"- Dwuwymiarowa CrA = "<<Co<<"	";
	cout<<"CrAx = "<<Cox<<"   CrAy = "<<Coy<<"   wypadkowa = "<<Co_wypadk<<endl;
	cout<<"---------------------------------------------------------------------------------"<<endl;
	cout<<endl;
	cout<<"-------------------------  Dyfuzyjne LICZBY COURANTA  ---------------------------"<<endl;
	cout<<"- CrDx = "<<CDox<<"   CrDy = "<<CDoy<<"   CrDxy = "<<CDxy<<endl;
	cout<<"---------------------------------------------------------------------------------"<<endl<<endl;

	if(Cox + Coy > 1){
	  cout<<">>>>>>>>>>>>>>>  UWAGA!  <<<<<<<<<<<<<<<"<<endl;
	  cout<<"Suma Adwekcyjnych Liczb Couranta > 1 -> "<<Cox+Coy<<"   -- Schemat UP niestabilny"<<endl<<endl;
	  out<<">>>>>>>>>>>>>>>  UWAGA!  <<<<<<<<<<<<<<<"<<endl;
	  out<<"Suma Adwekcyjnych Liczb Couranta > 1  -> "<<Cox+Coy<<"   -- Schemat UP niestabilny"<<endl<<endl;
	}

	if(CDox + CDoy > 0.5){
	  cout<<">>>>>>>>>>>>>>>  UWAGA!  <<<<<<<<<<<<<<<"<<endl;
	  cout<<"Suma Dyfuzyjnych  Liczb Couranta > 1/2   -> "<<CDox+CDoy<<"  -- Schemat UP niestabilny"<<endl<<endl;
	  out<<">>>>>>>>>>>>>>>  UWAGA!  <<<<<<<<<<<<<<<"<<endl;
	  out<<"Suma Dyfuzyjnych  Liczb Couranta > 1/2  -> "<<CDox+CDoy<<"   -- Schemat UP niestabilny"<<endl<<endl;
	}

	out.close();
	return max(Co,max(Cox,Coy));
};
