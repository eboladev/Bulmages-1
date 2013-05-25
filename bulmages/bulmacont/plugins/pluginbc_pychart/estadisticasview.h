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

#include "estadisticasdlg1.h"
#include "blpostgresqlclient.h"


class BcCompany;


class estadisticasview : public estadisticasdlg
{
    Q_OBJECT

public:
    BlPostgreSqlClient *dbConnection;
    int inicializa ( BlPostgreSqlClient *, int );
    estadisticasview ( QWidget *parent = 0, const char *name = 0 );
    ~estadisticasview();
    void presentar();
    void presentarbarras();
};


class myplugin : public QObject
{
    Q_OBJECT

public:
    BlPostgreSqlClient *dbConnection;
    empresa *company;

public:
    myplugin();
    ~myplugin();

public slots:
    virtual void BalanceGrafico();
    virtual void BalanceBarras();
};

#endif
