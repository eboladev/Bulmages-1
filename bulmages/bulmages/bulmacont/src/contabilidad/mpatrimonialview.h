/***************************************************************************
                          mpatrimonialview.h  -  description
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

#ifndef MPATRIMONIALVIEW_H
#define MPATRIMONIALVIEW_H

#include <qwidget.h>
#include <mpatrimonialdlg.h>

#include "postgresiface2.h"

/***@author Tomeu Borrás Riera */

class mpatrimonialview : public mpatrimonialdlg  {
   Q_OBJECT
public:
   postgresiface2 *conexionbase;   // La base de la empresa con la que trabajamos
   QString idmpatrimonial;          // El identificador de masa patrimonial. (Lo guardamos en forma de string pq es una prueba en la que no hay que hacer convesiones de tipos)
public: 
	mpatrimonialview(QWidget *parent=0, const char *name=0);
	~mpatrimonialview();
   int inicializa(postgresiface2 *);
   void inicializa1(QString);
   QString getidmasa()  {return idmpatrimonial;}
   QString getnommasa();

public slots:
   virtual void nuevasuma();
   virtual void nuevaresta();
   virtual void borrasuma();
   virtual void borraresta();
   virtual void accept();
   virtual void buscampatrimonial();
};

#endif
