/***************************************************************************
                          balancesprintview.h  -  description
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

#ifndef BALANCESPRINTVIEW_H
#define BALANCESPRINTVIEW_H

#include <qwidget.h>
#include <balancesprintdlg.h>
#ifndef WIN32
#include <unistd.h>
#include <sys/wait.h>
#endif
#include <errno.h>
#include <sys/types.h>
#include <string>
using std::string;
#include "postgresiface2.h"

/***@author Tomeu Borrás Riera */

class balancesprintview : public balancesprintdlg  {
   Q_OBJECT
public:
   char *fichero;
   string idbalance;

   postgresiface2 *conexionbase;
	balancesprintview(QWidget *parent=0, const char *name=0);
	~balancesprintview();
   void inicializa(postgresiface2 *con) {conexionbase = con;}
   void setidbalance(string);       // Establece cual será el balance a imprimir

public slots:
   virtual void accept();
   virtual void fechaincambiada();
   virtual void fechafincambiada();
};

#endif
