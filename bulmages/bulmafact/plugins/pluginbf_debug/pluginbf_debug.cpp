/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include <QtWidgets/QAction>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtCore/QObject>

#include "pluginbf_debug.h"
#include "blconfiguration.h"
#include "bfbulmafact.h"


///
/**
\param bcont
**/
int entryPoint ( BfBulmaFact *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_debug", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    /// A&ntilde;ade en el men&uacute; del programa la opci&oacuteMn para
    /// acceder al modo de depuraci&oacuteMm.
    BlAction *accionA = new BlAction ( _("&Modo depuracion"), 0 );
    accionA->setCheckable ( true );

    if ( g_confpr->valueTrue( CONF_DEBUG )) {
        accionA->setChecked ( true );
    } else {
        accionA->setChecked ( false );
    }

    accionA->setStatusTip ( _ ( "Activa/desactiva el modo de depuracion" ) );
    accionA->setWhatsThis ( _ ( "Depuracion.\n\nActiva/desactiva el modo de depuracion" ) );
    accionA->setObjectName("mui_actionDebug");
    QMenu *pPluginMenu = bcont->newMenu(_("&Herramientas"), "menuHerramientas", "menuAcerca_de");
    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( accionA );
    
    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionDebug") {

        if (accion->isChecked ()) {
            g_confpr->setValue( CONF_DEBUG, "true");
        } else {
            g_confpr->setValue( CONF_DEBUG, "false");
        } // end if

    } // end if
    return 0;
}
