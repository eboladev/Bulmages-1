/***************************************************************************
                          compbalanceview.h  -  description
                             -------------------
    begin                : sáb sep 13 2003
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

#ifndef COMPBALANCEVIEW_H
#define COMPBALANCEVIEW_H

#include <qwidget.h>
#include <compbalancedlg.h>
#include "postgresiface2.h"

/** *@author Tomeu Borrás Riera */

class compbalanceview : public compbalancedlg  {
   Q_OBJECT
public:
   postgresiface2 *conexionbase;
   string idbalance;    // Esta variable indica cual es el balance que se esta modificando/visualizando
public: 
	compbalanceview(QWidget *parent=0, const char *name=0);
	~compbalanceview();
   void inicializa(postgresiface2 *con) {conexionbase = con;}
   void inicializa1(string);
   string idmpatrimonial;   //este campo acompaña a la descripcion de la masa, pero no aparece pq no puede editarse.

public slots:
   virtual void agregalinea();
   virtual void borralinea();
   virtual void botonizq();
   virtual void botondrcha();
   virtual void botonarriba();
   virtual void botonabajo();
   virtual void listadopulsado(int, int, int, const QPoint &);
   virtual void listadodpulsado(int, int, int, const QPoint &);
   virtual void modificamasa();
   virtual void modificalinea();
   virtual void botonimprimir();
   virtual void cambiado(int, int);
   virtual void accept();
};

#endif
