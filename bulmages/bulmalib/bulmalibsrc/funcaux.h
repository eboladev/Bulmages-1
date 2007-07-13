/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/// Fichero de definicion de funciones auxiliares que no es preciso encapsular
/// En este fichero se definen todas aquellas funciones que por su brevedad o aislamiento son
/// utilizadas de forma regular en el programa. Implementadas en funcaux.cpp
/// Dichas funciones normalmente son de uso general, por lo que es normal ver este
/// archivo incluido en la practica totalidad de los demas ficheros.
#ifndef FUNCAUX_H
#define FUNCAUX_H

#include <QDateTime>
#include <QMessageBox>
#include <QDesktopWidget>

#include <stdio.h>
#include <sstream>
#include <string>

using std::string;

#include <QString>
#include <math.h>

#define __DEBUG__

/// g_main debe estar declarado en cada una de las aplicacioones y hace referencia al
/// QMainWindow respectivo en cada caso. Se usa para acceder a determiandos elementos
/// especificos de la ventana como el statusBar
#include <QMainWindow>
#include <QStatusBar>

#include "qapplication2.h"

extern QMainWindow *g_main;

extern QApplication2 *theApp;


template <typename T>
std::string Ttos(T arg) {
    std::ostringstream buffer;
    buffer << arg; /// send to the ostringstream.
    return buffer.str(); /// capture the string.
}

/// Extiende un string a un numero de cuenta sustituyendo los '.' por ceros.
string extiendecodigo (string, unsigned int);
/// Extiende un string a un numero de cuenta sustituyendo los '.' por ceros.
QString extiendecodigo (QString, unsigned int);
/// Redondeo de numeros en punto flotante.
float fround(float, unsigned);
/// Esta funcion convierte un numero con decimales a un entero.
int roundI(double);
/// Procesa el string pasado como parametro y devuelve una estructura del tipo QDate.
QDate normalizafecha(QString);
/// Proteje cadenas de texto pasandoles una sustitucion de codigos especiales de XML.
QString XMLProtect(const QString&);
/// Ajusta un codigo de cuenta a una longitud determinada pasada como parametro.
QString ajustacodigo(QString, unsigned int);
void reemplazaarchivo(QString, QString, QString, QString);
/// Esta funcion permite editar un texto en un QTextEdit y devuelve el texto editado.
QString editaTexto(QString texto);
void _depura(QString cad, int nivel = 0, QString param = "");
void mensajeInfo(QString cad);
void mensajeAviso(QString cad);
void mensajeError(QString cad);
/// En la impresion de documentos con trml2pdf esta funcion hace casi todo el trabajo.
/// de la invocacion de trml2pdf.
/// Para evitar trabajo duplicado. El archivo debe estar ubicado ya en el directorio.
/// especificado por CONF_DIR_USER.
void invocaPDF(const QString arch);
void generaPDF(const QString arch);
QString  num2texto(QString numero, QString moneda = "Euros", QString singular = "euro");
void centrarEnPantalla(QWidget *ventana);

#endif

