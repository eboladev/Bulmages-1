/***************************************************************************
                          balancesview.h  -  description
                             -------------------
    begin                : sáb oct 18 2003
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

#ifndef BALANCESVIEW_H
#define BALANCESVIEW_H

#include <qwidget.h>
#include <balancesdlg.h>

#include "postgresiface2.h"

/***@author Tomeu Borrás Riera */

class empresa;

class balancesview : public balancesdlg  {
   Q_OBJECT
private:
   empresa *empresaactual;
   postgresiface2 *conexionbase; // Esta variable indica cual es la base de datos con la que se trabaja.
//   int numdigitos;               // Esta variable indica el número de dítigos que tienen las cuentas de nivel más bajo.
   int modo;                     // Esta variable indica si se abre para busqueda o para edicion.
   string nombalance;               // Esta variable indica el nombre de la masa, luego se usara
   string idbalance;                // Esta variable indica el identificador de masa.
   int numdigitos;
   
public: 
	balancesview(empresa *, QWidget *parent=0, const char *name=0);
	~balancesview();
   void inicializatabla();
   void setmodoselector() {modo = 1;}
   void setmodoeditor() {modo = 0;}
   string getnombalance() {return nombalance;}
   string getidbalance() {return idbalance;}

public slots:
//   virtual void dbtabla(int, int, int, const QPoint &);
//   virtual void nuevamasa();
   virtual void nuevo();
   virtual void borrar();
   virtual void imprimir();
   virtual void dbtabla(int, int, int, const QPoint &);
  virtual void boton_exportar();
  virtual void boton_importar();
   
};

#endif
