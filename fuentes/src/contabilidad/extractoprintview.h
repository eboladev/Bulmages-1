/***************************************************************************
                          extractoprintview.h  -  description
                             -------------------
    begin                : jue jun 26 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
    email                : tborras@conetxia.com
    modificat per        : (C) 2003 Antoni Mirabete i Terés - amirabet@biada.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef EXTRACTOPRINTVIEW_H
#define EXTRACTOPRINTVIEW_H

#include <qwidget.h>
#include <extractoprintdlg.h>
#include "postgresiface2.h"

#include <qlineedit.h>
#include <qdatetimeedit.h>
#include <qfiledialog.h>
#include <qwidget.h>
#include <qtable.h>
#include <qtoolbutton.h>
#include <qradiobutton.h>

/***@author Tomeu Borrás Riera*/

class ExtractoPrintView : public ExtractoPrintDlg  {
   Q_OBJECT
public:
   char *fichero;
   postgresiface2 *conexionbase;
   int inicializa(postgresiface2 *);
	void inicializa1(QString , QString , QString , QString );
	ExtractoPrintView(QWidget *parent=0, const char *name=0);
   void presentar(char *tipus);
	~ExtractoPrintView();
public slots:
   virtual void accept();
   virtual void boton_codinicial();
   virtual void boton_codfinal();
};

#endif
