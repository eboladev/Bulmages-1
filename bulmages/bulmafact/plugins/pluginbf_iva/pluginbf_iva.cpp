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


BfBulmaFact *g_pluginbf_iva = NULL;



///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_iva", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    g_pluginbf_iva = bges;


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

int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, _("PluginBf_IVA") );

    if (accion->objectName() == "mui_actionIVATipos") {
         ListTipoIVAView *pag = new ListTipoIVAView ( g_pluginbf_iva->company(), NULL );
         g_pluginbf_iva->company()->m_pWorkspace->addSubWindow ( pag );
         pag->show();
    } // end if

    if (accion->objectName() == "mui_actionIVATasas") {
         ListTasaIVAView *pag = new ListTasaIVAView ( g_pluginbf_iva->company(), NULL );
         g_pluginbf_iva->company()->m_pWorkspace->addSubWindow ( pag );
         pag->show();
    } // end if

    
    return 0;
}
