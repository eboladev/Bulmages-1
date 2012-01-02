/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include <QMenu>
#include <QAction>
#include <QObject>
#include <QMessageBox>

#include <stdio.h>

#include "pluginbc_example.h"
#include "bccompany.h"
#include "blfunctions.h"

BcBulmaCont *g_pluginbc_example = NULL;


///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_example", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    g_pluginbc_example = bcont;

    /// Creamos el men&uacute;.
    QMenu *pPluginMenu = bcont->newMenu ( _ ( "&Plugin" ), "menuPlugin", "menuHerramientas" );
    BlAction *accionA = new BlAction ( _ ( "&Prueba de plugin BulmaCont" ), 0 );
    accionA->setStatusTip ( _ ( "Muestra statustip" ) );
    accionA->setWhatsThis ( _ ( "Muestra que es esto" ) );
    accionA->setObjectName("mui_actionExample");
    pPluginMenu->addAction ( accionA );

    /// S&Oacute;LO A MODO DE EJEMPLO: se modifica el t&iacute;tulo de la ventana principal
    /// del programa para indicar que el plugin se ha cargado.
    bcont->setWindowTitle ( _ ( "Prueba de plugin BulmaCont." ) );

    
    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, _("PluginBc_Example") );

    if (accion->objectName() == "mui_actionExample") {
        QMessageBox::warning ( 0,
                _( "Titulo de la ventana" ),
                _( "Mensaje." ),
                QMessageBox::Ok,
                QMessageBox::Cancel );
    } // end if

    
    return 0;
}

