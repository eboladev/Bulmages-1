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

#include "pluginbc_depuracion.h"
#include "blconfiguration.h"

///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_depuracion", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    QMenu *pPluginMenu = bcont->newMenu( _("&Ver"), "menuVer", "menuMaestro" );

    /// A&ntilde;ade en el men&uacute; del programa la opci&oacuteMn para
    /// acceder al corrector.
    BlAction *accionA = new BlAction ( _ ( "&Modo debug" ), 0 );
    accionA->setCheckable ( true );

    if ( g_confpr->valueTrue( CONF_DEBUG )) {
        accionA->setChecked ( true );
    } else {
        accionA->setChecked ( false );
    }

    accionA->setStatusTip ( _ ( "Activa/Desactiva el modo debug" ) );
    accionA->setWhatsThis ( _ ( "Debug.\n\nActiva/Desactiva el modo debug" ) );
    accionA->setObjectName("mui_actionDebug");
    pPluginMenu ->addSeparator();
    pPluginMenu ->addAction ( accionA );

    
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
