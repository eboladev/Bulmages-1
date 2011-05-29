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

#include "pluginbf_compraventa.h"
#include "compraventaview.h"
#include "bfcompany.h"
#include "blfunctions.h"


///
/**
**/
PluginBf_CompraVenta::PluginBf_CompraVenta()
{
    blDebug ( "PluginBf_CompraVenta::PluginBf_CompraVenta", 0 );
    blDebug ( "END PluginBf_CompraVenta::PluginBf_CompraVenta", 0 );
}


///
/**
**/
PluginBf_CompraVenta::~PluginBf_CompraVenta()
{
    blDebug ( "PluginBf_CompraVenta::~PluginBf_CompraVenta", 0 );
    blDebug ( "END PluginBf_CompraVenta::~PluginBf_CompraVenta", 0 );
}


///
/**
**/
void PluginBf_CompraVenta::elslot()
{
    blDebug ( "PluginBf_CompraVenta::elslot", 0 );

    CompraVentaView *tar = new CompraVentaView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( tar );
    tar->show();

    blDebug ( "END PluginBf_CompraVenta::elslot", 0 );
}


///
/**
\param bges
**/
void PluginBf_CompraVenta::inicializa ( BfBulmaFact *bges )
{
    blDebug ( "PluginBf_CompraVenta::inicializa", 0 );
    /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
    m_bges = bges;
    setMainCompany ( bges->company() );

    /// Miramos si existe un menu Ventas
    QMenu *pPluginMenu = bges->newMenu ( "&Ventas", "menuVentas", "menuMaestro" );

    QAction *planCuentas = new QAction ( _ ( "&CompraVenta" ), 0 );
    planCuentas->setStatusTip ( _ ( "CompraVenta" ) );
    planCuentas->setWhatsThis ( _ ( "CompraVenta" ) );
    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( planCuentas );
    bges->Listados->addAction ( planCuentas );
    connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    blDebug ( "END PluginBf_CompraVenta::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de Entrada del plugin de CompraVenta\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_compraventa", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    PluginBf_CompraVenta *plug = new PluginBf_CompraVenta();
    plug->inicializa ( bges );
    return 0;
}

