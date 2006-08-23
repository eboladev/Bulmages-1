/***************************************************************************
                          CAnualesPrintView.h  -  description
                             -------------------
    begin                : jue oct 9 2003
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

#ifndef CANUALESSPRINTVIEW_H
#define CANUALESPRINTVIEW_H

#include <qwidget.h>
#include "ui_canualesprintbase.h"
#ifndef WIN32
#include <unistd.h>
#include <sys/wait.h>
#endif
#include <errno.h>
#include <sys/types.h>

/** *@author Tomeu Borrás Riera */

class empresa;

class CAnualesPrintView : public QDialog, public Ui_CAnualesPrintBase  {
   Q_OBJECT
private:
   char *fichero;
   QString idbalance;
   empresa *empresaactual;
public:
	CAnualesPrintView(empresa *emp,QWidget *parent=0);
	~CAnualesPrintView();
   void setidbalance(QString);       // Establece cual será el balance a imprimir

public slots:
   virtual void accept();
};

#endif
