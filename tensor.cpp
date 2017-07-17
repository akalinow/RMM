
/***************************************************************************
 *   tensor.cpp
 *   Copyright (C) 2005 by Monika Kalinowska                               *
 *   mkalinow@igf.edu.pl                                                   *
 *                                                                         *
 ***************************************************************************/

 #include "tensor.h"
 
void DTensor::ComputeTensor (double Dl, double Dt, Point &p){
	//cout<<" Ustawiam wartosci wspolczyn tensora metoda simple"<<endl;
	p.set_Dxx(Dl);
	p.set_Dxy(0.0);
	p.set_Dyy(Dt);
};

void DFischer::ComputeTensor (double Dl, double Dt, Point &p){
	//cout<<" Dl: "<<Dl<<"	Dt: "<<Dt<<endl;
	double vx = p.get_vx(), vy = p.get_vy();
	long double v = sqrt((vx*vx) +(vy*vy));
	
	long double Dxx = -9999;
	long double Dxy = -9999;
	long double Dyy = -9999;
	
	if (v != 0){
		long double D = Dl - Dt;
		Dxx = Dt + (D*((vx*vx)/(v*v)));
		Dxy = (D*((vx*vy)/(v*v)));
		Dyy = Dt + (D*((vy*vy)/(v*v)));
	}
	else {
		cout<<"!!! UWAGA !!! Predkosc wypadkowa rowna zero; Nie ma sensu pojecie dyspersji!"<<endl;
		cout<<"przyjmuje: Dxx = Dyy = Dt; Dxy = 0"<<endl; 
		Dxx = Dt;
		Dxy = 0;
		Dyy = Dt;
	}
	
	
	p.set_Dxx(Dxx);
	p.set_Dxy(Dxy);
	p.set_Dyy(Dyy);
	
	//cout<<" Ustawiam wartosci wspolczyn tensora metoda fischer"<<endl;
};


void DFischer2::ComputeTensor (double Dl, double Dt, Point &p){
	//cout<<" Dl: "<<Dl<<"	Dt: "<<Dt<<endl;
	double vx = p.get_vx(), vy = p.get_vy();
	long double v = sqrt((vx*vx) +(vy*vy));
	
	long double Dxx = -9999;
	//long double Dxy = -9999;
	long double Dyy = -9999;
	
	if (v != 0){
		long double D = Dl - Dt;
		Dxx = Dt + (D*((vx*vx)/(v*v)));
		//Dxy = (D*((vx*vy)/(v*v)));
		Dyy = Dt + (D*((vy*vy)/(v*v)));
	}
	else {
		cout<<"!!! UWAGA !!! Predkosc wypadkowa rowna zero; Nie ma sensu pojecie dyspersji!"<<endl;
		cout<<"przyjmuje: Dxx = Dyy = Dt; Dxy = 0"<<endl; 
		Dxx = Dt;
		//Dxy = 0;
		Dyy = Dt;
	}
	
	
	p.set_Dxx(Dxx);
	p.set_Dxy(0.0);
	p.set_Dyy(Dyy);
	
	//cout<<" Ustawiam wartosci wspolczyn tensora metoda fischer"<<endl;
};


void DVector::ComputeTensor (double Dl, double Dt, Point &p){
	//cout<<" Dl: "<<Dl<<"	Dt: "<<Dt<<endl;
	double vx = p.get_vx(), vy = p.get_vy();
	long double v = sqrt((vx*vx) +(vy*vy));
	//cout<<"v: "<<v<<"  vx: "<<vx<<"  vy: "<<vy<<endl;
	long double Dxx = -9999.99;
	long double Dxy = -9999.99;
	long double Dyy = -9999.99;
	
	if (v != 0){
		Dxx = abs(Dl * (vx/v) + Dt * (vy/v));
		Dxy = 0.0;
		Dyy = Dl * (vy/v) + Dt * (vx/v);
		cout<<"Obliczone wartosci:  Dxx:"<<Dxx<<"  Dxy: "<<Dxy<<"  Dyy: "<<Dyy<<endl;
	}
	else {
		cout<<"!!! UWAGA !!! Predkosc wypadkowa rowna zero; Nie ma sensu pojecie dyspersji!"<<endl;
		cout<<"przyjmuje: Dxx = Dyy = Dt; Dxy = 0"<<endl; 
		Dxx = Dt;
		Dxy = 0;
		Dyy = Dt;
	}
	//cout<<" Dxx: "<<Dxx<<"	Dyy: "<<Dyy<<"	Dxy: "<<Dxy<<endl;
	p.set_Dxx(Dxx);
	p.set_Dxy(Dxy);
	p.set_Dyy(Dyy);
	
	//cout<<" Ustawiam wartosci wspolczyn tensora metoda fischer"<<endl;
};

