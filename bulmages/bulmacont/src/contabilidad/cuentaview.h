/***************************************************************************
                          cuentaview.h  -  description
                             -------------------
    begin                : Wed Nov 27 2002
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

#ifndef CUENTAVIEW_H
#define CUENTAVIEW_H

#include <stdlib.h>

#include "ui_cuentabase.h"
#include <qmessagebox.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <q3multilineedit.h>
#include <qcheckbox.h>
#include <qradiobutton.h>


/// Librerias de BulmaLib
#include "postgresiface2.h"
#include "funcaux.h"
#include "dialogchanges.h"
/** *@author Tomeu Borr� Riera */

class empresa;


class cuentaview : public QWidget, public Ui_CuentaBase , dialogChanges  {
Q_OBJECT
public:
empresa *empresaactual;
int idcuenta;
int idgrupos[100];
postgresiface2 *conexionbase;
unsigned int numdigitos; // Esta variable indica el nmero de d�itos que tiene la contabilidad en sus cuentas

public:
  cuentaview(empresa *emp,QWidget *, const char *name =0, int fl=0);
  ~cuentaview();
  int inicializa();
  int nuevacuenta(QString, int);
  void cuentanueva(QString);
  int cargacuenta(int);
  void cambiapadre( const QString &);  
  void closeEvent( QCloseEvent *);
public slots:
  virtual void codigo_ret();
  virtual void saveAccount();
  virtual void deleteAccount();
};
#endif
