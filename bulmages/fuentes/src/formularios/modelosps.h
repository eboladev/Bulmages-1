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

//Clase con los datos necesarios para el modelo 300 de IVA trimestral

#ifndef MOD300_H
#define MOD300_H
#include "mod300dlg.h"
#include <fstream.h>
#include "numerocuenta.h"
class Modgenps
  {
    /** Clase base para modelos de declaracin de la agencia tributaria en formato postscript.
      * Utiliza los formularios oficiales obtenidos de www.aeat.es convertidos de pdf a postscript.
      * funciona insertando codigo postscript para escribir los numeros en las casillas.
      * Usa la fuente Courier-Bold a 12 puntos. */

  public:
    Modgenps()
    {}
    ;
    ~Modgenps()
    {}
    ;

    void formatdigits(QString *,QString *,float); //genera 2 cadenas con parte entera y fraccionaria del float
    void escrizq(QString,int,int);//escribe el texto "a la izquierda de" (o sea, alineado a la derecha)
    void escrizq(float,int,int);//funcion sobrecargada, igual que antes pero primero convierte float en cadena
    void escrder(QString,int,int);//igual que escrizq pero alineado a la izquierda
    void escrder(float,int,int);//sobrecargada igual que la anterior
    void escrizqder(QString,QString,int,int);//escribe primera cadena a la izquierda y segunda a la derecha del punto dado
    void escrizqder(float,int,int); //igual, pero primero convierte float en 2 cadenas con formatdigits
    void marca_casilla(QString,int,int); //pone una cruz en la casilla dada por sus coordenadas

    ifstream fichlec;
    ofstream fich;
  };


class Mod300ps: public Modgenps, public mod300dlg
  {
    /** Clase para generar el modelo 300 de declaracin de IVA trimestral en formato postscript.
      * Utiliza los formularios oficiales obtenidos de www.aeat.es convertidos de pdf a postscript.
      * Hereda de Modgenps */

  public:
    Mod300ps(QWidget *parent);
    ~Mod300ps()
    {//cout << "Modelo 300 destruido\n";
    }
    ;

    void accept();
    void generaps();
    void escribe_cuenta_bancaria(int,int);
    void rellena_identificacion();
    void rellena_compensacion();
    void rellena_liquidacion();
    float baser0;
    float baser16;
    float baser7;
    float baser4;
    float bases0;
    float bases16;
    float bases7;
    float bases4;
    float ivas4;
    float ivas7;
    float ivas16;
    float ivar4;
    float ivar7;
    float ivar16;
    
    float cas34; //Contenido de la casilla 34 (Resultado)
  private:
    QString *nombresccc;
    QString *numerccc;
    numerocuenta *ccc;
  public slots:
    virtual void personalButtonPressed();
protected:
    QString ano;
    QString empresa;
  };













#endif
