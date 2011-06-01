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
BfBulmaFact *g_bges = NULL;


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de entrada de PluginBf_ProveedorPedido\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_proveedorpedido", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    g_bges = bges;
    if ( bges->company()->hasTablePrivilege ( "pedidoproveedor", "SELECT" ) ) {

        /// Miramos si existe un menu Compras
        QMenu *pPluginMenu = bges->newMenu ( _("&Compras"), "menuCompras", "menuMaestro" );
        pPluginMenu->addSeparator();

        BlAction *accionA = new BlAction ( _ ( "&Pedidos a proveedores" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier-purchase-order-list.png" ) ) );
        accionA->setStatusTip ( _ ( "Pedidos a proveedores" ) );
        accionA->setWhatsThis ( _ ( "Pedidos a proveedores" ) );
        accionA->setObjectName("mui_actionProveedoresPedidos");
        pPluginMenu->addAction ( accionA );
        bges->Listados->addAction ( accionA );

        BlAction *accionB = new BlAction ( _ ( "&Nuevo pedido a proveedor" ), 0 );
        accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier-purchase-order.png" ) ) );
        accionB->setStatusTip ( _ ( "Nuevo pedido a proveedor" ) );
        accionB->setWhatsThis ( _ ( "Nuevo pedido a proveedor" ) );
        accionB->setObjectName("mui_actionProveedorPedidoNuevo");
        pPluginMenu->addAction ( accionB );
        bges->Fichas->addAction ( accionB );


    } // end if
    
    return 0;
}

int BlAction_triggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionProveedoresPedidos") {
        blDebug ( "PluginBf_ProveedorPedido::BlAction_triggered::mui_actionProveedoresPedidos", 0 );
        if ( g_pedidosProveedorList ) {
            g_pedidosProveedorList->hide();
            g_pedidosProveedorList->show();
        } // end if
        blDebug ( "END PluginBf_ProveedorPedido::BlAction_triggered::mui_actionProveedoresPedidos", 0 );
    } // end if
    if (accion->objectName() == "mui_actionProveedorPedidoNuevo") {
        blDebug ( "PluginBf_ProveedorPedido::BlAction_triggered::mui_actionProveedorPedidoNuevo", 0 );
        PedidoProveedorView * bud = new PedidoProveedorView ( ( BfCompany * ) g_bges->company(), NULL );
        g_bges->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
        blDebug ( "PluginBf_ProveedorPedido::BlAction_triggered::mui_actionProveedorPedidoNuevo", 0 );
    } // end if
    return 0;
}

int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "pedidoproveedor", "SELECT" ) ) {
        g_pedidosProveedorList = new PedidosProveedorList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_pedidosProveedorList );
        g_pedidosProveedorList->hide();
    } // end if
    return 0;
}


int ProveedorView_ProveedorView_Post ( ProveedorView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "pedidoproveedor", "SELECT" ) ) {
        PedidosProveedorList *pedidosProveedorList = new PedidosProveedorList ( ( BfCompany * ) prov->mainCompany(), NULL, 0, BL_SELECT_MODE );
        pedidosProveedorList->setEditMode();
        pedidosProveedorList->setObjectName ( "listPedidosProveedorList" );
        pedidosProveedorList->hideBusqueda();
        prov->mui_tab->addTab ( pedidosProveedorList, "Pedidos" );
    } // end if
    return 0;
}

int ProveedorView_cargarPost_Post ( ProveedorView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "pedidoproveedor", "SELECT" ) ) {
        PedidosProveedorList *pedidosProveedorList = prov->findChild<PedidosProveedorList *> ( "listPedidosProveedorList" );
        pedidosProveedorList->setidproveedor ( prov->dbValue ( "idproveedor" ) );
        pedidosProveedorList->presentar();
    } // end if
    return 0;
} // end if


int BfBuscarReferencia_on_mui_abrirtodo_clicked_Post ( BfBuscarReferencia *ref )
{
    QString SQLQuery = "SELECT * FROM pedidoproveedor WHERE refpedidoproveedor = '" + ref->mui_referencia->text() + "'";
    BlDbRecordSet *cur = ref->mainCompany() ->loadQuery ( SQLQuery );
    while ( !cur->eof() ) {
        PedidoProveedorView * bud = new PedidoProveedorView ( ( BfCompany * ) ref->mainCompany(), NULL );
        ref->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
        bud->load ( cur->value( "idpedidoproveedor" ) );
        bud->show();
        cur->nextRecord();
    } // end while
    delete cur;
    return 0;
} // end if

