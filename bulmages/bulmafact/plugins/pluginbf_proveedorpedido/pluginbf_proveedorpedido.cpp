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

#include "pluginbf_proveedorpedido.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "pedidoproveedorview.h"
#include "pedidosproveedorlist.h"



PedidosProveedorList *g_pedidosProveedorList = NULL;

///
/**
**/
mypluginpedp::mypluginpedp()
{
    blDebug ( "mypluginpedp::mypluginpedp", 0 );
    blDebug ( "END mypluginpedp::mypluginpedp", 0 );
}


///
/**
**/
mypluginpedp::~mypluginpedp()
{
    blDebug ( "mypluginpedp::~mypluginpedp", 0 );
    blDebug ( "END mypluginpedp::~mypluginpedp", 0 );
}


///
/**
**/
void mypluginpedp::elslot()
{
    blDebug ( "mypluginpedp::elslot", 0 );
    if ( g_pedidosProveedorList ) {
        g_pedidosProveedorList->hide();
        g_pedidosProveedorList->show();
    }// end if
    blDebug ( "END mypluginpedp::elslot", 0 );
}

///
/**
**/
void mypluginpedp::elslot1()
{
    blDebug ( "mypluginpedp::elslot1", 0 );
    PedidoProveedorView * bud = new PedidoProveedorView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->show();
    blDebug ( "END mypluginpedp::elslot1", 0 );
}



///
/**
\param bges
**/
void mypluginpedp::inicializa ( BfBulmaFact *bges )
{
    blDebug ( "mypluginpedp::inicializa", 0 );

    if ( bges->company()->hasTablePrivilege ( "pedidoproveedor", "SELECT" ) ) {

        /// Miramos si existe un menu Compras
        QMenu *pPluginMenu = bges->newMenu ( "&Compras", "menuCompras", "menuMaestro" );
        pPluginMenu->addSeparator();

        /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
        m_bges = bges;
        setMainCompany ( bges->company() );
        QAction *planCuentas = new QAction ( _ ( "&Pedidos a proveedores" ), 0 );
        planCuentas->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier-purchase-order-list.png" ) ) );
        planCuentas->setStatusTip ( _ ( "Pedidos a proveedores" ) );
        planCuentas->setWhatsThis ( _ ( "Pedidos a proveedores" ) );
        pPluginMenu->addAction ( planCuentas );
        bges->Listados->addAction ( planCuentas );
        connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

        QAction *npago = new QAction ( _ ( "&Nuevo pedido a proveedor" ), 0 );
        npago->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier-purchase-order.png" ) ) );
        npago->setStatusTip ( _ ( "Nuevo pedido a proveedor" ) );
        npago->setWhatsThis ( _ ( "Nuevo pedido a proveedor" ) );
        pPluginMenu->addAction ( npago );
        bges->Fichas->addAction ( npago );
        connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );


    }// end if
    blDebug ( "END mypluginpedp::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de Entrada del plugin de pedidos a proveedor\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_proveedorpedido", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    mypluginpedp *plug = new mypluginpedp();
    plug->inicializa ( bges );
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "pedidoproveedor", "SELECT" ) ) {
        g_pedidosProveedorList = new PedidosProveedorList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_pedidosProveedorList );
        g_pedidosProveedorList->hide();
    }// end if
    return 0;
}


int ProveedorView_ProveedorView_Post ( ProveedorView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "pedidoproveedor", "SELECT" ) ) {
        PedidosProveedorList *pedidosProveedorList = new PedidosProveedorList ( ( BfCompany * ) prov->mainCompany(), NULL, 0, BL_SELECT_MODE );
        pedidosProveedorList->setModoEdicion();
        pedidosProveedorList->setObjectName ( "listPedidosProveedorList" );
        pedidosProveedorList->hideBusqueda();
        prov->mui_tab->addTab ( pedidosProveedorList, "Pedidos" );
    }// end if
    return 0;
}

int ProveedorView_cargarPost_Post ( ProveedorView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "pedidoproveedor", "SELECT" ) ) {
        PedidosProveedorList *pedidosProveedorList = prov->findChild<PedidosProveedorList *> ( "listPedidosProveedorList" );
        pedidosProveedorList->setidproveedor ( prov->dbValue ( "idproveedor" ) );
        pedidosProveedorList->presentar();
    }// end if
    return 0;
}// end if


int BfBuscarReferencia_on_mui_abrirtodo_clicked_Post ( BfBuscarReferencia *ref )
{
    QString SQLQuery = "SELECT * FROM pedidoproveedor WHERE refpedidoproveedor = '" + ref->mui_referencia->text() + "'";
    BlDbRecordSet *cur = ref->mainCompany() ->loadQuery ( SQLQuery );
    while ( !cur->eof() ) {
        PedidoProveedorView * bud = new PedidoProveedorView ( ( BfCompany * ) ref->mainCompany(), NULL );
        ref->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
        bud->cargar ( cur->valor ( "idpedidoproveedor" ) );
        bud->show();
        cur->nextRecord();
    } // end while
    delete cur;
    return 0;
}// end if

