/***************************************************************************
                          estadisticasview.h  -  description
                             -------------------
    begin                : mar jul 15 2003
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

#ifndef ESTADISTICASVIEW_H
#define ESTADISTICASVIEW_H

#include <qwidget.h>
#include "estadisticasdlg.h"

#include "estadisticas/topiechart.h"
#include "estadisticas/tobarchart.h"
#include "estadisticas/tolinechart.h"

#include "postgresiface2.h"
/** *@author Tomeu Borrás Riera  */

class estadisticasview : public estadisticasdlg  {
   Q_OBJECT
public:

postgresiface2 *conexionbase;

   int inicializa(postgresiface2 *);
	estadisticasview(QWidget *parent=0, const char *name=0);
	~estadisticasview();
   void presentar();
};

#endif
