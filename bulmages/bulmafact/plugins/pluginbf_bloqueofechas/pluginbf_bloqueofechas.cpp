/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include <stdio.h>

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>

#include "pluginbf_bloqueofechas.h"
#include "bfcompany.h"
#include "bcbloqueafechaview.h"

BfBulmaFact *g_pluginbf_bloqueofechas = NULL;

///
/**
\return
**/
int entryPoint ( BfBulmaFact *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_bloqueofechas", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    g_pluginbf_bloqueofechas = bcont;
    /// Miramos si existe un menu Herramientas
    QMenu *pPluginMenu = bcont->newMenu( _("&Herramientas"), "menuHerramientas", "menuVentana" );

    BlAction *accionA = new BlAction ( _ ( "&Bloqueo de Fechas" ), 0 );
    accionA->setStatusTip ( _ ( "Bloqueo de Fechas" ) );
    accionA->setWhatsThis ( _ ( "Bloqueo de Fechas" ) );
    accionA->setObjectName("mui_actionBloqueoFechas");

    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( accionA );

    return 0;
}

int BlAction_actionTriggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionBloqueoFechas") {
        BcBloqueaFechaView *tip = new BcBloqueaFechaView ( g_pluginbf_bloqueofechas->company(), 0 );
        g_pluginbf_bloqueofechas->workspace()->addSubWindow ( tip );
        tip->show();
    } // end if
    return 0;
}
