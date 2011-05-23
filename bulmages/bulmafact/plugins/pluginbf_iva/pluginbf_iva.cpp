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

#include "pluginbf_iva.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "listtipoivaview.h"
#include "listtasaivaview.h"


BfBulmaFact *g_bges = NULL;



///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de Entrada del plugin de IVA\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_iva", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
    g_bges = bges;


    if ( bges->company()->hasTablePrivilege ( "tipo_iva", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->menuMaestro;
        pPluginMenu->addSeparator();

        /// El men&uacute; de Tipos de IVA en la secci&oacute;n de art&iacute;culos.
        
        BlAction *accionA = new BlAction ( _ ( "&Tipos de IVA" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/vat.png" ) ) );
        accionA->setStatusTip ( _ ( "Tipos de IVA" ) );
        accionA->setWhatsThis ( _ ( "Tipos de IVA" ) );
        accionA->setObjectName("mui_actionIVATipos");
        pPluginMenu->addAction ( accionA );
        bges->Listados->addAction ( accionA );


        /// El men&uacute; de Tasa de IVA en la secci&oacute;n de art&iacute;culos.
        BlAction *accionB = new BlAction ( _ ( "&Tasas de IVA" ), 0 );
        accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/vattax.png" ) ) );
        accionB->setStatusTip ( _ ( "Tasas de IVA" ) );
        accionB->setWhatsThis ( _ ( "Tasas de IVA" ) );
        accionB->setObjectName("mui_actionIVATasas");
        pPluginMenu->addAction ( accionB );
        bges->Listados->addAction ( accionB );

    } // end if
    return 0;
}

int BlAction_triggered(BlAction *accion) {
     if (accion->objectName() == "mui_actionIVATipos") {
         blDebug ( "PluginBf_IVA::BlAction_triggered::mui_actionIVATipos", 0 );
         ListTipoIVAView *pag = new ListTipoIVAView ( ( BfCompany * ) g_bges->company(), NULL );
         g_bges->company()->m_pWorkspace->addSubWindow ( pag );
         pag->show();
         blDebug ( "END PluginBf_IVA::BlAction_triggered::mui_actionIVATipos", 0 );
    }
     if (accion->objectName() == "mui_actionIVATasas") {
         blDebug ( "PluginBf_IVA::PluginBf_IVA::BlAction_triggered::mui_actionIVATasas", 0 );
         ListTasaIVAView *pag = new ListTasaIVAView ( ( BfCompany * ) g_bges->company(), NULL );
         g_bges->company()->m_pWorkspace->addSubWindow ( pag );
         pag->show();
         blDebug ( "END PluginBf_IVA::PluginBf_IVA::BlAction_triggered::mui_actionIVATasas", 0 );
    }
    return 0;
}
