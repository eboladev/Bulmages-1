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
#include <qmessagebox.h>
#include <stdio.h>
#include <sstream>
#include <string>
using std::string;
#include <math.h>
#include <qstring.h>


#define __DEBUG__

/// g_main debe estar declarado en cada una de las aplicacioones y hace referencia al QMainWindow respectivo en cada caso. Se usa para acceder a determiandos elementos especificos de la ventana como el statusBar
#include <Q3MainWindow>
#include <QStatusBar>

extern Q3MainWindow *g_main;


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

/// Esta funcion permite editar un texto en un QTextEdit y devuelve el texto editado.
QString editaTexto(QString texto);


template <typename T>
std::string Ttos(T arg) {
   std::ostringstream buffer;
   buffer << arg; // send to the ostringstream
   return buffer.str(); // capture the string
}// end Ttos


inline void _depura(QString cad, int nivel=0) {
	if (nivel == 0) {
		if(g_main != NULL) {
			g_main->statusBar()->message(cad);
			fprintf(stderr,"%s", cad.toAscii().data());
		 } else
			fprintf(stderr,"%s", cad.toAscii().data());
	} else if (nivel == 1) {
		fprintf(stderr,"%s\n",cad.toAscii().data());
	} else if (nivel == 2) {
		QMessageBox::question(
                NULL,
                "Informacion de depuracion",
                cad,
                "&Salir",
                QString::null, 0);
	}// end if
// # endif
}// end depura



/// En la impresion de documentos con trml2pdf esta funcion hace casi todo el trabajo de la invocacion de trml2pdf
/// Para evitar trabajo duplicado. El archivo debe estar ubicado ya en el directorio especificado por CONF_DIR_USER
void invocaPDF(const QString arch);
void generaPDF(const QString arch);

#endif

