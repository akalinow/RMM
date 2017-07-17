/***************************************************************************
 *   $FILENAME$.cpp
 *   Copyright (C) 2005 by Monika Kalinowska                               *
 *   mkalinow@igf.edu.pl                                                   *
 *                                                                         *
 ***************************************************************************/
/**

@mainpage River Mixing Model 

@author Monika Kalinowska 

A two - dimensional River Mixing Model (RMM) 
has been created for simulating rivers mixing. 
The model uses finite difference numerical methods for solving 2D advection - diffusion transport equation.
\sa <a href = http://www.igf.edu.pl/~mkalinow/Monika/model.php>more information</a>
*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

//#define MY_DEBUG
//#include "SDL.h"

#include <fstream>
#include <iostream>                  
#include <cstdlib>


#include <ctime>   /* functions time and clock defined here */
#include <unistd.h> /* function sleep defined here */
#include <cmath>   /* function sqrt defined here */

#include "simulation.h"
#include "comments.h"
//#include "param.h"
//#include "point.h"

using namespace std;

int main(int argc, char *argv[])
{
 
	//string cout_opt = "file";
	string cout_opt = "screan";
	string param = "param.txt";
	string save = "save.txt";
	Helloo(cout_opt);
	ofstream out("Info.dat", ios::app);
	
//SYS - kozysta z funkcji systemowych - do zakomentowania przy kompilacji na win	
	////////////////////////////////////////////////////////////////
	time_t  t0, t1; /* time_t is defined on <bits/types.h> as long */
  	clock_t c0, c1; /* clock_t is defined on <bits/types.h> as long */
	t0 = time(NULL);
  	c0 = clock();
//	if (cout_opt == "screan"){
// 		printf ("\tbegin (wall):            %ld seconds\n", (long) t0);
//  		printf ("\tbegin (CPU):             %ld cycles\n", (long) c0);
//	}
//	else if (cout_opt == "file"){
//		out<<"begin (wall):            seconds"<<t0<<endl;
//  		out<<"begin (CPU):             cycles"<<c0<<endl;
//	}
//	cout<<endl;
//SYS - kozysta z funkcji systemowych - do zakomentowania przy kompilacji na win	

	
	FileWithParameters(param, cout_opt);
	Simulation Sym(param, save);

//SYS - kozysta z funkcji systemowych - do zakomentowania przy kompilacji na win		
	////////////////////////////////////////////////////////////////////////////////
  	cout<<endl;
	t1 = time(NULL);
  	c1 = clock();
	if (cout_opt == "screan"){
  		//printf ("\tend (wall):              %ld seconds\n", (long) t1);
  		//printf ("\tend (CPU);               %ld cycles\n", (long) c1);
  		printf ("\telapsed wall clock time: %ld seconds\n", (long) (t1 - t0));
  		printf ("\telapsed CPU time:        %f seconds\n", (float) (c1 - c0)/CLOCKS_PER_SEC);
	}
	else if (cout_opt == "file"){
		//out<<"END (wall):            seconds"<<t1<<endl;
  		//out<<"END (CPU):             cycles"<<c1<<endl;
		out<<"elapsed wall clock time: seconds"<<(t1 - t0)<<endl;
  		out<<"elapsed CPU time:        seconds"<<(c1 - c0)/CLOCKS_PER_SEC<<endl;
	}
	////////////////////////////////////////////////////////////////////////////////////// 
//SYS - kozysta z funkcji systemowych - do zakomentowania przy kompilacji na win	
	
	End(cout_opt);
}
