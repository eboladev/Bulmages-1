/***************************************************************************
                          resmensualview.h  -  description
                             -------------------
    begin                : dom jul 20 2003
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

#ifndef RESMENSUALVIEW_H
#define RESMENSUALVIEW_H

#include <qwidget.h>
#include "postgresiface2.h"

#ifdef ESTADISTICAS
#include "resmensualdlg.h"
#endif

#ifdef GDCHART
#include "resmensualdlg1.h"
#endif

#ifdef DISLIN
#include "resmensualdlg1.h"
#endif

class empresa;

/** *@author Tomeu Borrás Riera   */

class resmensualview : public resmensualdlg  {
   Q_OBJECT
private:
   empresa *empresaactual;
   postgresiface2 *conexionbase;
public: 
   string idmpatrimonial1, idmpatrimonial2, idmpatrimonial3;

   int inicializa();
	resmensualview(empresa *, QWidget *parent=0, const char *name=0);
	~resmensualview();
   void presentar();
   void presentarpie();
   void generargrafico(float *, char **);
public slots:
   virtual void accept();
   virtual void buscacodigocta();
   virtual void buscacodigocta1();
   virtual void buscacodigocta2();
   virtual void buscampatrimonial1();
   virtual void buscampatrimonial2();
   virtual void buscampatrimonial3();
};

#endif
