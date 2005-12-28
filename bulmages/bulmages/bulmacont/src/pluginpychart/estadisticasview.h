/***************************************************************************
                          estadisticasview.h  -  description
                             -------------------
    begin                : mar jul 15 2003
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

#ifndef ESTADISTICASVIEW_H
#define ESTADISTICASVIEW_H

#include <qwidget.h>

#include "postgresiface2.h"
#include "estadisticasdlg1.h"


#include "postgresiface2.h"
/** *@author Tomeu Borrás Riera  */

class empresa;


class estadisticasview : public estadisticasdlg  {
   Q_OBJECT
public:

postgresiface2 *conexionbase;

   int inicializa(postgresiface2 *, int);
   estadisticasview(QWidget *parent=0, const char *name=0);
   ~estadisticasview();
   void presentar();
   void presentarbarras();
};


class myplugin : public QObject {
Q_OBJECT
public:
  postgresiface2 *conexionbase;
  empresa *empresaactual;
public:
   myplugin();
   ~myplugin();
public slots:
	virtual void BalanceGrafico();
	virtual void BalanceBarras();
};

#endif
