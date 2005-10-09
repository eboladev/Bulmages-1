/***************************************************************************
                          funcaux.h  -  description
                             -------------------
    begin                : Fri Jan 24 2003
    copyright            : (C) 2003 by Tomeu Borr� Riera
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
 /** \file funcaux.h
  * Fichero de definici� de funciones auxiliares que no es preciso encapsular<BR>
  * <P>En este fichero se definen todas aquellas funciones que por su brevedad o aislamiento son
  * utilizadas de forma regular en el programa. Implementadas en \ref funcaux.cpp </P>
  * <P>Dichas funciones normalmente son de uso general, por lo que es normal ver este archivo incluido
  * en la pr�tica totalidad de los dem� ficheros</P>
  * \author Tomeu Borr� Riera
  */
#ifndef FUNCAUX_H
#define FUNCAUX_H

#include <qdatetime.h>
#include <stdio.h>
#include <sstream>
#include <string>
using std::string;
#include <math.h>
#include <qstring.h>

#define __DEBUG__


/** \brief Extiende un string a un numero de cuenta sustituyendo los '.' por ceros.  */
string extiendecodigo (string , unsigned int );
/** \brief Extiende un string a un numero de cuenta sustituyendo los '.' por ceros.  */
QString extiendecodigo (QString, unsigned int);
/** \brief Redondeo de numeros en punto flotante. */
float fround(float, unsigned);
/** \brief Esta funci� convierte un numero con decimales a un entero. */
int roundI(double);
/** \brief Procesa el string pasado como parametro y devuelve una estructura del tipo QDate */
QDate normalizafecha(QString);
/** \brief Proteje cadenas de texto pasandoles una sustituci� de codigos especiales de XML  */
QString XMLProtect( const QString& );
/** \brief Ajusta un c�igo de cuenta a una longitud determinada pasada como parametro */
QString ajustacodigo( QString, unsigned int);
void reemplazaarchivo(QString, QString, QString, QString);


template <typename T>
std::string Ttos(T arg) {
   std::ostringstream buffer;
   buffer << arg; // send to the ostringstream
   return buffer.str(); // capture the string
}// end Ttos


inline void _depura(QString cad) {
#ifdef __DEBUG__
	fprintf(stderr,"%s\n",cad.ascii());
#endif
}// end depura
#endif

