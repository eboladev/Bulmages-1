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

#include "abreempresadlg.h"


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
#include "bselector.h"

class BSelector;
/** @author Tomeu Borrás Riera */

class abreempresaview : public abreempresadlg  {
   Q_OBJECT
public:
   abreempresaview(BSelector *parent=0, const char *name=0, bool modal=true);
   ~abreempresaview();
   QString nombre;
   QString contrasena;
   QString empresabd;
   QString nombreempresa;
   

private:   
    int intentos;
    BSelector * padre;

private slots:
    virtual void closeEvent(QCloseEvent * e);
        
public slots:
   //virtual void slotabreempresaview();
   virtual void accept();
   
};

#endif
