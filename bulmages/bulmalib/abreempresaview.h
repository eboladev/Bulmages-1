/***************************************************************************
                          abreempresaview.h  -  description
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

#ifndef ABREEMPRESAVIEW_H
#define ABREEMPRESAVIEW_H

#include ".ui/abreempresadlg.h"


#include <qwidget.h>
#include <qlineedit.h>
#include <qtimer.h>
#include <qstring.h>
#include <stdio.h>
#include <qlistbox.h>
#include <qlistview.h>
#include <qmessagebox.h>

#include "postgresiface2.h"
#include "configuracion.h"


class BSelector;
/** @author Tomeu Borrás Riera */

class abreempresaview : public abreempresadlg  {
   Q_OBJECT
   
private:
   QString nombre;
   QString contrasena;
   QString empresabd;
   QString nombreempresa;
   QString tipo;
   
private:   
    int intentos;
    QString user;
    QString password;
       
public:
   QString nomDB() {return empresabd;};
   QString nomEmpresa() {return nombreempresa;};
   abreempresaview(QWidget *parent=0,int tipo=0, const char *name=0, bool modal=true);
   ~abreempresaview();
  /// Listamos las nuevas bases de datos.

private:
    void insertCompany(QString , QString , QString , QString);
    void listDB();

private slots:
    virtual void closeEvent(QCloseEvent * e);
    virtual void quit() {exit(1);};
        
public slots:
   //virtual void slotabreempresaview();
   virtual void accept();
   
};

#endif
