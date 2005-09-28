/***************************************************************************
                          filtrarasientosview.h  -  description
                             -------------------
    begin                : Sat Jan 4 2003
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

#ifndef FILTRARASIENTOSVIEW_H
#define FILTRARASIENTOSVIEW_H

#include <qwidget.h>
#include <qlineedit.h>
#include <filtrarasientosdlg.h>

/***@author Tomeu Borrás Riera */

class filtrarasientosview : public filtrarasientosdlg  {
   Q_OBJECT
public: 
	filtrarasientosview(QWidget *parent=0, const char *name=0);
	~filtrarasientosview();
public slots:
  virtual void boton_reset();
};

#endif
