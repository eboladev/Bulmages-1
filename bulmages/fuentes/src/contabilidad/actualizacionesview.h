/***************************************************************************
                          actualizacionesview.h  -  description
                             -------------------
    begin                : mar jul 22 2003
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

#ifndef ACTUALIZACIONESVIEW_H
#define ACTUALIZACIONESVIEW_H

#include <qwidget.h>
#include <actualizacionesdlg.h>

/**
  *@author Tomeu Borrás Riera
  */

class actualizacionesview : public actualizacionesdlg  {
   Q_OBJECT
public: 
	actualizacionesview(QWidget *parent=0, const char *name=0);
	~actualizacionesview();
};

#endif
