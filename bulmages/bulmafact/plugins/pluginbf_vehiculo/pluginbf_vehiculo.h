/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef PLUGINBF_VEHICULO_H
#define PLUGINBF_VEHICULO_H

#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>

#include "blpostgresqlclient.h"
#include "bfbulmafact.h"
#include "pdefs_pluginbf_vehiculo.h"


extern "C" PLUGINBF_VEHICULO_EXPORT int entryPoint ( BfBulmaFact * );


class myplugin : public QObject
{
    Q_OBJECT

public:
    BlPostgreSqlClient *m_dbConnection;
    BfBulmaFact *m_bulmafact;

public:
    myplugin();
    ~myplugin();
    void inicializa ( BfBulmaFact * );

public slots:
    void elslot();
};


#endif

