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
#include "modelosps.h"
#include "numerocuenta.h"
#include <qcheckbox.h>
/**
 * \class Mod300ps modelo300.h
 * \brief Modelo 300 del IVA
 * 
 * Clase para generar el modelo 300 de declaración de IVA trimestral en formato postscript.
Utiliza los formularios oficiales obtenidos de la <a href="http://www.aeat.es">Agencia Tributaria</a> convertidos de pdf a postscript.
Hereda de \ref Modgenps */
class Mod300ps: public Modgenps, public mod300dlg
  {
   
  public:
    Mod300ps(QWidget *parent);
    ~Mod300ps()
    {//cout << "Modelo 300 destruido\n";
    }
    ;

    void accept();
    void generaps();
    void escribe_cuenta_bancaria(int,int);
    void escribe_postscriptdefs(void);
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
    /** Bank account number */
    numerocuenta *ccc; 
    void personalButtonPressed();
    bool sincuentasbancarias;///<Si es true, no tenemos ninguna cuenta de banco en nuestra base de datos
protected:
    QString ano;
    QString empresa;
   
  };



#endif
