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
#include <QString>

#include <stdio.h>
#include <sstream>
#include <string>
#include <math.h>

#include <QMainWindow>
#include <QStatusBar>

#include "bldefs.h"
#include "blapplication.h"
#include "blconfig.h"
#include "bli18n.h"



/// Extiende un string a un numero de cuenta sustituyendo los '.' por ceros.
QString BL_EXPORT blExtendStringWithZeros ( QString, unsigned int );
/// Redondeo de numeros en punto flotante.
float BL_EXPORT blFloatRound ( float, unsigned );
/// Esta funcion convierte un numero con decimales a un entero.
int blDoubleToInt ( double );
/// Procesa el string pasado como parametro y devuelve una estructura del tipo QDate.
QDate BL_EXPORT blNormalizeDate ( QString );


/// Escapa los mínimos caracteres necesarios para 
/// escribir a cadena a XML, sin usar CDATA porque puede no 
/// estar permitido en todos los lugares de un fichero xml 
QString BL_EXPORT xmlEscape ( const QString& text );

/// Escapa una cadena para que pueda ser embebida en un script python.
QString BL_EXPORT pythonEscape ( const QString &text);

/// Sistema general de escape
QString BL_EXPORT genEscape (const QString &text, int tipoEscape = 0);

/// Proteje cadenas de texto pasandoles una sustitucion de codigos especiales de XML.
QString BL_EXPORT blXMLEncode ( const QString& );
QString BL_EXPORT blXMLDecode ( const QString& );
/// Ajusta un codigo de cuenta a una longitud determinada pasada como parametro.
QString BL_EXPORT ajustacodigo ( QString, unsigned int );
void BL_EXPORT reemplazaarchivo ( QString, QString, QString, QString );
/// Esta funcion permite editar un texto en un QTextEdit y devuelve el texto editado.
QString BL_EXPORT editaTexto ( QString texto );

#if CONFIG_DEBUG == TRUE
void BL_EXPORT blDebug ( const QString &cad, int nivel = 0, const QString & param = "" );
#else
#define blDebug(...)       // sin debug
#endif
void BL_EXPORT blDebugOn();
void BL_EXPORT blDebugOff();


void BL_EXPORT mensajeInfo ( QString cad, QWidget *parent = 0 );
void BL_EXPORT mensajeAviso ( QString cad, QWidget *parent = 0 );
void BL_EXPORT mensajeError ( QString cad, QWidget *parent = 0 );
/// En la impresion de documentos con trml2pdf esta funcion hace casi todo el trabajo.
/// de la invocacion de trml2pdf.
/// Para evitar trabajo duplicado. El archivo debe estar ubicado ya en el directorio.
/// especificado por CONF_DIR_USER.
void BL_EXPORT invocaPDF ( const QString arch );
void BL_EXPORT generaPDF ( const QString arch );
void BL_EXPORT invocaPYS ( const QString arch );
void BL_EXPORT generaPYS ( const QString arch );

QString BL_EXPORT num2texto ( QString numero, QString moneda = "euros", QString singular = "euro" );
void BL_EXPORT centrarEnPantalla ( QWidget *ventana );
QString BL_EXPORT windowID ( const QString & );
/// Convierte un string en un string valido para ser escrito desde un script.
QString BL_EXPORT parsearCode ( const QString &cad );
QString BL_EXPORT ascii127 ( const QString &orig );
QString BL_EXPORT data2python(QString string);
bool BL_EXPORT validarCIF(QString cif1, QChar &digit);
bool BL_EXPORT validarNIF(QString nif1, QChar &digit);
int BL_EXPORT sumaDigitos(int val);
bool BL_EXPORT validarCIFNIF(QString nifcif, QChar &digit);

#endif

