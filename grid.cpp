/***************************************************************************
 *   grid.cpp
 *   Copyright (C) 2005 by Monika Kalinowska                               *
 *   mkalinow@igf.edu.pl                                                   *
 *                                                                         *
 ***************************************************************************/
 
#include "grid.h"

using namespace std;

//*********************************************************************************************************

const int Grid::empty_index = -99999999;
//int Grid::empty_index = Point::empty_index;

int Grid::_xmin = 0;
int Grid::_xmax = 0;
int Grid::_ymin = 0;
int Grid::_ymax = 0;

//_________________________________________________________________________________________________________
void Grid::SetMaxValues(double &vx, double &vy, 
		double &Dxx, double &Dxy, double Dyy, SimulationParameters &param){
	param.set_Dxxmax(Dxx);
	param.set_Dxymax(Dxy);
	param.set_Dyymax(Dyy);	
	param.set_vxmax(vx);
	param.set_vymax(vy);
}

//_________________________________________________________________________________________________________
void Grid::SetMinValues(double &vx, double &vy, 
		double &Dxx, double &Dxy, double Dyy, SimulationParameters &param){
	param.set_Dxxmin(Dxx);
	param.set_Dxymin(Dxy);
	param.set_Dyymin(Dyy);	
	param.set_vxmin(vx);
	param.set_vymin(vy);
}

//_________________________________________________________________________________________________________
void Grid::SetExtremeValues (double &xmax, double &xmin, double &ymax, double &ymin){
	_xmax = xmax;
	_xmin = xmin;
	_ymax = ymax;
	_ymin = ymin;
}

//_________________________________________________________________________________________________________
void Grid::SetExtremePoints (Point &pxmax, Point &pxmin, Point &pymax, Point &pymin){

	cout<<"PUNKTY GRANICZNE SIARTKI:"<<
	"		("<<pxmax.get_x()<<", "<<pxmax.get_y()<<")"<<endl<<
	"		("<<pxmin.get_x()<<", "<<pxmin.get_y()<<")"<<endl<<
	"		("<<pymax.get_x()<<", "<<pymax.get_y()<<")"<<endl<<
	"		("<<pymin.get_x()<<", "<<pymin.get_y()<<")"<<endl;
	this->_pxmax = pxmax;
	this->_pxmin = pxmin;
	this->_pymax = pymax;
	this->_pymin = pymin;
};

//_________________________________________________________________________________________________________
void Grid::CompleteOutsideFlow (map<int, Point> &PointMap){
	Point pkt;
	int index = 0; 
	int k=0, l=0;
	for (k = this->get_xmin() - 1; k <= this->get_xmax() + 1 ; k++){
		for (l = this->get_ymin() - 1; l <= this->get_ymax() + 1 ; l++){
			index = index = k*10000 + l;
			//cout<<"index: "<<index<<"	"<<PointMap[index]<<endl;
			pkt = PointMap[index];
			if (pkt.get_flag() == -9999) {
				Point p(k,l,0);
				PointMap[index] = p;
			}
			//cout<<"                "<<PointMap[index]<<endl<<endl;
		}
	}
};

//_________________________________________________________________________________________________________
int Grid::CorrectGrid (map<int, Point> &PointMap){
	cout<<endl<<"... Poprawiam siatke ... "<<endl;
	int i=-9999, j=-9999, index = -9999, corr = 0;	
	pair<int, int> index_flag; //para indexu punkto w mapie i falgi jaka powinna mu byc przypisana 
	map<int, int> i_f; //mapa par
	map<int, Point>::const_iterator CI; //iterator do mapy punktow
	Point p;
	
	for(CI=PointMap.begin();CI!=PointMap.end();CI++){// po wszystkich elementach mapy czyli pkt
		i = CI->second.get_x();
		j = CI->second.get_y();
		index = i*10000 + j;
	//	cout<<"TUTAJ; index:"<<index<<endl;
		p = PointMap[index]; //rozpatrywany pkt siatki
		//cout<<endl<<"--------------------"<<endl<<"pkt:"<<p<<endl;
		int nn1 = p.get_near_neighbour(1); // i,j+1
		int nn4 = p.get_near_neighbour(4); // i,j
		int nn3 = p.get_near_neighbour(3); // i-1,j
		int nn5 = p.get_near_neighbour(5); // i+1,j
		int nn7 = p.get_near_neighbour(7); // i,j-1
		
		//cout<<"indexy: "<<nn3<<" "<<nn5<<" "<<nn7<<" "<<nn1<<endl; 		
		
		nn1 = PointMap[nn1].get_flag();
		nn4 = PointMap[nn4].get_flag();
		nn3 = PointMap[nn3].get_flag();
		nn5 = PointMap[nn5].get_flag();
		nn7 = PointMap[nn7].get_flag();
				
		//cout<<"flagi: "<<nn3<<" "<<nn5<<" "<<nn7<<" "<<nn1<<endl;
		
		//punkty dodtakowe potrzebne na oznaczenie pkt przy kantach
		int nn0 = p.get_near_neighbour(0); // i-1,j+1
		int nn2 = p.get_near_neighbour(2); // i+1,j+1
		int nn6 = p.get_near_neighbour(6); // i-1,j-1
		int nn8 = p.get_near_neighbour(8); // i+1,j+1
		//cout<<"indexy: "<<nn0<<" "<<nn2<<" "<<nn6<<" "<<nn8<<endl; 		
		nn0 = PointMap[nn0].get_flag();
		nn2 = PointMap[nn2].get_flag();
		nn6 = PointMap[nn6].get_flag();
		nn8 = PointMap[nn8].get_flag();
		//cout<<"flagi: "<<nn0<<" "<<nn2<<" "<<nn6<<" "<<nn8<<endl;
		
		if (nn4==1) {
			//cout<<"oznaczono pkt "<<p.get_x()<<","<<p.get_y()<<endl;
			
			if (nn0!=0 & nn1!=0 & nn2!=0 & nn3!=0 & nn5!=0 & nn6!=0 & nn7!=0 & nn8!=0){
				//cout<<endl<<"--->-> wszytskie punkty dookola to przeplyw; oznaczam wiec pkt jako przeplyw"<<endl;
				continue;
			}
			
			else if (nn3==0 & nn5==0) {
			  p.set_flag(0); // brak pktow z lewej i prawek
			  cout<<"!!!!!!!! UWAGA - sprawdz !!!!!!!!! PKT:"<<p.get_x()<<","<<p.get_y()<<" oznaczony jako poza przeplywem "<<endl;
			  corr = 1;
			}
			else if (nn1==0 & nn7==0) {
			  p.set_flag(0); // brak pktow z gory i dolu
			  cout<<"!!!!!!!! UWAGA - sprawdz !!!!!!!!! PKT:"<<p.get_x()<<","<<p.get_y()<<" oznaczony jako poza przeplywem "<<endl;
			  corr = 1;  
			}
			else if (nn0==0 & nn8==0 & (nn1==0 || nn3==0 || nn5==0 || nn7==0)){
			  p.set_flag(0); // brak pktow kantow po przekontnej i jakiegos z boku lub z gory/dolu
			  cout<<"!!!!!!!! UWAGA - sprawdz !!!!!!!!! PKT:"<<p.get_x()<<","<<p.get_y()<<" oznaczony jako poza przeplywem "<<endl;
			  corr = 1;  
			}
			else if (nn2==0 & nn6==0 & (nn1==0 || nn3==0 || nn5==0 || nn7==0)){
			  p.set_flag(0); // brak pktow kantow po przekontnej i jakiegos z boku lub z gory/dolu
			  cout<<"!!!!!!!! UWAGA - sprawdz !!!!!!!!! PKT:"<<p.get_x()<<","<<p.get_y()<<" oznaczony jako poza przeplywem "<<endl;
			  corr = 1;  
			}
		}

		//cout<<"flaga pkt"<<p.get_flag()<<endl;
		index_flag=make_pair(index, p.get_flag());
		i_f.insert(index_flag);
	}
	  
	map<int, int>::iterator TI;
	for (TI=i_f.begin(); TI!=i_f.end(); TI++){
		//cout<<TI->first<<"	"<<TI->second<<endl;
		Point * pkt = &PointMap[(TI->first)];
		pkt->set_flag(TI->second);
		if (pkt->get_flag() == 101) cout<<"falaga 101 dlapkt: "<<pkt->get_x()<<","<<pkt->get_y()<<endl;
	}
	return corr;
}


//_________________________________________________________________________________________________________
void Grid::MarkBorderPoints (map<int, Point> &PointMap){
	cout<<endl<<"... Oznaczam punkty brzegowe ... "<<endl;
	int i=-9999, j=-9999, index = -9999, worn = 0;	
	pair<int, int> index_flag; //para indexu punkto w mapie i falgi jaka powinna mu byc przypisana 
	map<int, int> i_f; //mapa par
	map<int, Point>::const_iterator CI; //iterator do mapy punktow
	Point p;
	
	for(CI=PointMap.begin();CI!=PointMap.end();CI++){// po wszystkich elementach mapy czyli pkt
		i = CI->second.get_x();
		j = CI->second.get_y();
		index = i*10000 + j;
	//	cout<<"TUTAJ; index:"<<index<<endl;
		p = PointMap[index]; //rozpatrywany pkt siatki
		//cout<<endl<<"--------------------"<<endl<<"pkt:"<<p<<endl;
		int nn1 = p.get_near_neighbour(1); // i,j+1
		int nn4 = p.get_near_neighbour(4); // i,j
		int nn3 = p.get_near_neighbour(3); // i-1,j
		int nn5 = p.get_near_neighbour(5); // i+1,j
		int nn7 = p.get_near_neighbour(7); // i,j-1
		
		//cout<<"indexy: "<<nn3<<" "<<nn5<<" "<<nn7<<" "<<nn1<<endl; 		
		
		nn1 = PointMap[nn1].get_flag();
		nn4 = PointMap[nn4].get_flag();
		nn3 = PointMap[nn3].get_flag();
		nn5 = PointMap[nn5].get_flag();
		nn7 = PointMap[nn7].get_flag();
				
		//cout<<"flagi: "<<nn3<<" "<<nn5<<" "<<nn7<<" "<<nn1<<endl;
		
		//punkty dodtakowe potrzebne na oznaczenie pkt przy kantach
		int nn0 = p.get_near_neighbour(0); // i-1,j+1
		int nn2 = p.get_near_neighbour(2); // i+1,j+1
		int nn6 = p.get_near_neighbour(6); // i-1,j-1
		int nn8 = p.get_near_neighbour(8); // i+1,j+1
		//cout<<"indexy: "<<nn0<<" "<<nn2<<" "<<nn6<<" "<<nn8<<endl; 		
		nn0 = PointMap[nn0].get_flag();
		nn2 = PointMap[nn2].get_flag();
		nn6 = PointMap[nn6].get_flag();
		nn8 = PointMap[nn8].get_flag();
		//cout<<"flagi: "<<nn0<<" "<<nn2<<" "<<nn6<<" "<<nn8<<endl;
		
		if (nn4==1) {
			//cout<<"oznaczono pkt "<<p.get_x()<<","<<p.get_y()<<endl;
			
			if (nn0!=0 & nn1!=0 & nn2!=0 & nn3!=0 & nn5!=0 & nn6!=0 & nn7!=0 & nn8!=0){
				//cout<<endl<<"--->-> wszytskie punkty dookola to przeplyw; oznaczam wiec pkt jako przeplyw"<<endl;
				continue;
			}
			else if (nn0==0 & nn1==0 & nn2==0 & nn3!=0 & nn5!=0 & nn6!=0 & nn7!=0 & nn8!=0) p.set_flag(2); //bg - brzeg gorny 
			else if (nn0!=0 & nn1!=0 & nn2!=0 & nn3!=0 & nn5!=0 & nn6==0 & nn7==0 & nn8==0) p.set_flag(3); //bd - brzeg dolny
			else if (nn0==0 & nn1!=0 & nn2!=0 & nn3==0 & nn5!=0 & nn6==0 & nn7!=0 & nn8!=0) p.set_flag(4); //bl - brzeg lewy			
			else if (nn0!=0 & nn1!=0 & nn2==0 & nn3!=0 & nn5==0 & nn6!=0 & nn7!=0 & nn8==0) p.set_flag(5); //bp - brzeg prawy
			
			else if (nn0==0 & nn1==0 & nn2==0 & nn3==0 & nn5!=0 & nn6==0 & nn7!=0 & nn8!=0) p.set_flag(724); //rgl - rog gorny lewy 
			else if (nn0==0 & nn1==0 & nn2==0 & nn3!=0 & nn5==0 & nn6!=0 & nn7!=0 & nn8==0) p.set_flag(725); //rgp - rog gorny prawy
			else if (nn0==0 & nn1!=0 & nn2!=0 & nn3==0 & nn5!=0 & nn6==0 & nn7==0 & nn8==0) p.set_flag(734); //rdl - rog dolny lewy
			else if (nn0!=0 & nn1!=0 & nn2==0 & nn3!=0 & nn5==0 & nn6==0 & nn7==0 & nn8==0) p.set_flag(735); //rdp - rog dolny prawy
			
			//zakomentowane bo funkcja CorrectGrid dla takich punktow ustawia flage 0 - moglyby byc problematyczne
			//else if (nn0==0 & nn1==0 & nn2==0 & nn3==0 & nn5==0 & nn6!=0 & nn7!=0 & nn8!=0) p.set_flag(7245); //rglp 
			//else if (nn0!=0 & nn1!=0 & nn2!=0 & nn3==0 & nn5==0 & nn6==0 & nn7==0 & nn8==0) p.set_flag(7345); //rdlp
			//else if (nn0==0 & nn1==0 & nn2!=0 & nn3==0 & nn5!=0 & nn6==0 & nn7==0 & nn8!=0) p.set_flag(7423); //rlgd
			//else if (nn0!=0 & nn1==0 & nn2==0 & nn3!=0 & nn5==0 & nn6!=0 & nn7==0 & nn8==0) p.set_flag(7523); //rpgd
			
			else if (nn0!=0 & nn1==0 & nn2!=0 & nn3!=0 & nn5!=0 & nn6!=0 & nn7!=0 & nn8!=0) p.set_flag(62); //kg - kant gorny - brak tylko 1 punktu
			else if (nn0!=0 & nn1!=0 & nn2!=0 & nn3!=0 & nn5!=0 & nn6!=0 & nn7==0 & nn8!=0) p.set_flag(63); //kd
			else if (nn0!=0 & nn1!=0 & nn2!=0 & nn3==0 & nn5!=0 & nn6!=0 & nn7!=0 & nn8!=0) p.set_flag(64); //kl
			else if (nn0!=0 & nn1!=0 & nn2!=0 & nn3!=0 & nn5==0 & nn6!=0 & nn7!=0 & nn8!=0) p.set_flag(65); //kp
			
			else if (nn0==0 & nn1!=0 & nn2!=0 & nn3!=0 & nn5!=0 & nn6!=0 & nn7!=0 & nn8!=0) p.set_flag(624); //kgl
			else if (nn0!=0 & nn1!=0 & nn2==0 & nn3!=0 & nn5!=0 & nn6!=0 & nn7!=0 & nn8!=0) p.set_flag(625); //kgp
			else if (nn0!=0 & nn1!=0 & nn2!=0 & nn3!=0 & nn5!=0 & nn6==0 & nn7!=0 & nn8!=0) p.set_flag(634); //kdl
			else if (nn0!=0 & nn1!=0 & nn2!=0 & nn3!=0 & nn5!=0 & nn6!=0 & nn7!=0 & nn8==0) p.set_flag(635); //kdp
			
			else if (nn0==0 & nn1==0 & nn2!=0 & nn3!=0 & nn5!=0 & nn6!=0 & nn7!=0 & nn8!=0) p.set_flag(24); //bgl
			else if (nn0!=0 & nn1==0 & nn2==0 & nn3!=0 & nn5!=0 & nn6!=0 & nn7!=0 & nn8!=0) p.set_flag(25); //bgp
			else if (nn0!=0 & nn1!=0 & nn2!=0 & nn3!=0 & nn5!=0 & nn6==0 & nn7==0 & nn8!=0) p.set_flag(34); //bdl
			else if (nn0!=0 & nn1!=0 & nn2!=0 & nn3!=0 & nn5!=0 & nn6!=0 & nn7==0 & nn8==0) p.set_flag(35); //bdp
			else if (nn0==0 & nn1!=0 & nn2!=0 & nn3==0 & nn5!=0 & nn6!=0 & nn7!=0 & nn8!=0) p.set_flag(42); //blg
			else if (nn0!=0 & nn1!=0 & nn2!=0 & nn3==0 & nn5!=0 & nn6==0 & nn7!=0 & nn8!=0) p.set_flag(43); //bld
			else if (nn0!=0 & nn1!=0 & nn2==0 & nn3!=0 & nn5==0 & nn6!=0 & nn7!=0 & nn8!=0) p.set_flag(52); //bpg
			else if (nn0!=0 & nn1!=0 & nn2!=0 & nn3!=0 & nn5==0 & nn6!=0 & nn7!=0 & nn8==0) p.set_flag(53); //bpd
			
			else if (nn0==0 & nn1==0 & nn2!=0 & nn3==0 & nn5!=0 & nn6!=0 & nn7!=0 & nn8!=0) p.set_flag(7624); //rkgl
			else if (nn0!=0 & nn1==0 & nn2==0 & nn3!=0 & nn5==0 & nn6!=0 & nn7!=0 & nn8!=0) p.set_flag(7625); //rkgp
			else if (nn0!=0 & nn1!=0 & nn2!=0 & nn3==0 & nn5!=0 & nn6==0 & nn7==0 & nn8!=0) p.set_flag(7634); //rkdl
			else if (nn0!=0 & nn1!=0 & nn2!=0 & nn3!=0 & nn5==0 & nn6!=0 & nn7==0 & nn8==0) p.set_flag(7635); //rkdp
			
			else if (nn0==0 & nn1==0 & nn2==0 & nn3==0 & nn5!=0 & nn6!=0 & nn7!=0 & nn8!=0) p.set_flag(274); //bgrl
			else if (nn0==0 & nn1==0 & nn2==0 & nn3!=0 & nn5==0 & nn6!=0 & nn7!=0 & nn8!=0) p.set_flag(275); //bgrp
			else if (nn0!=0 & nn1!=0 & nn2!=0 & nn3==0 & nn5!=0 & nn6==0 & nn7==0 & nn8==0) p.set_flag(374); //bdrl
			else if (nn0!=0 & nn1!=0 & nn2!=0 & nn3!=0 & nn5==0 & nn6==0 & nn7==0 & nn8==0) p.set_flag(375); //bdrp
			else if (nn0==0 & nn1==0 & nn2!=0 & nn3==0 & nn5!=0 & nn6==0 & nn7!=0 & nn8!=0) p.set_flag(472); //blrg
			else if (nn0!=0 & nn1==0 & nn2==0 & nn3!=0 & nn5==0 & nn6!=0 & nn7!=0 & nn8==0) p.set_flag(572); //bprg
			else if (nn0==0 & nn1!=0 & nn2!=0 & nn3==0 & nn5!=0 & nn6==0 & nn7==0 & nn8!=0) p.set_flag(473); //blrd
			else if (nn0!=0 & nn1!=0 & nn2==0 & nn3!=0 & nn5==0 & nn6!=0 & nn7==0 & nn8==0) p.set_flag(573); //bprd
			
			else if (nn0!=0 & nn1==0 & nn2!=0 & nn3!=0 & nn5!=0 & nn6!=0 & nn7!=0 & nn8==0){
			  p.set_flag(6235); //kg + kdp
			  worn = 1;
			}
			else if (nn0!=0 & nn1==0 & nn2!=0 & nn3!=0 & nn5!=0 & nn6==0 & nn7!=0 & nn8!=0){
			  p.set_flag(6234); //kg + kdl
			  worn = 1;
			}
			else if (nn0!=0 & nn1!=0 & nn2==0 & nn3==0 & nn5!=0 & nn6!=0 & nn7!=0 & nn8!=0){
			  p.set_flag(6425); //kl + kgp
			  worn = 1;
			}
			else if (nn0!=0 & nn1!=0 & nn2!=0 & nn3==0 & nn5!=0 & nn6!=0 & nn7!=0 & nn8==0){
			  p.set_flag(6435); //kl + kdp
			  worn = 1;
			}
			else if (nn0==0 & nn1!=0 & nn2!=0 & nn3!=0 & nn5!=0 & nn6!=0 & nn7==0 & nn8!=0){
			  p.set_flag(6324); //kd + kgl
			  worn = 1;
			}
			else if (nn0!=0 & nn1!=0 & nn2==0 & nn3!=0 & nn5!=0 & nn6!=0 & nn7==0 & nn8!=0){
			  p.set_flag(6325); //kd + kgp
			  worn = 1;
			}
			else if (nn0!=0 & nn1!=0 & nn2!=0 & nn3!=0 & nn5==0 & nn6==0 & nn7!=0 & nn8!=0){
			  p.set_flag(6534); //kp + kdl
			  worn = 1;
			}
			else if (nn0==0 & nn1!=0 & nn2!=0 & nn3!=0 & nn5==0 & nn6!=0 & nn7!=0 & nn8!=0){
			  p.set_flag(6524); //kp + kgl
			  worn = 1;
			}
			else if (nn0==0 & nn1!=0 & nn2!=0 & nn3!=0 & nn5!=0 & nn6!=0 & nn7!=0 & nn8==0){
			  p.set_flag(62435); //kgl + kdp
			  worn = 1;
			}
			else if (nn0!=0 & nn1!=0 & nn2==0 & nn3!=0 & nn5!=0 & nn6==0 & nn7!=0 & nn8!=0){
			  p.set_flag(62534); //kgp + kdl
			  worn = 1;
			}

			
			else cout<<"!!!!!!!! UWAGA !!!!!!!!! PKT:"<<p.get_x()<<","<<p.get_y()<<"- dziwne pojedyncze pkt na siatce - sprawdz siatke, usun pojedyncze punkty lub zmniejsz krok przestrzenny"<<endl;
 			
			if (worn==1) cout<<"!!!!!!!! UWAGA - sprawdz czy nie poprawic siatki !!!!!!!!! PKT:"<<p.get_x()<<","<<p.get_y()<<" oznaczony jako:"<<p.get_flag()<<endl;
			worn = 0;
			//else if (nn0==1 & nn1==1 & nn2==1 & nn3==1 & nn5==1 & nn6==1 & nn7==1 & nn8==1) p.set_flag();
/* tak bylo 			
			else if (nn3==1 & nn5==1 & nn7==1 & nn1==0) p.set_flag(2); //bg
			else if (nn3==1 & nn5==1 & nn7==0 & nn1==1) p.set_flag(3); //bd 
			else if (nn3==0 & nn5==1 & nn7==1 & nn1==1) p.set_flag(4); //bl
			else if (nn3==1 & nn5==0 & nn7==1 & nn1==1) p.set_flag(5); //bp
			else if (nn3==0 & nn5==1 & nn7==1 & nn1==0) p.set_flag(24); //rgl
			else if (nn3==1 & nn5==0 & nn7==1 & nn1==0) p.set_flag(25); //rgp
			else if (nn3==0 & nn5==1 & nn7==0 & nn1==1) p.set_flag(34); //rdl
			else if (nn3==1 & nn5==0 & nn7==0 & nn1==1) p.set_flag(35); //rdp
			else if (nn3==0 & nn5==1 & nn7==0 & nn1==0) p.set_flag(243); //rgld
			else if (nn3==0 & nn5==0 & nn7==1 & nn1==0) p.set_flag(425); //rlgp
			else if (nn3==0 & nn5==0 & nn7==0 & nn1==1) p.set_flag(534); //rpdl
			else if (nn3==1 & nn5==0 & nn7==0 & nn1==0) p.set_flag(253); //rgpl
*/			
			//24.10.06 --- tu dopisac pozostale opcje aby dobrze ustawic wszytskie flagi
			
			//dodatkowe pukty dla na kantach 
			//cout<<endl<<"--->-> oznaczylem pkt jako brzeg"<<endl;
		}
		/*else if (nn4 == 0) {
			if (nn0!=0 & nn1!=0 & nn2!=0 & nn3!=0 & nn5!=0 & nn6!=0 & nn7!=0 & nn8!=0){
				p.set_flag(101); //zdrodlo
				cout<<"oznaczono pkt "<<p.get_x()<<","<<p.get_y()<<
				" WYSPA?, Zrodlo?: "<<p.get_flag()<<endl;
			}
		}*/
		//cout<<"flaga pkt"<<p.get_flag()<<endl;
		index_flag=make_pair(index, p.get_flag());
		i_f.insert(index_flag);
	}
	  
	map<int, int>::iterator TI;
	for (TI=i_f.begin(); TI!=i_f.end(); TI++){
		//cout<<TI->first<<"	"<<TI->second<<endl;
		Point * pkt = &PointMap[(TI->first)];
		pkt->set_flag(TI->second);
		if (pkt->get_flag() == 101) cout<<"falaga 101 dlapkt: "<<pkt->get_x()<<","<<pkt->get_y()<<endl;
	}
}

//________________________________________________________________________________________________________
int Grid::MarkInOutPoints (map<int, Point> &PointMap){
	cout<<"Oznaczam Wejscia i Wyjscia"<<endl;
	string type = "undefined";
	string file = "InOut2.txt";
	int x1 = -9999, x2 = -9999, y1 = -9999, y2 = -9999;
	int index = Grid::empty_index;
	//sprawdzamy plik
	fstream in(file.c_str());
	if(!in) {
    	CheckFile(0,file);
		return 0;
    }
	else CheckFile(1,file);		
	
	int nr_in = 600; // kolejny numer wejscia
	int nr_out = 800; // kolejny numer wyjscia
	while(!in.eof()){

		in>>type>>x1>>x2>>y1>>y2;

		if (type == "in") nr_in ++;
		else if (type == "out") nr_out ++;
		//!!! moze pozniej bedzie potrzbne !! 
		cout<<"typ: "<<type<<"	x1: "<<x1<<", x2: "<<x2<<"	y1: "<<y1<<", y2: "<<y2<<endl;

		if (nr_in  >= 700) {
			cout<<"Za duzo wejsc!!! Wiecej nie moge oznaczyc"<<endl;
			return 0;
		}

		if (nr_out >= 900) {
			cout<<"Za duzo wyjsc!!! Wiecej nie moge oznaczyc"<<endl;
			return 0;
		}

		if (x1 == x2 || y1 == y2){
			for (int i = x1; i <= x2; i++ ){
				for (int j = y1; j <= y2; j++){
					index = 10000 * i + j;	
					if (type == "in") { 
						PointMap.find(index)->second.set_flag(nr_in);
						cout<<"zadalem wejscir x,y: "<<i<<","<<j<<" nr:"<<nr_in<<endl;
					}
					else if (type == "out"){ 
						PointMap.find(index)->second.set_flag(nr_out);
						cout<<"zadalem wyjscie x,y: "<<i<<","<<j<<" nr:"<<nr_out<<endl;
					}
				}
			}
		}
		
		else {
			double a = (y2 - y1)/(x2 - x1);
			double b = y1 - (a * x1);
			int x = -9999, y = -9999;
			map<int,Point>::iterator IT;
			for(IT = PointMap.begin(); IT != PointMap.end(); IT++){
				x = IT->second.get_x();
				y = IT->second.get_y();
				if ( y == a*x + b && IT->second.get_flag() != 0){
					if (type == "in") {
						IT->second.set_flag(nr_in);
						cout<<"zadalem wejscir x,y: "<<x<<","<<y<<" nr:"<<nr_in<<endl;
					}
					else if (type == "out"){ 
						IT->second.set_flag(nr_out);
						cout<<"zadalem wyjscie x,y: "<<x<<","<<y<<" nr:"<<nr_out<<endl;
					}
				}
			}
		}
		
		/*
		for (int i = x1; i <= x2; i++ ){
			for (int j = y1; j <= y2; j++){
				index = 10000 * i + j;
				//cout<<"i: "<<i<<" j: "<<j<<" index: "<<index<<
					//" x: "<<PointMap.find(index)->second.get_x()<<
					//" y: "<<PointMap.find(index)->second.get_y()<<endl;
					
				if (type == "in"){
					if (nr_in < 700 )PointMap.find(index)->second.set_flag(nr_in);
					else {
						cout<<"Za duzo wejsc!!! Wiecej nie moge oznaczyc"<<endl;
						return 0;
					}
				}
				else if (type == "out"){
					if (nr_out < 900 )PointMap.find(index)->second.set_flag(nr_out);
					else {
						cout<<"Za duzo wyjsc!!! Wiecej nie moge oznaczyc"<<endl;
						return 0;
					}			
				}
			}
		}
		*/
	}
	return 1;
}


//_________________________________________________________________________________________________________
void Grid::MarkNearBorderPoints (map<int, Point> &PointMap){
	
	//cout<<"w MarkNearBorderPoints"<<endl;
	int i=-9999, j=-9999, index = -9999;
	pair<int, int> index_flag; //para indexu punkto w mapie i falgi jaka powinna mu byc przypisana 
	map<int, int> i_f; //mapa par
	map<int, Point>::const_iterator CIi; //iterator do mapy punktow
	Point p;
	for(CIi=PointMap.begin();CIi!=PointMap.end();CIi++){// po wszystkich elementach mapy czyli pkt
		i = CIi->second.get_x();
		j = CIi->second.get_y();
		index = i*10000 + j;

		p = PointMap[index]; //rozpatrywany pkt siatki
	
		int nn1 = p.get_near_neighbour(1); // i,j+1
		int nn4 = p.get_near_neighbour(4); // i,j
		int nn3 = p.get_near_neighbour(3); // i-1,j
		int nn5 = p.get_near_neighbour(5); // i+1,j
		int nn7 = p.get_near_neighbour(7); // i,j-1
 		
		//cout<<"indexy: "<<nn3<<" "<<nn5<<" "<<nn7<<" "<<nn1<<endl; 		
		
		nn1 = PointMap[nn1].get_flag();
		nn4 = PointMap[nn4].get_flag();
		nn3 = PointMap[nn3].get_flag();
		nn5 = PointMap[nn5].get_flag();
		nn7 = PointMap[nn7].get_flag();
				
		//cout<<"flagi: "<<nn3<<" "<<nn5<<" "<<nn7<<" "<<nn1<<endl;
		
		if (nn4 == 1) {
			if (nn3==1 & nn5==1 & nn7==1 & nn1==2) p.set_flag(22); //pbg
			else if (nn3==1 & nn5==1 & nn7==3 & nn1==1) p.set_flag(33); //pbd 
			else if (nn3==4 & nn5==1 & nn7==1 & nn1==1) p.set_flag(44); //pbl
			else if (nn3==1 & nn5==5 & nn7==1 & nn1==1) p.set_flag(55); //pbp
		
			else if (nn3==4 & nn5==1 & nn7==1 & nn1==2) p.set_flag(2244); //prgl
			else if (nn3==1 & nn5==5 & nn7==1 & nn1==2) p.set_flag(2255); //prgp
			else if (nn3==4 & nn5==1 & nn7==3 & nn1==1) p.set_flag(3344); //prdl
			else if (nn3==1 & nn5==5 & nn7==3 & nn1==1) p.set_flag(3355); //prdp
		}
		//cout<<"flaga pkt"<<p.get_flag()<<endl;
		index_flag=make_pair(index, p.get_flag());
		i_f.insert(index_flag);
	}  
	map<int, int>::iterator TI;
	for (TI=i_f.begin(); TI!=i_f.end(); TI++){
		//cout<<TI->first<<"	"<<TI->second<<endl;
		Point * pkt = &PointMap[(TI->first)];
		pkt->set_flag(TI->second);
	}

}

//*********************************************************************************************************
int GridOnly::ReadData(map<int, Point> &PointMap, const string & file, SimulationParameters & sym_param) {

	//INFO
	ofstream out("Info.dat", ios::app);
	out<<" GridOnly->Tylko pkt siatki "<<endl;
	cout<<" GridOnly->Tylko pkt siatki "<<endl;

	int x = -9999, y = -9999, index = -9999;
	double  xmax = 0.0, ymax = 0.0, xmin = 0.0, ymin = 0.0; 
	double vx = sym_param.get_vx(), vy = sym_param.get_vy();	//vx, vy stale pobierana z obiektu z paremetrami symulacji
	double Dxxmax = 0.0, Dyymax = 0.0, Dxymax = 0.0; //potrzebne do wyznaczenia maksymalnych wrtosci wspolczynnikow
	double Dlong = sym_param.get_Dlong(), Dtrans = sym_param.get_Dtrans(); //potrzebne do wyznaczenia wspolcz tensora dysp
	DTensor * disp_tensor; //wskaznik pod ktory bedzie zapisany obiekt odpowiadajacy wlasciwemu obl tensora dyspers.
	
	disp_tensor = sym_param.get_tensor(); //wsadzamy do wskaznika odpowienini obiekt ktory jest zapisany w parametrach sym
	 
	fstream in(file.c_str());
	if(!in) {
	CheckFile(0,file);
		return 0;
	}
	else CheckFile(1,file);		
	
	int tmp = 0; // potrzebne do ustwaienia poierwszysch wartosci pkt brzegowych
	while(!in.eof()){
		in>>x>>y; //wspolrzedne pkt sczytywane z pliku
		Point pkt(x, y, vx, vy, 1); 
		
		/////obliczanie pkt brzegowych siatki 
		if (tmp != 0) {
			if (x > xmax) xmax = x;
			if (x < xmin) xmin = x;
			if (y > ymax) ymax = y;
			if (y < ymin) ymin = y;
		}
		else {
			tmp = 1;
			xmax = x; xmin = x; ymax = y; ymin = y;		
		} ///////////////////////////////////
	
		//// !!!!  pozniej prawdopodob bedzie musialo byc zmienione po dopisaniu innych sposobow obl
		disp_tensor ->ComputeTensor(Dlong, Dtrans, pkt); //obliczanie wspolczynikow ten dysper. dla kazdego pkt;

		//obl max wspolczynnikow tensora dyspers na siatce
		if (pkt.get_Dxx() > Dxxmax) Dxxmax = pkt.get_Dxx();
		if (pkt.get_Dxy() > Dxymax) Dxymax = pkt.get_Dxy();
		if (pkt.get_Dyy() > Dyymax) Dyymax = pkt.get_Dyy();
		//////////////////////////////////////////////////////////////////////
		
		index = x*10000 + y;
		//cout<<"x: "<<x<<"  y: "<<y<<"	index: "<<index<<endl;
		//cout<<"wspol dysp Dxx: "<<pkt.get_Dxx()<<"	Dxy: "<<pkt.get_Dxy()<<"	Dyy: "<<pkt.get_Dyy()<<endl;
		
		PointMap[index] = pkt;	
	}
	in.close();

	out<<endl<<"xmax:	"<<xmax<<"	xmin:	"<<xmin<<"	ymax:	"<<ymax<<"	ymin:	"<<ymin<<endl;
	out<<"Dxxmax: "<<Dxxmax<<"	Dxymax: "<<Dxymax<<"	Dyymax:	"<<Dyymax<<endl;
	out<<"vxmax: "<<vx<<"	vymax:	"<<vy<<endl;
	SetMaxValues(vx, vy, Dxxmax, Dxymax, Dyymax, sym_param);
	SetMinValues(vx, vy, Dxxmax, Dxymax, Dyymax, sym_param);
	SetExtremeValues(xmax, xmin, ymax, ymin);
	
	out.close();
	return 1;
}
//_______________________________________________________________________________________________________


//*********************************************************************************************************
int GridVelocity::ReadData(map<int, Point> &PointMap, const string & file, SimulationParameters & sym_param) {
	//INFO
	ofstream out("Info.dat", ios::app);
	cout<<"GridVelocity->pkt siatki z wartosciami predkosci w kazdym pkt"<<endl;
	out<<"GridVelocity->pkt siatki z wartosciami predkosci w kazdym pkt"<<endl;
	int x = -9999, y = -9999, index = -9999;
	double  xmax = 0.0, ymax = 0.0, xmin = 0.0, ymin = 0.0; 
	double vx = -9999.0, vy = -9999.0;
	double vxmax = 0.0, vymax = 0.0;//potrzebne do wyznaczenia max eatosci predkosci na siatce
	double Dlong = sym_param.get_Dlong(), Dtrans = sym_param.get_Dtrans(); //potrzebne do wyznaczenia wspolcz tensora dysp
	double Dxxmax = Dlong, Dyymax = Dtrans, Dxymax = 0.0; //potrzebne do wyznaczenia maksymalnych wrtosci wspolczynnikow

	DTensor * disp_tensor; //wskaznik pod ktory bedzie zapisany obiekt odpowiadajacy wlasciwemu obl tensora dyspers.
	
	disp_tensor = sym_param.get_tensor(); 
	
	fstream in(file.c_str());
	if(!in) {
    	CheckFile(0,file);
		return 0;
    }
	else CheckFile(1,file);		
	
	int tmp = 0; // potrzebne do ustwaienia poierwszysch wartosci pkt brzegowych
	while(!in.eof()){
    	in>>x>>y>>vx>>vy;
		Point pkt(x,y,vx,vy,1);
		// !!!!  pozniej prawdopodob bedzie musialo byc zmienione po dopisaniu innych sposobow obl
		disp_tensor ->ComputeTensor(Dlong, Dtrans, pkt); //obliczanie wspolczynikow ten dysper. dla kazdego pkt;
		
		if (tmp != 0) {
			if (x > xmax) xmax = x;
			if (x < xmin) xmin = x;
			if (y > ymax) ymax = y;
			if (y < ymin) ymin = y;
			
			///obl max weartosci predkosci
			if (vx > vxmax) vxmax = vx;
			if (vy > vymax) vymax = vy;
			
			//obl max wspolczynnikow tensora dyspers na siatce
			if (pkt.get_Dxx() > Dxxmax) Dxxmax = pkt.get_Dxx();
			if (pkt.get_Dxy() > Dxymax) Dxymax = pkt.get_Dxy();
			if (pkt.get_Dyy() > Dyymax) Dyymax = pkt.get_Dyy();
		}
		else {
			tmp = 1;
			xmax = x; xmin = x; ymax = y; ymin = y;	
			vxmax = vx; vymax = vy;
			Dxxmax = pkt.get_Dxx(); Dxymax = pkt.get_Dxy(); Dyymax = pkt.get_Dyy();
		} 		
		index = x*10000 + y;
		//cout<<"x: "<<x<<"  y: "<<y<<"	index: "<<index<<endl;
		//cout<<"wspol dysp Dxx: "<<pkt.get_Dxx()<<"	Dxy: "<<pkt.get_Dxy()<<"	Dyy: "<<pkt.get_Dyy()<<endl;
		PointMap[index] = pkt;	//wpisujemy pkt do mapy  
	}
	in.close();
	
	cout<<endl<<"xmax:	"<<xmax<<"	xmin:	"<<xmin<<"	ymax:	"<<ymax<<"	ymin:	"<<ymin<<endl;
	cout<<"Dxxmax: "<<Dxxmax<<"	Dxymax: "<<Dxymax<<"	Dyymax:	"<<Dyymax<<endl;
	cout<<"vxmax: "<<vxmax<<"	vymax:	"<<vymax<<endl;
	SetMaxValues(vx, vy, Dxxmax, Dxymax, Dyymax, sym_param);
	SetExtremeValues(xmax, xmin, ymax, ymin);
	out.close();
	return 1;
}


//*********************************************************************************************************
int GridAll::ReadData(map<int, Point> &PointMap, const string & file, SimulationParameters & sym_param) {
	//INFO
	ofstream out("Info.dat", ios::app);
	out<<"GridAll->Pkt siatki z wartosciami predkosci i wspolczynikow tensora dyspersji"<<endl;
	cout<<"GridAll->Pkt siatki z wartosciami predkosci i wspolczynikow tensora dyspersji"<<endl;
	int x = -9999, y = -9999, index = -9999;
	double vx = -9999.0, vy = -9999.0, Dxx = -9999.0, Dyy = -9999.0, Dxy = -9999.0;
	double  xmax = 0.0, ymax = 0.0, xmin = 0.0, ymin = 0.0; 
	double vxmax = 0.0, vymax = 0.0;//potrzebne do wyznaczenia max eatosci predkosci na siatce
	double Dxxmax = 0.0, Dyymax = 0.0, Dxymax = 0.0; //potrzebne do wyznaczenia maksymalnych wrtosci wspolczynnikow	
	
	fstream in(file.c_str());
	if(!in) {
    	CheckFile(0,file);
		return 0;
    }
	else CheckFile(1,file);		
	cout<<"+++++++++++++++++++++++++ CZYTAM PLIK ++++++++++++++++++++++++"<<endl;
	int tmp = 0;
	while(!in.eof()){
    	in>>x>>y>>vx>>vy>>Dxx>>Dyy>>Dxy;
	//cout<<"x: "<<x<<"  y: "<<y<<"  vx: "<<vx<<"  vy: "<<vy<<"  Dxx: "<<Dxx<<"  Dyy: "<<Dyy<<"  Dxy: "<<Dxy<<endl;
    	Point pkt(x, y, vx, vy, Dxx, Dyy, Dxy, 1);
		if (tmp != 0) {
			if (x > xmax) xmax = x;
			if (x < xmin) xmin = x;
			if (y > ymax) ymax = y;
			if (y < ymin) ymin = y;
			
			///obl max weartosci predkosci
			if (vx > vxmax) vxmax = vx;
			if (vy > vymax) vymax = vy;
			
			//obl max wspolczynnikow tensora dyspers na siatce
			if (Dxx > Dxxmax) Dxxmax = Dxx;
			if (Dxy > Dxymax) Dxymax = Dxy;
			if (Dyy > Dyymax) Dyymax = Dyy;
			
			if (fabs(Dxx) <= 1e-2) cout<<"Dxx="<<Dxx<<endl;
			//if (fabs(Dxy) <= 1e-2) cout<<"Dxy="<<Dxy<<endl;
			if (fabs(Dyy) <= 1e-2) cout<<"Dxx="<<Dyy<<endl;
		}
		else {
			tmp = 1;
			xmax = x; xmin = x; ymax = y; ymin = y;	
			vxmax = vx; vymax = vy;
			Dxxmax = Dxx; Dxymax = Dxy; Dyymax = Dyy;
		}
	  	index = x*10000 + y;
		//cout<<"x: "<<x<<"  y: "<<y<<"	index: "<<index<<endl;
		//cout<<"wspol dysp Dxx: "<<pkt.get_Dxx()<<"	Dxy: "<<pkt.get_Dxy()<<"	Dyy: "<<pkt.get_Dyy()<<endl;
		PointMap[index] = pkt;	
	}
	in.close();
	
	cout<<endl<<"xmax:	"<<xmax<<"	xmin:	"<<xmin<<"	ymax:	"<<ymax<<"	ymin:	"<<ymin<<endl;
	cout<<"Dxxmax: "<<Dxxmax<<"	Dxymax: "<<Dxymax<<"	Dyymax:	"<<Dyymax<<endl;
	cout<<"vxmax: "<<vxmax<<"	vymax:	"<<vymax<<endl;
	SetMaxValues(vxmax, vymax, Dxxmax, Dxymax, Dyymax, sym_param);
	SetExtremeValues(xmax, xmin, ymax, ymin);
	out.close();
	return 1;
}


//*********************************************************************************************************
int GridDispersion::ReadData(map<int, Point> &PointMap, const string & file, SimulationParameters & sym_param) {
	//INFO
	ofstream out("Info.dat", ios::app);
	cout<<"GridDispersion->Pkt siatki z wartosciami wspolczynikow tensora dyspersji"<<endl;
	out<<"GridDispersion->Pkt siatki z wartosciami wspolczynikow tensora dyspersji"<<endl;
	int x = -9999, y = -9999, index = -9999;
	double Dxx = -9999.0, Dyy = -9999.0, Dxy = -9999.0;
	double vx = sym_param.get_vx(), vy = sym_param.get_vy();	//vx, vy stale pobierana z obiektu z paremetrami symulacji	
	double  xmax = 0.0, ymax = 0.0, xmin = 0.0, ymin = 0.0; 
	double Dxxmax = 0.0, Dyymax = 0.0, Dxymax = 0.0; //potrzebne do wyznaczenia maksymalnych wrtosci wspolczynnikow

	fstream in(file.c_str());
	if(!in) {
    	CheckFile(0,file);
		return 0;
    }
	else CheckFile(1,file);		
	
	int tmp = 0;	
	while(!in.eof()){
    	in>>x>>y>>Dxx>>Dyy>>Dxy;
    	Point pkt(x, y, Dxx, Dyy, Dxy, 1);
		
		if (tmp != 0) {
			if (x > xmax) xmax = x;
			if (x < xmin) xmin = x;
			if (y > ymax) ymax = y;
			if (y < ymin) ymin = y;
			
			//obl max wspolczynnikow tensora dyspers na siatce
			if (Dxx > Dxxmax) Dxxmax = Dxx;
			if (Dxy > Dxymax) Dxymax = Dxy;
			if (Dyy > Dyymax) Dyymax = Dyy;
		}
		else {
			tmp = 1;
			xmax = x; xmin = x; ymax = y; ymin = y;	
			Dxxmax = Dxx; Dxymax = Dxy; Dyymax = Dyy;
		} 		
	
	  
	  index = x*10000 + y;
	  //cout<<"x: "<<x<<"  y: "<<y<<"	index: "<<index<<endl;
		
		PointMap[index] = pkt;	
  	}
	in.close();
	
	cout<<endl<<"xmax:	"<<xmax<<"	xmin:	"<<xmin<<"	ymax:	"<<ymax<<"	ymin:	"<<ymin<<endl;
	cout<<"Dxxmax: "<<Dxxmax<<"	Dxymax: "<<Dxymax<<"	Dyymax:	"<<Dyymax<<endl;
	cout<<"vxmax: "<<vx<<"	vymax:	"<<vy<<endl;
	SetMaxValues(vx, vy, Dxxmax, Dxymax, Dyymax, sym_param);
	SetExtremeValues(xmax, xmin, ymax, ymin);
	out.close();
	return 1;
}
//*********************************************************************************************************
