/***************************************************************************
                          nuevaempresaview.h  -  description
                             -------------------
    begin                : Mon Jan 20 2003
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

#ifndef NUEVAEMPRESAVIEW_H
#define NUEVAEMPRESAVIEW_H

#include <qwidget.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <nuevaempresadlg.h>
#include "postgresiface2.h"

#include "configuracion.h"

/*** @author Tomeu Borrás Riera */

class nuevaempresaview : public nuevaempresadlg  {
   Q_OBJECT
public: 
  nuevaempresaview(QWidget *parent=0, const char *name=0, bool modal=true);
  ~nuevaempresaview();
  void crearempresa();
  int empresacreada;    // Indica si la ejecución de esta clase ha terminado en creación o no. 
public slots:
  virtual void accept();
  virtual void boton_usalectura();
  virtual void boton_lecturaaus();
  virtual void boton_usatotal();
  virtual void boton_totalaus();  
};
#endif

