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

class empresa;

class compbalanceview : public compbalancedlg  {
   Q_OBJECT
public:
   empresa *empresaactual;
   postgresiface2 *conexionbase;
   QString idbalance;    // Esta variable indica cual es el balance que se esta modificando/visualizando
public: 
	compbalanceview(empresa *, QWidget *parent=0, const char *name=0);
	~compbalanceview();
   void inicializa1(QString);
   QString idmpatrimonial;   //este campo acompaña a la descripcón de la masa, pero no aparece pq no puede editarse.

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
