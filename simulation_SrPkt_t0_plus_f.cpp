/***************************************************************************
 *   simulation.cpp
 *   Copyright (C) 2005 by Monika Kalinowska                               *
 *   mkalinow@igf.edu.pl                                                   *
 *                                                                         *
 ***************************************************************************/

 #include "simulation.h"
 #include <sstream>
 #define cout_opt 1 //1 = file, 0 = screan - opcja wypisywnaia czy do pliku czy na ekran
 					// jesli do pliku to plik o nzazwie Info.dat
 #include <ctime>
 
 
 Simulation::Simulation(const string &param): _param(param), 
 		_file_grid("gridVistula"), _file_v("v"),
		_file_data("data10"), _file_d("d"){
 	//cout<<"	<< Simulation object >>"<<endl;
	//cout<<param<<endl;
	MySimulation(param);
 };
 //_______________________________________________________________________________
 
 
 void Simulation::MySimulation (const string &param_file){
	//cout<<param_file<<endl;

	map<int, Point> PointMap;
	SimulationParameters sym_param(param_file);	
 
	int check = SetParam(&sym_param, param_file);
	if (check==0) return;

	check = PrepareMap(PointMap, sym_param);
	if (check==0) return;	
	cout<<"test 4 sty 2010"<<endl;
	//SetInitialConcentrationGauss(PointMap, sym_param);
	//SetInitialConcentrationGaussNonDiag(PointMap, sym_param);
	SetInitialConcentration (PointMap, sym_param);
	//SetInitialConcentrationPlaneWave(PointMap); 
	//SaveMap(PointMap,  _file + "_all.dat", "all");
	
	double Cn = sym_param.ComputeCourantNumber();
	double Pe = sym_param.ComputePecletNumber();
	//INFO
	//cout<<"Courant number: "<<Cn<<"	";
	//cout<<"Pecklet number: "<<Pe<<endl;	
	ofstream out("Info.dat", ios::app);
	out<<"Courant number: "<<Cn<<"	";
	out<<"Pecklet number: "<<Pe<<endl;	
	cout<<"test luty 2010"<<endl;
	SScheme *my_scheme;
	cout<<"test1 luty 2010"<<endl;
	my_scheme = sym_param.get_s_scheme();
	cout<<"test2 luty 2010"<<endl;	
	my_scheme->SetFactors(PointMap);
	cout<<"test3 luty 2010"<<endl;
	string sch = sym_param.get_scheme();
	cout<<"test4 luty 2010"<<endl;
	my_scheme->ReadMethod(sch);
	cout<<"test5 luty 2010"<<endl;
	int i = 0;
	string step, step_plus;
	cout<<"test6 luty 2010"<<endl;
	SaveMap(PointMap, _file + "_Conc_" + sch + "_0.dat", "conc");
	//my_scheme->set_omega(1.0);
	my_scheme->powrot = 0;
	
	time_t  t0, t1; 
  	clock_t c0, c1; 
	ofstream out1("Szybkosc.txt", ios::app);
	//double val = 1; 
	//double val= 7.0; //dla wisly ekspertyza 

	//srodek
	//double val = 4.3; //wisla, ekspertyza dla kroku dx =10
	//double val = 2.7; //wisla, ekspertyza dla kroku dx =25
	//double val = 5.3; //wisla, ekspertyza dla kroku dx =5
	//double val = 5.32; //wisla, ekspertyza dla kroku dx =5
	double val = 4.317; //wisla, ekspertyza dla kroku dx =10; po przesunieciu zrodla
	

	//lewy brzeg
	//double val = 4.785; //wisla, ekspertyza dla kroku dx =5 lewy b
	//double val = 4.567; //wisla, ekspertyza dla kroku dx =5 lewy b wersja II
	//double val = 3.635; //wisla, ekspertyza dla kroku dx =10 lewy b
	//double val = 3.389; //wisla, ekspertyza dla kroku dx =10 lewy b wersja II
	//double val = 1.910; //wisla, ekspertyza dla kroku dx =25 lewy b wersja II
	//double val = 3.458; //wisla, ekspertyza dla kroku dx =10 lewy brzeg pkt: 1710,670
	//double val = 3.274; //wisla, ekspertyza dla kroku dx =10 lewy brzeg pkt: 1720,680
	

	//krotki odcinek 14 m
	//!double val = 2.0; //wisla, ekspertyza dla kroku dx =10, na krotkim odcinku
	//po przesunieciu poczatku odcinka do pkt w ktorym jest zrodlo na lewym brzegu
	//double val = 2.235; //wisla, ekspertyza dla kroku dx =10, na krotkim odcinku


	//dlugi odcinek 28 m, poczatek,
	//double val = 1.636; //wisla, ekspertyza dla kroku dx =10, na krotkim odcinku



	for (i = 1; i < sym_param.get_n() + 1; i++ ){
		ostringstream buffer;
		buffer << i;		// send the int to the ostringstream
		step =  buffer.str();	// capture the string
		cout<<"KROK: "<<i<<"  ";
		out<<"KROK: "<<i<<endl;
		my_scheme->set_sym_step(i);
		t0 = time(NULL);
  		c0 = clock();
		my_scheme->ComputeConcentration(PointMap);
		//SetInOutPointValue (PointMap);	
		t1 = time(NULL);
  		c1 = clock();
		//cout<<endl;
		//printf ("\telapsed wall clock time in this step:	%ld seconds\n", (long) (t1 - t0));
  		//printf ("\telapsed CPU time in this step:	%f seconds\n", (float) (c1 - c0)/CLOCKS_PER_SEC);
		//cout<<endl;
		out1.precision(10);
		out1<<(t1 - t0)<<"	"<<(float) (c1 - c0)/CLOCKS_PER_SEC<<endl;
		
		/*()if ( i % 10 == 0) {
			step_plus = _file + "_Conc_" + sch + ".dat";
			SaveMap(PointMap, step_plus, "sym");
			SaveMap(PointMap, step_plus, "line");
		}*/
		SetSourceConcentrationTemp (PointMap, sym_param, val);	
		if ( i % 100 == 0 || i==1 || i==10 ) {
			step_plus = _file + "_Conc_" + sch + "_" + step + ".dat";
			SaveMap(PointMap, step_plus, "conc");
		}
		//SetInitialConcentrationGauss(PointMap, sym_param);	
	}
	//string step = i;
//	if ( i % 20 != 0){
		//step_plus = _file + "_Conc_" + sch + "_" + step + ".dat";
		//SaveMap(PointMap, step_plus, "conc");
//	}
	
	//INFO cz2
	out.close();
	
}
 //_______________________________________________________________________________
 
 
 int Simulation::SetParam(SimulationParameters *param, const string &file){
		
 	int flag=-1, n=-1;
	double dx=-1.0, dy=-1.0, dt=-1.0, vx=-9999.0, vy=-9999.0, Dlong=-9999.0, Dtrans=-9999.0;
	string scheme = "nie zdefinowany", disp = "nie okreslony";
	cout<<"falga:"<<flag<<"  n: "<<n<<"  dx: "<<dx<<"  dy: "<<dy<<"  dt: "<<dt<<"  schem: "<<scheme<<endl;
	cout<<"vx: "<<vx<<"  vy: "<<vy<<"  Dlong: "<<Dlong<<"  Dtrans: "<<Dtrans<<endl;
	//string file1="test";			
	fstream in(file.c_str());
	if(!in) {
    	CheckFile(0,file);
		return 0;// jesli nie ma pliku wychodzi z funkci zwracajac 0
    }
	else CheckFile(1,file);		
	in>>flag;
		
	switch(flag){
		case 0:
			in>>n>>dx>>dy>>dt>>scheme>>disp>>vx>>vy>>Dlong>>Dtrans;
			break;
		case 1:
			in>>n>>dx>>dy>>dt>>scheme>>disp>>Dlong>>Dtrans;
			break;
		case 2:
			in>>n>>dx>>dy>>dt>>scheme>>disp;
			break;
		case 3:
			in>>n>>dx>>dy>>dt>>scheme>>disp>>vx>>vy;
	}
	in.close();
		
	param->set_flag(flag);
	param->set_n(n);
	param->set_dx(dx);
	param->set_dy(dy);
	param->set_dt(dt);
	param->set_scheme(scheme);
	param->set_dispersion(disp);
	param->set_vx(vx);
	param->set_vy(vy);
	param->set_Dlong(Dlong);
	param->set_Dtrans(Dtrans);
	
	//INFO 	
	cout<<"falga: "<<flag<<"  n: "<<n<<"  dx: "<<dx<<"  dy: "<<dy<<"  dt: "
			<<dt<<endl<<"schem: "<<scheme<<"  sposob obliczania dyspersji: "<<disp<<endl;
	cout<<"vx: "<<vx<<"  vy: "<<vy<<"  Dlong: "<<Dlong<<"  Dtrans: "<<Dtrans<<endl;	
	ofstream out("Info.dat", ios::app);
	out<<"falga: "<<flag<<"  n: "<<n<<"  dx: "<<dx<<"  dy: "<<dy<<"  dt: "
			<<dt<<endl<<"schem: "<<scheme<<"  sposob obliczania dyspersji: "<<disp<<endl;
	out<<"vx: "<<vx<<"  vy: "<<vy<<"  Dlong: "<<Dlong<<"  Dtrans: "<<Dtrans<<endl;	
	out.close();
	return 1; //zwraca 1 jesli wszystko bylo ok
 };
 //_______________________________________________________________________________________________________
 
 
 int Simulation::PrepareMap(map<int, Point> &PointMap, SimulationParameters &param){
 	ofstream out("Info.dat", ios::app);
 	//cout<<"	Przygotowuje mape ...	";
	out<<"	Przygotowuje mape ...	";
	out.close();
	
	int flag=param.get_flag();// x = -9999, y = -9999, index = -9999;
	string file="notdefine";

	Grid grid_main;
	GridOnly grid_only;
	GridVelocity grid_velocity;
	GridAll grid_all;
	GridDispersion grid_dysp;
	
	Grid *my_grid;
	my_grid = & grid_main;

	// w zaleznosci od opcji z jaka zostal uruchomiony program 
	// bedziemy czytac z odpowieniego piku o oreslonym formacie 	
	switch(flag){
		case 0:
			my_grid = & grid_only;
			this->set_file(_file_grid);//file = "grid.txt";
			file = _file_grid;
			break;
		case 1:
			my_grid = & grid_velocity;
			this->set_file(_file_v);
			file = _file_v;
			break;
		case 2:
			this->set_file(_file_data);
			my_grid = & grid_all;
			file = _file_data;
			break;
		case 3:
			this->set_file(_file_d);
			my_grid = & grid_dysp;
			file = _file_d;
			break;
	}
	
	//czyta okreslony plik z danymi i uzupemnie mape sczytami punktami
	//cout<<"RRRRRRRRR "<<_file<<endl;
	int check = my_grid->ReadData(PointMap, _file + ".txt", param);
	
	//ustwaiamy mape dla naszych pkt	
  	Point dummy;//pusty pkt
  	PointMap[my_grid->empty_index] = dummy;
  	PointMap[my_grid->empty_index].set_map(&PointMap);
	
	//korzystajac z wyznaczony wartosci namniejszych najwiekszych wspolrzednych na siatce,
	//uzuplniamy siatke pkt ktore leza poza przeplywem nadajac im flage 0
	my_grid->CompleteOutsideFlow(PointMap);
	
	//Zapisujemy mape z oznaczonymi pkt przeplywu i poza nim (Flagami odpowienio 1,0))	
//luty 2010	SaveMap(PointMap, file + ".dat", "grid");
	
	//ustawiamy indexy punktow bedacych sasiadami danego pkt
	// i obliczamy liczby Couranta dla kazdego pkt 
	map<int, Point>::iterator CI; //iterator do mapy punktow 
	for(CI=PointMap.begin();CI!=PointMap.end();CI++){// po 
		CI->second.set_near_neighbours(1);
		CI->second.set_distant_neighbours(1);
		CI->second.set_Courant_numbers(param.get_dx(), param.get_dy(), param.get_dt());
	}
	
	//onaczamy brzegi nadajac im odpowienidnie flagi
	//my_grid->MarkBorderPoints(PointMap);		
	//SaveMap(PointMap, file + "a.dat", "grid");
	
	//onaczamy pkt przy brzegach nadajac im odpowienidnie flagi	
	//my_grid->MarkNearBorderPoints(PointMap);
	//SaveMap(PointMap, file + "b.dat", "grid");
	
	check = my_grid->MarkInOutPoints(PointMap);
	cout<<"przed zapisem"<<endl;
//	SaveMap(PointMap, file + "c.dat", "grid");
	cout<<"po zapisie, check "<<check<<endl;
	return check; //zwraca 1 gdy wszytko poszlo ok, 0 gdy nie znaleziono potrzebnego pliku
 };
 //_______________________________________________________________________________________________________
 
 
void Simulation::SaveMap(map<int, Point> &PointMap, const string &file, const string &opt){

	ofstream out;
	ofstream out1("Info.dat", ios::app);
 	
	//zapisuje mape z oznaczonymi flagami do pliku
	if (opt == "sym" || opt == "line") out.open(file.c_str(),ios::app);
	else out.open(file.c_str(),ios::trunc);
  	//INFO
	//cout<<"Zapisuje mape do pliku "<<file
      //<<" rozmiar mapy = "<<PointMap.size()<<endl;
	out1<<"Zapisuje mape do pliku "<<file
      <<" rozmiar mapy = "<<PointMap.size()<<endl;
  	Point tmp;
	
	//Zapisz zawartosc mapy.
  	map<int, Point>::const_iterator CI;
  	
	if (opt == "sym"){
		for(CI=PointMap.begin();CI!=PointMap.end();CI++){
        	if (CI->second.get_flag() != -9999){
				//if (CI->second.get_Curr() != 0){
				if (fabs(CI->second.get_Curr()) > 1e-10){
					out<<CI->second.get_x()<<" "
        				<<CI->second.get_y()<<" "
        				<<CI->second.get_Curr()<<endl;
				}
			}
		}
	}

	else if (opt == "line") out<<-9999<<" "<<-9999<<" "<<-9999<<" "<<-9999<<endl;

	else if (opt == "conc"){
		for(CI=PointMap.begin();CI!=PointMap.end();CI++){
        	if (CI->second.get_flag() != -9999){
			//if (CI->second.get_Curr() != 0){
				out<<CI->second.get_x()<<" "
        			<<CI->second.get_y()<<" "
        			//<<CI->second.get_flag()<<" "
					//<<CI->second.get_vx()<<" "
        			//<<CI->second.get_vy()<<" "
        			//<<CI->second.get_Dxx()<<" "
					//<<CI->second.get_Dxy()<<" "
        			//<<CI->second.get_Dyy()<<" "
					//<<CI->second.get_Prev()<<" "
				   <<(double)CI->second.get_Curr()<<endl;
					//<<CI->second.get_Next()<<endl;
				//}
			}
		}
	}	
			
	else if (opt == "grid"){
		for(CI=PointMap.begin();CI!=PointMap.end();CI++){
        	if (CI->second.get_flag() != -9999){
				out<<CI->second.get_x()<<" "
        			<<CI->second.get_y()<<" "
        			<<CI->second.get_flag()<<endl;
			}
      	}
	}
	else if (opt == "all"){
		for(CI=PointMap.begin();CI!=PointMap.end();CI++){
        	if (CI->second.get_flag() != -9999){
				out<<CI->second.get_x()<<" "
        			<<CI->second.get_y()<<" "
        			<<CI->second.get_flag()<<" "
        			<<CI->second.get_vx()<<" "
        			<<CI->second.get_vy()<<" "
        			<<CI->second.get_Dxx()<<" "
					<<CI->second.get_Dxy()<<" "
        			<<CI->second.get_Dyy()<<" "
					<<CI->second.get_Prev()<<" "
					<<CI->second.get_Curr()<<" "
					<<CI->second.get_Next()<<endl;
			}
		}
	}	
  	out.close();
	//INFO cz 2
	out1.close();
}
 //_______________________________________________________________________________________________________

void Simulation::SetSourceConcentration (map<int, Point> &PointMap, double &val){
	ofstream out("Info.dat", ios::app);
	out<<"Ustawiam wartosc koncentracji w miejscu źródła"<<endl;
	
//!!!!!! zrobic by bylo to czytane z pliku
	int i1,i2,j1,j2;
	//i1 = 100;// poczatek obszaru wsp x
	//i2 = 100;// koniec obszaru wsp x
	//j1 = 100;// poczatkek obszaru wsp y
	//j2 = 100;// koniec obszaru wsp y
	
	//i1 = 99;// poczatek obszaru wsp x
	//i2 = 101;// koniec obszaru wsp x
	//j1 = 99;// poczatkek obszaru wsp y
	//j2 = 101;// koniec obszaru wsp y
	
//	i1 = 98;// poczatek obszaru wsp x
//	i2 = 102;// koniec obszaru wsp x
//	j1 = 98;// poczatkek obszaru wsp y
//	j2 = 102;// koniec obszaru wsp y
	
/*	i1 = 298;// poczatek obszaru wsp x
	i2 = 302;// koniec obszaru wsp x
	j1 = 48;// poczatkek obszaru wsp y
	j2 = 52;// koniec obszaru wsp y
*/
	i1 = 150-2;// poczatek obszaru wsp x
	i2 = 150+2;// koniec obszaru wsp x
	j1 = 25-2; // poczatkek obszaru wsp y
	j2 = 25+2; // koniec obszaru wsp y


	map<int,Point>::iterator IT;

	int index;
	double total = 0; //calkowita zadna na siatce koncentracja
	//Zadaj koncentacje w wybranym obszarze.
	for(int i = i1; i < i2+1; i++){
		for(int j = j1; j < j2+1; j++){
			index = i*10000 + j;
			if(PointMap.find(index) == PointMap.end()){
				cout<<"Nie znany index = "<<index<<endl;
				continue;
			}
			if(PointMap[index].get_flag() == 0){
				cout<<"Zadaje poza obszarem przeplywu!"<<endl
				<<PointMap[index]<<endl;
			}
			else {
				//cout<<"		zadaje, index: "<<index<<"  wartosc: "<<val<<endl;
				//cout<<"przed zadaniem: "<<PointMap[index].get_Curr()<<endl;
      				if (i==i1+2 && j==j1+2) PointMap[index].set_Curr(val+PointMap[index].get_Curr());
				else if ((i==i1+1 && (j==j1+1 || j==j1+2 || j==j2-1)) || (i==i1+2 && (j==j1+1 || j==j2-1)) 
						|| (i==i2-1 && (j==j1+99 || j==j1+2 || j==j2-1)) ) PointMap[index].set_Curr((0.5) + PointMap[index].get_Curr());
				else PointMap[index].set_Curr((0.3125) + PointMap[index].get_Curr());
				total+=val;
				//cout<<"po zadaniu : "<<PointMap[index].get_Curr()<<endl;
      			}
/*			else {
				//cout<<"		zadaje, index: "<<index<<"  wartosc: "<<val<<endl;
				//cout<<"przed zadaniem: "<<PointMap[index].get_Curr()<<endl;
      				if (i==100 && j==100) PointMap[index].set_Curr(val+PointMap[index].get_Curr());
				else if ((i==99 && (j==99 || j==100 || j==101)) || (i==100 && (j==99 || j==101)) 
						|| (i==101 && (j==99 || j==100 || j==101)) ) PointMap[index].set_Curr((0.5) + PointMap[index].get_Curr());
				else PointMap[index].set_Curr((0.3125) + PointMap[index].get_Curr());
				total+=val;
				//cout<<"po zadaniu : "<<PointMap[index].get_Curr()<<endl;
      			}
*/

    		}
	}
	out.close();
}


 //_______________________________________________________________________________________________________

void Simulation::SetSourceConcentrationTemp (map<int, Point> &PointMap, SimulationParameters &param, double &val){
	ofstream out("Info.dat", ios::app);
	out<<"Ustawiam wartosc koncentracji w miejscu źródła"<<endl;
//	double x0 = 1400/param.get_dx(), y0 = 250/param.get_dy();//dla Wisly - ekspertyza 
//	double x0 = 1400/param.get_dx(), y0 = 300/param.get_dy();//dla Wisly - ekspertyza 
//	double x0 = 1350/param.get_dx(), y0 = 200/param.get_dy();//dla Wisly - ekspertyza 
//	double x0 = 1500/param.get_dx(), y0 = 400/param.get_dy();//dla Wisly - ekspertyza 
//	double x0 = 1550/param.get_dx(), y0 = 350/param.get_dy();//dla Wisly - ekspertyza 

//nowa siatak
//	double x0 = 700/param.get_dx(), y0 = 200/param.get_dy();//dla Wisly - ekspertyza 
//	double x0 = 600/param.get_dx(), y0 = 100/param.get_dy();//dla Wisly - ekspertyza 

//ostat siatak
	//double x0 = 2700/param.get_dx(), y0 = 250/param.get_dy();//dla Wisly - ekspertyza 

	//zrodlo punktowe na srodku kanalu
	//double x0 = 1850/param.get_dx(), y0 = 775/param.get_dy();//dla Wisly - ekspertyza 
	double x0 = 1850/param.get_dx(), y0 = 800/param.get_dy();//dla Wisly - ekspertyza 

	//zrodlo punktowe na lewym brzegu
	//double x0 = 1700/param.get_dx(), y0 = 670/param.get_dy();//dla Wisly - ekspertyza 
	//double x0 = 1710/param.get_dx(), y0 = 680/param.get_dy();//dla Wisly - ekspertyza -wariant ost
	//double x0 = 1710/param.get_dx(), y0 = 670/param.get_dy();//dla Wisly - ekspertyza -wariant przesuniety wzdluz rurociagu
	//double x0 = 1720/param.get_dx(), y0 = 680/param.get_dy();//dla Wisly - ekspertyza -wariant przesuniety wzdluz rurociagu


	//krotki odcinek 14m 
	//!double x0 = 1730/param.get_dx(), y0 = 690/param.get_dy();//dla Wisly - ekspertyza 
	//double x0 = 1710/param.get_dx(), y0 = 680/param.get_dy();//dla Wisly - ekspertyza - poczatek odcinka przesunety


	//dlugi odcinek 28m - poczatek
	//double x0 = 1780/param.get_dx(), y0 = 730/param.get_dy();//dla Wisly - ekspertyza - poczatek odcinka przesunety


	int i1,i2,j1,j2;

	i1 = x0-2;// poczatek obszaru wsp x
	i2 = x0+2;// koniec obszaru wsp x
	j1 = y0-2; // poczatkek obszaru wsp y
	j2 = y0+2; // koniec obszaru wsp y

	map<int,Point>::iterator IT;


  
	int index;
	double total = 0; //calkowita zadna na siatce koncentracja
	double c_val = 0; //zmienna pomocnicza do skopiowania wartosci aktualna w danym puncie 
	//Zadaj koncentacje w wybranym obszarze.
	for(int i = i1; i < i2+1; i++){
		for(int j = j1; j < j2+1; j++){
			index = i*10000 + j;
			if(PointMap.find(index) == PointMap.end()){
				cout<<"Nie znany index = "<<index<<endl;
				continue;
			}
			if(PointMap[index].get_flag() == 0){
				cout<<"Zadaje poza obszarem przeplywu!"<<endl
				<<PointMap[index]<<endl;
			}
			else {
      				if (i==i1+2 && j==j1+2) 
                      {         
                                c_val = PointMap[index].get_Curr();
                                cout<<"TEMP  w punkcie "<<i<<","<<j<<"aktualna wartosc temp wynosi: "<<c_val<<endl;  
                                out<<"TEMP  w punkcie "<<i<<","<<j<<"aktualna wartosc temp wynosi: "<<c_val<<endl;
                                
                                   //Zrodl w pkt: (1850,800)
                                   //Czyli dla kroku przestrzennego dx=dy= 10
                                   //Zrodl w pkt: (185,80)
                                   //h: 2.058233041
                                   //v: 0.4226119404
                                   //q: 8.69833859
                                   //qw: 22.69833859
                                   //------------> dla kroku przestrzennego dx=10:
                                   //T: 4.317496614
                                   double _qz = 14.0;
                                   double _q;
                                   double _qw = 0.0; // przeplyw wypadkowy
                                   double _T_rz = c_val; //temp wody w rzece w danym pkt zrodla po poprzednim kroku
                                   double _T_z = 7.0; //temp zrodla
                                   double _T;
                                   double _h = 2.058233041;
                                   double _v = 0.4226119404;
                                   
                                    _q = _v * _h * param.get_dx();
                                    std::cout<<"q: "<<_q<<std::endl;
                                    _qw = _qz + _q;
                                    std::cout<<"qw: "<<_qw<<std::endl;
                                    _T = ((_q * _T_rz) + (_qz * _T_z))/_qw;
                                    std::cout<<"  T: "<<_T<<std::endl;
                                    std::cout<<""<<std::endl;
                                  
                                PointMap[index].set_Curr(_T);
                                c_val = PointMap[index].get_Curr();
                                cout<<"TEMP  w punkcie "<<i<<","<<j<<"po dodaniu odpowiedniej wartosc temp wynosi: "<<c_val<<endl;  
                                out<<"TEMP  w punkcie "<<i<<","<<j<<"po dodaniu odpowiedniej wartosc temp wynosi: "<<c_val<<endl;  
                      }
				//else if ((i==i1+1 && (j==j1+1 || j==j1+2 || j==j2-1)) || (i==i1+2 && (j==j1+1 || j==j2-1)) 
				//		|| (i==i2-1 && (j==j1+1 || j==j1+2 || j==j2-1)) ) 
				//	PointMap[index].set_Curr((3.0));
					//PointMap[index].set_Curr((0.5));
				//else PointMap[index].set_Curr((0.3125));
				//else PointMap[index].set_Curr((1.1875));
				total+=val;
      			}
    		}
	}

	//dodano co ponizej bo musze zadawac wartosc w dwoch pktach
/*
	//krotki odcinek 14m 
	x0 = 1740/param.get_dx(), y0 = 700/param.get_dy();//dla Wisly - ekspertyza 
	//x0 = 1720/param.get_dx(), y0 = 690/param.get_dy();//dla Wisly - ekspertyza - poczatek odcinka przesunety
	//val = 2.107;

	//dlugi odcinek 28m - srodrk
//	x0 = 1790/param.get_dx(), y0 = 740/param.get_dy();//dla Wisly - ekspertyza - poczatek odcinka przesunety
//	val = 1.819;

	i1 = x0-2;// poczatek obszaru wsp x
	i2 = x0+2;// koniec obszaru wsp x
	j1 = y0-2; // poczatkek obszaru wsp y
	j2 = y0+2; // koniec obszaru wsp y
	for(int i = i1; i < i2+1; i++){
		for(int j = j1; j < j2+1; j++){
			index = i*10000 + j;
			if(PointMap.find(index) == PointMap.end()){
				cout<<"Nie znany index = "<<index<<endl;
				continue;
			}
			if(PointMap[index].get_flag() == 0){
				cout<<"Zadaje poza obszarem przeplywu!"<<endl
				<<PointMap[index]<<endl;
			}
			else {
      				if (i==i1+2 && j==j1+2) PointMap[index].set_Curr(val);
				//else if ((i==i1+1 && (j==j1+1 || j==j1+2 || j==j2-1)) || (i==i1+2 && (j==j1+1 || j==j2-1)) 
				//		|| (i==i2-1 && (j==j1+1 || j==j1+2 || j==j2-1)) ) 
				//	PointMap[index].set_Curr((3.0));
					//PointMap[index].set_Curr((0.5));
				//else PointMap[index].set_Curr((0.3125));
				//else PointMap[index].set_Curr((1.1875));
				total+=val;
      			}
    		}
	}
/*
	//dlugi odcinek 28m - koniec
	x0 = 1800/param.get_dx(), y0 = 750/param.get_dy();//dla Wisly - ekspertyza - poczatek odcinka przesunety
	val = 1.987;

	i1 = x0-2;// poczatek obszaru wsp x
	i2 = x0+2;// koniec obszaru wsp x
	j1 = y0-2; // poczatkek obszaru wsp y
	j2 = y0+2; // koniec obszaru wsp y
	for(int i = i1; i < i2+1; i++){
		for(int j = j1; j < j2+1; j++){
			index = i*10000 + j;
			if(PointMap.find(index) == PointMap.end()){
				cout<<"Nie znany index = "<<index<<endl;
				continue;
			}
			if(PointMap[index].get_flag() == 0){
				cout<<"Zadaje poza obszarem przeplywu!"<<endl
				<<PointMap[index]<<endl;
			}
			else {
      				if (i==i1+2 && j==j1+2) PointMap[index].set_Curr(val);
				//else if ((i==i1+1 && (j==j1+1 || j==j1+2 || j==j2-1)) || (i==i1+2 && (j==j1+1 || j==j2-1)) 
				//		|| (i==i2-1 && (j==j1+1 || j==j1+2 || j==j2-1)) ) 
				//	PointMap[index].set_Curr((3.0));
					//PointMap[index].set_Curr((0.5));
				//else PointMap[index].set_Curr((0.3125));
				//else PointMap[index].set_Curr((1.1875));
				total+=val;
      			}
    		}
	}

	val = 1.636;
*/
	out.close();
}
  

  
void Simulation::SetInitialConcentration (map<int, Point> &PointMap, SimulationParameters &param){
	ofstream out("Info.dat", ios::app);
	out<<"Ustawiam Poczatkowe wartosci koncentracji DELTA"<<endl;

//	double x0 = 1400/param.get_dx(), y0 = 250/param.get_dy();//dla Wisly - ekspertyza 
//	double x0 = 1400/param.get_dx(), y0 = 300/param.get_dy();//dla Wisly - ekspertyza 
//	double x0 = 1350/param.get_dx(), y0 = 200/param.get_dy();//dla Wisly - ekspertyza 
//	double x0 = 1500/param.get_dx(), y0 = 400/param.get_dy();//dla Wisly - ekspertyza 
//	double x0 = 1550/param.get_dx(), y0 = 350/param.get_dy();//dla Wisly - ekspertyza 

//nowa siatak
//	double x0 = 700/param.get_dx(), y0 = 200/param.get_dy();//dla Wisly - ekspertyza 
//	double x0 = 600/param.get_dx(), y0 = 100/param.get_dy();//dla Wisly - ekspertyza 

//ostat siatak
	//double x0 = 2700/param.get_dx(), y0 = 250/param.get_dy();//dla Wisly - ekspertyza 

	//zrodlo punktowe na srodku kanalu
	//double x0 = 1850/param.get_dx(), y0 = 775/param.get_dy();//dla Wisly - ekspertyza 
	double x0 = 1850/param.get_dx(), y0 = 800/param.get_dy();//dla Wisly - ekspertyza 


	//zrodlo punktowe na lewym brzegu
	//double x0 = 1700/param.get_dx(), y0 = 670/param.get_dy();//dla Wisly - ekspertyza 
	//double x0 = 1710/param.get_dx(), y0 = 680/param.get_dy();//dla Wisly - ekspertyza 
	//double x0 = 1710/param.get_dx(), y0 = 670/param.get_dy();//dla Wisly - ekspertyza -wariant przesuniety wzdluz rurociagu
	//double x0 = 1720/param.get_dx(), y0 = 680/param.get_dy();//dla Wisly - ekspertyza -wariant przesuniety wzdluz rurociagu


	//krotki odcinek 14m 
	//!double x0 = 1730/param.get_dx(), y0 = 690/param.get_dy();//dla Wisly - ekspertyza 
	//double x0 = 1710/param.get_dx(), y0 = 680/param.get_dy();//dla Wisly - ekspertyza - poczatek odcinka przesunety

	//dlugi odcinek 28m - poczatek
	//double x0 = 1780/param.get_dx(), y0 = 730/param.get_dy();//dla Wisly - ekspertyza - poczatek odcinka przesunety


//!!!!!! zrobic by bylo to czytane z pliku
	int i1,i2,j1,j2;
	//i1 = 100;// poczatek obszaru wsp x
	//i2 = 100;// koniec obszaru wsp x
	//j1 = 100;// poczatkek obszaru wsp y
	//j2 = 100;// koniec obszaru wsp y
	
	//i1 = 99;// poczatek obszaru wsp x
	//i2 = 101;// koniec obszaru wsp x
	//j1 = 99;// poczatkek obszaru wsp y
	//j2 = 101;// koniec obszaru wsp y
	
//	i1 = 98;// poczatek obszaru wsp x
//	i2 = 102;// koniec obszaru wsp x
//	j1 = 98;// poczatkek obszaru wsp y
//	j2 = 102;// koniec obszaru wsp y
	
/*	i1 = 298;// poczatek obszaru wsp x
	i2 = 302;// koniec obszaru wsp x
	j1 = 48;// poczatkek obszaru wsp y
	j2 = 52;// koniec obszaru wsp y
*/
	i1 = x0-2;// poczatek obszaru wsp x
	i2 = x0+2;// koniec obszaru wsp x
	j1 = y0-2; // poczatkek obszaru wsp y
	j2 = y0+2; // koniec obszaru wsp y

	//double val = 1;
	//double val = 7.0; //wisla, ekspertyza 
	
	//srpdek
	//double val = 4.3; //wisla, ekspertyza dla kroku dx =10
	//double val = 2.7; //wisla, ekspertyza dla kroku dx =25
	//double val = 5.3; //wisla, ekspertyza dla kroku dx =5
	//double val = 5.32; //wisla, ekspertyza dla kroku dx =5
	double val = 4.317; //wisla, ekspertyza dla kroku dx =10; po przesunieciu zrodla

	//lewy brzeg
	//double val = 4.785; //wisla, ekspertyza dla kroku dx =5 lewy b
	//double val = 4.567; //wisla, ekspertyza dla kroku dx =5 lewy b wersja II
	//double val = 3.635; //wisla, ekspertyza dla kroku dx =10 lewy b
//	double val = 3.389; //wisla, ekspertyza dla kroku dx =10 lewy b wersja II
	//double val = 1.910; //wisla, ekspertyza dla kroku dx =25 lewy b wersja II
	//double val = 3.458; //wisla, ekspertyza dla kroku dx =10 lewy brzeg pkt: 1710,670
	//double val = 3.274; //wisla, ekspertyza dla kroku dx =10 lewy brzeg pkt: 1710,670

	

	//krotki odcinek 14 m
	//!double val = 2.0; //wisla, ekspertyza dla kroku dx =10, na krotkim odcinku
	//po przesunieciu poczatku odcinka do pkt w ktorym jest zrodlo na lewym brzegu
	//double val = 2.235; //wisla, ekspertyza dla kroku dx =10, na krotkim odcinku

	//dlugi odcinek 28 m, poczatek,
	//double val = 1.636; //wisla, ekspertyza dla kroku dx =10, na krotkim odcinku

	//Wyczysc wartosci.
	map<int,Point>::iterator IT;
	for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
		IT->second.set_Prev(0);
		IT->second.set_Curr(0);
		IT->second.set_Next(0);
	}
	
	int index;
	double total = 0; //calkowita zadna na siatce koncentracja
	//Zadaj koncentacje w wybranym obszarze.
	for(int i = i1; i < i2+1; i++){
		for(int j = j1; j < j2+1; j++){
			index = i*10000 + j;
			if(PointMap.find(index) == PointMap.end()){
				out<<"Nie znany index = "<<index<<endl;
				continue;
			}
			if(PointMap[index].get_flag() == 0){
				out<<"Zadaje poza obszarem przeplywu!"<<endl
				<<PointMap[index]<<endl;
			}
			else {
      				if (i==i1+2 && j==j1+2) PointMap[index].set_Curr(val);
			//	else if ((i==i1+1 && (j==j1+1 || j==j1+2 || j==j2-1)) || (i==i1+2 && (j==j1+1 || j==j2-1)) 
			//			|| (i==i2-1 && (j==j1+1 || j==j1+2 || j==j2-1)) )
				//PointMap[index].set_Curr(0.5);
			//	PointMap[index].set_Curr(3.0);
				//else PointMap[index].set_Curr(0.3125);
		//		else PointMap[index].set_Curr(1.1875);
      				total+=val;
				cout<<"		zadaje, index: "<<index<<"  wartosc: "<<PointMap[index].get_Curr()<<endl;
				out<<"		zadaje, index: "<<index<<"  wartosc: "<<PointMap[index].get_Curr()<<endl;
      			}
/*			else {
      				if (i==100 & j == 100) PointMap[index].set_Curr(val);
				else if ((i==99 && (j==99 || j==100 || j==101)) || (i==100 && (j==99 || j==101)) 
						|| (i==101 && (j==99 || j==100 || j==101)))
				PointMap[index].set_Curr(0.5);
				else PointMap[index].set_Curr(0.3125);
      				total+=val;
				cout<<"		zadaje, index: "<<index<<"  wartosc: "<<PointMap[index].get_Curr()<<endl;
				out<<"		zadaje, index: "<<index<<"  wartosc: "<<PointMap[index].get_Curr()<<endl;
      			}
*/
    		}
	}

/*
	//krotki odcinek 14m 
	x0 = 1740/param.get_dx(), y0 = 700/param.get_dy();//dla Wisly - ekspertyza 
	//x0 = 1720/param.get_dx(), y0 = 690/param.get_dy();//dla Wisly - ekspertyza - koniec odcinka przesunety
	//val = 2.107;

	//dlugi odcinek 28m - srodrk
	//x0 = 1790/param.get_dx(), y0 = 740/param.get_dy();//dla Wisly - ekspertyza - poczatek odcinka przesunety
	//val = 1.819;

	i1 = x0-2;// poczatek obszaru wsp x
	i2 = x0+2;// koniec obszaru wsp x
	j1 = y0-2; // poczatkek obszaru wsp y
	j2 = y0+2; // koniec obszaru wsp y
	for(int i = i1; i < i2+1; i++){
		for(int j = j1; j < j2+1; j++){
			index = i*10000 + j;
			if(PointMap.find(index) == PointMap.end()){
				out<<"Nie znany index = "<<index<<endl;
				continue;
			}
			if(PointMap[index].get_flag() == 0){
				out<<"Zadaje poza obszarem przeplywu!"<<endl
				<<PointMap[index]<<endl;
			}
			else {
      				if (i==i1+2 && j==j1+2) PointMap[index].set_Curr(val);
			//	else if ((i==i1+1 && (j==j1+1 || j==j1+2 || j==j2-1)) || (i==i1+2 && (j==j1+1 || j==j2-1)) 
			//			|| (i==i2-1 && (j==j1+1 || j==j1+2 || j==j2-1)) )
				//PointMap[index].set_Curr(0.5);
			//	PointMap[index].set_Curr(3.0);
				//else PointMap[index].set_Curr(0.3125);
		//		else PointMap[index].set_Curr(1.1875);
      				total+=val;
				cout<<"		zadaje, index: "<<index<<"  wartosc: "<<PointMap[index].get_Curr()<<endl;
				out<<"		zadaje, index: "<<index<<"  wartosc: "<<PointMap[index].get_Curr()<<endl;
      			}
/*			else {
      				if (i==100 & j == 100) PointMap[index].set_Curr(val);
				else if ((i==99 && (j==99 || j==100 || j==101)) || (i==100 && (j==99 || j==101)) 
						|| (i==101 && (j==99 || j==100 || j==101)))
				PointMap[index].set_Curr(0.5);
				else PointMap[index].set_Curr(0.3125);
      				total+=val;
				cout<<"		zadaje, index: "<<index<<"  wartosc: "<<PointMap[index].get_Curr()<<endl;
				out<<"		zadaje, index: "<<index<<"  wartosc: "<<PointMap[index].get_Curr()<<endl;
      			}
*/
/*!    		}
	}
!*/
/*
	//dlugi odcinek 28m - koniec
	x0 = 1800/param.get_dx(), y0 = 750/param.get_dy();//dla Wisly - ekspertyza - poczatek odcinka przesunety
	val = 1.987;

	i1 = x0-2;// poczatek obszaru wsp x
	i2 = x0+2;// koniec obszaru wsp x
	j1 = y0-2; // poczatkek obszaru wsp y
	j2 = y0+2; // koniec obszaru wsp y
	for(int i = i1; i < i2+1; i++){
		for(int j = j1; j < j2+1; j++){
			index = i*10000 + j;
			if(PointMap.find(index) == PointMap.end()){
				out<<"Nie znany index = "<<index<<endl;
				continue;
			}
			if(PointMap[index].get_flag() == 0){
				out<<"Zadaje poza obszarem przeplywu!"<<endl
				<<PointMap[index]<<endl;
			}
			else {
      				if (i==i1+2 && j==j1+2) PointMap[index].set_Curr(val);
			//	else if ((i==i1+1 && (j==j1+1 || j==j1+2 || j==j2-1)) || (i==i1+2 && (j==j1+1 || j==j2-1)) 
			//			|| (i==i2-1 && (j==j1+1 || j==j1+2 || j==j2-1)) )
				//PointMap[index].set_Curr(0.5);
			//	PointMap[index].set_Curr(3.0);
				//else PointMap[index].set_Curr(0.3125);
		//		else PointMap[index].set_Curr(1.1875);
      				total+=val;
				cout<<"		zadaje, index: "<<index<<"  wartosc: "<<PointMap[index].get_Curr()<<endl;
				out<<"		zadaje, index: "<<index<<"  wartosc: "<<PointMap[index].get_Curr()<<endl;
      			}
/*			else {
      				if (i==100 & j == 100) PointMap[index].set_Curr(val);
				else if ((i==99 && (j==99 || j==100 || j==101)) || (i==100 && (j==99 || j==101)) 
						|| (i==101 && (j==99 || j==100 || j==101)))
				PointMap[index].set_Curr(0.5);
				else PointMap[index].set_Curr(0.3125);
      				total+=val;
				cout<<"		zadaje, index: "<<index<<"  wartosc: "<<PointMap[index].get_Curr()<<endl;
				out<<"		zadaje, index: "<<index<<"  wartosc: "<<PointMap[index].get_Curr()<<endl;
      			}
*/
/*    		}
	}
*/
	out.close();
}

void Simulation::SetInitialConcentrationGauss (map<int, Point> &PointMap, SimulationParameters &param){
	
	ofstream out("Info.dat", ios::app);
 	
	out<<"Ustawiam Poczatkowe wartosci koncentracji - GAUSS"<<endl;
	
	//double x0 = 500/param.get_dx(), y0 = 500/param.get_dy();
	//double x0 = 800/param.get_dx(), y0 = 700/param.get_dy();
	//double x0 = 680/param.get_dx(), y0 = 60/param.get_dy();
	//double x0 = 1000/param.get_dx(), y0 = 700/param.get_dy();//dla Mississipi (poczatek kanalu)
	double x0 = 1200/param.get_dx(), y0 = 500/param.get_dy();//dla Wisly - ekspertyza 
	
	int krok = 1, i = -9999, j = -9999, flag = -9999;
	double val = 10; // wartosc koncentracji
	double c0 = 0;
	double gx = 0, gy = 0; 
	double sigmaX, sigmaY, meanX, meanY;
	double dx = param.get_dx(), dy = param.get_dy(), dt = param.get_dt();
	double vx, vy, Dxx, Dyy; 
	double pi =  3.141592654;
	double total = 0; //calkowita zadna na siatce koncentracja
	//Wyczysc wartosci.
	map<int,Point>::iterator IT;
	for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
	  //AK IT->second.set_Prev(0);
	  //AK IT->second.set_Curr(0);
	  //AK IT->second.set_Next(0);
		flag = IT->second.get_flag();
		if (flag == 1){
			//IT->second.print();
			i = IT->second.get_x();
			j = IT->second.get_y();
		
			//vx = IT->second.get_vx();
			//vy = IT->second.get_vy();
			//Dxx = IT->second.get_Dxx();
			//Dyy = IT->second.get_Dyy();
			vx = 0.15;
			vy = 0.0;
			Dxx = 0.016;
			Dyy = 0.016;
		
			sigmaX=sqrt(Dxx*2.0*krok*dt/dx/dx);
			sigmaY=sqrt(Dyy*2.0*krok*dt/dy/dy);
			sigmaX=sqrt(1.0);
			sigmaY=sqrt(1.0);
   			meanX = (x0+vx*krok*dt/dx);
			meanY = (y0+vy*krok*dt/dy);
			
			//gx = exp(  ( - pow((i-meanX),2)  /  (2*pow(sigmaX,2)))  ) / (sqrt(2*pi)*sigmaX);
			//if (sigmaY == 0) gy = 1;
			//else gy = exp(  ( - pow((j-meanY),2)  /  (2*pow(sigmaY,2)))  ) / (sqrt(2*pi)*sigmaY);
			//sigmaX = 1.0;
			//sigmaY = 1.0;
			//val = 1.0;
			gx = Gaus(i, meanX, sigmaX, true);
			gy = Gaus(j, meanY, sigmaY, true);
			
			c0 = val * gx * gy;
			//if (c0 > 1e-2){
			//cout<<"gx: "<<gx<<"  gy: "<<gy<<endl;
			//cout<<"		zadaje, index: "<<i<<", "<<j<<"  wartosc: "<<c0<<endl;
			//}
                        double tmp = IT->second.get_Curr();
			IT->second.set_Curr(c0+tmp);
			total = total + c0;//}
		}
	} 
    out.precision(20);
	out<<"3SigmaX: "<<3*sigmaX<<"   3SigmaY: "<<3*sigmaY<<endl;
  	out<<"Zadano koncentracje" 
		<<" Calkowita koncentracja = "<<sqrt(total)<<" "<<total<<endl;
	
	out.close();
	
}

 double Simulation::Gaus(double x, double mean, double sigma, bool norm)
{
   // Calculate a gaussian function with mean and sigma.
   // If norm=kTRUE (default is kFALSE) the result is divided
   // by sqrt(2*Pi)*sigma.

   if (sigma == 0) return 1;
   double arg = (x-mean)/sigma;
   double res = exp(-0.5*arg*arg); //musze zminic ze zwraca arg a nie res bo  
   if (!norm) return res;
   //return res/(2.5066282880429056057*sigma); //Normalised on grid;
   return res/(2.50662827463100024*sigma); //sqrt(2*Pi)=2.50662827463100024
}

void Simulation::SetInitialConcentrationGaussNonDiag (map<int, Point> &PointMap, SimulationParameters &param){
	
	ofstream out("Info.dat", ios::app);
	out<<"Ustawiam Poczatkowe wartosci koncentracji - GAUSSNonDiag"<<endl;
	
	double x0 = 50.0, y0 = 50.0;
	int krok = 200, i = -9999, j = -9999, flag = -9999;
	double val = 10; // wartosc koncentracji
	double c0 = 0;
	double gx = 0, gy = 0, gxy = 0; 
	double sigmaX, sigmaY, meanX, meanY, sigmaXY;
	double dx = param.get_dx(), dy = param.get_dy(), dt = param.get_dt();
	double vx, vy, Dxx, Dyy, Dxy; 
	double pi =  3.141592654;
	double total = 0; //calkowita zadna na siatce koncentracja
     
   	//Dxx = 1.6, Dyy = 1.6, Dxy = 1.4;
   	//vx = sqrt(0.08), vy = sqrt(0.08);
	//Dxx = 0.425, Dyy = 0.425, Dxy = 0.325;
  	//vx = sqrt(0.01125), vy = sqrt(0.01125);
	//vx = 0.106066,  vy = 0.106066;
	Dxx = param.get_Dxxmax(), Dyy = param.get_Dyymax(), Dxy = param.get_Dxymax();
  	vx = param.get_vxmax(), vy = param.get_vymax();
	
   	double D = (Dxx*Dyy - Dxy*Dxy);
   	
	//Double_t norm = 2*2.50662827463100024*sqrt(D*krok*dt);
   	//double norm = 2*pi*sqrt(D*krok*dt);
	double norm = 2*3.141*sqrt(4*krok*dt*dt*krok*D);
	double argX=0.0, argY=0.0, argXY=0.0;
	//Wyczysc wartosci.
	map<int,Point>::iterator IT;
	int tmp = 0;
	for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
		IT->second.set_Prev(0);
		IT->second.set_Curr(0);
		IT->second.set_Next(0);
		flag = IT->second.get_flag();
		if (flag == 1){
			//IT->second.print();
			i = IT->second.get_x();
			j = IT->second.get_y();
		
			/*vx = IT->second.get_vx();
			vy = IT->second.get_vy();
			Dxx = IT->second.get_Dxx();
			Dyy = IT->second.get_Dyy();
			sigmaX=sqrt((D/Dyy)*krok*dt/(dx*dx));
			sigmaY=sqrt((D/Dxx)*krok*dt/(dy*dy)); 
			sigmaXY = sqrt((D/Dxy)*krok*dt/(dx*dy));
   			meanX = x0+(vx*krok*dt/dx);
			meanY = y0+(vy*krok*dt/dy);
			*/
   			
			//vx = IT->second.get_vx();
			//vy = IT->second.get_vy();
			//Dxx = IT->second.get_Dxx();
			//Dyy = IT->second.get_Dyy();
			//D = (Dxx*Dyy - Dxy*Dxy);
			
			sigmaX=sqrt((2*krok*dt*D)/Dyy);
			sigmaY=sqrt((2*krok*dt*D)/Dxx); 
			sigmaXY = sqrt((krok*dt*D)/Dxy);
   			meanX = x0+(vx*krok*dt);
			meanY = y0+(vy*krok*dt);
			
			if (tmp == 0){
				cout<<" SigmaX: "<<sigmaX<<endl;
				cout<<" SigmaY: "<<sigmaY<<endl;
				cout<<" SigmaXY: "<<sigmaXY<<endl;
				cout<<" meanX: "<<meanX<<endl;
				cout<<" meanY: "<<meanY<<endl;
				cout<<" norm: "<<norm<<endl;
				cout<<endl;
				tmp = 1;
			}
			
			//gx = Gaus(i, meanX, sigmaX, false);
			//gy = Gaus(j, meanY, sigmaY, false);
			//gxy = My_Gaus(i, j, meanX, meanY, sigmaXY);
			
			//if (sigma == 0)	
   			argX = (i-meanX)/sigmaX;
			argY = (j-meanY)/sigmaY;
			
			argXY = -(i-meanX)*(j-meanY)/(sigmaXY*sigmaXY);
			
			
   			c0 = exp((-0.5*argX*argX)+(-0.5*argY*argY)+(-0.5*argXY) ); //musze zminic ze zwraca arg a nie re
			c0= (val*c0)/norm;
			//c0 = (val * gx * gy * gxy)/norm;
			//if (c0 > 1e-2){
			//cout<<"gx: "<<gx<<"  gy: "<<gy<<endl;
			//cout<<"		zadaje, index: "<<i<<", "<<j<<"  wartosc: "<<c0<<endl;
			//}
			//cout<<i<<","<<j<<"   ";
			//cout<<"gx: "<<gx<<"  gy: "<<gy<<"  gxy: "<<gxy<<"  calkowita: "<<c0<<endl;	
			
			IT->second.set_Curr(c0);
			total = total + c0;//}
		}
	} 
    out.precision(20);
	out<<"3SigmaX: "<<3*sigmaX<<"   3SigmaY: "<<3*sigmaY<<endl;
  	out<<"Zadano koncentracje" 
		<<" Calkowita koncentracja = "<<sqrt(total)<<" "<<total<<endl;
	out.close();
	
}


void Simulation::SetInitialConcentrationPlaneWave (map<int, Point> &PointMap){

  cout<<"test 2 4 sty 2010"<<endl;
  char* text = getenv("lambda"); 
  if (text==NULL) {
	cout<<"Wybrano funkcje \"SetInitialConcentrationPlaneWave\" do zadawania koncentracji poczatkowej, w sytemie nie zdefiniowano dlugosci fali - lambda"<<endl<<"zdefiniuj zmienna labda lub wybierz inny sposob zadawania koncentracji poczatkowej :  "<<endl;
	;
	return;
  }
  int lambda = atoi(text);
  double ampl = 10;
  cout<<"lambda: "<<lambda<<endl;
  double k = 2*3.14159265358979312/lambda;

  int i,j;
  double c0;
  map<int,Point>::iterator IT;
  for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
    IT->second.set_Prev(0);
    IT->second.set_Curr(0);
    IT->second.set_Next(0);
    /*
    if (IT->second.get_flag() == 1 ||
	IT->second.get_flag() == 601 ||
	IT->second.get_flag() == 801){
    */
      i = IT->second.get_x();
      j = IT->second.get_y();     
      c0 = ampl*sin(k*i)+ampl;
      IT->second.set_Curr(c0);
      //}
  }
}

//******* wyznacza rozklad gausa w przypdaku niediagonalnego tensora dyspersji *****//
double Simulation::My_Gaus(double x, double y, double meanX, double meanY, double sigma)
{  
  if (sigma == 0) return 1.e30;
  long double arg = -(x-meanX)*(y-meanY)/(sigma*sigma);
  long double res = exp(-0.5*arg);
  if (res>10) cout<<" 	res: "<<res<<"  arg: "<<arg<<endl;
  return res;
  //   return res/(2.50662827463100024*sigma); //sqrt(2*Pi)=2.50662827463100024
}


void Simulation::SetInOutPointValue (map<int, Point> &PointMap){

  cout<<"ustawiam puknty wejscia wyjscia"<<endl;
  map<int,Point>::iterator IT;
  int  i = -9999, j = -9999, flag = -9999, index=-9999;
  double val=0.0;
  for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
    flag = IT->second.get_flag();
    if (flag > 600 && flag < 700) {
      i = IT->second.get_x();
      j = IT->second.get_y();
      index = (i+1)*10000 + j;
      val = (PointMap.find(index)->second.get_Curr());
      IT->second.set_Curr(val);
      /*
      index = (i+1)*10000 + j;
      PointMap.find(index)->second.set_flag(flag);
      index = (i+2)*10000 + j;
      PointMap.find(index)->second.set_flag(flag);
      index = (i+3)*10000 + j;
      PointMap.find(index)->second.set_flag(flag);
      IT->second.set_Curr(0);
      */
    }
    if (flag > 800 && flag < 900) {
      i = IT->second.get_x();
      j = IT->second.get_y();
      //if(i<350) continue;
      index = (i-1)*10000 + j;
      PointMap.find(index)->second.set_flag(0);
      index = (i-2)*10000 + j;
      val = val + PointMap.find(index)->second.get_Curr();
      PointMap.find(index)->second.set_flag(0);
      index = (i-3)*10000 + j;
      val = val - PointMap.find(index)->second.get_Curr();
      PointMap.find(index)->second.set_flag(0);
      PointMap.find(index)->second.set_Curr(0);
      IT->second.set_flag(0);
      cout<<"i: "<<i<<" flag: "<<flag<<endl;
    }
  }
}
