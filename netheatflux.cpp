/*
 * ***************************************************************************		
 *  *   Copyright (C) by Monika Kalinowska                               *
 *  *   mkalinow@igf.edu.pl                                                   *
 *  *                                                                         *
 *  ***************************************************************************
 */

#include "netheatflux.h"

long double NetHeatFlux::_sigma = 5.67e-8;
double NetHeatFlux::_kelvin = 273.15;
float NetHeatFlux::_p0 = 1013.0;

//_____________________________________________________________________________________________________________
long double NetHeatFlux::ComputeNetHeatFlux(long double Tw)
{ 
  cout.precision(10);
 // cout<<"ComputeNetHeatFlux"<<endl;
  long double Jsn = ComputeNetSolar();
  long double Ja = ComputeLongWaveAtmosf();
  long double Jw = ComputeLongWaveWater(Tw);
  long double Je = ComputeEvapCondens(Tw);
  long double Jc = ComputeCondConv(Tw);
  //cout<<Jsn<<" "<<Ja<<" "<<Jw<<" "<<Je<<" "<<Jc<<endl;
  long double J = Jsn + Ja - Jw - Je - Jc;
  //cout<<">>>>>>>>> Net Heat Flux = "<<J<<endl;
  return J;
};
    
//_____________________________________________________________________________________________________________
long double NetHeatFlux::ComputeNetSolar()
{  
  long double Jsn = _Js*(1.0-_aw);
  //cout<<"NetSolarRadiation: "<<Jsn<<endl;
  return Jsn;
};

//_____________________________________________________________________________________________________________
long double NetHeatFlux::ComputeLongWaveAtmosf()
{  
  long double emis = Emissivity_Brunt1932();
  long double Ja = emis * _sigma * pow((_Ta + _kelvin), 4)*(1.0 - 0.03);
  //cout<<"Long Wave Atmosferic Radiation: "<<Ja<<endl;
  return Ja;
};

//_____________________________________________________________________________________________________________
long double NetHeatFlux::Emissivity_Brunt1932(double a1, double a2)
{
  long double Emissivity = a1 + a2 * pow(_ea,0.5);
  //return 0.769578;
  //cout<<"_ea: "<<_ea<<"  Emissivity Brunt (1932)"<<Emissivity<<endl;
  return Emissivity;
};


void NetHeatFlux::set_ea()
{
  /*
  long double pom1 = b2*_Ta;
  cout<<b2<<"* _Ta = "<<pom1<<endl;
  long double pom2 = _Ta + b3;
  cout<<"_Ta + "<<b3<<" = "<<pom2<<endl;
  long double pom3 = pom1/pom2;
  cout<<"("<<b2<<"* _Ta) /"<<"(_Ta + "<<b3<<") = "<<pom3<<endl;
  long double pom4 = exp(pom3);
  cout<<"e^("<<b2<<"* _Ta /"<<"(_Ta + "<<b3<<") = "<<pom4<<endl;
  long double esat = b1*(exp((pom1)/(pom2))); 
  */
  
  long double esat = e_sat(_Ta);
  _ea = esat*(_Rh/100);
  //cout<<"esat: "<<esat<<"  ea: "<<_ea<<endl;
};

long double NetHeatFlux::e_sat (long double T, double b1, double  b2, double b3)
{
  long double esat = b1*(exp((b2*T)/(T+b3))); 
  return esat;
};  

long double NetHeatFlux::ComputeLongWaveWater( long double &Tw)
{
  long double Jw = _emis_w * _sigma * pow((Tw + _kelvin),4);
  //cout<<"Long Wave Water: "<<Jw<<endl;
  return Jw;
};


long double NetHeatFlux::ComputeEvapCondens(long double& Tw)
{
  long double es = e_sat(Tw); 
  long double Je = WindFunction_Chapra2008() * (es - _ea);
  //cout<<"es: "<<es<<endl<<"Evaporation/Condensation: "<<Je<<endl;
  return Je;
};


long double NetHeatFlux::WindFunction_Chapra2008(float c1, float c2)
{
  //cout<<"f(u): "<<(c1 + (c2 * _u * _u))<<endl;
  return (c1 + (c2 * _u * _u));
};

long double NetHeatFlux::ComputeCondConv(long double& Tw)
{
  long double Jc = _cb * (_pa/_p0) * WindFunction_Chapra2008() * (Tw - _Ta);
  //cout<<"Conduation and Convection: "<<Jc<<endl;
  return Jc;
}
