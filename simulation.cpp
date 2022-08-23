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
 
 
 Simulation::Simulation(const string &paramFile, const string &saveFile ): 
 		_file_grid("grid"), _file_v("v"),
		_file_data("data"), _file_d("d"){
	MySimulation(paramFile, saveFile);
 };
 //_______________________________________________________________________________
 
 
 void Simulation::MySimulation (const string &param_file, const string &save_file){

	map<int, Point> PointMap;
	SimulationParameters sym_param(param_file);	

	int check = SetParam(&sym_param, param_file);
	if (check==0) return;

	check = PrepareMap(PointMap, sym_param);
	if (check==0) return;	

	check = SetInitialConcentration (PointMap, sym_param);
	if (check==0) return;

	check = SetSaveParameters(save_file, sym_param.get_n());	
	if (check==0) return;
	
	SaveMap(PointMap,  _file + "_all.dat", "all");

	cout.precision(4);
	double Cn = sym_param.ComputeCourantNumber();
	double Pe = sym_param.ComputePecletNumber(PointMap);


	cout<<"Max Courant number: "<<Cn<<"	";
	cout<<"Max Pecklet number: "<<Pe<<endl;	
	ofstream out("Info.dat", ios::app);
	out<<"Max Courant number: "<<Cn<<"	";
	out<<"Max Pecklet number: "<<Pe<<endl;	
	
	SScheme *my_scheme;
	my_scheme = sym_param.get_s_scheme();
	
	my_scheme->SetFactors(PointMap);
	
	string sch = sym_param.get_scheme();
	my_scheme->ReadMethod(sch);

	int i = 0;
	string step, step_plus;
	
	//Zapisuję koncentracje poczatkowa do pliku
	SaveMap(PointMap, _file + "_Conc_" + sch + "_0.dat", "conc");
	
	//return;
	
	//my_scheme->set_omega(1.0);
	my_scheme->powrot = 0;
	
	time_t  t0, t1; 
  	clock_t c0, c1; 
	ofstream out1("Szybkosc.txt", ios::app);

	//Zapisujemy mape z oznaczonymi pkt przeplywu i poza nim (Flagami odpowienio 1,0))	
	SaveMap(PointMap, _file + "_po.dat", "grid");
	
	//tutaj nowa funkcja ustawiajaca zapis
	
	for (i = 1; i < sym_param.get_n() + 1; i++ ){
	  cout<<"KROK: "<<i<<"  "<<std::endl;
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
		//SetSourceConcentrationTemp (PointMap, sym_param, val);	
		/*if ( i % 100 == 0 || i==1 || i==1 ||i==2 ||i==3 ||i==4 ||i==5 || i==10 || i==50 ) {
			step_plus = _file + "_Conc_" + sch + "_" + step + ".dat";
			SaveMap(PointMap, step_plus, "conc");
		}*/
		if ( _save.find(i) != _save.end()) {
			cout<<"Zapisuję krok: "<<i<<endl;			
			step_plus = _file + "_Conc_" + sch + "_" + std::to_string(i) + ".dat";
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
	string scheme = "nie zdefinowany", disp = "nie okreslony", inFile = "nie znany";
	//cout<<"falga:"<<flag<<"  n: "<<n<<"  dx: "<<dx<<"  dy: "<<dy<<"  dt: "<<dt<<"  schem: "<<scheme<<endl;
	//cout<<"vx: "<<vx<<"  vy: "<<vy<<"  Dlong: "<<Dlong<<"  Dtrans: "<<Dtrans<<endl;
		
	fstream in(file.c_str());
	if(!in) {
		CheckFile(0,file);
		return 0;// jesli nie ma pliku wychodzi z funkci zwracajac 0
	}
	else CheckFile(1,file);		
	in>>flag;
		
	switch(flag){
		case 0:
			in>>inFile>>n>>dx>>dy>>dt>>scheme>>disp>>vx>>vy>>Dlong>>Dtrans;
			break;
		case 1:
			in>>inFile>>n>>dx>>dy>>dt>>scheme>>disp>>Dlong>>Dtrans;
			break;
		case 2:
			in>>inFile>>n>>dx>>dy>>dt>>scheme>>disp;
			break;
		case 3:
			in>>inFile>>n>>dx>>dy>>dt>>scheme>>disp>>vx>>vy;
	}
	in.close();
	
	//wyciana rozszerzenia z nazwy schytanego pliku - jesli w nazwie jest "." (".txt")
	string inFile_org = inFile;
	size_t found = inFile.find(".");
	if (found!=string::npos) inFile.replace(inFile.end()-4,inFile.end(),""); 
	
	param->set_flag(flag);
	param->set_n(n);
	param->set_gridFile(inFile);
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
	cout<<endl<<"  falga: "<<flag<<"  plik wejsciowy z siatką: "<<inFile_org<<endl
			<<"  n: "<<n<<"  dx: "<<dx<<"  dy: "<<dy<<"  dt: "<<dt<<endl
			<<"  schemat: "<<scheme<<"  sposob obliczania dyspersji: "<<disp<<endl
			<<"  vx: "<<vx<<"  vy: "<<vy<<"  Dlong: "<<Dlong<<"  Dtrans: "<<Dtrans<<endl;	
	ofstream out("Info.dat", ios::app);
	out<<endl<<"  falga: "<<flag<<"  plik wejsciowy z siatką: "<<inFile<<endl
			<<"  n: "<<n<<"  dx: "<<dx<<"  dy: "<<dy<<"  dt: "<<dt<<endl
			<<"  schemat: "<<scheme<<"  sposob obliczania dyspersji: "<<disp<<endl
			<<"  vx: "<<vx<<"  vy: "<<vy<<"  Dlong: "<<Dlong<<"  Dtrans: "<<Dtrans<<endl;	
	out.close();
	return 1; //zwraca 1 jesli wszystko bylo ok 
 };
 //_______________________________________________________________________________________________________
 
 
 int Simulation::PrepareMap(map<int, Point> &PointMap, SimulationParameters &param){
 	ofstream out("Info.dat", ios::app);
	cout<<endl<<"... Przygotowuje mape ...   "<<endl;
	out<<endl<<"... Przygotowuje mape ...   "<<endl;
	out.close();
	
	int flag=param.get_flag();
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
	
	if (param.get_gridFile() != " ") {
	  file = param.get_gridFile();
	  this->set_file(file);
	}
	//czyta okreslony plik z danymi i uzupemnie mape sczytami punktami
	int check = my_grid->ReadData(PointMap, _file + ".txt", param);

	SaveMap(PointMap, file + "_00_grid.dat", "grid");

	//ustwaiamy mape dla naszych pkt	
  	Point dummy;//pusty pkt
  	PointMap[my_grid->empty_index] = dummy;
  	PointMap[my_grid->empty_index].set_map(&PointMap);
	
	
	//korzystajac z wyznaczony wartosci namniejszych najwiekszych wspolrzednych na siatce,
	//uzuplniamy siatke pkt ktore leza poza przeplywem nadajac im flage 0
	my_grid->CompleteOutsideFlow(PointMap);


	SaveMap(PointMap, file + "01_grid.dat", "grid");
	SaveMap(PointMap, file + "01_all.dat", "all");
	SaveMap(PointMap, file + "01_conc.dat", "conc");
	
	//Zapisujemy mape z oznaczonymi pkt przeplywu i poza nim (Flagami odpowienio 1,0))	
	SaveMap(PointMap, file + ".dat", "grid");
		
	//ustawiamy indexy punktow bedacych sasiadami danego pkt
	// i obliczamy liczby Couranta dla kazdego pkt 
	map<int, Point>::iterator CI; //iterator do mapy punktow 
	for(CI=PointMap.begin();CI!=PointMap.end();CI++){// po 
		CI->second.set_near_neighbours(1);
		CI->second.set_distant_neighbours(1);
 		CI->second.set_Courant_numbers(param.get_dx(), param.get_dy(), param.get_dt());
	}

	
	//czy poprawic siatke ???
	int popraw = 1;
	//poprawia siatke
	do {
	  popraw = my_grid->CorrectGrid(PointMap);
	} while (popraw==1);

	SaveMap(PointMap, file + "02_grid.dat", "grid");
	SaveMap(PointMap, file + "02_all.dat", "all");
	SaveMap(PointMap, file + "02_conc.dat", "conc");

	//return 0;
	//onaczamy brzegi nadajac im odpowienidnie flagi
	my_grid->MarkBorderPoints(PointMap);	

	SaveMap(PointMap, file + "03_grid.dat", "grid");
	SaveMap(PointMap, file + "03_all.dat", "all");
	SaveMap(PointMap, file + "03_conc.dat", "conc");
	
	SaveMap(PointMap, file + "a.dat", "grid");
	
	//onaczamy pkt przy brzegach nadajac im odpowienidnie flagi	
	//my_grid->MarkNearBorderPoints(PointMap);
	//SaveMap(PointMap, file + "b.dat", "grid");
	
	//check = my_grid->MarkInOutPoints(PointMap);

	//	SaveMap(PointMap, file + "c.dat", "grid");

	return check; //zwraca 1 gdy wszytko poszlo ok, 0 gdy nie znaleziono potrzebnego pliku
 };
 //_______________________________________________________________________________________________________
 
int Simulation::SetSaveParameters(const string &file, int n){
 	ofstream out("Info.dat", ios::app);
	cout<<endl<<"... Ustawiam parametry zapisu symulacji ...   "<<endl;
	out<<endl<<"... Ustawiam parametry zapisu symulacji ...   "<<endl;
	out.close();
	
	string range = "notdefine", pattern = "notdefine";
	
	int step = 0;
	string step_tmp;
	
	fstream in(file.c_str());
	if(!in) {
		CheckFile(0,file);
		return 0;// jesli nie ma pliku wychodzi z funkci zwracajac 0
	}
	else CheckFile(1,file);		
	in>>range;
	//cout<<range<<endl;
	
	if (range == "all") {
		for (int i = 0; i <= n; i++) _save.insert(i);
		cout<<"Wyniki symulacji zostaną zapisane po każdym kroku czasowym;"<<endl;
		return 1;
	}
	else 
	{
		in>>pattern;
		//cout<<pattern<<endl;

		if (pattern == "last") _save.insert(n);
		else {
			int pat;
			bool spr = pat = atoi(pattern.c_str());
			if (spr == 0 || pat < 0 )
			{
				cout<<"!!!! Cos poszlo nie tak, sprawdz format pliku wejściowego z opcjami zapisu (save.txt)"<<endl;
				return 0;
			}
			else {
				for (int i = 0; i <= n; i=i+pat) {
					_save.insert(i);
				}
			}
		}
		
		while(!in.eof()){
			in>>step_tmp;
			if (in.eof()) {
				cout<<"Wyniki symulacji zostaną zapisane po następujących krokach czasowych: "; 
				set<int>::iterator it;
				for (it=_save.begin(); it!=_save.end(); it++) cout<<*it<<"; ";
				cout<<endl;
				return 1;
			}
			else{
				bool spr = step = atoi(step_tmp.c_str()); 
				if (spr == 0 || step < 0)
				{
					cout<<"!!!! Cos poszlo nie tak, sprawdz format pliku wejściowego z opcjami zapisu (save.txt)"<<endl;
					return 0;
				}
				else{
					_save.insert(step);
				}
			}
		}
		cout<<"Wyniki symulacji zostaną zapisane po następujących krokach czasowych: "; 
		set<int>::iterator it;
		for (it=_save.begin(); it!=_save.end(); it++) cout<<*it<<"; ";
		cout<<endl;
		return 1;
	}
}
 
 
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
		else out<<CI->second.get_x()<<" "
        			<<CI->second.get_y()<<endl;
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
	
// zrobic by bylo to czytane z pliku
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

	//////double x0 = 50/param.get_dx(), y0 = 50/param.get_dy();//testy sztucznego zrodla grid14 
	
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

	////////// double x0 = 14300/param.get_dx(), y0 = 1200/param.get_dy();//dla Wisly - ekspertyza 


	int i1,i2,j1,j2;

	i1 = x0-2;// poczatek obszaru wsp x
	i2 = x0+2;// koniec obszaru wsp x
	j1 = y0-2; // poczatkek obszaru wsp y
	j2 = y0+2; // koniec obszaru wsp y

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
			if(PointMap.find(index) == PointMap.end()){	SetInitialConcentration (PointMap, sym_param);

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

void Simulation::SetInitialConcentrationPoint (map<int, Point> &PointMap, SimulationParameters &param, double x0, double y0, double val){
	ofstream out("Info.dat", ios::app);
	out<<endl<<"... Ustawiam Poczatkowe wartosci koncentracji POINT (DELTA) ... "<<endl;
	cout<<endl<<"... Ustawiam Poczatkowe wartosci koncentracji POINT (DELTA) ... "<<endl;

	int i = x0/param.get_dx(), j = y0/param.get_dy(), index;
	double tmp;
	//Zadaj koncentacje w wybranym pkt
	index = i*10000 + j;
		if(PointMap.find(index) == PointMap.end()){
			out<<"Nie znany index = "<<index<<endl;
			cout<<"Nie znany index = "<<index<<endl;
			return;	
		}
		else if(PointMap[index].get_flag() == 0){
			out<<"Zadaje poza obszarem przeplywu!"<<endl<<PointMap[index]<<endl;
			cout<<"Zadaje poza obszarem przeplywu!"<<endl<<PointMap[index]<<endl;
		}
		else {

			PointMap[index].set_Curr(val);
			cout<<"	Zadaje koncentracje poczatkowa, index: "<<index<<" Pkt: ("<<x0<<", "<<y0<<") "<<"  wartosc: "<<PointMap[index].get_Curr()<<endl;
			out<<"	Zadaje koncentracje poczatkowa, index: "<<index<<" Pkt: ("<<x0<<", "<<y0<<") "<<"  wartosc: "<<PointMap[index].get_Curr()<<endl;
		}
	out.close();
}

void Simulation::SetInitialConcentrationGauss (map<int, Point> &PointMap, SimulationParameters &param, double x0, double y0, double val, int krok){
	
	ofstream out("Info.dat", ios::app); 	
	out<<endl<<"... Ustawiam Poczatkowe wartosci koncentracji - GAUSS ..."<<endl;
	cout<<endl<<"... Ustawiam Poczatkowe wartosci koncentracji - GAUSS ..."<<endl;
	
	//int krok = 100;
	int i = -9999, j = -9999, flag = -9999;

	double c0 = 0.0;  
	double gx = 0.0, gy = 0.0; 
	double sigmaX = 0.0, sigmaY = 0.0, meanX = 0.0, meanY = 0.0;
	double dx = param.get_dx(), dy = param.get_dy(), dt = param.get_dt();
	double vx, vy, Dxx, Dyy; 
//	double pi =  3.14159265358979323846;
	double pi =  M_PI;

	double total = 0; //calkowita zadna na siatce koncentracja

	int index = x0/dx*10000 + y0/dy;

	vx = PointMap[index].get_vx();
	vy = PointMap[index].get_vy();
	Dxx = PointMap[index].get_Dxx();
	Dyy = PointMap[index].get_Dyy();
	cout<<"Guass parametry: "<<"vx = "<<vx<<"  vy = "<<vy<<"  Dxx = "<<Dxx<<"  Dyy = "<<Dyy<<endl;	

	sigmaX=sqrt(Dxx*2.0*krok*dt/(dx*dx));
	sigmaY=sqrt(Dyy*2.0*krok*dt/(dy*dy));
	//sigmaX=sqrt(1.0);
	//sigmaY=sqrt(1.0);
	meanX = (x0/dx+(vx*krok*dt/dx));
	meanY = (y0/dy+(vy*krok*dt/dy));
	cout<<"                 "<<"sigmaX = "<<sigmaX<<"  sigmaY = "<<sigmaY<<"  meanX = "<<meanX<<"  meanY = "<<meanY<<endl;	

	
	map<int,Point>::iterator IT;
	for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
		IT->second.set_Prev(0);
		IT->second.set_Curr(0);
		IT->second.set_Next(0);
		flag = IT->second.get_flag();
		if (flag == 1){
			//IT->second.print();
			i = IT->second.get_x();
			j = IT->second.get_y();

			gx = Gaus(i, meanX, sigmaX, true);
			gy = Gaus(j, meanY, sigmaY, true);
			
			c0 = val * gx * gy;

			double tmp = IT->second.get_Curr();
			IT->second.set_Curr(c0+tmp);
			total = total + c0;//}
		}
	} 
	cout.precision(10);
	cout<<"                 3SigmaX: "<<3*sigmaX<<"   3SigmaY: "<<3*sigmaY<<endl;
  	cout<<"Zadano koncentracje ->" 
		<<" Calkowita zadana koncentracja "<<" total = "<<total<<endl;	
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
   //if (res > 0.1) cout<<endl<<res<<endl;
   if (!norm) return res;
   //if (res > 0.1) cout<<"i licze norme"<<endl;
   //if (res > 0.1) cout<<res/(2.50662827463100024*sigma)<<endl;
   //return res/(2.5066282880429056057*sigma); //Normalised on grid;
   return res/(2.50662827463100024*sigma); //sqrt(2*Pi)=2.50662827463100024
}

void Simulation::SetInitialConcentrationGaussNonDiag (map<int, Point> &PointMap, SimulationParameters &param, double x0, double y0, double val, int krok){
	
	ofstream out("Info.dat", ios::app);
	out<<endl<<"... Ustawiam Poczatkowe wartosci koncentracji - GAUSSNonDiag ..."<<endl;
	cout<<endl<<"... Ustawiam Poczatkowe wartosci koncentracji - GAUSSNonDiag ..."<<endl;
	
	//int krok = 100;
	int i = -9999, j = -9999, flag = -9999;

	double c0 = 0.0;  
	double gx = 0.0, gy = 0.0, gxy = 0.0; 
	double sigmaX = 0.0, sigmaY = 0.0, sigmaXY = 0.0, meanX = 0.0, meanY = 0.0;
	double dx = param.get_dx(), dy = param.get_dy(), dt = param.get_dt();
	double vx, vy, Dxx, Dyy, Dxy; 
	double pi =  3.14159265358979323846;
	double total = 0; //calkowita zadna na siatce koncentracja
     
	int index = x0/dx*10000 + y0/dy;
	vx = PointMap[index].get_vx();
	vy = PointMap[index].get_vy();
	Dxx = PointMap[index].get_Dxx();
	Dxy = PointMap[index].get_Dxy();
	Dyy = PointMap[index].get_Dyy();
	
   	double D = (Dxx*Dyy - Dxy*Dxy);
	cout<<"GuassNonDiag parametry: "<<"vx = "<<vx<<"  vy = "<<vy<<"  Dxx = "<<Dxx<<"  Dyy = "<<Dyy<<"  Dxy = "<<Dxy<<"  D = "<<D<<endl;;	

	//sigmaX=sqrt((D/Dyy)*2.0*krok*dt);
	//sigmaY=sqrt((D/Dxx)*2.0*krok*dt); 
	//sigmaXY = sqrt(fabs((D/Dxy)*krok*dt));
	//sigmaXY = sqrt(fabs((D/Dxy)*krok*dt));
	
	sigmaX=sqrt((D/Dyy)*2*krok*dt/(dx*dx));
	sigmaY=sqrt((D/Dxx)*2*krok*dt/(dy*dy)); 
	sigmaXY = sqrt(fabs((D/Dxy)*krok*dt/(dx*dy)));
	//meanX = x0+(vx*krok*dt/dx);
	//meanY = y0+(vy*krok*dt/dy);
			
	//meanX = (x0/dx+(vx*krok*dt));
	//meanY = (y0/dy+(vy*krok*dt));
	meanX = (x0/dx+(vx*krok*dt/dx));
	meanY = (y0/dy+(vy*krok*dt/dy));

	cout<<"                        "<<"sigmaX = "<<sigmaX<<"  sigmaY = "<<sigmaY<<"  sigmaXY = "<<sigmaXY<<"  meanX = "<<meanX<<"  meanY = "<<meanY<<endl;	

	
	//Double_t norm = 2*2.50662827463100024*sqrt(D*krok*dt);
///!!!   	double norm = 4*pi*krok*dt*sqrt(D);
   	double norm = 4*pi*krok*dt*sqrt(D)/(dx*dy);
	//double norm = 2*3.141*sqrt(4*krok*dt*dt*krok*D);

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
		
			/*
			//gdybysmy chcieli brac inne wartosci wspolczynikow dla kazdego z punktow
			vx = IT->second.get_vx();
			vy = IT->second.get_vy();
			Dxx = IT->second.get_Dxx();
			Dyy = IT->second.get_Dyy();
			Dxy = IT->second.get_Dxy();
			//Dyx = IT->second.get_Dxx();

			D = (Dxx*Dyy - Dxy*Dxy);
			*/
			
			//gx = Gaus(i, meanX, sigmaX, false);
			//gy = Gaus(j, meanY, sigmaY, false);
			//gxy = My_Gaus(i, j, meanX, meanY, sigmaXY);
			
			//if (sigma == 0)	
   			argX = (i-meanX)/sigmaX;
			argY = (j-meanY)/sigmaY;
			
			argXY = -(i-meanX)*(j-meanY)/(sigmaXY*sigmaXY);
			
			
   			c0 = exp((-0.5*argX*argX)+(-0.5*argY*argY)+(-0.5*argXY) ); //musze zminic ze zwraca arg a nie re
			c0= (val*c0)/norm; // zeby bylo dobrze musialm przemnozyc przez krok czasowy i przestrzenyy - sparwdzic dlaczego
			
			IT->second.set_Curr(c0);
			total = total + c0;//}
		}
	} 
	cout.precision(10);
	cout<<"                 3SigmaX: "<<3*sigmaX<<"   3SigmaY: "<<3*sigmaY<<endl;
  	cout<<"Zadano koncentracje ->" 
		<<" Calkowita zadana koncentracja "<<" total = "<<total<<endl;	
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
	IT->second.get_flag() == 601 ||	SetInitialConcentration (PointMap, sym_param);

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


double Simulation::ComputeEffectiveTemp(Point &p, double dx, double val, double hz, double Qz){
	cout<<endl<<"... Obliczam  temperature efektywna w punkcie..."<<endl;
	cout<<"x: "<<p.get_x()<<"  y: "<<p.get_y()<<";   hz: "<<hz<<"  Qz: "<<Qz<<endl;
	
	double tmp;
	tmp = pow(p.get_vx(),2) + pow(p.get_vy(),2);
	tmp = pow(tmp,0.5); //[predkosc wypadkowa w pkcie
	cout<<"Predkosc wypadkowa w pkcie: "<<tmp<<endl;
	tmp = hz * tmp * dx;// Qw -  natezenie przeplywu wody w miejscu zrodła
	cout<<"Qw natezenie przeplywu wody w miejscu zrodła: "<<tmp<<endl;
	val = (Qz*val)/(Qz + tmp);
	return val;
}

int Simulation::SetInitialConcentration (map<int, Point> &PointMap, SimulationParameters &param){
	
	ofstream out("Info.dat", ios::app);
	cout<<endl<<"... Wczytuje warunki poczatkowe ..."<<endl;  
	out<<endl<<"... Wczytuje warunki poczatkowe ..."<<endl;  

	string pollutant = "conc";// rodzaj zenieszcyszenia dra ktorego wykonywane beda obliczenia
				  //"conc" - zanieczyszczenie chemiczne - sybstancja pasywana 
				  //"temp" - zanieczyszczenie teramiczne 
	
	string dump = "inst";// sposób zrzutu zanieczyszczen - chwilowy - "inst",  lub ciągły - "cont"
	
	string area = "point"; 	//obszar zrzutu: 
				//"point" - w jednym pukcie, 
				//"point" - w jednym pukcie, 
				//"points" - w kilku zdefiniowanych punktach, 
				//"area" - na konkretnym obszare
				//"gauss" - rokład gausa wokół daneg punktu (diagonalny)
				//"gaussnondiag" - rozkład gaussa niediagonalny
	
	int krok = 100; //liczba kroków czasowych po ilu ma być wyznaczony warunek poczatkowy zadawany rozkładem gaussa

	string mod = "val"; //czy podawana wartosc temperatury/concentracji jest okreslona w danym punkcie jako wartosc zakladana czy efektywana
			    //"val" - wartosc nominalan koncentracji/temperatury jaka chcemy miec w danym punkcie 
				//- nie uwzglednia kroku przestrzennego na siatce i glebokosci w danym punkcie siatki 
				// na jej podstawie i na podstawie danych o głebokosci w danym pukcie i wartosci natęzenia przepływu zanieczyszcznie
				// wyznaczana jest wartosc efektywna koncentracji/temperatury
			    //"val_e" - wartosc efektywna koncentracji/temperatury w danym punkcie (powinna uwzgledniac juz  juz krok przestrzenny, glebokosc i natezenie zrzutu) 
	
	string file = "ini_cond.txt";
	
	fstream in(file.c_str());
	if(!in) {
		CheckFile(0,file);
		return 0;// jesli nie ma pliku wychodzi z funkci zwracajac 0
	}
	else CheckFile(1,file);		
	
	in>>pollutant>>dump>>area;	
	cout<<"Pollutant: "<<pollutant<<"   Dump: "<<dump<<"   Area: "<<area;
	if (area == "gaussnondiag" || area == "gauss") 
	{
		in>>krok;
		cout<<" after "<<krok<<" time steps = "<<krok*param.get_dt()<<" s"<<endl;
	}
	else cout<<endl;

	param.set_pollutant(pollutant);
	param.set_dump(dump);
	param.set_area(area);
	
	int x, y;
	double val1 = 0;
	double hz = -9999.99, Qz = -9999.99;
	int index;
	double tmp;
	while(!in.eof()) {
		in>>mod;
		param.set_mod(mod);

		in>>x>>y;
		
		index = x/param.get_dx()*10000 + y/param.get_dy();
		if(PointMap.find(index) == PointMap.end()){
			cout<<"Nie znany index = "<<index<<endl;
			return 0;	
		}
		Point pkt;
		pkt = PointMap[index];
		
		if (mod == "val") {
			in>>val1>>hz>>Qz;
			val1 = ComputeEffectiveTemp(pkt, param.get_dx(), val1, hz, Qz);			
		}
		else if (mod == "val_e"){
			in>>val1;
		}
		else {
			cout<<"Nieznana wartośc parametru 'mod' - sprawdz plik wejsciowy ini_cond.txt"<<endl;
			return 0;
		}

		cout<<"Pkt: ("<<x<<", "<<y<<")";
		if (hz != -9999.99) cout<<" głębokość: "<<hz<<"  natężenie zrzutu:"<<Qz;
		cout<<"  wartość efektywna: "<<val1<<endl;
		
		if (area == "point" || area == "points") SetInitialConcentrationPoint(PointMap, param, x, y, val1);
		if (area == "gauss") SetInitialConcentrationGauss(PointMap, param, x, y, val1, krok);
		if (area == "gaussnondiag") SetInitialConcentrationGaussNonDiag(PointMap, param, x, y, val1, krok);
		if (area == "points") continue; 
		else break; 
	}
	  
	in.close();
	out.close();
	return 1;
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
