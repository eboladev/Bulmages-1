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
/// utilizadas de forma regular en el programa. Implementadas en blfunctions.cpp
/// Dichas funciones normalmente son de uso general, por lo que es normal ver este
/// archivo incluido en la practica totalidad de los demas ficheros.
#ifndef BLFUNCTIONS_H
#define BLFUNCTIONS_H

#include <QDateTime>
#include <QMessageBox>
#include <QDesktopWidget>

#include <stdio.h>
#include <sstream>
#include <string>

#include <QString>
#include <math.h>


/// g_main debe estar declarado en cada una de las aplicacioones y hace referencia al
/// QMainWindow respectivo en cada caso. Se usa para acceder a determiandos elementos
/// especificos de la ventana como el statusBar
#include <QMainWindow>
#include <QStatusBar>

#include "blapplication.h"
#include "blconfig.h"
#include "bli18n.h"



/// Extiende un string a un numero de cuenta sustituyendo los '.' por ceros.
QString extiendecodigo ( QString, unsigned int );
/// Extiende un string a un numero de cuenta sustituyendo los '.' por ceros.
QString extiendecodigo ( QString, unsigned int );
/// Redondeo de numeros en punto flotante.
float fround ( float, unsigned );
/// Esta funcion convierte un numero con decimales a un entero.
int roundI ( double );
/// Procesa el string pasado como parametro y devuelve una estructura del tipo QDate.
QDate normalizafecha ( QString );


/// Escapa los mínimos caracteres necesarios para 
/// escribir a cadena a XML, sin usar CDATA porque puede no 
/// estar permitido en todos los lugares de un fichero xml 
QString xmlEscape ( const QString& text );

/// Escapa una cadena para que pueda ser embebida en un script python.
QString pythonEscape ( const QString &text);

/// Sistema general de escape
QString genEscape (const QString &text, int tipoEscape = 0);

/// Proteje cadenas de texto pasandoles una sustitucion de codigos especiales de XML.
QString XMLProtect ( const QString& );
QString XMLDesProtect ( const QString& );
/// Ajusta un codigo de cuenta a una longitud determinada pasada como parametro.
QString ajustacodigo ( QString, unsigned int );
void reemplazaarchivo ( QString, QString, QString, QString );
/// Esta funcion permite editar un texto en un QTextEdit y devuelve el texto editado.
QString editaTexto ( QString texto );

#if CONFIG_DEBUG == TRUE
void _depura ( const QString &cad, int nivel = 0, const QString & param = "" );
#else
#define _depura(...)       // sin debug
#endif
void _debugOn();
void _debugOff();


void mensajeInfo ( QString cad, QWidget *parent = 0 );
void mensajeAviso ( QString cad, QWidget *parent = 0 );
void mensajeError ( QString cad, QWidget *parent = 0 );
/// En la impresion de documentos con trml2pdf esta funcion hace casi todo el trabajo.
/// de la invocacion de trml2pdf.
/// Para evitar trabajo duplicado. El archivo debe estar ubicado ya en el directorio.
/// especificado por CONF_DIR_USER.
void invocaPDF ( const QString arch );
void generaPDF ( const QString arch );
void invocaPYS ( const QString arch );
void generaPYS ( const QString arch );

QString num2texto ( QString numero, QString moneda = "euros", QString singular = "euro" );
void centrarEnPantalla ( QWidget *ventana );
QString windowID ( const QString & );
/// Convierte un string en un string valido para ser escrito desde un script.
QString parsearCode ( const QString &cad );
QString ascii127 ( const QString &orig );
QString data2python(QString string);
bool validarCIF(QString cif1, QChar &digit);
bool validarNIF(QString nif1, QChar &digit);
int sumaDigitos(int val);
bool validarCIFNIF(QString nifcif, QChar &digit);

#endif

