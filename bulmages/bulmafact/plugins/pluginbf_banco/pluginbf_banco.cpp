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

#include "pluginbf_banco.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "bancoview.h"

BfBulmaFact *g_pluginbf_banco = NULL;

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
    blBindTextDomain ( "pluginbf_banco", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );


    if ( bges->company()->hasTablePrivilege ( "banco", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->menuMaestro;
        pPluginMenu->addSeparator();

        /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
        g_pluginbf_banco = bges;
        BlAction *accionA = new BlAction ( _ ( "&Bancos" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/bank.png" ) ) );
        accionA->setStatusTip ( _ ( "Bancos" ) );
        accionA->setWhatsThis ( _ ( "Bancos" ) );
        accionA->setObjectName("mui_actionBancos");
        
        pPluginMenu->addAction ( accionA );
        bges->Listados->addAction ( accionA );

    } // end if
    return 0;
}

int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    if (accion->objectName() == "mui_actionBancos") {
        BancoView * bud = new BancoView ( g_pluginbf_banco->company(), NULL );
        g_pluginbf_banco->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if
    
	return 0;
}
 


/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewBancoView ( BfCompany *v )
{
    BancoView *h = new BancoView ( v, 0 );
    g_plugParams = h;
    return 1;
}
