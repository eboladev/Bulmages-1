/***************************************************************************
                          asientoview.h  -  description
                             -------------------
    begin                : Thu Dec 19 2002
    copyright            : (C) 2002 by Tomeu Borr� Riera
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
#include <q3datetimeedit.h>
#include <qcombobox.h>
#include <qmessagebox.h>
#include <q3textedit.h>
#include "postgresiface2.h"
#include "empresa.h"
#include "dialogchanges.h"

/** *@author Tomeu Borr� Riera */

class asientoview : public asientodlg , dialogChanges {
   Q_OBJECT
public:
   postgresiface2 *conexionbase;
   int idasiento_mostrado;
   int ordenasiento_mostrado;
   empresa *empresaactual;
public: 
   asientoview(empresa *emp,QWidget *parent=0, const char *name=0, bool modal=true);
   ~asientoview();
   void inicializa(postgresiface2 *);
   void cargaasiento(int);
   int creaasiento(QString, QString, int numasiento=0, int clase=1);
   int modificaasiento(QString, QString, int, QString, QString);
public slots:
   virtual void return_fecha();
   virtual void acceptar();
   virtual void close();
   virtual void s_saveAsiento();
};
#endif
