/***************************************************************************
                          diarioprintview.h  -  description
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

#ifndef DIARIOPRINTVIEW_H
#define DIARIOPRINTVIEW_H

#include <qwidget.h>
#include "diarioprintdlg.h"
#include "postgresiface2.h"
#include <qlineedit.h>
#include <qdatetimeedit.h>
#include <qfiledialog.h>
#include <qwidget.h>
#include <qtable.h>
#include <qtoolbutton.h>
#include <qradiobutton.h>

#include "empresa.h"

/***@author Tomeu Borrás Riera */

class DiarioPrintView : public DiarioPrintDlg  {
   Q_OBJECT
public:
   char *fichero;
   postgresiface2 *conexionbase;  
   empresa *empresaactual;
   int numdigitos;
   
public:
   int inicializa(postgresiface2 *);
	void inicializa1(QString, QString);
	DiarioPrintView(empresa *emp, QWidget *parent, const char *name);
	~DiarioPrintView();
   void presentar(char *tipus="html");
   // OJO con esta función que es una funcion para probar el Report Tool Kit
   void pruebasRTK();
public slots:
   virtual void accept();
   virtual void boton_canales();
   virtual void boton_ccostes();
   
};

#endif
