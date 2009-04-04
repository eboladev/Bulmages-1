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

#include "pluginbf_clientepedido.h"
#include "blplugins.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "pedidoclienteview.h"
#include "pedidosclientelist.h"

#include "genpedqtoolbutton.h"


PedidosClienteList *g_pedidosClienteList=NULL;

///
/**
**/
mypluginped::mypluginped()
{
    _depura ( "mypluginped::mypluginped", 0 );
    _depura ( "END mypluginped::mypluginped", 0 );
}


///
/**
**/
mypluginped::~mypluginped()
{
    _depura ( "mypluginped::~mypluginped", 0 );
    _depura ( "END mypluginped::~mypluginped", 0 );
}


///
/**
**/
void mypluginped::elslot()
{
    _depura ( "mypluginped::elslot", 0 );
    if (g_pedidosClienteList) {
	g_pedidosClienteList->hide();
	g_pedidosClienteList->show();
    }// end if
    _depura ( "END mypluginped::elslot", 0 );
}

///
/**
**/
void mypluginped::elslot1()
{
    _depura ( "mypluginped::elslot1", 0 );
        PedidoClienteView * bud = new PedidoClienteView((BfCompany *)mainCompany(), NULL);
        mainCompany() ->m_pWorkspace->addWindow ( bud );
        bud->show();
    _depura ( "END mypluginped::elslot1", 0 );
}



///
/**
\param bges
**/
void mypluginped::inicializa ( Bulmafact *bges )
{
    _depura ( "mypluginped::inicializa", 0 );

    if ( bges->getcompany()->hasTablePrivilege ( "pedidocliente", "SELECT" ) ) {

    /// Miramos si existe un menu Ventas
	QMenu *pPluginMenu = bges->newMenu("&Ventas", "menuVentas", "menuMaestro");
	pPluginMenu->addSeparator();

	m_bges = bges;
	setMainCompany ( bges->getcompany() );
	QAction *planCuentas = new QAction ( _( "&Pedidos de clientes" ), 0 );
	planCuentas->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/client-delivery-order-list.svg" ) ));
	planCuentas->setStatusTip ( _( "Pedidos de clientes" ) );
	planCuentas->setWhatsThis ( _( "Pedidos de clientes" ) );
	pPluginMenu->addAction ( planCuentas );
	bges->Listados->addAction (planCuentas);
	connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

	QAction *npago = new QAction ( _( "&Nuevo pedido de cliente" ), 0 );
	npago->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/client-delivery-order.svg" ) ));
	npago->setStatusTip ( _( "Nuevo pedido de cliente" ) );
	npago->setWhatsThis ( _( "Nuevo pedido de cliente" ) );
	pPluginMenu->addAction ( npago );
	pPluginMenu->addAction (npago);
	connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );


    }// end if
    _depura ( "END mypluginped::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Punto de Entrada del plugin de Pedidos de Clientes\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginbf_clientepedido", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    mypluginped *plug = new mypluginped();
    plug->inicializa ( bges );
    return 0;
}


int BfCompany_createMainWindows_Post(BfCompany *comp) {
    if ( comp->hasTablePrivilege ( "pedidocliente", "SELECT" ) ) {
	g_pedidosClienteList = new PedidosClienteList( comp, NULL );	
	comp->m_pWorkspace->addWindow ( g_pedidosClienteList );
	g_pedidosClienteList->hide();
    }// end if
    return 0;
}


int ClienteView_ClienteView_Post (ClienteView *prov) {
    if ( prov->mainCompany()->hasTablePrivilege ( "pedidocliente", "SELECT" ) ) {
	PedidosClienteList *pedidosClienteList = new PedidosClienteList( (BfCompany *)prov->mainCompany(), NULL, 0, BlFormList::SelectMode );
	pedidosClienteList->setModoEdicion();
	pedidosClienteList->setObjectName("listpedidoclientees");
	pedidosClienteList->hideBusqueda();
        prov->mui_tab->addTab ( pedidosClienteList, "Pedidos" );
    }// end if
    return 0;
}

int ClienteView_cargarPost_Post (ClienteView *prov) {
    if ( prov->mainCompany()->hasTablePrivilege ( "pedidocliente", "SELECT" ) ) {
	PedidosClienteList *pedidosClienteList = prov->findChild<PedidosClienteList *> ( "listpedidoclientees" );
        pedidosClienteList->setidcliente ( prov->dbValue ( "idcliente" ) );
        pedidosClienteList->presentar();
    }// end if
    return 0;
}// end if


int BusquedaReferencia_on_mui_abrirtodo_clicked_Post (BusquedaReferencia *ref) {
    QString SQLQuery = "SELECT * FROM pedidocliente WHERE refpedidocliente = '" + ref->mui_referencia->text() + "'";
    BlDbRecordSet *cur = ref->mainCompany() ->loadQuery ( SQLQuery );
    while ( !cur->eof() ) {
        PedidoClienteView * bud = new PedidoClienteView((BfCompany *)ref->mainCompany(), NULL);
        ref->mainCompany() ->m_pWorkspace->addWindow ( bud );
        bud->cargar ( cur->valor ( "idpedidocliente" ) );
        bud->show();
        cur->nextRecord();
    } // end while
    delete cur;
}// end if


///
/**
\param l
\return
**/
int PresupuestoView_PresupuestoView ( PresupuestoView *l )
{

    _depura ( "PluginPedidoCliente_PresupuestoView_PresupuestoView", 0 );

    GenPedQToolButton *mui_exporta_efactura2 = new GenPedQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

	if (!m_hboxLayout1) {
		m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
		m_hboxLayout1->setSpacing ( 5 );
		m_hboxLayout1->setMargin ( 0 );
		m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
	}// end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );

    _depura ( "END PluginPedidoCliente_PresupuestoView_PresupuestoView", 0 );

    return 0;
}

/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewPedidoClienteView (BfCompany *v)
{
	PedidoClienteView *h = new PedidoClienteView(v, 0);
	g_plugParams = h;
	return 1;
}

