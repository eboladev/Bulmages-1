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

BfBulmaFact *g_bges = NULL;

///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de Entrada del plugin de PluginBf_Trabajador\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_trabajador", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    g_bges = bges;


    if ( bges->company()->hasTablePrivilege ( "trabajador", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->menuMaestro;
        pPluginMenu->addSeparator();

        BlAction *accionA = new BlAction ( _ ( "&Trabajadores" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/employee-list.png" ) ) );
        accionA->setStatusTip ( _ ( "Trabajadores" ) );
        accionA->setWhatsThis ( _ ( "Trabajadores" ) );
        accionA->setObjectName("mui_actionTrabajadores");
        pPluginMenu->addAction ( accionA );
        bges->Listados->addAction ( accionA );

    } // end if
    blDebug ( ("END ", Q_FUNC_INFO), 0 );

    return 0;
}


int BlAction_triggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionTrabajadores") {
        blDebug ( "PluginBf_Trabajador::BlAction_triggered::mui_actionTrabajadores", 0 );
        TrabajadorView * bud = new TrabajadorView ( ( BfCompany * ) g_bges->company(), NULL );
        g_bges->company() ->m_pWorkspace->addSubWindow ( bud );
        bud->show();
        blDebug ( ("END ", Q_FUNC_INFO), 0 );
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
