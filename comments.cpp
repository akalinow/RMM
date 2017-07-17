
/***************************************************************************
 *   comments.cpp
 *   Copyright (C) 2005 by Monika Kalinowska                               *
 *   mkalinow@igf.edu.pl                                                   *
 *                                                                         *
 ***************************************************************************/

#include "comments.h"
#include <iostream>
#include <fstream>
  
char * GetTime(){	
	time_t rawtime;
	time ( &rawtime );
	// cout<<time ( &rawtime );
	return ctime(&rawtime);
}
//__________________________________________________________________________


void Helloo(const string &opt){
	if (opt == "screan"){
		cout<<endl<<"////////////////////////////////////////////////////////////////////////";
		cout<<endl<<"////            RIVER MIXING MODEL -- RivMix                        ////"<<endl;
		cout<<"////         The twodimensional numerical model                     ////"<<endl
			  <<"////  of the spread of passive pollutants in flowing surface water  ////"<<endl;
		cout<<"////           2005-2015 by Monika Kalinowska                       ////"<<endl;
		cout<<"////            Institute of Geophysics PAN                         ////"<<endl;
		cout<<"////             email: mkalinow@igf.edu.pl                         ////"; 
		cout<<endl<<"////////////////////////////////////////////////////////////////////////"<<endl<<endl<<endl;
		cout<<"Czas rozpoczecia: ";
		cout<<GetTime()<<endl;
	}
	else if (opt == "file"){
		ofstream out("Info.dat", ios::trunc);
		out<<endl<<"     RIVER MIXING MODEL -- RivMix    "<<endl;
  		out<<"2005-2015 by Monika Kalinowska"<<endl;
  		out<<"email: mkalinow@igf.edu.pl"<<endl<<endl; 
		out<<"Czas rozpoczecia: ";
		out<<GetTime()<<endl;
		out.close();
	}
}
//____________________________________________________________________________


void End(const string &opt){
	if (opt == "screan"){
		cout<<endl<<"Koniec: ";
		cout<<GetTime()<<endl;
		cout<<"____________________________________________________________________________________________"<<endl;
	}
	else if (opt == "file"){
		ofstream out("Info.dat", ios::app);
		out<<endl<<"Koniec: ";
		out<<GetTime()<<endl;
		out.close();
	}

}

//_____________________________________________________________


void FileWithParameters(const string &file, const string &opt){
	string komunikat="Parametry symulacji zostana popbrane z pliku " + file + "";
	if (opt == "screan"){
		cout<<komunikat<<endl;
	}
	else if (opt == "file"){
		ofstream out("Info.dat", ios::app);
		out<<komunikat<<endl;
		out.close();
	}


}
//__________________________________________________________________________________


void CheckFile(int opt, const string &file){
	if(opt==0) cout<<"Nie znaleziono pliku: "<<file<<endl;
	else cout<<endl<<" -> Czytam z pliku: "<<file<<endl;
}
