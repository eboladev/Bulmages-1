/***************************************************************************
                          asientoview.h  -  description
                             -------------------
    begin                : Thu Dec 19 2002
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

#ifndef ASIENTOVIEW_H
#define ASIENTOVIEW_H

#include <qwidget.h>
#include "asientodlg.h"
#include <qinputdialog.h>
#include <qdatetime.h>
#include <qdatetimeedit.h>
#include <qcombobox.h>
#include <qtextedit.h>
#include "postgresiface2.h"

/** *@author Tomeu Borrás Riera */

class asientoview : public asientodlg  {
   Q_OBJECT
public:
   postgresiface2 *conexionbase;
   int idasiento_mostrado;
   int ordenasiento_mostrado;
public: 
   asientoview(QWidget *parent=0, const char *name=0, bool modal=true);
   ~asientoview();
   void inicializa(postgresiface2 *);
   void cargaasiento(int);
   int creaasiento(QString, QString, int numasiento=0, int clase=1);
   int modificaasiento(QString, QString, int, QString, QString);
public slots:
   virtual void return_fecha();
   virtual void acceptar();
};
#endif
