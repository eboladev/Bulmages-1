/***************************************************************************
                          aboutview.h  -  description
                             -------------------
    begin                : Fri Dec 27 2002
    copyright            : (C) 2002 by Tomeu Borrás Riera
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

#ifndef ADOCUMENTAL_H
#define ADOCUMENTAL_H

#include <qwidget.h>
#include "adocumentalbase.h"
#include "postgresiface2.h"
/**  *@author Tomeu Borrás Riera  */

class empresa;

class adocumental : public adocumentalbase  {
   Q_OBJECT
private:
   empresa *empresaactual;
   postgresiface2 *conexionbase;
   QString RutaADocumental;
public: 
	adocumental(empresa *, QWidget *parent=0, const char *name=0);
	~adocumental();
public slots:
   virtual void doubleclicked(int, int, int, const QPoint &);
   virtual void boton_newadocumental();
};

#endif
