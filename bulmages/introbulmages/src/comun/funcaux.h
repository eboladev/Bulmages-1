/***************************************************************************
                          funcaux.h  -  description
                             -------------------
    begin                : Fri Jan 24 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
    email                : tborras@conetxia.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef FUNCAUX_H
#define FUNCAUX_H

#include <qdatetime.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <math.h>
#include <qstring.h>

using std::string;


// Esta funcion es utilizada para extender la introduccion de codigos
// Asi se convierten cadenas del estilo 1.3 en 10000003
// Los parametros que acepta son el string inicial y el numero de digitos
// Que debe tener la cadena resultante.
string extiendecodigo (string , unsigned int );
QString extiendecodigo (QString, unsigned int);
float fround(float, unsigned);
int roundI(double);
QDate normalizafecha(QString);
//string modificafecha(string);
//QString modificafecha(QString);
//QDate devuelvefecha(string);

template <typename T>
std::string Ttos(T arg) {
   std::ostringstream buffer;
   buffer << arg; // send to the ostringstream
   return buffer.str(); // capture the string
}// end Ttos

#endif

