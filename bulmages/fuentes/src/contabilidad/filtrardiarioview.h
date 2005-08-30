/***************************************************************************
                          filtrardiarioview.h  -  description
                             -------------------
    begin                : mar jul 29 2003
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

#ifndef FILTRARDIARIOVIEW_H
#define FILTRARDIARIOVIEW_H

#include <qwidget.h>
#include <qpopupmenu.h>
#include <qcombobox.h>

#include "filtrardiariodlg.h"
#include "postgresiface2.h"
#include "busquedacuenta.h"

/***@author Tomeu Borrás Riera */
class empresa;

class filtrardiarioview : public filtrardiariodlg  {
   Q_OBJECT
private: 
  postgresiface2* conexionbase;
  int numdigitos;
  int ccostes[200];
  empresa *empresaactual;
//  QString idcontrapartida; // Si hay una este campo vale su id
  
public:
   QString idcontrapartida() {return m_contrapartida->idcuenta();};
   filtrardiarioview(empresa *, QWidget *parent=0, const char *name=0);
	~filtrardiarioview();
   
   public slots:
   virtual void boton_canales();
   virtual void boton_ccostes();
};
#endif
