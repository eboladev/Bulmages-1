/***************************************************************************
                          mpatrimonialesview.h  -  description
                             -------------------
    begin                : mié sep 10 2003
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

#ifndef MPATRIMONIALESVIEW_H
#define MPATRIMONIALESVIEW_H

#include <qwidget.h>
#include <mpatrimonialesdlg.h>

#include "postgresiface2.h"

/***@author Tomeu Borrás Riera */

class mpatrimonialesview : public mpatrimonialesdlg  {
   Q_OBJECT
public:
   postgresiface2 *conexionbase; // Esta variable indica cual es la base de datos con la que se trabaja.
   int numdigitos;               // Esta variable indica el número de dítigos que tienen las cuentas de nivel más bajo.
   int modo;                     // Esta variable indica si se abre para busqueda o para edicion.
   string nommasa;               // Esta variable indica el nombre de la masa, luego se usara
   string idmasa;                // Esta variable indica el identificador de masa.

public: 
	mpatrimonialesview(QWidget *parent=0, const char *name=0, int fl=0);
	~mpatrimonialesview();
   int inicializa(postgresiface2 *);
   void inicializatabla();
   void setmodoselector() {modo = 1;}
   void setmodoeditor() {modo = 0;}
   string getnommasa() {return nommasa;}
   string getidmasa() {return idmasa;}
   
public slots:
   virtual void dbtabla(int, int, int, const QPoint &);
   virtual void nuevamasa();
   virtual void borrarmasa();
   virtual void editarmasa();

};

#endif
