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

#include "pluginpresupuesto.h"
#include "plugins.h"
#include "company.h"
#include "funcaux.h"
#include "presupuestoview.h"
#include "presupuestolist.h"

#include "genpreqtoolbutton.h"


PresupuestoList *g_presupuestosList=NULL;

///
/**
**/
mypluginpres::mypluginpres()
{
    _depura ( "mypluginpres::mypluginpres", 0 );
    _depura ( "END mypluginpres::mypluginpres", 0 );
}


///
/**
**/
mypluginpres::~mypluginpres()
{
    _depura ( "mypluginpres::~mypluginpres", 0 );
    _depura ( "END mypluginpres::~mypluginpres", 0 );
}


///
/**
**/
void mypluginpres::elslot()
{
    _depura ( "mypluginpres::elslot", 0 );
    if (g_presupuestosList) {
	g_presupuestosList->hide();
	g_presupuestosList->show();
    }// end if
    _depura ( "END mypluginpres::elslot", 0 );
}

///
/**
**/
void mypluginpres::elslot1()
{
    _depura ( "mypluginpres::elslot1", 0 );
        PresupuestoView * bud = new PresupuestoView((Company *)empresaBase(), NULL);
        empresaBase() ->m_pWorkspace->addWindow ( bud );
        bud->show();
    _depura ( "END mypluginpres::elslot1", 0 );
}



///
/**
\param bges
**/
void mypluginpres::inicializa ( Bulmafact *bges )
{
    _depura ( "mypluginpres::inicializa", 0 );

    if ( bges->getcompany()->has_table_privilege ( "presupuesto", "SELECT" ) ) {


    /// Miramos si existe un menu Ventas
	QMenu *pPluginMenu = bges->newMenu("&Ventas", "menuVentas", "menuMaestro");

	/// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
	m_bges = bges;
	setEmpresaBase ( bges->getcompany() );
	QAction *planCuentas = new QAction ( tr ( "&Presupuestos a Clientes" ), 0 );
	planCuentas->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/client-quote-list.svg" ) ));
	planCuentas->setStatusTip ( tr ( "Presupuestos a Clientes" ) );
	planCuentas->setWhatsThis ( tr ( "Presupuestos a Clientes" ) );
	pPluginMenu->addAction ( planCuentas );
	bges->Listados->addAction (planCuentas);
	connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

	QAction *npago = new QAction ( tr ( "&Nuevo Presupuesto a Cliente" ), 0 );
	npago->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/client-quote.svg" ) ));
	npago->setStatusTip ( tr ( "Nuevo Presupuesto a Cliente" ) );
	npago->setWhatsThis ( tr ( "Nuevo Presupuesto a Cliente" ) );
	pPluginMenu->addAction ( npago );
	bges->Fichas->addAction (npago);
	connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );


    }// end if
    _depura ( "END mypluginpres::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Punto de Entrada del plugin de Presupuestos a Clientes\n", 0 );
    mypluginpres *plug = new mypluginpres();
    plug->inicializa ( bges );
    return 0;
}


int Company_createMainWindows_Post(Company *comp) {
    if ( comp->has_table_privilege ( "presupuesto", "SELECT" ) ) {
	g_presupuestosList = new PresupuestoList( comp, NULL );	
	comp->m_pWorkspace->addWindow ( g_presupuestosList );
	g_presupuestosList->hide();
    }// end if
    return 0;
}


int ClienteView_ClienteView_Post (ClienteView *prov) {
    if ( prov->empresaBase()->has_table_privilege ( "presupuesto", "SELECT" ) ) {
	PresupuestoList *presupuestosList = new PresupuestoList( (Company *)prov->empresaBase(), NULL, 0, Listado::SelectMode );
	presupuestosList->setModoEdicion();
	presupuestosList->setObjectName("listpresupuestos");
	presupuestosList->hideBusqueda();
        prov->mui_tab->addTab ( presupuestosList, "Presupuestos" );
    }// end if
    return 0;
}

int ClienteView_cargarPost_Post (ClienteView *prov) {
    if ( prov->empresaBase()->has_table_privilege ( "presupuesto", "SELECT" ) ) {
	PresupuestoList *presupuestosList = prov->findChild<PresupuestoList *> ( "listpresupuestos" );
        presupuestosList->setidcliente ( prov->DBvalue ( "idcliente" ) );
        presupuestosList->presentar();
    }// end if
    return 0;
}// end if


int BusquedaReferencia_on_mui_abrirtodo_clicked_Post (BusquedaReferencia *ref) {
    QString SQLQuery = "SELECT * FROM presupuesto WHERE refpresupuesto = '" + ref->mui_referencia->text() + "'";
    cursor2 *cur = ref->empresaBase() ->cargacursor ( SQLQuery );
    while ( !cur->eof() ) {
        PresupuestoView * bud = new PresupuestoView((Company *)ref->empresaBase(), NULL);
        ref->empresaBase() ->m_pWorkspace->addWindow ( bud );
        bud->cargar ( cur->valor ( "idpresupuesto" ) );
        bud->show();
        cur->siguienteregistro();
    } // end while
    delete cur;
}// end if




///
/**
\param l
\return
**/
int PedidoClienteView_PedidoClienteView ( PedidoClienteView *l )
{

    _depura ( "PluginPresupuesto_PedidoClienteView_PedidoClienteView", 0 );

    VerPreQToolButton *mui_exporta_epresupuesto2 = new VerPreQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

	if (!m_hboxLayout1) {
		m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
		m_hboxLayout1->setSpacing ( 5 );
		m_hboxLayout1->setMargin ( 5 );
		m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
	}// end if
    m_hboxLayout1->addWidget ( mui_exporta_epresupuesto2 );

    _depura ( "END PluginPresupuesto_PedidoClienteView_PedidoClienteView", 0 );

    return 0;
}




/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewPresupuestoView (Company *v)
{
	PresupuestoView *h = new PresupuestoView(v, 0);
	g_plugParams = h;
	return 1;
}

