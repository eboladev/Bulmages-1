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

#include "pluginbf_trabajador.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "trabajadorview.h"

BfBulmaFact *g_pluginbf_trabajador = NULL;

///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bf )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_trabajador", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    g_pluginbf_trabajador = bf;


    if ( g_pluginbf_trabajador->company()->hasTablePrivilege ( "trabajador", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = g_pluginbf_trabajador->menuMaestro;
        pPluginMenu->addSeparator();

        BlAction *accionA = new BlAction ( _ ( "&Trabajadores" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/employee-list.png" ) ) );
        accionA->setStatusTip ( _ ( "Trabajadores" ) );
        accionA->setWhatsThis ( _ ( "Trabajadores" ) );
        accionA->setObjectName("mui_actionTrabajadores");
        pPluginMenu->addAction ( accionA );
        g_pluginbf_trabajador->Listados->addAction ( accionA );

    } // end if
    

    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, "PluginBf_Trabajador" );

    if (accion->objectName() == "mui_actionTrabajadores") {
        TrabajadorView * bud = new TrabajadorView ( g_pluginbf_trabajador->company(), NULL );
        g_pluginbf_trabajador->company() ->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if

    return 0;
}



/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewTrabajadorView ( BfCompany *v )
{
    TrabajadorView *h = new TrabajadorView ( v, 0 );
    g_plugParams = h;
    return 1;
}
