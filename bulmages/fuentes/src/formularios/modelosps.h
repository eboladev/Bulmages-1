/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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

//Clase base para rellenar todos los formularios oficiales desde el pdf de www.aeat.es

#ifndef MODGEN_H
#define MODGEN_H
#include <qfile.h>
#include <qthread.h>
#include <qprogressdialog.h>
#include <qevent.h>

enum {sleep10=1001,sleep3=1002,acabado=1003};


class genps_thread:public QThread

{
public:
genps_thread(QString,QString,QProgressDialog *);
void run();

QString pdfname,tempname;
QProgressDialog *progressdia;
};
 
class psprogressdialog:public QProgressDialog
{
public:
psprogressdialog(QString,QString,int);
void customEvent(QCustomEvent *);
};

class Modgenps: public QObject
  {
    /** Clase base para modelos de declaracin de la agencia tributaria en formato postscript.
      * Utiliza los formularios oficiales obtenidos de www.aeat.es convertidos de pdf a postscript.
      * funciona insertando codigo postscript para escribir los numeros en las casillas.
      * Usa la fuente Courier-Bold a 12 puntos. */
// Q_OBJECT
  public:
    Modgenps()
    {}
    ;
    ~Modgenps()
    {}
    ;
    void download_form(QWidget *,QString ,QString );
    genps_thread *convierte_a_postscript;
    void formatdigits(QString *,QString *,float); //genera 2 cadenas con parte entera y fraccionaria del float
    void escrizq(QString,int,int);//escribe el texto "a la izquierda de" (o sea, alineado a la derecha)
    void escrizq(float,int,int);//funcion sobrecargada, igual que antes pero primero convierte float en cadena
    void escrder(QString,int,int);//igual que escrizq pero alineado a la izquierda
    void escrder(float,int,int);//sobrecargada igual que la anterior
    void escrizqder(QString,QString,int,int);//escribe primera cadena a la izquierda y segunda a la derecha del punto dado
    void escrizqder(float,int,int); //igual, pero primero convierte float en 2 cadenas con formatdigits
    void marca_casilla(QString,int,int); //pone una cruz en la casilla dada por sus coordenadas
    void marcadeagua_borrador();//inserta una marca de agua en el documento con la palabra BORRADOR bien grande, que sea vea!!
    QFile fichlec;
    QFile fich;
    QTextStream output;
    bool es_borrador;//True si el documento a generar es un borrador (mucho mas rapido de generar, pero sin numero de serie valido)
     /*
     public slots:
    void yata(bool);
    */
  };


  
  
  
  
  
  
  
  
  
  
  










#endif
