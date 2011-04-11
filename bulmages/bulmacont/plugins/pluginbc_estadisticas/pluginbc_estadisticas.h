/***************************************************************************
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

#ifndef PLUGINBC_ESTADISTICAS_H
#define PLUGINBC_ESTADISTICAS_H

#include <QStringList>
#include <QWidget>
#include <QApplication>
#include <QObject>

#include <qwidgetplugin.h>
#include <qiconset.h>

#include "bulmages01.h"
#include "blpostgresqlclient.h"
#include "bccompany.h"
#include "pdefs_pluginbc_estadisticas.h"


extern "C" PLUGINBC_ESTADISTICAS_EXPORT int entryPoint ( Bulmages01 * );
extern QApplication *g_theApp;


class QT_WIDGET_PLUGIN_EXPORT myplugin : public QObject
{
    Q_OBJECT

public:
    BlPostgreSqlClient *dbConnection;
    empresa *empresaactual;

public:
    myplugin();
    ~myplugin();
    void inicializa ( Bulmages01 * );

public slots:
    virtual void SegCuentas();
    virtual void BalanceGrafico();
};


#endif

