/***************************************************************************
                          listcuentasview.h  -  description
                             -------------------
    begin                : Wed Nov 27 2002
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

#ifndef LISTCUENTASVIEW1_H
#define LISTCUENTASVIEW1_H


#include <qlistview.h>
#include <stdlib.h>
#include <qmessagebox.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qtable.h>

#include "postgresiface2.h"
#include "funcaux.h"
#include "listcuentasdlg1.h"
/*** @author Tomeu Borrás Riera */

class listcuentasview1 : public listcuentasdlg1  {
Q_OBJECT

public:
int ccuenta,cdesccuenta;
int cidcuenta, cbloqueada, cdebe, chaber, cnodebe, cnohaber, cregularizacion, cimputacion, ctipocuenta;
int cgrupo;
int modo;       // Esta variable indica si se abre para busqueda o para edicion.
unsigned int numdigitos; // Esta variable indica el numero de digitos que tienen por defecto todas las cuentas que se crean.
QString codcuenta;      // El codigo de la cuenta
QString idcuenta;
postgresiface2 *conexionbase;

public:
   listcuentasview1(QWidget *parent=0, const char *name=0, bool modal=true);
   ~listcuentasview1();
   void arbolcuentas(QListViewItem *, int );
   void listpulsada(QListViewItem *);
   void listdblpulsada(QListViewItem *);
   int inicializa(postgresiface2 *);
   void inicializatabla();
public slots:
   virtual void dbtabla(int, int, int, const QPoint &);
   virtual void editarcuenta();
   virtual void borrarcuenta();
   virtual void nuevacuenta();
   virtual void descripcioncambiada(const QString &);
   virtual void codigocambiado(const QString &);
   virtual void return_codigo();
   virtual void return_descripcion();
};
#endif
