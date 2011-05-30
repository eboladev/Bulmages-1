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

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>

#include "pluginbc_depuracion.h"
#include "blconfiguration.h"


///
/**
**/
PluginBc_Debug::PluginBc_Debug()
{
    blDebug ( "PluginBc_Debug::PluginBc_Debug", 0 );
    blDebug ( "END PluginBc_Debug::PluginBc_Debug", 0 );
}


///
/**
**/
PluginBc_Debug::~PluginBc_Debug()
{
    blDebug ( "PluginBc_Debug::~PluginBc_Debug", 0 );
    blDebug ( "END PluginBc_Debug::~PluginBc_Debug", 0 );
}


///
/**
\return
**/
void PluginBc_Debug::cambia ( bool valor )
{
    blDebug ( "PluginBc_Debug::cambia", 0 );
    g_confpr->setValue ( CONF_DEBUG, ( valor ? "TRUE" : "FALSE" ) );
    blDebug ( "END PluginBc_Debug::cambia", 0 );
}



///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    blDebug ( "Entrada del plugin Corrector", 10 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_depuracion", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    PluginBc_Debug *corr = new PluginBc_Debug();

    QMenu *pPluginMenu = bcont->newMenu(_("&Ver"), "menuVer", "menuMaestro");

    /// A&ntilde;ade en el men&uacute; del programa la opci&oacuteMn para
    /// acceder al corrector.
    QAction *viewCorrector = new QAction ( _ ( "&Modo debug" ), 0 );
    viewCorrector->setCheckable ( TRUE );

    if ( g_confpr->value( CONF_DEBUG ) == "TRUE" ) {
        viewCorrector->setChecked ( TRUE );
    } else {
        viewCorrector->setChecked ( FALSE );
    }

    viewCorrector->setStatusTip ( _ ( "Activa/Desactiva el modo debug" ) );
    viewCorrector->setWhatsThis ( _ ( "Debug.\n\nActiva/Desactiva el modo debug" ) );
    QObject::connect ( viewCorrector, SIGNAL ( toggled ( bool ) ), corr, SLOT ( cambia ( bool ) ) );
    pPluginMenu ->addSeparator();
    pPluginMenu ->addAction ( viewCorrector );
    blDebug ( "Iniciado correctamente el plugin Corrector", 10 );
    return 0;
}

