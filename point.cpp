
/***************************************************************************
 *   point.cpp
 *   Copyright (C) 2005 by Monika Kalinowska                               *
 *   mkalinow@igf.edu.pl                                                   *
 *                                                                         *
 ***************************************************************************/

 #include "point.h"
 #include "grid.h"
 
void Point::print() const {
	cout<<"Flaga: "<<_flag<<"	x= "<<_x<<" y= "<<_y<<" type= "<<_type
		<<" valPrev = "<<_valPrev<<" valCurr = "<<_valCurr
		<<" vx = "<<_vx<<" vy = "<<_vy<<endl;
};

 map<int, Point>  * Point::_PointMap = 0;
 
 //int Point::empty_index = -89589599;

//____________________________________________________________________________________________
void Point::set_near_neighbours(int opt){

	int i = this->get_x(), j = this->get_y();
   
	int index = Grid::empty_index;
	if (opt == 0) {
		_near_neighbours.clear();
		_near_neighbours.push_back(index); //0 
		_near_neighbours.push_back(index); //1
		_near_neighbours.push_back(index); //2
		_near_neighbours.push_back(index); //3
		_near_neighbours.push_back(index); //4
		_near_neighbours.push_back(index); //5
		_near_neighbours.push_back(index); //6
		_near_neighbours.push_back(index); //7
		_near_neighbours.push_back(index); //8
	}
	
	else {
	_near_neighbours.clear();


	index = (i-1)*10000 + (j+1); //pkt (i-1, j+1)
	if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //0
	else _near_neighbours.push_back(Grid::empty_index);  

	index = i*10000 + (j+1); //pkt (i, j+1)
	if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //1
	else _near_neighbours.push_back(Grid::empty_index);  
	
	
	index = (i+1)*10000 + (j+1); //pkt (i+!, j+1)
	if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //2
	else _near_neighbours.push_back(Grid::empty_index);
	
	index = (i-1)*10000 + j; //pkt (i-1, j)
	if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //3
	else _near_neighbours.push_back(Grid::empty_index);
	
	index = i*10000 + j; //pkt (i, j)
	if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //4
	else _near_neighbours.push_back(Grid::empty_index);
	
	index = (i+1)*10000 + j; //pkt (i+1, j)
	if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //5
	else _near_neighbours.push_back(Grid::empty_index);
	
	index = (i-1)*10000 + (j-1); //pkt (i-1, j-1)
	if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //6
	else _near_neighbours.push_back(Grid::empty_index);
	
	index = i*10000 + (j-1); //pkt (i, j-1)
	if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //7
	else _near_neighbours.push_back(Grid::empty_index);
	
	index = (i+1)*10000 + (j-1); //pkt (i+1, j-1)
	if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //8
	else _near_neighbours.push_back(Grid::empty_index);
	

////////////////////UWAGA dla preriodycznych warunkow przegowych w kanale guymera/////////////////////	
	/*//BEGIN PERIODYCZNE
	index = (i-1)*10000 + (j+1); //pkt (i-1, j+1)
	if(i==0){
	  cout<<"(i,j):"<<i<<" "<<j<<endl;
	  index = 895*10000 + j+1; //pkt (i+1, j)
	  if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //5	  
	}
	else if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //3
	else _near_neighbours.push_back(Grid::empty_index);

	index = i*10000 + (j+1); //pkt (i, j+1)
	if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //1
	else _near_neighbours.push_back(Grid::empty_index);  
	
	
	index = (i+1)*10000 + (j+1); //pkt (i+!, j+1)
	if(i==895) {
	  cout<<"(i,j):"<<i<<" "<<j<<endl;
	  index = 0*10000 + j+1; //pkt (i+1, j)
	  if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //5
	}
	else if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //5
	else _near_neighbours.push_back(Grid::empty_index);

	index = (i-1)*10000 + j; //pkt (i-1, j)
	if(i==0){
	  cout<<"(i,j):"<<i<<" "<<j<<endl;
	  index = 895*10000 + j; //pkt (i+1, j)
	  if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //5	  
	}
	else if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //3
	else _near_neighbours.push_back(Grid::empty_index);

       	index = i*10000 + j; //pkt (i, j)
	if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //4
	else _near_neighbours.push_back(Grid::empty_index);
	
	index = (i+1)*10000 + j; //pkt (i+1, j)
	if(i==895) {
	  cout<<"(i,j):"<<i<<" "<<j<<endl;
	  index = 0*10000 + j; //pkt (i+1, j)
	  if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //5
	}
	else if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //5
	else _near_neighbours.push_back(Grid::empty_index);
	
	index = (i-1)*10000 + (j-1); //pkt (i-1, j-1)
	if(i==0){
	  cout<<"(i,j):"<<i<<" "<<j<<endl;
	  index = 895*10000 + j-1; //pkt (i+1, j)
	  if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //5	  
	}
	else if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //3
	else _near_neighbours.push_back(Grid::empty_index);
	
	index = i*10000 + (j-1); //pkt (i, j-1)
	if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //7
	else _near_neighbours.push_back(Grid::empty_index);
	
	index = (i+1)*10000 + (j-1); //pkt (i+1, j-1)
	if(i==895) {
	  cout<<"(i,j):"<<i<<" "<<j<<endl;
	  index = 0*10000 + j-1; //pkt (i+1, j)
	  if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //5
	}
	else if (_PointMap->find(index)!=_PointMap->end()) _near_neighbours.push_back(index); //5
	else _near_neighbours.push_back(Grid::empty_index);
	//END PERIODYCZNE*/
////////////////////UWAGA dla preriodycznych warunkow przegowych w kanale guymera/////////////////////	
/*
	  cout<<"	PUNKT: ("<<i<<","<<j<<") najblizsi sasiedzi:"<<endl;
	  cout<<_PointMap->find(_near_neighbours[0])->second.get_x()<<","<<_PointMap->find(_near_neighbours[0])->second.get_y()<<"	"
	      <<_PointMap->find(_near_neighbours[1])->second.get_x()<<","<<_PointMap->find(_near_neighbours[1])->second.get_y()<<"	"	
	      <<_PointMap->find(_near_neighbours[2])->second.get_x()<<","<<_PointMap->find(_near_neighbours[2])->second.get_y()<<endl;
	  cout<<_PointMap->find(_near_neighbours[3])->second.get_x()<<","<<_PointMap->find(_near_neighbours[3])->second.get_y()<<"	"
	      <<_PointMap->find(_near_neighbours[4])->second.get_x()<<","<<_PointMap->find(_near_neighbours[4])->second.get_y()<<"	"	
	      <<_PointMap->find(_near_neighbours[5])->second.get_x()<<","<<_PointMap->find(_near_neighbours[5])->second.get_y()<<endl;
	  cout<<_PointMap->find(_near_neighbours[6])->second.get_x()<<","<<_PointMap->find(_near_neighbours[6])->second.get_y()<<"	"
	      <<_PointMap->find(_near_neighbours[7])->second.get_x()<<","<<_PointMap->find(_near_neighbours[7])->second.get_y()<<"	"	
	      <<_PointMap->find(_near_neighbours[8])->second.get_x()<<","<<_PointMap->find(_near_neighbours[8])->second.get_y()<<endl;
*/
	}
};

//_____________________________________________________________________________________________________________
void Point::set_distant_neighbours (int opt){
	int i = this->get_x(), j = this->get_y();
	
	int index = Grid::empty_index;
	
	if (opt == 0) {
		//cout <<"Tworze sztucznych dalszych sasiadow dla pkt - opt = "<<opt<<endl;
		_distant_neighbours.clear();
		_distant_neighbours.push_back(index);//0	
		_distant_neighbours.push_back(index);//1	
		_distant_neighbours.push_back(index);//2	
		_distant_neighbours.push_back(index);//3
		_distant_neighbours.push_back(index);//4		
	}
	
	else {
		_distant_neighbours.clear();
		index = i*10000 + j; //pkt (i, j)
		if (_PointMap->find(index)!=_PointMap->end()) _distant_neighbours.push_back(index); //0
		else _distant_neighbours.push_back(Grid::empty_index);
		
		index = (i)*10000 + (j+2); //pkt (i, j+2)
		if (_PointMap->find(index)!=_PointMap->end()) _distant_neighbours.push_back(index); //1
		else _distant_neighbours.push_back(Grid::empty_index);
		
		index = (i+2)*10000 + (j); //pkt (i+2, j)
		if (_PointMap->find(index)!=_PointMap->end()) _distant_neighbours.push_back(index); //2
		else _distant_neighbours.push_back(Grid::empty_index);
		
		index = i*10000 + (j-2); //pkt (i, j-2)
		if (_PointMap->find(index)!=_PointMap->end()) _distant_neighbours.push_back(index); //3
		else _distant_neighbours.push_back(Grid::empty_index);
		
		index = (i-2)*10000 + (j); //pkt (i-2, j)
		if (_PointMap->find(index)!=_PointMap->end()) _distant_neighbours.push_back(index); //4
		else _distant_neighbours.push_back(Grid::empty_index);
/*		
		cout<<"	PUNKT: ("<<i<<","<<j<<") dalsi sasiedzi: "<<endl;
		cout<<"	"<<_PointMap->find(_distant_neighbours[1])->second.get_x()<<","<<_PointMap->find(_distant_neighbours[1])->second.get_y()<<endl;
		cout<<_PointMap->find(_distant_neighbours[4])->second.get_x()<<","<<_PointMap->find(_distant_neighbours[4])->second.get_y()<<"	"
			<<_PointMap->find(_distant_neighbours[0])->second.get_x()<<","<<_PointMap->find(_distant_neighbours[0])->second.get_y()<<"	"
			<<_PointMap->find(_distant_neighbours[2])->second.get_x()<<","<<_PointMap->find(_distant_neighbours[2])->second.get_y()<<endl;
		cout<<"	"<<_PointMap->find(_distant_neighbours[3])->second.get_x()<<","<<_PointMap->find(_distant_neighbours[3])->second.get_y()<<endl;
*/
		
	}
};
	
//_____________________________________________________________________________________________________________
void Point::set_Courant_numbers (double dx, double dy, double dt){
	//cout<<"-->Wyznaczam liczby Couranta dla punktu ";
	//this->print();//_x<<","<<_y<<endl;
	if (dx != 0 ) {
		_CurAx = (_vx * dt)/dx;
		_CurDx = (_Dxx * dt)/(dx * dx);
	}
	else cout<<" UWAGA! Krok przestrzenny dx = 0! "<<endl;
	
	if (dy != 0 ) {
		_CurAy = (_vy * dt)/dy;
		_CurDy = (_Dyy * dt)/(dy * dy);
	} 
	else cout<<" UWAGA! Krok przestrzenny dy = 0! "<<endl;

	if (dx != 0 && dy != 0) _CurDxy = (_Dxy * dt)/(4 * dx * dy);
	else cout<<" UWAGA! Kroki przestrzenne dx = dy = 0! "<<endl;
	//cout<<"	Ax: "<<_CurAx<<"  Ay: "<<_CurAy<<"  Dx: "<<_CurDx<<"  Dxy: "<<_CurDxy<<"  Dy: "<<_CurDy<<endl<<endl;
};
