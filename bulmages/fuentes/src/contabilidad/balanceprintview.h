/***************************************************************************
                          balanceprintview.h  -  description
                             -------------------
    begin                : mié jun 25 2003
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

#ifndef BALANCEPRINTVIEW_H
#define BALANCEPRINTVIEW_H

#include <errno.h>

#include "balanceprintdlg.h"


//#include "listcuentasview1.h"
//#include "libromayorprint.h"

//#include "intapunts3view.h"
//#include "diarioview1.h"
//#include "extractoview1.h"
//#include "qtable1.h"
//#include "qlayout.h"
#include "postgresiface2.h"


class empresa;

/***@author Tomeu Borrás Riera */

class BalancePrintView : public BalancePrintDlg  {
   Q_OBJECT
public:
   char *fichero;
   int ccostes[200];
   postgresiface2 *conexionbase;   
   empresa *empresaactual;
public: 
	BalancePrintView(empresa *emp, QWidget *parent=0, const char *name=0);
	~BalancePrintView();
   int inicializa(postgresiface2 *);
   void inicializa1(QString, QString, QString, QString, bool);
   void inicializa2(char *);
   void accept();
   void presentar(char *tipus);
public slots:
   virtual void boton_codigoinicial();
   virtual void boton_codigofinal();
};

#endif
