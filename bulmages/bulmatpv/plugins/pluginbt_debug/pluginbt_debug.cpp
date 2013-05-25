/***************************************************************************
 *   Copyright (C) 2011 by Daniel Ripoll Osma                              *
 *   dripoll.public@gmail.com                                              *
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

#include <stdio.h>

#include <QtWidgets/QAction>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtCore/QObject>

#include "pluginbt_debug.h"
#include "btbulmatpv.h"
#include "blconfiguration.h"
#include "debug.h"
#include "bldockwidget.h"

CambiarDebug *g_debug;
BlDockWidget *g_dock;

///
/**
\param btpv
**/
int entryPoint ( BtBulmaTPV *bges )
{
    BlDebug::blDebug ( Q_FUNC_INFO, 0, "Punto de entrada de PluginBt_Debug" );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbt_debug", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    
    return 0;
}

///
/**
\param btpv
**/
int BtCompany_createMainWindows_Post ( BtCompany *bges )
{

    /// Creamos un widget sin area definida, para que no moleste al depurado
    BL_FUNC_DEBUG
    g_debug = new CambiarDebug ( bges, g_dock );

    QDockWidget *dockDebug = new QDockWidget("Debug", g_main);
    dockDebug->setFeatures(QDockWidget::AllDockWidgetFeatures);
    g_main->addDockWidget(Qt::NoDockWidgetArea, dockDebug);
    dockDebug->setWidget(g_debug);
    
    return 0;
}
