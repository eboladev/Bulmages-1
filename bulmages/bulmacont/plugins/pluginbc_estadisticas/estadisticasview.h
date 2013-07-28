/***************************************************************************
                          estadisticasview.h  -  description
                             -------------------
    begin                : mar jul 15 2003
    copyright            : (C) 2003 by Tomeu Borras Riera
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

#include <QtWidgets/QWidget>

#ifdef ESTADISTICAS
#include "estadisticasdlg.h"
#include "estadisticas/topiechart.h"
#include "estadisticas/tobarchart.h"
#include "estadisticas/tolinechart.h"
#endif

#ifdef GDCHART
#include "estadisticasdlg1.h"
#endif

#ifdef DISLIN
#include "estadisticasdlg1.h"
#endif

#include "blpostgresqlclient.h"


class estadisticasview : public estadisticasdlg
{
    Q_OBJECT

public:
    BlPostgreSqlClient *dbConnection;
    int inicializa ( BlPostgreSqlClient * );
    estadisticasview ( QWidget *parent = 0, const char *name = 0 );
    ~estadisticasview();
    void presentar();

#ifdef GDCHART
    void sacapie ( float *, char **, int );
#endif
};

#endif
