/***************************************************************************
                          asientosview.h  -  description
                             -------------------
    begin                : Sat Dec 21 2002
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

#ifndef ASIENTOSVIEW_H
#define ASIENTOSVIEW_H

#include <qwidget.h>
#include <asientosdlg.h>
#include <q3table.h>
#include "postgresiface2.h"
#include "filtrarasientosview.h"
#include "empresa.h"

/** @author Tomeu Borr� Riera 
  * \brief Muestra un listado de todos los asientos existentes e informa de si estan abiertos o cerrados.
*/

class asientosview : public asientosdlg  {
   Q_OBJECT
public:
  empresa *companyact;

  filtrarasientosview *filt ; // Este objeto contiene todas las opciones de filtraje necesarias para funcionar.
  empresa *empresaactual;
public: 
  asientosview(empresa *emp, QWidget *parent=0, const char *name=0, bool=true);
  ~asientosview();
	void inicializa();
public slots:
  virtual void pulsado(int, int, int,const QPoint &);
  virtual void boton_filtrar();
};
#endif
