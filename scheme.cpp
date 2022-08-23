
/***************************************************************************
 *   scheme.cpp
 *   Copyright (C) 2005 by Monika Kalinowska                               *
 *   mkalinow@igf.edu.pl                                                   *
 *                                                                         *
 ***************************************************************************/

#include "scheme.h"
#include "grid.h"
#include "comments.h"
#include "param.h"
#include "netheatflux.h"

 #include <sstream>
#include <ctime>   /* functions time and clock defined here */
#include <unistd.h> /* function sleep defined here */
#include <iomanip>
#include <algorithm> /*algorytm sortowania tutaj*/


using namespace std;

//***************************************************SSCHEME***********************************************

 void SScheme::ComputeConcentration (map <int, Point> & PointMap){};
 
//_________________________________________________________________________________________________________
 void SScheme::SetFactors(map <int, Point> & PointMap){
	cout<<"SScheme::SetFactors"<<endl;
 	map<int,Point>::iterator IT;
	//double CAx = 0.0, CAy = 0.0, CDx = 0.0, CDxy = 0.0, CDy = 0.0;
	vector<int> flaga;
	flaga.clear();
	cout<<"SScheme::SetFactors 1"<<endl;
	int nn0;
	int nn1;
	int nn2;
	int nn3;
	int nn4;
	int nn5;
	int nn6;
	int nn7;
	int nn8;
	for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
		if (IT->second.get_flag() != 0){
		//cout<<"1"<<endl;
		flaga.clear();	
		/*CAx = IT->second.get_CurAx();
			CAy = IT->second.get_CurAy();
			CDx = IT->second.get_CurDx();
			CDxy = IT->second.get_CurDxy();
			CDy = IT->second.get_CurDy();
			Point *p = &(IT->second);
			GetFactors(p, CAx, CAy, CDx, CDxy, CDy);*/

			//---------------------------08 11 2006 ----------------
			nn0 = IT->second.get_near_neighbour(0);
			nn1 = IT->second.get_near_neighbour(1);
			nn2 = IT->second.get_near_neighbour(2);
			nn3 = IT->second.get_near_neighbour(3);
			nn4 = IT->second.get_near_neighbour(4);
			nn5 = IT->second.get_near_neighbour(5);
			nn6 = IT->second.get_near_neighbour(6);
			nn7 = IT->second.get_near_neighbour(7);
			nn8 = IT->second.get_near_neighbour(8);

			flaga.push_back(PointMap[nn0].get_flag()); //0 
			flaga.push_back(PointMap[nn1].get_flag()); //0 
			flaga.push_back(PointMap[nn2].get_flag()); //0 
			flaga.push_back(PointMap[nn3].get_flag()); //0 
			flaga.push_back(PointMap[nn4].get_flag()); //0 
			flaga.push_back(PointMap[nn5].get_flag()); //0 
			flaga.push_back(PointMap[nn6].get_flag()); //0 
			flaga.push_back(PointMap[nn7].get_flag()); //0 
			flaga.push_back(PointMap[nn8].get_flag()); //0 

			//int indx;
			/*cout << "----------------------"<<endl;
			for (indx = 0; indx < flaga.size(); indx++)
			{
  				cout << indx<< " : " <<flaga[indx] <<  endl;
			}
			cout << "----------------------"<<endl; 			
			//-------------------------------------------------------
			*/
			GetFactors(&(IT->second), IT->second.get_CurAx(), IT->second.get_CurAy(), 
						IT->second.get_CurDx(), IT->second.get_CurDxy(), IT->second.get_CurDy(), flaga);
		}
	} 	
	cout<<"SScheme::SetFactors end"<<endl;
 };

 //_________________________________________________________________________________________________________
 void SScheme::GetFactors(Point *p, const double &CAx, const double &CAy, const double &CDx, const double &CDxy, const double &CDy, const vector<int> &flag){};
			

 //_________________________________________________________________________________________________________
 long double SScheme::Jakobi(Point * p, map<int, Point> &PointMap, double fij,
 	double a, double b, double c, double d, double e, double f, double g, double h, double i){
 	
	return -(b * (PointMap.find(p->get_near_neighbour(5)))->second.get_Curr() +
					  	c * (PointMap.find(p->get_near_neighbour(3)))->second.get_Curr() +
					  	d * (PointMap.find(p->get_near_neighbour(1)))->second.get_Curr() +
					  	e * (PointMap.find(p->get_near_neighbour(7)))->second.get_Curr() +
					  	f * (PointMap.find(p->get_near_neighbour(2)))->second.get_Curr() +
					  	g * (PointMap.find(p->get_near_neighbour(8)))->second.get_Curr() +
					  	h * (PointMap.find(p->get_near_neighbour(0)))->second.get_Curr() +
					  	i * (PointMap.find(p->get_near_neighbour(6)))->second.get_Curr() -
						fij)/a;
 }
 
 //_________________________________________________________________________________________________________
 long double SScheme::Jakobi(map<int, Point> &PointMap, map<int,long double> &f, vector<string> &a){
	
 	int index = Grid::empty_index;
	Point *p = &(PointMap.find(index)->second);
	long double ck1 = 0.0, change = 0.0;
	
	for (int i = Grid::_xmin; i <= Grid::_xmax + 1; i++){
		for (int j = Grid::_ymin; j <= Grid::_ymax + 1; j++){
			index = i * 10000 + j;
			p = &(PointMap.find(index)->second);
			if (p->get_flag() != 0) {
					
				//Jacobi
				ck1 = Jakobi(p, PointMap, f[index], p->get_factor(a[0]), p->get_factor(a[1]), p->get_factor(a[2]),
						p->get_factor(a[3]), p->get_factor(a[4]), p->get_factor(a[5]), p->get_factor(a[6]), 
						p->get_factor(a[7]), p->get_factor(a[8])); 			
					
				change+=fabs(p->get_Next() - ck1);
				p->set_Next(ck1);
			}
		}
	}
	return change;
 }
 
 //_________________________________________________________________________________________________________
 long double SScheme::Gauss_Seidel(Point * p, map<int, Point> &PointMap, double fij,
 	double a, double b, double c, double d, double e, double f, double g, double h, double i){
 	
	return 	(- (c * (PointMap.find(p->get_near_neighbour(3)))->second.get_Next() +
				e * (PointMap.find(p->get_near_neighbour(7)))->second.get_Next() +
				g * (PointMap.find(p->get_near_neighbour(8)))->second.get_Next() +
				i * (PointMap.find(p->get_near_neighbour(6)))->second.get_Next())
			
			- (b * (PointMap.find(p->get_near_neighbour(5)))->second.get_Curr() +
			  	d * (PointMap.find(p->get_near_neighbour(1)))->second.get_Curr() +
			  	f * (PointMap.find(p->get_near_neighbour(2)))->second.get_Curr() +
			  	h * (PointMap.find(p->get_near_neighbour(0)))->second.get_Curr() -
				fij)
			)/a;
 }

 //_________________________________________________________________________________________________________
 long double SScheme::Gauss_Seidel(map<int, Point> &PointMap, map<int,long double> &f, vector<string> &a){
	
 	int index = Grid::empty_index;
	Point *p = &(PointMap.find(index)->second);
	long double ck1 = 0.0, change = 0.0;
	
	for (int i = Grid::_xmin; i <= Grid::_xmax + 1; i++){
		for (int j = Grid::_ymin; j <= Grid::_ymax + 1; j++){
			index = i * 10000 + j;
			p = &(PointMap.find(index)->second);
			if (p->get_flag() != 0) {
					
				//Gauss-Seidel
				ck1 = Gauss_Seidel(p, PointMap, f[index], p->get_factor(a[0]), p->get_factor(a[1]), p->get_factor(a[2]),
						p->get_factor(a[3]), p->get_factor(a[4]), p->get_factor(a[5]), p->get_factor(a[6]), 
						p->get_factor(a[7]), p->get_factor(a[8])); 
				change+=fabs(p->get_Next() - ck1);
				p->set_Next(ck1);
			}
		}
	}
	return change;
 }
  
 //_________________________________________________________________________________________________________
 long double SScheme::SOR (Point * p, map<int, Point> &PointMap, double fij,
 	double a, double b, double c, double d, double e, double f, double g, double h, double i, long double omega){
// 	cout<<"omega: "<<omega<<endl;
//	omega = 1.13799999999999999;
	long double ck1 = - (c * (PointMap.find(p->get_near_neighbour(3)))->second.get_Next() +
					  	e * (PointMap.find(p->get_near_neighbour(7)))->second.get_Next() +
					  	g * (PointMap.find(p->get_near_neighbour(8)))->second.get_Next() +
					  	i * (PointMap.find(p->get_near_neighbour(6)))->second.get_Next());
	//if (p->get_y() == 50 || p->get_y() == 49 || p->get_y() == 48) cout<<endl<<"y: "<<p->get_y()<<"1 ck1: "<<ck1;
	ck1 = ck1 - (a * p->get_Curr() + 
				b * (PointMap.find(p->get_near_neighbour(5)))->second.get_Curr() +
			  	d * (PointMap.find(p->get_near_neighbour(1)))->second.get_Curr() +
			  	f * (PointMap.find(p->get_near_neighbour(2)))->second.get_Curr() +
			  	h * (PointMap.find(p->get_near_neighbour(0)))->second.get_Curr() -
				fij);	
	//if (p->get_y() == 50 || p->get_y() == 49 || p->get_y() == 48) cout<<endl<<"y: "<<p->get_y()<<"  2 ck1: "<<ck1;
	ck1 = ck1/a;
	//if (p->get_y() == 50 || p->get_y() == 49 || p->get_y() == 48) cout<<endl<<"y: "<<p->get_y()<<"  3 ck1: "<<ck1<<endl;
	return (p->get_Curr() + omega * ck1);
 }
 
 
 //_________________________________________________________________________________________________________
 long double SScheme::SOR (map<int, Point> &PointMap, map<int,long double> &f, vector<string> &a, long double omega){
	if (omega > 2.0 || omega < 0.0 ) cout<<"UWAGA!!! zly paramet relaksacji - metoda SOR rozbiezna"<<endl;
 	int index = Grid::empty_index;
	Point *p = &(PointMap.find(index)->second);
	long double ck1 = 0.0, change = 0.0;
	

	for (int i = Grid::_xmin; i <= Grid::_xmax + 1; i++){
		for (int j = Grid::_ymin; j <= Grid::_ymax + 1; j++){
			index = i * 10000 + j;
			p = &(PointMap.find(index)->second);
			if (p->get_flag() != 0) {
				//SOR
				ck1 = SOR (p, PointMap, f[index], p->get_factor(a[0]), p->get_factor(a[1]), p->get_factor(a[2]),
						p->get_factor(a[3]), p->get_factor(a[4]), p->get_factor(a[5]), p->get_factor(a[6]), 
						p->get_factor(a[7]), p->get_factor(a[8]), omega); 
				change+=fabs(p->get_Next() - ck1);
				p->set_Next(ck1);
			}
		}
	}
	return change;	
 }
 
 //_________________________________________________________________________________________________________
 float SScheme::FindBestOmega (map<int, Point> &PointMap, map<int,long double> &f){
 return 1;
}

//funkcja tymczasowa !!! 
//jesli nei to dorobic zabezpieczenie na aby nie bylo neiodpowiedniej methody dla danego schematu
void SScheme::ReadMethod (string scheme){
	string method = get_method();
	long double omega = 1.0;
	fstream in("method.txt");
	if(!in) {
    	CheckFile(0,"method.txt");
		return;// jesli nie ma pliku wychodzi z funkci zwracajac 0
    }
	else CheckFile(1,"method.txt");		
	in>>method;
	if (method == "SOR") in>>omega;
	cout<<"schemat : "<<scheme<<"	";
	cout<<"sczytane omega: "<<omega<<"	";
	set_method(method);
	set_omega(omega);
	if (scheme == "adi" || scheme == "adi2") {
		if (method == "Thomasa" || method == "SOR" || method == "Jacobi" || method == "Gauss-S") set_method(method);
		else {
			cout<<" UWAGA BLAD !!!"<<endl<<"	Wybrano zla metode rozwiazywania ukladuow rownan dla schematu ADI"
				<<endl<<"	Uzyta zostanie domyslna dla tego schemtatu metoda Thomasa!"<<endl;
			set_method("Thomasa");
		}	
	}
	else if (scheme == "cn") {
		if (method == "SOR" || method == "Jacobi" || method == "Gauss-S") set_method(method);
		else {
			cout<<" UWAGA BLAD !!!"<<endl<<"	Wybrano zla metode rozwiazywania ukladuow rownan dla schematu Cranka Nicolson"
				<<endl<<"	Uzyta zostanie domyslna dla tego schematu metoda SOR!"<<endl;
			set_method("SOR");
		}
	}
	else cout<<"INFO - dla schematu "<<scheme<<" nie ma potrzeby ustawiania metody do rozwiazywania ukladow rownan"<<endl;	
	cout<<"method:	"<<method<<endl;
};
 
//***************************************************SCrankNicolson****************************************

 void SCrankNicolson::IterativeSolveSetOfEquations (map <int, Point> & PointMap, map<int,long double> &f){
 	map<int, Point>::const_iterator CI;
	ofstream out;
	/*
	out.open("1.dat",ios::trunc);
	for(CI=PointMap.begin();CI!=PointMap.end();CI++){
        if (CI->second.get_flag() != -9999){
			out<<CI->second.get_x()<<" "
        		<<CI->second.get_y()<<" "
        			<<CI->second.get_Curr()<<endl;
		}
	}
	out.close();
	*/
	cout.precision(20);
	long double change = 0.0, val = -9999.99;
	vector<string> a; 
	
	a.push_back("a"); a.push_back("b"); a.push_back("c"); a.push_back("d");
	a.push_back("e"); a.push_back("f"); a.push_back("g"); a.push_back("h"); a.push_back("i"); 
	
	
	int k = 0;
	cout<<"Iteracja:";
	for (k = 0; k < 200; k++){
		change = 0.0;
		cout<<k<<"	"<<flush;
		
		//wywolanie odpowiedniej metody iteracyjnej
		if (get_method() == "SOR") change = SOR (PointMap, f, a, this->get_omega());
		else if (get_method() == "Gauss-S") change = Gauss_Seidel (PointMap, f, a);
		else change = Jakobi(PointMap, f, a);

		map<int,Point>::iterator IT;
		long double suma = 0.0, suma1 = 0.0;
		int index = Grid::empty_index, pktprzed = 0, pktpo = 0;
		//przepisanie wratosci z kroku Next na Current - przygotownnanie do nastepnej iteracji
		for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
			val = IT->second.get_Next();
			suma = suma + val;
			pktprzed ++;
			if (abs(val) > 1e-10) { //nie przepisuejmy b. malych wartosci
				IT->second.set_Curr(val);
				suma1 = suma1 + IT->second.get_Curr();
				pktpo ++;
				//if (IT->second.get_y() == -1 || IT->second.get_y() == 0 || IT->second.get_y() == 1) 
				  //cout<<"x,y: "<<IT->second.get_x()<<IT->second.get_y()<<"  VAL:"<<val<<endl;
			}
		}
		//cout<<"Chenge: "<<change<<"		";
		//cout<<endl<<"  suma: "<<suma<<"  suma1: "<<suma1<<"  pktprzed: "<<pktprzed<<"  pktpo: "<<pktpo<<endl<<endl;
		/*
		map<int, Point>::const_iterator CI;		
		ostringstream buffer;
    		buffer << k;		// send the int to the ostringstream
    		string i =  buffer.str();	// capture the string
		ofstream out;
		string file = i + "a.dat";
		out.open(file.c_str(),ios::trunc);
		for(CI=PointMap.begin();CI!=PointMap.end();CI++){
		  if (CI->second.get_flag() != -9999){
		    out<<CI->second.get_x()<<" "
		       <<CI->second.get_y()<<" "
		       <<CI->second.get_Curr()<<endl;
		  }
		}
		out.close();	
		*/
		if(change < 1e-15) break;
	}
	cout<<endl;
	
	/*
	/////////////////////////////////////////////////////////////////////////////////////////////
	int last = 0;	
	if  (get_method() == "SOR") {
	////!!! Kawalek do wyznaczania optymalnego paramtru relakascaji 
	///powinin byc udoskonlaony w przyszlosci!!!!
	if(get_sym_step() == 1) { // dla pierwszego kroku omega wynosi 1 (usatwiane w symuleation.cpp)
		this->_i.push_back(k);
		this->_w[k]=get_omega();
		set_omega(get_omega()+0.1);//po pierwszym kroku zwiekszamo 0.1 by sprwadzic czy nei bedzie lepiej
	}
	else {// dla pozostalych krokow
		last = _i.size()-1; 
		cout<<"last"<<last; 
		this->_i.push_back(k); //zapisuje liczbe iteracji
		this->_w[k]=get_omega(); //zapisuje omega indeksowane w mapioe liczba iteracji (nei najlepeij)
		cout<<"  k: "<<k<<" i[last]: "<<_i[last]<<"  _w[_i[last]] : "<<_w[_i[last]]<<endl;
		if (k<=_i[last]){// kiedy liczba iteracji jest mneijsza lub taka sam a jak poprzednio
			cout<<"		k jest mniejsze ziwkszam wiec omega by sprawdzicc czy nastepne jeszcz sie nei zmniejszy"<<endl;
			set_omega(get_omega()+0.1/(this->powrot+1)); // sprawdzam dla nowego troche zwiakszonego omega - o ile zalezy od parametru powrot
			cout<<"		omega = "<<get_omega()<<endl;
		}
		else{// jesli liczba iteracji jest wieksa niz poprzednio
			set_omega(_w[_i[last]]);
			cout<<" k jest teraz wieksz lub takie samo == gorsze wiec zminiam omega na poprzednie"<<endl;
			cout<<"		omega = "<<get_omega()<<endl;//wracam do poprzedniego omegiu
			this->powrot = this->powrot+1;//wiekszam parametr powrot by moc potem wykorzysrtac do zwikszenai omegi o odpowiednia ale mneijsza wartosc niz w kroku poprzednim
		}
	}
	cout<<"OMEGA: "<<get_omega()<<endl;
	}*/
 }
 
 //_________________________________________________________________________________________________________
 void SCrankNicolson::ComputeConcentration (map <int, Point> & PointMap){
	
	map<int,long double> f;
	int index = Grid::empty_index;
	Point *p = &(PointMap.find(index)->second);
		
	//oblicznie wktora wyrazow wolnych w ukladzie rownan 
	for (int i = Grid::_xmax; i >= Grid::_xmin; i--){
		for (int j = Grid::_ymax; j >= Grid::_ymin; j--){
			index = i * 10000 + j;
			p = &(PointMap.find(index)->second);
			//if (p->get_flag() != 0) {
				f[index] = p->get_factor_f("fa") * p->get_Curr() +
					  p->get_factor_f("fb") * (PointMap.find(p->get_near_neighbour(5)))->second.get_Curr() +
					  p->get_factor_f("fc") * (PointMap.find(p->get_near_neighbour(3)))->second.get_Curr() +
					  p->get_factor_f("fd") * (PointMap.find(p->get_near_neighbour(1)))->second.get_Curr() +
					  p->get_factor_f("fe") * (PointMap.find(p->get_near_neighbour(7)))->second.get_Curr() +
					  p->get_factor_f("ff") * (PointMap.find(p->get_near_neighbour(2)))->second.get_Curr() +
					  p->get_factor_f("fg") * (PointMap.find(p->get_near_neighbour(8)))->second.get_Curr() +
					  p->get_factor_f("fh") * (PointMap.find(p->get_near_neighbour(0)))->second.get_Curr() +
					  p->get_factor_f("fi") * (PointMap.find(p->get_near_neighbour(6)))->second.get_Curr();
					  //pamietac zew przyszlosci trzeba dodac jeszcze czlony z distatnt neighbeurs
				//if ( f[index] !=0 || p->grt_Curr() != 0) cout<<endl<<p->get_x()<<","<<p->get_y()<<" f: "<<f[index]<<endl;
//				int mx=p->get_x(), my=p->get_y();
				//if ((my==50 && mx==25) || (my==49 && (mx==24 || mx==25 || mx==26)) || (my==48 && mx==25)){
/*				if ((my==4 && mx==25) || (my==5 && (mx==24 || mx==25 || mx==26)) || (my==6 && mx==25)){
					 cout<<endl<<mx<<","<<my<<" f: "<<f[index]<<endl;
					 cout<<p->get_factor_f("fa")<<" * "<<p->get_Curr()<<" + "<<endl
					  <<p->get_factor_f("fb")<<" * "<<(PointMap.find(p->get_near_neighbour(5)))->second.get_Curr()<<" + "<<endl
					  <<p->get_factor_f("fc")<<" * "<<(PointMap.find(p->get_near_neighbour(3)))->second.get_Curr()<<" + "<<endl
					  <<p->get_factor_f("fd")<<" * "<<(PointMap.find(p->get_near_neighbour(1)))->second.get_Curr()<<" + "<<endl
					  <<p->get_factor_f("fe")<<" * "<<(PointMap.find(p->get_near_neighbour(7)))->second.get_Curr()<<" + "<<endl
					  <<p->get_factor_f("ff")<<" * "<<(PointMap.find(p->get_near_neighbour(2)))->second.get_Curr()<<" + "<<endl
					  <<p->get_factor_f("fg")<<" * "<<(PointMap.find(p->get_near_neighbour(8)))->second.get_Curr()<<" + "<<endl
					  <<p->get_factor_f("fh")<<" * "<<(PointMap.find(p->get_near_neighbour(0)))->second.get_Curr()<<" + "<<endl
					  <<p->get_factor_f("fi")<<" * "<<(PointMap.find(p->get_near_neighbour(6)))->second.get_Curr()<<endl;
				}*/
			//}
		}
	}
	this->IterativeSolveSetOfEquations (PointMap, f);
 };
 
//_________________________________________________________________________________________________________
 void SCrankNicolson::GetFactors(Point *p, const double &CAx, const double &CAy, const double &CDx, const double &CDxy, const double &CDy, const vector<int> &flag){

	p->set_factor_fij("fa", 1 - CDx - CDy);//fa przy i,j
	p->set_factor_fij("fb", (-CAx/4) + (CDx/2));//fb przy i+1,j
	p->set_factor_fij("fc", (CAx/4) + (CDx/2));//fc przy i-1,j
	p->set_factor_fij("fd", (-CAy/4) + (CDy/2));//fd przy i,j+1
	p->set_factor_fij("fe", (CAy/4) + (CDy/2));//fe przy i,j-1
	
	//------------------OPCJA I---------------------------//
	/*p->set_factor_fij("ff", CDxy/2);//ff przy i+1,j+1
	p->set_factor_fij("fg", -CDxy/2);//fg przy i+1,j-1
	p->set_factor_fij("fh", -CDxy/2);//fh przy i-1,j+1
	p->set_factor_fij("fi", CDxy/2);//fi przy i-1,j-1*/
	
	//------------------OPCJA II---------------------------//
	p->set_factor_fij("ff", CDxy);//ff przy i+1,j+1
	p->set_factor_fij("fg", -CDxy);//fg przy i+1,j-1
	p->set_factor_fij("fh", -CDxy);//fh przy i-1,j+1
	p->set_factor_fij("fi", CDxy);//fi przy i-1,j-1
	
	p->set_factor_fij("fj", 0);//fj przy i+2,j
	p->set_factor_fij("fk", 0);//fk przy i-2,j
	p->set_factor_fij("fl", 0);//fl przy i,j+2
	p->set_factor_fij("fm", 0);//fm przy i,j-2
	
	p->set_factor("a",1 + CDx + CDy);//a przy i,j
	p->set_factor("b",(CAx/4) - (CDx/2));//b przy i+1,j
	p->set_factor("c",-(CAx/4) - (CDx/2));//c przy i-1,j
	p->set_factor("d",(CAy/4) - (CDy/2));//d przy i,j+1
	p->set_factor("e",-(CAy/4) - (CDy/2));//e przy i,j-1
	
	//------------------OPCJA I---------------------------//
	/*p->set_factor("f",-CDxy/2);//f przy i+1,j+1
	p->set_factor("g",CDxy/2);//g przy i+1,j-1
	p->set_factor("h",CDxy/2);//h przy i-1,j+1
	p->set_factor("i",-CDxy/2);//i przy i-1,j-1
	*/
	//------------------OPCJA II---------------------------//
	p->set_factor("f",-CDxy);//f przy i+1,j+1
	p->set_factor("g",CDxy);//g przy i+1,j-1
	p->set_factor("h",CDxy);//h przy i-1,j+1
	p->set_factor("i",-CDxy);//i przy i-1,j-1
	
	p->set_factor("j",0);//j przy i+2,j
	p->set_factor("k",0);//k przy i-2,j
	p->set_factor("l",0);//l przy i,j+2
	p->set_factor("m",0);//m przy i,j-2

	if (flag[1]==0 || flag[2]==0 || flag[3]==0 || flag[5]==0 || flag[6]==0 || flag[7]==0 || flag[8]==0 || flag[0]==0)
	{
//	if (flag[1]!=1 || flag[2]!=1 || flag[3]!=1 || flag[5]!=1 || flag[6]!=1 || flag[7]!=1 || flag[8]!=1 || flag[0]!=1){
//		cout<<"UWAG - BRAK KTOREGOS Z PKTOW -> musze oznaczyc JAK brzegi"<<endl;
									//	* * * * *
									//	* 0 1 2 *		0->(i-1,j+1)	1->(i,j+1)	2->(i+1,j+1)
									//	* 3 4 5 *		3->(i-1,j)		4->(i,j)	5->(i+1,j) 
									//	* 6 7 8 *		6->(i-1,j-1)	7->(i,j-1)	8->(i+1,j-1)
									//	* * * * *		

//-----------------------------------------------------------------------------------
		float od_adv_y = 0.0, od_adv_x = 0.0; 
		float w1 = -1.0, w2 = -1.0, w3 = 1.0, w4 = 1.0;
		//gdy brak pkt c[i+1,j] - ozn 5 
		//czyli gdy "bp", "rgp", "rdp", "szp", "szg", "szd"
		if(flag[5]==0) {
			p->set_factor_fij("fb", 0);//fb przy i+1,j
			p->set_factor_fij("fc", (CAx/4) + (CDx/2));//fc przy i-1,j
			p->set_factor("b", 0);//fb przy i+1,j
			p->set_factor("c", - (CAx/4) - (CDx/2));//fc przy i-1,j
			od_adv_x = CAx/4;
			w1 = -0.5; //wspolczynnik przy Dx == -0.5
			w3 = 0.5;	
		} 

		//gdy brak pkt c[i-1,j] - ozn 3 
		//czyli gdy "bl", "rgl", "rdl", "szl", "szg", "szd"
		if(flag[3]==0) {
			p->set_factor_fij("fc", 0);//fc przy i-1,j
			p->set_factor_fij("fb", - (CAx/4) + (CDx/2));//fb przy i+1,j
			p->set_factor("c", 0);//fc przy i-1,j
			p->set_factor("b", (CAx/4) - (CDx/2));//fb przy i+1,j
			od_adv_x = - CAx/4;
			w1 = -0.5; //wspolczynnik przy Dx == -0.5
			w3 = 0.5;	
		} 

		//gdy brak pkt c[i,j+1] - ozn 1 
		//czyli gdy "bg", "rgl", "rgp", "szl", "szg", "szp"
		if(flag[1]==0) {
			//cout<<"HELOOO MALARO gorny "<<endl;
			p->set_factor_fij("fd", 0);//fd przy i,j+1
			p->set_factor_fij("fe",(CAy/4) + (CDy/2));//fe przy i,j-1		
			p->set_factor("d", 0);//fd przy i,j+1
			p->set_factor("e", - (CAy/4) - (CDy/2));//fe przy i,j-1
			od_adv_y = CAy/4;	
			w2 = -0.5; //wspolczynnik przy Dy == -1
			w4 = 0.5;
		} 

		//gdy brak pkt c[i,j-1] - ozn 7 
		//czyli gdy "bd", "rdl", "rdp", "szl", "szp", "szd"
		if(flag[7]==0) {
			//cout<<"HELOOO MALARO dolny"<<endl;
			p->set_factor_fij("fe", 0);//fe przy i,j-1
			p->set_factor_fij("fd", - (CAy/4) + (CDy/2));//fd przy i,j+1
			p->set_factor("e", 0);//fe przy i,j-1
			p->set_factor("d", (CAy/4) - (CDy/2));//fd przy i,j+1
			od_adv_y = - CAy/4;
			w2 = -0.5; //wspolczynnik przy Dy == -1
			w4 = 0.5;
		}

		//jezeli nie ma obu c[i+1,j] i c[i-1,j] 
		if (flag[5] == 0 & flag[3] == 0) {w1 = 0; w3 = 0;}
		//jezeli nie ma obu c[i,j+1] i c[i,j-1] 
		if (flag[1] == 0 & flag[7] == 0) {w2 = 0; w4 = 0;}

 
//-----------------------------------------------------------------------------------
		

//---------------WSPOLCZYNNIKI PRZY PKT POCHODZACYCH Z DYSKRET. POCHODNYCH MIESZANYCH --------------//
		//wspolczynnik "ff" - zmienia sie tylko przy braku pkt c[i+1,j+1] - ozn 2
		//czyli "bg", "bp" i "szp", "rgl", "rgp", "rdp", "kgp" iin gdzie brakuje pkt 2
		if(flag[2]==0) {
			p->set_factor_fij("ff", 0);
			p->set_factor("f", 0);
		}

		//wspolczynnik "fg" - zmienia sie tylko przy braku pkt c[i+1,j-1] - ozn 8
		//czyli "bg", "bp" i "szp", "rgl", "rgp", "rdp", "kgp" iin gdzie brakuje pkt 8
		if(flag[8]==0) {
			p->set_factor_fij("fg", 0);
			p->set_factor("g", 0);
		}
		//wspolczynnik "fh" - zmienia sie tylko przy braku pkt c[i-1,j+1] - ozn 0
		//czyli "bg", "bp" i "szp", "rgl", "rgp", "rdp", "kgp" iin gdzie brakuje pkt 0
		if(flag[0]==0) {
			p->set_factor_fij("fh", 0);
			p->set_factor("h", 0);
		}

		//wspolczynnik "fi" - zmienia sie tylko przy braku pkt c[i-1,j-1] - ozn 6
		//czyli "bg", "bp" i "szp", "rgl", "rgp", "rdp", "kgp" iin gdzie brakuje pkt 6
		if(flag[6]==0) {
			p->set_factor_fij("fi", 0);
			p->set_factor("i", 0);
		}
//--------------------------------------------------------------------------------------------------//
	

//-------WSPOLCZYNNIK "fa" PRZED C[I,J] - ZMIENIA SIE GDY BRAK JAKIEGOKOLWIEK PKT I ZALEZY OD ZNAKU PREDKOSCI ----//

		// fa = 1 + w1*Dx + w2*Dy + od_mieszanych

		// POCZATEK +++++++++++++++++++++ od_miezanych  ++++++++++++++++++++++++++++++++++//
		//++++++++++++czesc od mieszanych pochodnych we wspolczyniku "fa" ++++++++++++++//
		double od_mieszanych = 0.0; //skladnik pochodzacy od mieszanych pochodnych we spolczyniiku przed cij
		int tmpf=1, tmpg=1, tmph=1, tmpi=1; //zmienne pozwalajace okreslic czy w ogole wystepuje skaladnik "od_mieszanych"
		int znak=0; //zmienna sluzaca do okreslenia znaku skladnika "od_mieszanych" 
		int zf=1, zg=-1, zh=-1, zi=1; //okreslaja znaki przed wspolczynnikami ff fg fh fi w normalnej sytuacji przeplywu

		if (flag[2]==0) {tmpf=-1; zf=0;} // jezeli nie ma pkt 2 to przestawiam tmpf na -1 i ustawiam zmienna zf na 0 
		if (flag[8]==0) {tmpg=-1; zg=0;}
		if (flag[0]==0) {tmph=-1; zh=0;}
		if (flag[6]==0) {tmpi=-1; zi=0;}

		znak=zf+zg+zh+zi; // licze sume jesli np nie ma pkt 2 to mam 0-1-1+1 = -1; znak przy skladniku co cij musi byc przciwny czyli czyli +; 
				
		if ((tmpf*tmpg*tmph*tmpi)<0) od_mieszanych = -1*znak *CDxy; //gdy iloczyn <0 to dodaajemy dodatkowy czlon uwzgledniony we wpolczynniku przed cij
		// KONIEC ++++++++++++++++++++++++++++++++ od_miezanych  ++++++++++++++++++++++++++++++++//	


		// POCZATEK ---- w1 -------- w2 ----------- w3 ----------- w4 -------------------------//
		// fa = 1 + w1*Dx + w2*Dy + pd_mieszanych
		// a  = 1 + w3*Dx + w4*Dy - pd_mieszanych
//		float w1 = -1.0, w2 = -1.0, w3 = 1.0, w4 = 1.0;
		//gdy ktoregos brakuje
		//if (flag[5] == 0 || flag[3] == 0){ //brak pkt c[i+1,j] -> 5 || c[i-1,j] -> 3
	//		w1 = -0.5; //wspolczynnik przy Dx == -0.5
//			w3 = 0.5;	
//		} 

//		if (flag[1] == 0 || flag[7] == 0){ //brak pkt c[i,j+1] -> 1 || c[i,j-1] -> 7
//			w2 = -0.5; //wspolczynnik przy Dy == -1
//			w4 = 0.5;
//		}

//		//jezeli nie ma obu c[i+1,j] i c[i-1,j] 
//		if (flag[5] == 0 & flag[3] == 0) {w1 = 0; w3 = 0;}
		//jezeli nie ma obu c[i,j+1] i c[i,j-1] 
//		if (flag[1] == 0 & flag[7] == 0) {w2 = 0; w4 = 0;}

		// KONIEC ---- w1 -------- w2 ----------- w3 ----------- w4 -------------------------//
		

		// fa = 1 + w1*Dx + w2*Dy + pd_mieszanych
		// a  = 1 + w3*Dx + w4*Dy + pd_mieszanych
		//dla b gornego 2 lnijiki ponizej
		//p->set_factor_fij("fa", 1.0 + w1*CDx + w2*CDy - od_mieszanych + od_adv_y);
		//p->set_factor("a",  1.0 + w3*CDx + w4*CDy + od_mieszanych - od_adv_y);
		
		p->set_factor_fij("fa", 1.0 + w1*CDx + w2*CDy + od_mieszanych + od_adv_y + od_adv_x);
		p->set_factor("a",  1.0 + w3*CDx + w4*CDy - od_mieszanych - od_adv_y - od_adv_x);
		//cout<<"--------------Ustawiono wspolczynnik 'fa' -----------------------"<<endl
		//<<"-- fa = 1.0 + ( "<<w1<<" ) * CDx + ( "<<w2<<" ) * CDy + ( "<< od_mieszanych<<" )"<<endl;
		//cout<<"--------------Ustawiono wspolczynnik 'fa' -----------------------"<<endl
		//<<"-- a = 1.0 + ( "<<w3<<" ) * CDx + ( "<<w4<<" ) * CDy - ( "<< od_mieszanych<<" )"<<endl;

// to dzila ok dal gornego brzegu
		/*if(flag[1]==0) {
			cout<<" wub 6 gora "<<endl<<"flaga:"<<p->get_flag()<<endl;
			cout<<"CAx: "<<CAx<<" CAy: "<<CAy<<" CDx: "<<CDx<<" CDy: "<<CDy<<endl;
			cout<<"zmieniam x,y: "<<p->get_x()<<", "<<p->get_y()<<endl;
			p->set_factor_fij("fa", 1.0 - CDx - (CDy/2) + (CAy/4));
			p->set_factor_fij("fb", -(CAx/4) + (CDx/2));
			p->set_factor_fij("fc", (CAx/4) + (CDx/2));
			p->set_factor_fij("fd", 0);//fd przy i,j+1
			p->set_factor_fij("fe",  (CAy/4) + (CDy/2));//fe przy i,j-1	
			p->set_factor_fij("ff", 0);
			p->set_factor_fij("fg", -CDxy);
			p->set_factor_fij("fh", 0);
			p->set_factor_fij("fi", CDxy);
			p->set_factor("a",  1.0 + CDx + (CDy/2) - (CAy/4));
			p->set_factor("b", (CAx/4) - (CDx/2));
			p->set_factor("c", -(CAx/4) - (CDx/2));
			p->set_factor("d", 0);//fd przy i,j+1
			p->set_factor("e", - (CAy/4) - (CDy/2));//fe przy i,j-1
			p->set_factor("f", 0);
			p->set_factor("g", CDxy);
			p->set_factor("h", 0);
			p->set_factor("i", - CDxy);
			//p->set_factor_fij("fa", 1.0 + w1*CDx + w2*CDy + od_mieszanych + (CAy/4)); //p->set_factor_fij("a",  1.0 + w3*CDx + w4*CDy - od_mieszanych - (CAy/4)); //p->set_factor_fij("fa", 1.0 - (CAy/4) - CDx - (CDy/2));

		cout<<endl<<"--->-> wszytskie punkty dookola to przeplyw; oznaczono wiec pkt jako przeplyw"<<endl;
	cout<<endl<<"USTAWIONO Gorny:"<<endl;
	cout<<"		fa: "<<p->get_factor_f("fa")<<",	";
	cout<<"		fb: "<<p->get_factor_f("fb")<<",	";
	cout<<"		fc: "<<p->get_factor_f("fc")<<",	";
	cout<<"		fd: "<<p->get_factor_f("fd")<<",	";
	cout<<"		fe: "<<p->get_factor_f("fe")<<endl;
	cout<<"		ff: "<<p->get_factor_f("ff")<<",	";
	cout<<"		fg: "<<p->get_factor_f("fg")<<",	";
	cout<<"		fh: "<<p->get_factor_f("fh")<<",	";
	cout<<"		fi: "<<p->get_factor_f("fi")<<endl;
	cout<<"__________________________________________________________________________________"<<endl;

		}
//dla dolnego brzegu
	if(flag[7]==0) {
			cout<<" wub 6a "<<endl<<"flaga:"<<p->get_flag()<<endl;
			cout<<"CAx: "<<CAx<<" CAy: "<<CAy<<" CDx: "<<CDx<<" CDy: "<<CDy<<endl;
			cout<<"zmieniam x,y: "<<p->get_x()<<", "<<p->get_y()<<endl;
			p->set_factor_fij("fa", 1.0 - CDx - (CDy/2) - (CAy/4));
		//	p->set_factor_fij("fb", -(CAx/4) + (CDx/2));
		//	p->set_factor_fij("fc", (CAx/4) + (CDx/2));
			p->set_factor_fij("fd", - (CAy/4) + (CDy/2));//fd przy i,j+1
			p->set_factor_fij("fe", 0);//fe przy i,j-1	
			p->set_factor_fij("ff", CDxy);
			p->set_factor_fij("fg", 0);
			p->set_factor_fij("fh", -CDxy);
			p->set_factor_fij("fi", 0);
			p->set_factor("a",  1.0 + CDx + (CDy/2) + (CAy/4));
		//	p->set_factor("b", (CAx/4) - (CDx/2));
		//	p->set_factor("c", -(CAx/4) - (CDx/2));
			p->set_factor("d",  (CAy/4) - (CDy/2));//fd przy i,j+1
			p->set_factor("e", 0);//fe przy i,j-1
			p->set_factor("f", - CDxy);
			p->set_factor("g", 0);
			p->set_factor("h", CDxy);
			p->set_factor("i", 0);
			//p->set_factor_fij("fa", 1.0 + w1*CDx + w2*CDy + od_mieszanych + (CAy/4));
			//p->set_factor_fij("a",  1.0 + w3*CDx + w4*CDy - od_mieszanych - (CAy/4));
			//p->set_factor_fij("fa", 1.0 - (CAy/4) - CDx - (CDy/2));				

cout<<endl<<"--->-> wszytskie punkty dookola to przeplyw; oznaczono wiec pkt jako przeplyw"<<endl;
	cout<<endl<<"USTAWIONO dolny:"<<endl;
	cout<<"		fa: "<<p->get_factor_f("fa")<<",	";
	cout<<"		fb: "<<p->get_factor_f("fb")<<",	";
	cout<<"		fc: "<<p->get_factor_f("fc")<<",	";
	cout<<"		fd: "<<p->get_factor_f("fd")<<",	";
	cout<<"		fe: "<<p->get_factor_f("fe")<<endl;
	cout<<"		ff: "<<p->get_factor_f("ff")<<",	";
	cout<<"		fg: "<<p->get_factor_f("fg")<<",	";
	cout<<"		fh: "<<p->get_factor_f("fh")<<",	";
	cout<<"		fi: "<<p->get_factor_f("fi")<<endl;
	cout<<"__________________________________________________________________________________"<<endl;
	}

		//gdy brak pkt c[i-1,j] - ozn 3 
		//czyli gdy "bl", "rgl", "rdl", "szl", "szg", "szd"
		if(flag[3]==0) {
			p->set_factor_fij("fa", 1.0 - (CDx/2) - (CDy));
			p->set_factor("a",  1.0 + (CDx/2) + (CDy));
			p->set_factor_fij("fc", 0);//fc przy i-1,j
			p->set_factor_fij("fb", (CDx/2));//fb przy i+1,j
			p->set_factor("c", 0);//fc przy i-1,j
			p->set_factor("b", -(CDx/2));//fb przy i+1,j
cout<<endl<<"--->-> wszytskie punkty dookola to przeplyw; oznaczono wiec pkt jako przeplyw"<<endl;
	cout<<endl<<"USTAWIONO dolny:"<<endl;
	cout<<"		fa: "<<p->get_factor_f("fa")<<",	";
	cout<<"		fb: "<<p->get_factor_f("fb")<<",	";
	cout<<"		fc: "<<p->get_factor_f("fc")<<",	";
	cout<<"		fd: "<<p->get_factor_f("fd")<<",	";
	cout<<"		fe: "<<p->get_factor_f("fe")<<endl;
	cout<<"		ff: "<<p->get_factor_f("ff")<<",	";
	cout<<"		fg: "<<p->get_factor_f("fg")<<",	";
	cout<<"		fh: "<<p->get_factor_f("fh")<<",	";
	cout<<"		fi: "<<p->get_factor_f("fi")<<endl;
	cout<<"__________________________________________________________________________________"<<endl;

		} 
*/

/*		if(flag[1]==0) {
			cout<<"zmieniam x,y: "<<p->get_x()<<", "<<p->get_y()<<endl;
			p->set_factor_fij("fa", 1.0 - CDx - (CDy/2));
			p->set_factor_fij("fb", -(CAx/4) + (CDx/2));
			p->set_factor_fij("fc", (CAx/4) + (CDx/2));
			p->set_factor_fij("fd", 0);//fd przy i,j+1
			p->set_factor_fij("fe", (CDy/2));//fe przy i,j-1	
			p->set_factor_fij("ff", 0);
			p->set_factor_fij("fg", -CDxy);
			p->set_factor_fij("fh", 0);
			p->set_factor_fij("fi", CDxy);
			p->set_factor("a",  1.0 + CDx + (CDy/2));
			p->set_factor("b", (CAx/4) - (CDx/2));
			p->set_factor("c", -(CAx/4) - (CDx/2));
			p->set_factor("d", 0);//fd przy i,j+1
			p->set_factor("e", -(CDy/2));//fe przy i,j-1
			p->set_factor("f", 0);
			p->set_factor("g", CDxy);
			p->set_factor("h", 0);
			p->set_factor("i", - CDxy);
			//p->set_factor_fij("fa", 1.0 + w1*CDx + w2*CDy + od_mieszanych + (CAy/4));
			//p->set_factor_fij("a",  1.0 + w3*CDx + w4*CDy - od_mieszanych - (CAy/4));
			//p->set_factor_fij("fa", 1.0 - (CAy/4) - CDx - (CDy/2));
				
		}
*/

/*		if(flag[1]==0) {
			cout<<"zmieniam x,y: "<<p->get_x()<<", "<<p->get_y()<<endl;
			p->set_factor_fij("fa", 1.0 - CDx - (CDy/2)- (CAy/2));
			p->set_factor_fij("fb", -(CAx/4) + (CDx/2));
			p->set_factor_fij("fc", (CAx/4) + (CDx/2));
			p->set_factor_fij("fd", 0);//fd przy i,j+1
			p->set_factor_fij("fe", (CAy/2) + (CDy/2));//fe przy i,j-1	
			p->set_factor_fij("ff", 0);
			p->set_factor_fij("fg", -CDxy);
			p->set_factor_fij("fh", 0);
			p->set_factor_fij("fi", CDxy);
			p->set_factor("a",  1.0 + CDx + (CDy/2) + (CAy/2));
			p->set_factor("b", (CAx/4) - (CDx/2));
			p->set_factor("c", -(CAx/4) - (CDx/2));
			p->set_factor("d", 0);//fd przy i,j+1
			p->set_factor("e", -(CAy/2)-(CDy/2));//fe przy i,j-1
			p->set_factor("f", 0);
			p->set_factor("g", CDxy);
			p->set_factor("h", 0);
			p->set_factor("i", - CDxy);
			//p->set_factor_fij("fd", 0);//fd przy i,j+1
			//p->set_factor_fij("fe", (CAy/2)+(CDy/2));//fe przy i,j-1		
			//p->set_factor("d", 0);//fd przy i,j+1
			//p->set_factor("e", -(CAy/2)-(CDy/2));//fe przy i,j-1
			//p->set_factor_fij("fa", 1.0 - (CAy/2) - CDx - (CDy/2));
			//p->set_factor("a",  1.0 + (CAy/2) + CDx + (CDy/2));			
			//p->set_factor_fij("fa", 1.0 + w1*CDx + w2*CDy + od_mieszanych + (CAy/4));
			//p->set_factor_fij("a",  1.0 + w3*CDx + w4*CDy - od_mieszanych - (CAy/4));
		}
*/


	}	

		

};
 
//_________________________________________________________________________________________________________ 

 
//***************************************************SUpWind***********************************************
 
 void SUpWind::ComputeConcentration (map <int, Point> & PointMap){
 
	long double cn1 = 0.0;
	int index = Grid::empty_index;
	Point *p = &(PointMap.find(index)->second);

	
	for (int i = Grid::_xmin; i <= Grid::_xmax + 1; i++){
		for (int j = Grid::_ymin; j <= Grid::_ymax + 1; j++){
			index = i * 10000 + j;
			p = &(PointMap.find(index)->second);
			if (p->get_flag() != 0) {
				cn1 = p->get_factor_f("fa") * p->get_Curr() +
					  p->get_factor_f("fb") * (PointMap.find(p->get_near_neighbour(5)))->second.get_Curr() +
					  p->get_factor_f("fc") * (PointMap.find(p->get_near_neighbour(3)))->second.get_Curr() +
					  p->get_factor_f("fd") * (PointMap.find(p->get_near_neighbour(1)))->second.get_Curr() +
					  p->get_factor_f("fe") * (PointMap.find(p->get_near_neighbour(7)))->second.get_Curr() +
					  p->get_factor_f("ff") * (PointMap.find(p->get_near_neighbour(2)))->second.get_Curr() +
					  p->get_factor_f("fg") * (PointMap.find(p->get_near_neighbour(8)))->second.get_Curr() +
					  p->get_factor_f("fh") * (PointMap.find(p->get_near_neighbour(0)))->second.get_Curr() +
					  p->get_factor_f("fi") * (PointMap.find(p->get_near_neighbour(6)))->second.get_Curr();
					  //pamietac zew przyszlosci trzeba dodac jeszcze czlony z distatnt neighbeurs
				p->set_Next(cn1);
			}
		}
	}
	
	long double val = -9999.99, suma = 0.0, suma1 = 0.0;
	int pktprzed = 0, pktpo = 0;
	map<int,Point>::iterator IT;
	
	
	///10.06.2015 - dodajemy czlon zrodluwy
	float Hsr = 2.0;//srednia glebokosc
	long double  Dodatek;
//	double T0 = 10.4;
//	NetHeatFlux NHeat(10.5, 89.7, 997.8, 62.0, 0.0);
	double T0 = 8.9;
	NetHeatFlux NHeat(9.2, 92.9, 998.8, 464.0, 1.8);
	long double Q0 = NHeat.ComputeNetHeatFlux(T0);
	long double Q1 = 0.0;
	double ro = 999.73;//wartosc gestosci wody [kg/m3] - w przyszlosci zrobic tabele
	double cp = 4182.0; //wartosc ciepla wlasciwego wody [J/(kg*C)] - w przyszlosci zrobic tabele
	long double dT0 = 0.0, dtp = 0.0;
	dT0 = Q0/(ro*cp*Hsr); //pamietac w przyszlosci o kroku czasowym gdy rozny od 1s
	//cout<<"zmiana temp w punkcie o temp T0: "<<dT0<<endl;	
	///10.06.2015 - dodajemy czlon zrodluwy

	for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
		val = IT->second.get_Next();
		suma = suma + val;
		pktprzed ++;
		
		//12.06.2015 dodalam warunke ze lda ujemnych temperatur liczy jak dla tempeertury poczatkowej
		if (val > 0.0) Q1 = NHeat.ComputeNetHeatFlux(val + T0);
		else { val = 0.0; Q1 = Q0;}//zeruje ujemne wartosci 
		
		dtp = Q1/(ro*cp*Hsr);
		Dodatek = dtp - dT0;
		val = val + Dodatek;
		//12.06.2015 dodalam warunke ze lda ujemnych temperatur liczy jak dla tempeertury poczatkowej
		
		//if (val > 1e-30) { //nie przepisujemy b malych wartosci
			IT->second.set_Curr(val);
			suma1 = suma1 + IT->second.get_Curr();
			pktpo ++;
		//}
	}
	//ofstream out("Info.dat", ios::app);
	//out<<"  suma: "<<suma<<"  suma1: "<<suma1<<"  przed: "<<pktprzed<<"  po: "<<pktpo<<endl<<endl;
	//out.close();
 };

 //_________________________________________________________________________________________________________
 void SUpWind::GetFactors(Point *p, const double &CAx, const double &CAy, const double &CDx, const double &CDxy, const double &CDy, const vector<int> &flag){
   //cout<<endl<<endl<<"SUpWind::GetFactors"<<endl;
   int indx;
   //cout<<"x: "<<p->get_x()<<"    y: "<<p->get_y()<<endl;
   //cout << "===----------------------"<<endl;
   //for (indx = 0; indx < flag.size(); indx++)
   //{
   //cout << indx<< " : " <<flag[indx] <<  endl;
   //}
   //			cout << "===----------------------"<<endl; 			
   //-------------------------------------------------------

 	double vx = p->get_vx(), vy = p->get_vy();

	if (vx >= 0){
		if (vy >= 0) p->set_factor_fij("fa", 1.0 - CAx - CAy - 2.0*CDx - 2.0*CDy);//fa przy i,j
		else p->set_factor_fij("fa", 1.0 - CAx + CAy - 2.0*CDx - 2.0*CDy);//fa przy i,j
	}
	else {
		if (vy >= 0) p->set_factor_fij("fa", 1.0 + CAx - CAy - 2.0*CDx - 2.0*CDy);//fa przy i,j
		else p->set_factor_fij("fa", 1.0 + CAx + CAy - 2.0*CDx - 2.0*CDy);//fa przy i,j
	}

	if (vx > 0){
		p->set_factor_fij("fb", CDx);//fb przy i+1,j
		p->set_factor_fij("fc", CAx + CDx);//fc przy i-1,j
	}
	else {
		p->set_factor_fij("fb", (-CAx) + CDx);//fb przy i+1,j
		p->set_factor_fij("fc", CDx);//fc przy i-1,j
	}

	if (vy > 0){
		p->set_factor_fij("fd", CDy);//fd przy i,j+1
		p->set_factor_fij("fe", CAy + CDy);//fe przy i,j-1
	}
	else {
		p->set_factor_fij("fd", -CAy + CDy);//fd przy i,j+1
		p->set_factor_fij("fe", CDy);//fe przy i,j-1
	}

	//to bylao	
//	p->set_factor_fij("ff", CDxy/2);//ff przy i+1,j+1
//	p->set_factor_fij("fg", -CDxy/2);//fg przy i+1,j-1
//	p->set_factor_fij("fh", -CDxy/2);//fh przy i-1,j+1
//	p->set_factor_fij("fi", CDxy/2);//fi przy i-1,j-1

	//to chyba byc powinno <= ze wzru
	p->set_factor_fij("ff", 2*CDxy);//ff przy i+1,j+1
	p->set_factor_fij("fg", -2*CDxy);//fg przy i+1,j-1
	p->set_factor_fij("fh", -2*CDxy);//fh przy i-1,j+1
	p->set_factor_fij("fi", 2*CDxy);//fi przy i-1,j-1


//	p->set_factor_fij("ff", CDxy);//ff przy i+1,j+1
//	p->set_factor_fij("fg", -CDxy);//fg przy i+1,j-1
	//p->set_factor_fij("fh", -CDxy);//fh przy i-1,j+1
	//p->set_factor_fij("fi", CDxy);//fi przy i-1,j-1

//	p->set_factor_fij("ff", CDxy);//ff przy i+1,j+1
//	p->set_factor_fij("fg", CDxy);//fg przy i+1,j-1
	//p->set_factor_fij("fh", CDxy);//fh przy i-1,j+1
	//p->set_factor_fij("fi", CDxy);//fi przy i-1,j-1

		
	p->set_factor_fij("fj", 0);//fj przy i+2,j
	p->set_factor_fij("fk", 0);//fk przy i-2,j
	p->set_factor_fij("fl", 0);//fl przy i,j+2
	p->set_factor_fij("fm", 0);//fm przy i,j-2
	
	//reszta 0 bo schamat jest jawny
	p->set_factor("a", 0);//a przy i,j
	p->set_factor("b", 0);//b przy i+1,j
	p->set_factor("c", 0);//c przy i-1,j
	p->set_factor("d", 0);//d przy i,j+1
	p->set_factor("e", 0);//e przy i,j-1
	p->set_factor("f", 0);//f przy i+1,j+1
	p->set_factor("g", 0);//g przy i+1,j-1
	p->set_factor("h", 0);//h przy i-1,j+1
	p->set_factor("i", 0);//i przy i-1,j-1
	p->set_factor("j", 0);//j przy i+2,j
	p->set_factor("k", 0);//k przy i-2,j
	p->set_factor("l", 0);//l przy i,j+2
	p->set_factor("m", 0);//m przy i,j-2

	//cout<<"Ustawiono wstepnie wartosci dla punktu jak wewnatrz przeplywu"<<endl;
 	
	if (flag[1]==0 || flag[2]==0 || flag[3]==0 || flag[5]==0 || flag[6]==0 || flag[7]==0 || flag[8]==0 || flag[0]==0){
//	if (flag[1]!=1 || flag[2]!=1 || flag[3]!=1 || flag[5]!=1 || flag[6]!=1 || flag[7]!=1 || flag[8]!=1 || flag[0]!=1){
//		cout<<"UWAG - BRAK KTOREGOS Z PKTOW -> musze oznaczyc JAK brzegi"<<endl;

									//	* * * * *
  									//	* 0 1 2 *		0->(i-1,j+1)	1->(i,j+1)	2->(i+1,j+1)
  									//	* 3 4 5 *		3->(i-1,j)		4->(i,j)	5->(i+1,j) 
									//	* 6 7 8 *		6->(i-1,j-1)	7->(i,j-1)	8->(i+1,j-1)
									//	* * * * *		

//-----------------------------------------------------------------------------------
		//wspolczennik "fb" - zmienia sie tylko gdy brak pkt c[i+1,j] - ozn 5 
		//czyli gdy "bp", "rgp", "rdp", "szp", "szg", "szd"
		if(flag[5]==0) p->set_factor_fij("fb", 0);//fb przy i+1,j 

		//wspolczennik "fc" - zmienia sie tylko gdy brak pkt c[i-1,j] - ozn 3 
		//czyli gdy "bl", "rgl", "rdl", "szl", "szg", "szd"
		if(flag[3]==0) p->set_factor_fij("fc", 0);//fc przy i-1,j 

		//wspolczennik "fd" - zmienia sie tylko gdy brak pkt c[i,j+1] - ozn 1 
		//czyli gdy "bg", "rgl", "rgp", "szl", "szg", "szp"
		if(flag[1]==0) p->set_factor_fij("fd", 0);//fd przy i,j+1 

		//wspolczennik "fe" - zmienia sie tylko gdy brak pkt c[i,j-1] - ozn 7 
		//czyli gdy "bd", "rdl", "rdp", "szl", "szp", "szd"
		if(flag[7]==0) p->set_factor_fij("fe", 0);//fe przy i,j-1 
//-----------------------------------------------------------------------------------
		

//---------------WSPOLCZYNNIKI PRZY PKT POCHODZACYCH Z DYSKRET. POCHODNYCH MIESZANYCH --------------//
		//wspolczynnik "ff" - zmienia sie tylko przy braku pkt c[i+1,j+1] - ozn 2
		//czyli "bg", "bp" i "szp", "rgl", "rgp", "rdp", "kgp" iin gdzie brakuje pkt 2
		if(flag[2]==0) p->set_factor_fij("ff", 0);

		//wspolczynnik "fg" - zmienia sie tylko przy braku pkt c[i+1,j-1] - ozn 8
		//czyli "bg", "bp" i "szp", "rgl", "rgp", "rdp", "kgp" iin gdzie brakuje pkt 8
		if(flag[8]==0) p->set_factor_fij("fg", 0);

		//wspolczynnik "fh" - zmienia sie tylko przy braku pkt c[i-1,j+1] - ozn 0
		//czyli "bg", "bp" i "szp", "rgl", "rgp", "rdp", "kgp" iin gdzie brakuje pkt 0
		if(flag[0]==0) p->set_factor_fij("fh", 0);

		//wspolczynnik "fi" - zmienia sie tylko przy braku pkt c[i-1,j-1] - ozn 6
		//czyli "bg", "bp" i "szp", "rgl", "rgp", "rdp", "kgp" iin gdzie brakuje pkt 6
		if(flag[6]==0) p->set_factor_fij("fi", 0);
//--------------------------------------------------------------------------------------------------//
	

//-------WSPOLCZYNNIK "fa" PRZED C[I,J] - ZMIENIA SIE GDY BRAK JAKIEGOKOLWIEK PKT I ZALEZY OD ZNAKU PREDKOSCI ----//

		// fa = 1 + w1*Ax + w2*Ay + w3*Dx + w4*Dy + od_mieszanych

		// POCZATEK ***************************** od_miezanych  *******************************//
		//***********czesc od mieszanych pochodnych we wspolczyniku "fa" ***************//
		double od_mieszanych = 0.0; //skladnik pochodzacy od mieszanych pochodnych we spolczyniiku przed cij
		int tmpf=1, tmpg=1, tmph=1, tmpi=1; //zmienne pozwalajace okreslic czy w ogole wystepuje skaladnik "od_mieszanych"
		int znak=0; //zmienna sluzaca do okreslenia znaku skladnika "od_mieszanych" 
		int zf=1, zg=-1, zh=-1, zi=1; //okreslaja znaki przed wspolczynnikami ff fg fh fi w normalnej sytuacji przeplywu

		if (flag[2]==0) {tmpf=-1; zf=0;} // jezeli nie ma pkt 2 to przestawiam tmpf na -1 i ustawiam zmienna zf na 0 
		if (flag[8]==0) {tmpg=-1; zg=0;}
		if (flag[0]==0) {tmph=-1; zh=0;}
		if (flag[6]==0) {tmpi=-1; zi=0;}

		znak=zf+zg+zh+zi; // licze sume jesli np nie ma pkt 2 to mam 0-1-1+1 = -1; znak przy skladniku co cij musi byc przciwny czyli czyli +; 
				
		if ((tmpf*tmpg*tmph*tmpi)<0) od_mieszanych = -1*znak *2*CDxy; //gdy iloczyn <0 to dodaajemy dodatkowy czlon uwzgledniony we wpolczynniku przed cij
		// KONIEC ******************************* od_miezanych  *******************************//	

		// POCZATEK **** w1 ********** w2 *********** w3 ************* w4 **********************//
		// fa = 1 + w1*Ax + w2*Ay + w3*Dx + w4*Dy + pd_mieszanych
		int w1 = 0, w2 = 0, w3 = -2, w4 = -2;
		//w sytuacji gdy sa wszystkie punkty 1, 5, 7 i 3
		if (vx >= 0) w1 = -1; else w1 = 1;
		if (vy >= 0) w2 = -1; else w2 = 1; //tu byl blad po else bylo w1 = 1
		
		//gdy ktoregos brakuje
		if (flag[5] == 0){ //brak pkt c[i+1,j] -> 5
			w3 = -1; //wspolczynnik przy Dx == -1
			if (vx >= 0) w1 = 0; // wspolczynnnik przy Ax = 0 
			else w1 = 1; // wspolczynnnik przy Ax = 1
		} 

		if (flag[3] == 0){ //brak pkt c[i-1,j] -> 3
			w3 = -1; //wspolczynnik przy Dx == -1
			if (vx >= 0) w1 = -1; // wspolczynnnik przy Ax = -1 
			else w1 = 0; // wspolczynnnik przy Ax = 0
		} 

		if (flag[1] == 0){ //brak pkt c[i,j+1] -> 1
			w4 = -1; //wspolczynnik przy Dy == -1
			if (vy >= 0) w2 = 0; // wspolczynnnik przy Ay = 0 
			else w2 = 1; // wspolczynnnik przy Ay = 1
		}

		if (flag[7] == 0){ //brak pkt c[i,j-1] -> 7
			w4 = -1; //wspolczynnik przy Dy == -1
			if (vy >= 0) w2 = -1; // wspolczynnnik przy Ay = -1 
			else w2 = 0; // wspolczynnnik przy Ay = 0   //nastepny blad bylo w1=0 a nei w2
		}

		//jezeli nie ma obu c[i+1,j] i c[i-1,j] 
		if (flag[5] == 0 & flag[3] == 0) {w1 = 0; w3 = 0;}
		//jezeli nie ma obu c[i,j+1] i c[i,j-1] 
		if (flag[1] == 0 & flag[7] == 0) {w2 = 0; w4 = 0;}

		// KONIEC ****** w1 ********** w2 *********** w3 ************* w4 *******************//
		

		// fa = 1 + w1*Ax + w2*Ay + w3*Dx + w4*Dy + pd_mieszanych
		p->set_factor_fij("fa", 1.0 + w1*CAx + w2*CAy + w3*CDx + w4*CDy + od_mieszanych);
		//cout<<"--------------Ustawiono wspolczynnik 'fa' -----------------------"<<endl
		//<<"-- fa = 1.0 + ( "<<w1<<" ) * CAx + ( "<<w2<<" ) * CAy + ( "<<w3<<" ) * CDx + ( "<<w4<<" ) * CDy + ( "<< od_mieszanych<<" )"<<endl;
		

//-----------------------------------------------------------------------------------------------------------// 
	
	}
	//cout<<endl<<"--->-> wszytskie punkty dookola to przeplyw; oznaczono wiec pkt jako przeplyw"<<endl;
	//cout<<endl<<"USTAWIONO:"<<endl;
	//cout<<"		fa: "<<p->get_factor_f("fa")<<",	";
	//cout<<"		fb: "<<p->get_factor_f("fb")<<",	";
	//cout<<"		fc: "<<p->get_factor_f("fc")<<",	";
	//cout<<"		fd: "<<p->get_factor_f("fd")<<",	";
	//cout<<"		fe: "<<p->get_factor_f("fe")<<endl;
	//cout<<"		ff: "<<p->get_factor_f("ff")<<",	";
	//cout<<"		fg: "<<p->get_factor_f("fg")<<",	";
	//cout<<"		fh: "<<p->get_factor_f("fh")<<",	";
	//cout<<"		fi: "<<p->get_factor_f("fi")<<endl;
	//cout<<"__________________________________________________________________________________"<<endl;
	//gorny warunek brzegowy
	/*if (p->get_flag() == 2){
		
		p->set_factor_fij("fd", 0);//fd przy i,j+1
		
		if (vx >= 0){
			if (vy >= 0) p->set_factor_fij("fa", 1.0 - CAx - 2.0*CDx - CDy);//fa przy i,j
			else p->set_factor_fij("fa", 1.0 - CAx + CAy - 2.0*CDx - CDy);//fa przy i,j
		}
		else {
			if (vy >= 0) p->set_factor_fij("fa", 1.0 + CAx - 2.0*CDx - CDy);//fa przy i,j
			else p->set_factor_fij("fa", 1.0 + CAx + CAy - 2.0*CDx - CDy);//fa przy i,j
		}
				
		p->set_factor_fij("ff", 0);//ff przy i+1,j+1
		p->set_factor_fij("fg", -CDxy/2);//fg przy i+1,j-1
		p->set_factor_fij("fh", 0);//fh przy i-1,j+1
		p->set_factor_fij("fi", CDxy/2);//fi przy i-1,j-1
	}
	
	*/
	
	
 };

 
 //***************************************************SADI**************************************************
 
 //_________________________________________________________________________________________________________
 void SADI::IterativeSolveSetOfEquations (map <int, Point> & PointMap, map<int,long double> &f, vector<string> &a){
 	
	cout.precision(20);
 	long double ck1 = 0.0, tmps = 0.0, val = 0.0, change = 0.0;
	int index = Grid::empty_index;
	Point *p = &(PointMap.find(index)->second);
	map<int,Point>::iterator IT;  
	cout<<"Iteracja: ";
	for (int k = 0; k < 200; k++){
		cout<<k<<"	";
		change = 0.0;

		//this->set_omega(1.138);
		//wywolanie odpowiedniej metody iteracyjnej
		if (get_method() == "SOR") change = SOR (PointMap, f, a, this->get_omega());//1.13799999999999999);//1.138);//this->get_omega());
		else if (get_method() == "Gauss-S") change = Gauss_Seidel (PointMap, f, a);
		else  change = Jakobi(PointMap, f, a);
		
		for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
			val = IT->second.get_Next();
			//if (val > 1e-10) {
				IT->second.set_Curr(val);
			//}
		}
		if (change < 1e-6) break;
	}
	cout<<endl;
 };

//_________________________________________________________________________________________________________  
 void SADI::DirectSolveSetOfEquation1 (map <int, Point> & PointMap, map<int,long double> &f){
 	//metoda Thomasa
	int index = Grid::empty_index, help = 0;
	Point *p = &(PointMap.find(index)->second);
	map<int,Point>::iterator IT;  
 	long double u = 0.0, y = 0.0, l = 0.0, ck1 = 0.0;
	map<int,long double> uij;
	uij.clear();
	double bminus1 = 0.0;

	//2011T for (int j = Grid::_ymin; j <= Grid::_ymax; j++){
	//2011T	for (int i = Grid::_xmin; i <= Grid::_xmax; i++){

	  for (int j = Grid::_ymin; j < Grid::_ymax; j++){
		for (int i = Grid::_xmin; i < Grid::_xmax; i++){
			index = i * 10000 + j;
			p = &(PointMap.find(index)->second);	
			if (p->get_flag() != 0) {
				if ( help == 0 ) {
					u = p->get_factor("a");
					y = f[index];
					help = 1;
					bminus1=p->get_factor("b");
					
				}
				else {
					l = p->get_factor("c") / u;
					u = p->get_factor("a") - l * bminus1;//p->get_factor("b");
					y = f[index] - l * y;
					bminus1=p->get_factor("b");
				}
				f[index] = y;
				uij[index] = u;
			}
		}
	}	
	help = 0;
//2011T	for (int j = Grid::_ymax; j >= Grid::_ymin; j--) {	
//2011T		for (int i = Grid::_xmax; i >= Grid::_xmin; i--) {

	for (int j = Grid::_ymax -1 ; j >= Grid::_ymin; j--) {	
		for (int i = Grid::_xmax -1; i >= Grid::_xmin; i--) {
			index = i * 10000 + j;
			p = &(PointMap.find(index)->second);	
			if (p->get_flag() != 0) {
				if ( help == 0 ) {
					ck1 = f[index] / uij[index];	
					help =1;
				}
				else ck1 = (f[index] - p->get_factor("b") * ck1 ) / uij[index];
				p->set_Curr(ck1);
			}
		}	
	}
 };
  
 //_________________________________________________________________________________________________________
 void SADI::DirectSolveSetOfEquation2 (map <int, Point> & PointMap, map<int,long double> &f1){
 	//metoda Thomasa
	int index = Grid::empty_index, help = 0;
	Point *p = &(PointMap.find(index)->second);
	map<int,Point>::iterator IT;  
 	long double u = 0.0, y = 0.0, l = 0.0, ck1 = 0.0;
	map<int,long double> uij;
	uij.clear();
	double dminus1 = 0.0;
		
//2011T	for (int i = Grid::_xmin; i <= Grid::_xmax; i++){
//2011T		for (int j = Grid::_ymin; j <= Grid::_ymax; j++){

	for (int i = Grid::_xmin; i < Grid::_xmax; i++){
		for (int j = Grid::_ymin; j < Grid::_ymax; j++){
			index = i * 10000 + j;
			p = &(PointMap.find(index)->second);
			if (p->get_flag() != 0) {
				//f1[index] = p->get_Prev() + f1[index];	
				if ( help == 0 ) {
					u = p->get_factor("a1");
					y = f1[index];
					help = 1;
					dminus1 = p->get_factor("d1");
				}
				else {
				        
					l = p->get_factor("e1") / u;
					//if(help<5) l = 0.0;
					u = p->get_factor("a1") - l * dminus1;//p->get_factor("d1");
					y = f1[index] - l * y;
					//if(help<5){
					//  cout<<p->get_x()<<","<<p->get_y()<<":  ";
					//  cout<<"l:"<<l<<endl;
				        // help++;
					//}
					dminus1 = p->get_factor("d1");
				}
				f1[index] = y;
				uij[index] = u;
			}
		}
	}	
	help = 0;
	
//2011T	for (int i = Grid::_xmax; i >= Grid::_xmin; i--) {
//2011T		for (int j = Grid::_ymax; j >= Grid::_ymin; j--) {	
	
	for (int i = Grid::_xmax -1; i >= Grid::_xmin; i--) {
		for (int j = Grid::_ymax -1; j >= Grid::_ymin; j--) {	
			index = i * 10000 + j;
			p = &(PointMap.find(index)->second);	
			if (p->get_flag() != 0) {
				if (help == 0){
					ck1 = f1[index] / uij[index];	
					help ++;
				}
				else ck1 = (f1[index] - p->get_factor("d1") * ck1 ) / uij[index];
				p->set_Next(ck1);
			}
		}	
	}
 };
 
 //_________________________________________________________________________________________________________
 void SADI::ComputeConcentration (map <int, Point> & PointMap){
 	
	cout.precision(20);
	long double fij = 0.0, ck1 = 0.0, val = 0.0;
	map<int,long double> f, f1; //wektory wyrazow wolnych w ukladach rownan 
	int index = Grid::empty_index;
	Point *p = &(PointMap.find(index)->second);
	map<int,Point>::iterator IT;  

	//ofstream out("Info.dat", ios::app);
  
	for (int i = Grid::_xmin; i <= Grid::_xmax + 1; i++){
		for (int j = Grid::_ymin; j <= Grid::_ymax + 1; j++){
			index = i * 10000 + j;
			p = &(PointMap.find(index)->second);
			if (p->get_flag() != 0) {
				//wektor wyrazow wolnych dla pierwszego ukladu
				fij = p->get_factor_f("fa") * p->get_Curr() +
					  p->get_factor_f("fb") * (PointMap.find(p->get_near_neighbour(5)))->second.get_Curr() +
					  p->get_factor_f("fc") * (PointMap.find(p->get_near_neighbour(3)))->second.get_Curr() +
					  p->get_factor_f("fd") * (PointMap.find(p->get_near_neighbour(1)))->second.get_Curr() +
					  p->get_factor_f("fe") * (PointMap.find(p->get_near_neighbour(7)))->second.get_Curr() +
					  p->get_factor_f("ff") * (PointMap.find(p->get_near_neighbour(2)))->second.get_Curr() +
					  p->get_factor_f("fg") * (PointMap.find(p->get_near_neighbour(8)))->second.get_Curr() +
					  p->get_factor_f("fh") * (PointMap.find(p->get_near_neighbour(0)))->second.get_Curr() +
					  p->get_factor_f("fi") * (PointMap.find(p->get_near_neighbour(6)))->second.get_Curr();
					  //pamietac zew przyszlosci trzeba dodac jeszcze czlony z distatnt neighbeurs
				f[index] = fij;
				// wektor wyrazow wolnych dla drugiego ukladu
				fij = p->get_factor_f("fa1") * p->get_Curr() +
					  p->get_factor_f("fb1") * (PointMap.find(p->get_near_neighbour(5)))->second.get_Curr() +
					  p->get_factor_f("fc1") * (PointMap.find(p->get_near_neighbour(3)))->second.get_Curr() +
					  p->get_factor_f("fd1") * (PointMap.find(p->get_near_neighbour(1)))->second.get_Curr() +
					  p->get_factor_f("fe1") * (PointMap.find(p->get_near_neighbour(7)))->second.get_Curr() +
					  p->get_factor_f("ff1") * (PointMap.find(p->get_near_neighbour(2)))->second.get_Curr() +
					  p->get_factor_f("fg1") * (PointMap.find(p->get_near_neighbour(8)))->second.get_Curr() +
					  p->get_factor_f("fh1") * (PointMap.find(p->get_near_neighbour(0)))->second.get_Curr() +
					  p->get_factor_f("fi1") * (PointMap.find(p->get_near_neighbour(6)))->second.get_Curr();
					  //pamietac zew przyszlosci trzeba dodac jeszcze czlony z distatnt neighbeurs
				f1[index] = fij;
			}
		}
	}
	
	for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
		val = IT->second.get_Curr();
		IT->second.set_Prev(val);
		IT->second.set_Curr(0);
		IT->second.set_Next(0);
	}
	
//-----------------pierwsze rownanie----------------------------------------------------
	vector<string> a;
	a.push_back("a"); a.push_back("b"); a.push_back("c"); a.push_back("d"); a.push_back("e");
	a.push_back("f"); a.push_back("g"); a.push_back("h"); a.push_back("i"); 

	//wywolanie metody nrozw ukl rownan
	if (get_method() == "Thomasa") this->DirectSolveSetOfEquation1(PointMap, f);
	else this->IterativeSolveSetOfEquations(PointMap, f, a);
	
	//przepsiaanie i modyfikacja wektora wyrazow w olnych w drugim rownniu o obliczona wartosc z pierwszego w kazdym pkt
	for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
		val = IT->second.get_Curr();
		IT->second.set_Prev(val);
		index = IT->first;
		f1[index] = f1[index] + val;
		IT->second.set_Curr(0);
		IT->second.set_Next(0);
	}
//------------------------------------------------------------------------------------------
	
//------------------drugie rownano----------------------------------------------------------
	a.clear();
	a.push_back("a1"); a.push_back("b1"); a.push_back("c1"); a.push_back("d1"); a.push_back("e1");
	a.push_back("f1"); a.push_back("g1"); a.push_back("h1"); a.push_back("i1"); 
	
	//wywolanie odpowieniej methody
	if (get_method() == "Thomasa") this->DirectSolveSetOfEquation2(PointMap, f1);
	else this->IterativeSolveSetOfEquations (PointMap, f1, a);
	//this->IterativeSolveSetOfEquations2(PointMap, f1);			
//-------------------------------------------------------------------------------------------

//----------------------prepisanie wartosci Next na Curent----------------------------------

	
	///------------10.06.2015 - dodajemy czlon zrodluwy
	float Hsr = 2.0;//srednia glebokosc
	long double  Dodatek;
//	double T0 = 10.4;
//	NetHeatFlux NHeat(10.5, 89.7, 997.8, 62.0, 0.0);
	double T0 = 8.9;
	NetHeatFlux NHeat(9.2, 92.9, 998.8, 464.0, 1.8);

	long double Q0 = NHeat.ComputeNetHeatFlux(T0);
	long double Q1 = 0.0;
	double ro = 999.73;//wartosc gestosci wody [kg/m3] - w przyszlosci zrobic tabele
	double cp = 4182.0; //wartosc ciepla wlasciwego wody [J/(kg*C)] - w przyszlosci zrobic tabele
	long double dT0 = 0.0, dtp = 0.0;
	dT0 = Q0/(ro*cp*Hsr); //pamietac w przyszlosci o kroku czasowym gdy rozny od 1s
	//cout<<"zmiana temp w punkcie o temp T0: "<<dT0<<endl;
	///------------10.06.2015 - dodajemy czlon zrodluwy		
	
	val = -9999.99;
	long double suma = 0.0, suma1 = 0.0;
	int pktprzed = 0, pktpo = 0;
	for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
		val = IT->second.get_Next();
		suma = suma + val;
		pktprzed ++;

/*		if ((dtp-dT0) != 0.0){
		  cout<<"     T0: "<<T0<<" val: "<<val<<" val+T0: "<<val+T0<<endl;
		  cout<<"     "<<"Q0: "<<Q0<<" Q1: "<<Q1<<" Q1 - Q0 :"<<Q1-Q0<<endl;
		  cout<<"     "<<"dT0: "<<dT0<<" dtp: "<<dtp<<" dtp - dT0: "<<dtp-dT0<<endl;
		}
		*/
		
		///----------LICZENIE HEAT FLUX --------------------//
		Q1 = NHeat.ComputeNetHeatFlux(val + T0);
		dtp = Q1/(ro*cp*Hsr);
		Dodatek = dtp - dT0;
		val = val + Dodatek;
		///----------LICZENIE HEAT FLUX --------------------//
		
		
		
//		if (fabs(val) > 1e-40) {
		if (fabs(val) > 1e-10 && val >= 0) { // zmiana dokladnosci - nie ignoruje berdzo malych wartosci, //usuwa wartosci ujemne  9.3.2010)
//		if (fabs(val) > 1e-10) { // zmiana dokladnosci - nie ignoruje berdzo malych wartosci, // nie usuwa wartosci ujemnych  5.10.2010)
			IT->second.set_Curr(val);
			suma1 = suma1 + IT->second.get_Curr();
			pktpo ++;
			//TEST 8 03 2010 - uwzgledninie warunkow brzegowych
			if (IT->second.get_flag() > 600 && IT->second.get_flag() < 900){
				IT->second.set_Curr(0);
				cout<<"flaga pkt: "<<IT->second.get_flag()<<"ustawiono wartosc zero dla pkt: "<<IT->second.get_x()<<","<<IT->second.get_y()<<endl; 
			}
		}
		IT->second.set_Prev(0);
		IT->second.set_Next(0);
	}
//----------------------prepisanie wartosci Next na Curent END----------------------------------

//cout<<endl<<"  suma: "<<suma<<"  suma1: "<<suma1<<"  przed: "<<pktprzed<<"  po: "<<pktpo<<endl<<endl;	
 	//out.close();

 };

 //_________________________________________________________________________________________________________
 void SADI::GetFactors(Point *p, const double &CAx, const double &CAy, const double &CDx, const double &CDxy, const double &CDy, const vector<int> &flag){
	
	//dla kroku n+1/2 czyli po x-saxh
	p->set_factor_fij("fa", 1 - 2*CDy - CDx);//fa przy i,j
	p->set_factor_fij("fb", (-CAx/4) + (CDx/2));//fb przy i+1,j
	p->set_factor_fij("fc", (CAx/4) + (CDx/2));//fc przy i-1,j
	p->set_factor_fij("fd", (-CAy/2) + (CDy));//fd przy i,j+1
	p->set_factor_fij("fe", (CAy/2) + (CDy));//fe przy i,j-1
	
	//---------opcja II ze wspolczynnikami przy Dxy dodatkowow pomnozylam----//
	p->set_factor_fij("ff", 2*CDxy);//ff przy i+1,j+1
	p->set_factor_fij("fg", -2*CDxy);//fg przy i+1,j-1
	p->set_factor_fij("fh", -2*CDxy);//fh przy i-1,j+1
	p->set_factor_fij("fi", 2*CDxy);//fi przy i-1,j-1
	//-----------------------------------------------//
	
	//---------opcja I ze wspolczynnikami przy Dxy----//
	/*p->set_factor_fij("ff", CDxy);//ff przy i+1,j+1
	p->set_factor_fij("fg", -CDxy);//fg przy i+1,j-1
	p->set_factor_fij("fh", -CDxy);//fh przy i-1,j+1
	p->set_factor_fij("fi", CDxy);//fi przy i-1,j-1*/
	//-----------------------------------------------//
	
	p->set_factor_fij("fj", 0);//fj przy i+2,j
	p->set_factor_fij("fk", 0);//fk przy i-2,j
	p->set_factor_fij("fl", 0);//fl przy i,j+2
	p->set_factor_fij("fm", 0);//fm przy i,j-2
	
	p->set_factor("a", 1 + CDx);//a przy i,j
	p->set_factor("b", (CAx/4) - (CDx/2));//b przy i+1,j
	p->set_factor("c", -(CAx/4) - (CDx/2));//c przy i-1,j
	p->set_factor("d", 0);//d przy i,j+1
	p->set_factor("e", 0);//e przy i,j-1
	p->set_factor("f", 0);//f przy i+1,j+1
	p->set_factor("g", 0);//g przy i+1,j-1
	p->set_factor("h", 0);//h przy i-1,j+1
	p->set_factor("i", 0);//i przy i-1,j-1
	p->set_factor("j", 0);//j przy i+2,j
	p->set_factor("k", 0);//k przy i-2,j
	p->set_factor("l", 0);//l przy i,j+2
	p->set_factor("m", 0);//m przy i,j-2

	//dla kroku n+1 czyli po y-kach
	p->set_factor_fij("fa1", CDy);//fa przy i,j
	p->set_factor_fij("fb1", 0);//fb przy i+1,j
	p->set_factor_fij("fc1", 0);//fc przy i-1,j
	p->set_factor_fij("fd1", (CAy/4) - (CDy/2));//fd przy i,j+1
	p->set_factor_fij("fe1", -(CAy/4) - (CDy/2));//fe przy i,j-1
	p->set_factor_fij("ff1", 0);//ff przy i+1,j+1
	p->set_factor_fij("fg1", 0);//fg przy i+1,j-1
	p->set_factor_fij("fh1", 0);//fh przy i-1,j+1
	p->set_factor_fij("fi1", 0);//fi przy i-1,j-1
	p->set_factor_fij("fj1", 0);//fj przy i+2,j
	p->set_factor_fij("fk1", 0);//fk przy i-2,j
	p->set_factor_fij("fl1", 0);//fl przy i,j+2
	p->set_factor_fij("fm1", 0);//fm przy i,j-2
	
	p->set_factor("a1", 1 + CDy);//a przy i,j
	p->set_factor("b1", 0);//b przy i+1,j
	p->set_factor("c1", 0);//c przy i-1,j
	p->set_factor("d1", (CAy/4) - (CDy/2));//d przy i,j+1
	p->set_factor("e1", -(CAy/4) - (CDy/2));//e przy i,j-1
	p->set_factor("f1", 0);//f przy i+1,j+1
	p->set_factor("g1", 0);//g przy i+1,j-1

	p->set_factor("h1", 0);//h przy i-1,j+1
	p->set_factor("i1", 0);//i przy i-1,j-1
	p->set_factor("j1", 0);//j przy i+2,j
	p->set_factor("k1", 0);//k przy i-2,j
	p->set_factor("l1", 0);//l przy i,j+2
	p->set_factor("m1", 0);//m przy i,j-2

/*	if ((flag[4] > 600 && flag[4] < 700) || (flag[4] > 800 && flag[4] < 900)){ 
		cout<<"wejscie x,y: "<<p->get_x()<<","<<p->get_y()<<"  flaga:"<<p->get_flag()<<endl;
			//dla kroku n+1/2 czyli po x-saxh
		p->set_factor_fij("fa", 1);//fa przy i,j
		p->set_factor_fij("fb", 0);//fb przy i+1,j
		p->set_factor_fij("fc", 0);//fc przy i-1,j
		p->set_factor_fij("fd", 0);//fd przy i,j+1
		p->set_factor_fij("fe", 0);//fe przy i,j-1
	
		//---------opcja II ze wspolczynnikami przy Dxy dodatkowow pomnozylam----//
		p->set_factor_fij("ff", 0);//ff przy i+1,j+1
		p->set_factor_fij("fg", 0);//fg przy i+1,j-1
		p->set_factor_fij("fh", 0);//fh przy i-1,j+1
		p->set_factor_fij("fi", 0);//fi przy i-1,j-1
		//-----------------------------------------------//
		p->set_factor("a", 0);//a przy i,j
		p->set_factor("b", 0);//b przy i+1,j
		p->set_factor("c", 0);//c przy i-1,j

		//dla kroku n+1 czyli po y-kach
		p->set_factor_fij("fa1", 0);//fa przy i,j
		p->set_factor_fij("fd1", 0);//fd przy i,j+1
		p->set_factor_fij("fe1", 0);//fe przy i,j-1

		p->set_factor("a1", 0);//a przy i,j
		p->set_factor("d1", 0);//d przy i,j+1
		p->set_factor("e1", 0);//e przy i,j-1

	}*/
	//if (p->get_flag()==802) cout<<"wyjscie x,y: "<<p->get_x()<<","<<p->get_y()<<"  flaga: 601"<<endl;
	//else cout<<"."<<endl;
/*
  //start - testowo zakomentowano 11.02.2010 - ze wzgledu na inne wrunki brzegowe dla tempoertaury -
	if (flag[1]==0 || flag[2]==0 || flag[3]==0 || flag[5]==0 || flag[6]==0 || flag[7]==0 || flag[8]==0 || flag[0]==0)
	{
  //		cout<<"UWAG - BRAK KTOREGOS Z PKTOW -> musze oznaczyc JAK brzegi"<<endl;
									//	* * * * *
									//	* 0 1 2 *		0->(i-1,j+1)	1->(i,j+1)	2->(i+1,j+1)
									//	* 3 4 5 *		3->(i-1,j)		4->(i,j)	5->(i+1,j) 
									//	* 6 7 8 *		6->(i-1,j-1)	7->(i,j-1)	8->(i+1,j-1)
									//	* * * * *		

//-----------------------------------------------------------------------------------
		float od_adv_y = 0.0, od_adv_x = 0.0; 
		float w1 = -1.0, w2 = -2.0, w3 = 1.0, w4 = 0.0;
		float w12 = 1.0, w14 = 1.0, od_adv_1y = 0.0;
		//gdy brak pkt c[i+1,j] - ozn 5 
		//czyli gdy "bp", "rgp", "rdp", "szp", "szg", "szd"
		if(flag[5]==0) {
			p->set_factor_fij("fb", 0);//fb przy i+1,j
			//p->set_factor_fij("fc", (CAx/4) + (CDx/2));//fc przy i-1,j
			p->set_factor("b", 0);//fb przy i+1,j
			//p->set_factor("c", - (CAx/4) - (CDx/2));//fc przy i-1,j
			od_adv_x = CAx/4;
			w1 = -0.5; //wspolczynnik przy Dx == -0.5
			w3 = 0.5;	
		} 

		//gdy brak pkt c[i-1,j] - ozn 3 
		//czyli gdy "bl", "rgl", "rdl", "szl", "szg", "szd"
		if(flag[3]==0) {
			p->set_factor_fij("fc", 0);//fc przy i-1,j
			//p->set_factor_fij("fb", - (CAx/4) + (CDx/2));//fb przy i+1,j
			p->set_factor("c", 0);//fc przy i-1,j
			//p->set_factor("b", (CAx/4) - (CDx/2));//fb przy i+1,j
			od_adv_x = - CAx/4;
			w1 = -0.5; //wspolczynnik przy Dx == -0.5
			w3 = 0.5;	
		} 

		//gdy brak pkt c[i,j+1] - ozn 1 
		//czyli gdy "bg", "rgl", "rgp", "szl", "szg", "szp"
		if(flag[1]==0) {
		  //cout<<endl<<"Brzeg gorny "<<endl;
		  //cout<<"x: "<<p->get_x()<<"  y: "<<p->get_y()<<endl; 
			p->set_factor_fij("fd", 0);//fd przy i,j+1
			p->set_factor_fij("fd1", 0);//fd1 przy i,j+1
			//p->set_factor_fij("fe",(CAy/4) + (CDy/2));//fe przy i,j-1		
			p->set_factor("d1", 0);//fd1 przy i,j+1
			//p->set_factor("e", - (CAy/4) - (CDy/2));//fe przy i,j-1
			od_adv_y = CAy/2;
			od_adv_1y = CAy/4;		
			w2 = -1.0; //wspolczynnik przy Dy == -1
			w12 = 0.5;
			w14 = 0.5;
			//cout<<"w2: "<<w2<<"  w12: "<<w12<<"  w14: "<<w14<<endl;
		} 

		//gdy brak pkt c[i,j-1] - ozn 7 
		//czyli gdy "bd", "rdl", "rdp", "szl", "szp", "szd"
		if(flag[7]==0) {
			//cout<<"HELOOO MALARO dolny"<<endl;
			p->set_factor_fij("fe", 0);//fe przy i,j-1
			p->set_factor_fij("fe1", 0);//fe1 przy i,j-1
			//p->set_factor_fij("fd", - (CAy/4) + (CDy/2));//fd przy i,j+1
			p->set_factor("e1", 0);//fe1 przy i,j-1
			//p->set_factor("d", (CAy/4) - (CDy/2));//fd przy i,j+1
			od_adv_y = - CAy/2;
			od_adv_1y = - CAy/4;
			w2 = -1.0; //wspolczynnik przy Dy == -1
			w12 = 0.5;
			w14 = 0.5;
		}

		//jezeli nie ma obu c[i+1,j] i c[i-1,j] 
		if (flag[5] == 0 & flag[3] == 0) {w1 = 0; w3 = 0;}
		//jezeli nie ma obu c[i,j+1] i c[i,j-1] 
		if (flag[1] == 0 & flag[7] == 0) {w2 = 0; w4 = 0;} 
//-----------------------------------------------------------------------------------
		

//---------------WSPOLCZYNNIKI PRZY PKT POCHODZACYCH Z DYSKRET. POCHODNYCH MIESZANYCH --------------//
		//wspolczynnik "ff" - zmienia sie tylko przy braku pkt c[i+1,j+1] - ozn 2
		//czyli "bg", "bp" i "szp", "rgl", "rgp", "rdp", "kgp" iin gdzie brakuje pkt 2
		if(flag[2]==0) {
			p->set_factor_fij("ff", 0);
			//p->set_factor("f", 0);
		}

		//wspolczynnik "fg" - zmienia sie tylko przy braku pkt c[i+1,j-1] - ozn 8
		//czyli "bg", "bp" i "szp", "rgl", "rgp", "rdp", "kgp" iin gdzie brakuje pkt 8
		if(flag[8]==0) {
			p->set_factor_fij("fg", 0);
			//p->set_factor("g", 0);
		}
		//wspolczynnik "fh" - zmienia sie tylko przy braku pkt c[i-1,j+1] - ozn 0
		//czyli "bg", "bp" i "szp", "rgl", "rgp", "rdp", "kgp" iin gdzie brakuje pkt 0
		if(flag[0]==0) {
			p->set_factor_fij("fh", 0);
			//p->set_factor("h", 0);
		}

		//wspolczynnik "fi" - zmienia sie tylko przy braku pkt c[i-1,j-1] - ozn 6
		//czyli "bg", "bp" i "szp", "rgl", "rgp", "rdp", "kgp" iin gdzie brakuje pkt 6
		if(flag[6]==0) {
			p->set_factor_fij("fi", 0);
			//p->set_factor("i", 0);
		}
//--------------------------------------------------------------------------------------------------//	

//-------WSPOLCZYNNIK "fa" PRZED C[I,J] - ZMIENIA SIE GDY BRAK JAKIEGOKOLWIEK PKT I ZALEZY OD ZNAKU PREDKOSCI ----//

		// POCZATEK +++++++++++++++++++++ od_miezanych  ++++++++++++++++++++++++++++++++++//
		//++++++++++++czesc od mieszanych pochodnych we wspolczyniku "fa" ++++++++++++++//
		double od_mieszanych = 0.0; //skladnik pochodzacy od mieszanych pochodnych we spolczyniiku przed cij
		int tmpf=1, tmpg=1, tmph=1, tmpi=1; //zmienne pozwalajace okreslic czy w ogole wystepuje skaladnik "od_mieszanych"
		int znak=0; //zmienna sluzaca do okreslenia znaku skladnika "od_mieszanych" 
		int zf=1, zg=-1, zh=-1, zi=1; //okreslaja znaki przed wspolczynnikami ff fg fh fi w normalnej sytuacji przeplywu

		if (flag[2]==0) {tmpf=-1; zf=0;} // jezeli nie ma pkt 2 to przestawiam tmpf na -1 i ustawiam zmienna zf na 0 
		if (flag[8]==0) {tmpg=-1; zg=0;}
		if (flag[0]==0) {tmph=-1; zh=0;}
		if (flag[6]==0) {tmpi=-1; zi=0;}

		znak=zf+zg+zh+zi; // licze sume jesli np nie ma pkt 2 to mam 0-1-1+1 = -1; znak przy skladniku co cij musi byc przciwny czyli czyli +; 
				
		if ((tmpf*tmpg*tmph*tmpi)<0) od_mieszanych = -1*znak *2*CDxy; //gdy iloczyn <0 to dodaajemy dodatkowy czlon uwzgledniony we wpolczynniku przed cij
		// KONIEC ++++++++++++++++++++++++++++++++ od_miezanych  ++++++++++++++++++++++++++++++++//	

		p->set_factor_fij("fa", 1.0 + w1*CDx + w2*CDy + od_mieszanych + od_adv_y + od_adv_x);
		p->set_factor("a",  1.0 + w3*CDx - od_adv_x);
		p->set_factor_fij("fa1", w12*CDy -  od_adv_1y);
		p->set_factor("a1",  1.0 + w14*CDy - od_adv_1y);

		/*	if(flag[1]==0) {
		cout<<"--------------Ustawiono wspolczynnik 'fa' -----------------------"<<endl
			<<"-- fa = 1.0 + ( "<<w1<<" ) * CDx + ( "<<w2<<" ) * CDy + ( "<< od_mieszanych<<" )"<<endl;
		cout<<"--------------Ustawiono wspolczynnik 'fa' -----------------------"<<endl
			<<"-- a = 1.0 + ( "<<w3<<" ) * CDx + ( "<<w4<<" ) * CDy "<<endl;
		cout<<"--------------Ustawiono wspolczynnik 'fa1' -----------------------"<<endl
			<<"-- fa = ( "<<w12<<" ) * CDy "<<endl;
		cout<<"--------------Ustawiono wspolczynnik 'a1' -----------------------"<<endl
			<<"-- a = 1.0 + ( "<<w14<<" ) * CDy  "<<endl;
		*/	
		/*
		cout<<endl<<"USTAWIONO:"<<endl;
	cout<<"		fa: "<<p->get_factor_f("fa")<<",	";
	cout<<"		fb: "<<p->get_factor_f("fb")<<",	";
	cout<<"		fc: "<<p->get_factor_f("fc")<<",	";
	cout<<"		fd: "<<p->get_factor_f("fd")<<",	";
	cout<<"		fe: "<<p->get_factor_f("fe")<<endl;
	cout<<"		ff: "<<p->get_factor_f("ff")<<",	";
	cout<<"		fg: "<<p->get_factor_f("fg")<<",	";
	cout<<"		fh: "<<p->get_factor_f("fh")<<",	";
	cout<<"		fi: "<<p->get_factor_f("fi")<<endl;
	cout<<"__________________________________________________________________________________"<<endl;

	cout<<endl<<"USTAWIONO:"<<endl;
	cout<<"		a: "<<p->get_factor("a")<<",	";
	cout<<"		b: "<<p->get_factor("b")<<",	";
	cout<<"		c: "<<p->get_factor("c")<<",	";
	cout<<"		d: "<<p->get_factor("d")<<",	";
	cout<<"		e: "<<p->get_factor("e")<<endl;
	cout<<"		f: "<<p->get_factor("f")<<",	";
	cout<<"		g: "<<p->get_factor("g")<<",	";
	cout<<"		h: "<<p->get_factor("h")<<",	";
	cout<<"		i: "<<p->get_factor("i")<<endl;
	cout<<"__________________________________________________________________________________"<<endl;

	cout<<endl<<"USTAWIONO:"<<endl;
	cout<<"		fa1: "<<p->get_factor_f("fa1")<<",	";
	cout<<"		fb1: "<<p->get_factor_f("fb1")<<",	";
	cout<<"		fc1: "<<p->get_factor_f("fc1")<<",	";
	cout<<"		fd1: "<<p->get_factor_f("fd1")<<",	";
	cout<<"		fe1: "<<p->get_factor_f("fe1")<<endl;
	cout<<"		ff1: "<<p->get_factor_f("ff1")<<",	";
	cout<<"		fg1: "<<p->get_factor_f("fg1")<<",	";
	cout<<"		fh1: "<<p->get_factor_f("fh1")<<",	";
	cout<<"		fi1: "<<p->get_factor_f("fi1")<<endl;
	cout<<"__________________________________________________________________________________"<<endl;

	cout<<endl<<"USTAWIONO:"<<endl;
	cout<<"		a1: "<<p->get_factor("a1")<<",	";
	cout<<"		b1: "<<p->get_factor("b1")<<",	";
	cout<<"		c1: "<<p->get_factor("c1")<<",	";
	cout<<"		d1: "<<p->get_factor("d1")<<",	";
	cout<<"		e1: "<<p->get_factor("e1")<<endl;
	cout<<"		f1: "<<p->get_factor("f1")<<",	";
	cout<<"		g1: "<<p->get_factor("g1")<<",	";
	cout<<"		h1: "<<p->get_factor("h1")<<",	";
	cout<<"		i1: "<<p->get_factor("i1")<<endl;
	cout<<"__________________________________________________________________________________"<<endl;
	
	}
		*/
/*s
	}	 // - testowo zakomentowano 11.02.2010 - ze wzgledu na inne wrunki brzegowe dla tempoertaury -

*/

};


 //***************************************************SADI2**************************************************
 //_________________________________________________________________________________________________________
 void SADI2::ComputeConcentration (map <int, Point> & PointMap){
 	
	long double fij = 0, ck1 = 0;
	map<int,long double> f, f1;
	int index = Grid::empty_index;
	Point *p;
	cout<<Grid::_xmin<<" "<<Grid::_xmax<<" "<<Grid::_ymin<<" "<<Grid::_ymax<<endl;

	for (int i = Grid::_xmin; i <= Grid::_xmax + 1; i++){
		for (int j = Grid::_ymin; j <= Grid::_ymax + 1; j++){
			index = i * 10000 + j;
			p = &(PointMap.find(index)->second);
			if (p->get_flag() != 0) {
				//cout<<i<<","<<j<<"  ";
				fij = p->get_factor_f("fa") * p->get_Curr() +
					  p->get_factor_f("fb") * (PointMap.find(p->get_near_neighbour(5)))->second.get_Curr() +
					  p->get_factor_f("fc") * (PointMap.find(p->get_near_neighbour(3)))->second.get_Curr() +
					  p->get_factor_f("fd") * (PointMap.find(p->get_near_neighbour(1)))->second.get_Curr() +
					  p->get_factor_f("fe") * (PointMap.find(p->get_near_neighbour(7)))->second.get_Curr() +
					  p->get_factor_f("ff") * (PointMap.find(p->get_near_neighbour(2)))->second.get_Curr() +
					  p->get_factor_f("fg") * (PointMap.find(p->get_near_neighbour(8)))->second.get_Curr() +
					  p->get_factor_f("fh") * (PointMap.find(p->get_near_neighbour(0)))->second.get_Curr() +
					  p->get_factor_f("fi") * (PointMap.find(p->get_near_neighbour(6)))->second.get_Curr();
					  //pamietac zew przyszlosci trzeba dodac jeszcze czlony z distatnt neighbeurs
				f[index] = fij;
				
				fij = p->get_factor_f("fa1") * p->get_Curr() +
					  p->get_factor_f("fb1") * (PointMap.find(p->get_near_neighbour(5)))->second.get_Curr() +
					  p->get_factor_f("fc1") * (PointMap.find(p->get_near_neighbour(3)))->second.get_Curr() +
					  p->get_factor_f("fd1") * (PointMap.find(p->get_near_neighbour(1)))->second.get_Curr() +
					  p->get_factor_f("fe1") * (PointMap.find(p->get_near_neighbour(7)))->second.get_Curr() +
					  p->get_factor_f("ff1") * (PointMap.find(p->get_near_neighbour(2)))->second.get_Curr() +
					  p->get_factor_f("fg1") * (PointMap.find(p->get_near_neighbour(8)))->second.get_Curr() +
					  p->get_factor_f("fh1") * (PointMap.find(p->get_near_neighbour(0)))->second.get_Curr() +
					  p->get_factor_f("fi1") * (PointMap.find(p->get_near_neighbour(6)))->second.get_Curr();
					  //pamietac zew przyszlosci trzeba dodac jeszcze czlony z distatnt neighbeurs
				f1[index] = fij;
			}
		}
	}
	double tmps = 0;
	long double val = 0;
	map<int,Point>::iterator IT;

	for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
			val = IT->second.get_Curr();
			//if (val > 1e-20) {
				IT->second.set_Prev(val);
			//}
			IT->second.set_Curr(0);
			IT->second.set_Next(0);
	}
	
	for (int k = 0; k < 200; k++){
		cout<<"Iteracja: "<<k<<endl;
		double change = 0.0;
		for (int i = Grid::_xmin; i <= Grid::_xmax + 1; i++){
			for (int j = Grid::_ymin; j <= Grid::_ymax + 1; j++){
				index = i * 10000 + j;
				p = &(PointMap.find(index)->second);
				if (p->get_flag() != 0) {
					//cout<<i<<","<<j<<"  ";
					//Jacobi
					ck1 = - (p->get_factor("b") * (PointMap.find(p->get_near_neighbour(5)))->second.get_Curr() +
					  	p->get_factor("c") * (PointMap.find(p->get_near_neighbour(3)))->second.get_Curr() +
					  	p->get_factor("d") * (PointMap.find(p->get_near_neighbour(1)))->second.get_Curr() +
					  	p->get_factor("e") * (PointMap.find(p->get_near_neighbour(7)))->second.get_Curr() +
					  	p->get_factor("f") * (PointMap.find(p->get_near_neighbour(2)))->second.get_Curr() +
					  	p->get_factor("g") * (PointMap.find(p->get_near_neighbour(8)))->second.get_Curr() +
					  	p->get_factor("h") * (PointMap.find(p->get_near_neighbour(0)))->second.get_Curr() +
					  	p->get_factor("i") * (PointMap.find(p->get_near_neighbour(6)))->second.get_Curr() -
						f[index])/p->get_factor("a");
						change+=fabs(p->get_Next() - ck1);
					//cout<<"ck1: "<<ck1<<endl;
					p->set_Next(ck1); //zapisuej wartosci z kroku n+1/2 do Prev (moze nalezy to potem uporzadkowac)
					tmps = tmps + ck1;
					//if (ck1 > 1e-10) cout<<"	i,j:"<<i<<","<<j<<" : "<<ck1<<endl;
				}
			}
		}
		cout.precision(20);
		cout<<"Change: "<<change<<endl;
		
		for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
			val = IT->second.get_Next();
			//if (val > 1e-20) {
				IT->second.set_Curr(val);
			//}
		//cout<<"VAL: "<<val<<" ustawiona"<<IT->second.get_Curr()<<endl;
		}
		if (change < 1e-6) break;
	}

	for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
			val = IT->second.get_Curr();
			if (val > 1e-20) {
				IT->second.set_Prev(val);
			}
			IT->second.set_Curr(0);
			IT->second.set_Next(0);
		}
		
	//f.clear();
	
	/*for (int i = Grid::_xmin; i <= Grid::_xmax + 1; i++){
		for (int j = Grid::_ymin; j <= Grid::_ymax + 1; j++){
			index = i * 10000 + j;
			p = &(PointMap.find(index)->second);
			if (p->get_flag() != 0) {
				//cout<<i<<","<<j<<"  ";
				fij = p->get_Prev() + //wartosc z "pol kroku" n+1/2
					  p->get_factor_f("fa1") * p->get_Curr() +
					  p->get_factor_f("fb1") * (PointMap.find(p->get_near_neighbour(5)))->second.get_Curr() +
					  p->get_factor_f("fc1") * (PointMap.find(p->get_near_neighbour(3)))->second.get_Curr() +
					  p->get_factor_f("fd1") * (PointMap.find(p->get_near_neighbour(1)))->second.get_Curr() +
					  p->get_factor_f("fe1") * (PointMap.find(p->get_near_neighbour(7)))->second.get_Curr() +
					  p->get_factor_f("ff1") * (PointMap.find(p->get_near_neighbour(2)))->second.get_Curr() +
					  p->get_factor_f("fg1") * (PointMap.find(p->get_near_neighbour(8)))->second.get_Curr() +
					  p->get_factor_f("fh1") * (PointMap.find(p->get_near_neighbour(0)))->second.get_Curr() +
					  p->get_factor_f("fi1") * (PointMap.find(p->get_near_neighbour(6)))->second.get_Curr();
					  //pamietac zew przyszlosci trzeba dodac jeszcze czlony z distatnt neighbeurs

				f[index] = fij;
				//cout<<"Fij: "<<fij<<endl;
			}
		}
	}*/
	
	for (int k = 0; k < 100; k++){
		cout<<"Iteracja 2: "<<k<<endl;
		double change = 0.0;
		for (int i = Grid::_xmin; i <= Grid::_xmax + 1; i++){
			for (int j = Grid::_ymin; j <= Grid::_ymax + 1; j++){
				index = i * 10000 + j;
				p = &(PointMap.find(index)->second);
				if (p->get_flag() != 0) {
					//cout<<i<<","<<j<<"  ";
					//Jacobi
					ck1 = - (p->get_factor("b1") * (PointMap.find(p->get_near_neighbour(5)))->second.get_Curr() +
					  	p->get_factor("c1") * (PointMap.find(p->get_near_neighbour(3)))->second.get_Curr() +
					  	p->get_factor("d1") * (PointMap.find(p->get_near_neighbour(1)))->second.get_Curr() +
					  	p->get_factor("e1") * (PointMap.find(p->get_near_neighbour(7)))->second.get_Curr() +
					  	p->get_factor("f1") * (PointMap.find(p->get_near_neighbour(2)))->second.get_Curr() +
					  	p->get_factor("g1") * (PointMap.find(p->get_near_neighbour(8)))->second.get_Curr() +
					  	p->get_factor("h1") * (PointMap.find(p->get_near_neighbour(0)))->second.get_Curr() +
					  	p->get_factor("i1") * (PointMap.find(p->get_near_neighbour(6)))->second.get_Curr() -
						(p->get_Prev() + f1[index]))/p->get_factor("a1");
					//cout<<"ck1: "<<ck1<<endl;
					change+=fabs(p->get_Next() - ck1);
					p->set_Next(ck1); //zapisuej wartosci z kroku n+1/2 do Prev (moze nalezy to potem uporzadkowac)
					tmps = tmps + ck1;
					//if (ck1 > 1e-10) cout<<"	i,j:"<<i<<","<<j<<" : "<<ck1<<endl;
				}
			}
		}
		//cout<<"tmps "<<tmps<<endl;
		//tmps = 0;
		cout<<"Change: "<<change<<endl;
		
		val = -9999;;
		for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
			val = IT->second.get_Next();
			if (val > 1e-20) {
				IT->second.set_Curr(val);
			}
		}

		if (change < 1e-6) break;
	}
		
		
		
		val = -9999;
		long double suma = 0;
		long double suma1 = 0;
		int pktprzed = 0, pktpo = 0;
		for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
			val = IT->second.get_Next();
			suma = suma + val;
			pktprzed ++;
			if (val > 1e-20) {
				IT->second.set_Curr(val);
				suma1 = suma1 + IT->second.get_Curr();
				pktpo ++;
			}
			IT->second.set_Prev(0);
			IT->second.set_Next(0);
		//cout<<"VAL: "<<val<<" ustawiona"<<IT->second.get_Curr()<<endl;
		}
		cout<<"tmps: "<<tmps<<"  suma: "<<suma<<"  suma1: "<<suma1<<"  przed: "<<pktprzed<<"  po: "<<pktpo<<endl<<endl;	
	
 
 };

 //_________________________________________________________________________________________________________
 void SADI2::GetFactors(Point *p, const double &CAx, const double &CAy, const double &CDx, const double &CDxy, const double &CDy, const vector<int> &flag){
	
	//dla kroku n+1/2 czyli po x-saxh
	p->set_factor_fij("fa", 1 - CDy - CDx);//fa przy i,j
	p->set_factor_fij("fb", (-CAx/4) + (CDx/2));//fb przy i+1,j
	p->set_factor_fij("fc", (CAx/4) + (CDx/2));//fc przy i-1,j
	p->set_factor_fij("fd", (-CAy/4) + (CDy/2));//fd przy i,j+1
	p->set_factor_fij("fe", (CAy/4) + (CDy/2));//fe przy i,j-1
	p->set_factor_fij("ff", 2*CDxy);//ff przy i+1,j+1
	p->set_factor_fij("fg", -2*CDxy);//fg przy i+1,j-1
	p->set_factor_fij("fh", -2*CDxy);//fh przy i-1,j+1
	p->set_factor_fij("fi", 2*CDxy);//fi przy i-1,j-1
	p->set_factor_fij("fj", 0);//fj przy i+2,j
	p->set_factor_fij("fk", 0);//fk przy i-2,j
	p->set_factor_fij("fl", 0);//fl przy i,j+2
	p->set_factor_fij("fm", 0);//fm przy i,j-2
	
	p->set_factor("a", 1 + CDx);//a przy i,j
	p->set_factor("b", (CAx/4) - (CDx/2));//b przy i+1,j
	p->set_factor("c", -(CAx/4) - (CDx/2));//c przy i-1,j
	p->set_factor("d", 0);//d przy i,j+1
	p->set_factor("e", 0);//e przy i,j-1
	p->set_factor("f", 0);//f przy i+1,j+1
	p->set_factor("g", 0);//g przy i+1,j-1
	p->set_factor("h", 0);//h przy i-1,j+1
	p->set_factor("i", 0);//i przy i-1,j-1
	p->set_factor("j", 0);//j przy i+2,j
	p->set_factor("k", 0);//k przy i-2,j
	p->set_factor("l", 0);//l przy i,j+2
	p->set_factor("m", 0);//m przy i,j-2

	//dla kroku n+1 czyli po y-kach
	p->set_factor_fij("fa1", 0);//fa przy i,j
	p->set_factor_fij("fb1", 0);//fb przy i+1,j
	p->set_factor_fij("fc1", 0);//fc przy i-1,j
	p->set_factor_fij("fd1", 0);//fd przy i,j+1
	p->set_factor_fij("fe1", 0);//fe przy i,j-1
	p->set_factor_fij("ff1", 0);//ff przy i+1,j+1
	p->set_factor_fij("fg1", 0);//fg przy i+1,j-1
	p->set_factor_fij("fh1", 0);//fh przy i-1,j+1
	p->set_factor_fij("fi1", 0);//fi przy i-1,j-1
	p->set_factor_fij("fj1", 0);//fj przy i+2,j
	p->set_factor_fij("fk1", 0);//fk przy i-2,j
	p->set_factor_fij("fl1", 0);//fl przy i,j+2
	p->set_factor_fij("fm1", 0);//fm przy i,j-2
	
	p->set_factor("a1", 1 + CDy);//a przy i,j
	p->set_factor("b1", 0);//b przy i+1,j
	p->set_factor("c1", 0);//c przy i-1,j
	p->set_factor("d1", (CAy/4) - (CDy/2));//d przy i,j+1
	p->set_factor("e1", -(CAy/4) - (CDy/2));//e przy i,j-1
	p->set_factor("f1", 0);//f przy i+1,j+1
	p->set_factor("g1", 0);//g przy i+1,j-1
	p->set_factor("h1", 0);//h przy i-1,j+1
	p->set_factor("i1", 0);//i przy i-1,j-1
	p->set_factor("j1", 0);//j przy i+2,j
	p->set_factor("k1", 0);//k przy i-2,j
	p->set_factor("l1", 0);//l przy i,j+2
	p->set_factor("m1", 0);//m przy i,j-2
};
