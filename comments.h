
/***************************************************************************
 *   comments.h							   *
 *   Copyright (C) 2005 by Monika Kalinowska                               *
 *   mkalinow@igf.edu.pl                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef COMMENTS_H
#define COMMENTS_H

#include <cstdlib>
#include <string>
#include <time.h>

#include "simulation.h"


using namespace std;

/*! \file comments.h
    \brief Help and coments.
	    
    Zawiera kometarze i opisy wyswietla na ekran podczas dzilania programu.
*/

/*! Zwraca aktulany czas w odpowiednim formacie. */
char * GetTime();
//_______________________________________________________________________


/*! Wyswietla komunikat powitalny.*/
void Helloo(const string &opt = "screan");
//_______________________________________________________________________


/*! Wyswietla komunikat koncowy. */
void End(const string &opt = "screan");
//_______________________________________________________________________


/*! Wypisuje informacje z jakiego pliku czytane sa parmetry symulacji.*/
void FileWithParameters( const string &file, const string &opt = "screan");
//________________________________________________________________________


/*! Sprawdza czy plik z ktorego chcemy czytac istnieje i w zaleznosci od 
wyniku wypisuje stosowny komentarz*/
void CheckFile(int opt, const string &file);
//__________________________________________________________________________


#endif
