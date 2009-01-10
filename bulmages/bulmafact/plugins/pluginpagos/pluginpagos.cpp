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

#include "pluginpagos.h"
#include "company.h"
#include "funcaux.h"
#include "pagoview.h"
#include "pagoslist.h"
#include "impqtoolbutton.h"


PagosList *g_pagosList=NULL;

///
/**
**/
mypluginpag::mypluginpag()
{
    _depura ( "mypluginpag::mypluginpag", 0 );
    _depura ( "END mypluginpag::mypluginpag", 0 );
}


///
/**
**/
mypluginpag::~mypluginpag()
{
    _depura ( "mypluginpag::~mypluginpag", 0 );
    _depura ( "END mypluginpag::~mypluginpag", 0 );
}


///
/**
**/
void mypluginpag::elslot()
{
    _depura ( "mypluginpag::elslot", 0 );
    if (g_pagosList) {
	g_pagosList->hide();
	g_pagosList->show();
    }// end if
    _depura ( "END mypluginpag::elslot", 0 );
}

///
/**
**/
void mypluginpag::elslot1()
{
    _depura ( "mypluginpag::elslot1", 0 );
        PagoView * bud = new PagoView((Company *)empresaBase(), NULL);
        empresaBase() ->m_pWorkspace->addWindow ( bud );
        bud->show();
    _depura ( "END mypluginpag::elslot1", 0 );
}



///
/**
\param bges
**/
void mypluginpag::inicializa ( Bulmafact *bges )
{
    _depura ( "mypluginpag::inicializa", 0 );

    if ( bges->getcompany()->has_table_privilege ( "pago", "SELECT" ) ) {
    /// Miramos si existe un menu Compras
    QMenu *pPluginMenu = NULL;
    pPluginMenu = bges->menuBar() ->findChild<QMenu *> ( "menuCompras" );
    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
    	QMenu *pPluginMaestro = NULL;
	    pPluginMaestro = bges->menuBar() ->findChild<QMenu *> ( "menuMaestro" );
        pPluginMenu = new QMenu ( "&Compras", bges->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuCompras" ));
		bges->menuBar()->insertMenu(pPluginMaestro->menuAction(), pPluginMenu);
    } // end if
	/// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
	m_bges = bges;
	setEmpresaBase ( bges->getcompany() );
	QAction *planCuentas = new QAction ( tr ( "&Pagos a Proveedores" ), 0 );
	planCuentas->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/pay-list.svg" ) ));
	planCuentas->setStatusTip ( tr ( "Pagos a Proveedores" ) );
	planCuentas->setWhatsThis ( tr ( "Pagos a Proveedores" ) );
	pPluginMenu->addAction ( planCuentas );
	bges->Listados->addAction (planCuentas);
	connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

	QAction *npago = new QAction ( tr ( "&Nuevo Pago a Proveedor" ), 0 );
	npago->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/pay.svg" ) ));
	npago->setStatusTip ( tr ( "Nuevo Pago a Proveedor" ) );
	npago->setWhatsThis ( tr ( "Nuevo Pago a Proveedor" ) );
	pPluginMenu->addAction ( npago );
	bges->Fichas->addAction (npago);
	connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );


    }// end if
    _depura ( "END mypluginpag::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Punto de Entrada del plugin de Tarifas\n", 0 );
    mypluginpag *plug = new mypluginpag();
    plug->inicializa ( bges );
    return 0;
}


int Company_createMainWindows_Post(Company *comp) {
    if ( comp->has_table_privilege ( "pago", "SELECT" ) ) {
	g_pagosList = new PagosList( comp, NULL );	
	comp->m_pWorkspace->addWindow ( g_pagosList );
	g_pagosList->hide();
    }// end if
    return 0;
}


int ProveedorView_ProveedorView_Post (ProveedorView *prov) {
    if ( prov->empresaBase()->has_table_privilege ( "pago", "SELECT" ) ) {
	PagosList *pagosList = new PagosList( (Company *)prov->empresaBase(), NULL );
	pagosList->setObjectName("listpagosproveedor");
	pagosList->hideBusqueda();
        prov->mui_tab->addTab ( pagosList, "Pagos" );
    }// end if
    return 0;
}

int ProveedorView_cargarPost_Post (ProveedorView *prov) {
    if ( prov->empresaBase()->has_table_privilege ( "pago", "SELECT" ) ) {
	PagosList *pagosList = prov->findChild<PagosList *> ( "listpagosproveedor" );
        pagosList->setidproveedor ( prov->DBvalue ( "idproveedor" ) );
        pagosList->presentar();
    }// end if
    return 0;
}// end if


int BusquedaReferencia_on_mui_abrirtodo_clicked_Post (BusquedaReferencia *ref) {
    QString SQLQuery = "SELECT * FROM pago WHERE refpago = '" + ref->mui_referencia->text() + "'";
    cursor2 *cur = ref->empresaBase() ->cargacursor ( SQLQuery );
    while ( !cur->eof() ) {
        PagoView * bud = new PagoView((Company *)ref->empresaBase(), NULL);
        ref->empresaBase() ->m_pWorkspace->addWindow ( bud );
        bud->cargar ( cur->valor ( "idpago" ) );
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
int FacturaProveedorView_FacturaProveedorView ( FacturaProveedorView *l )
{
    _depura ( "PluginPagos_FacturaProveedorView_FacturaProveedorView", 0 );
    GenPagoQToolButton *mui_exporta_efactura2 = new GenPagoQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

	if (!m_hboxLayout1) {
		m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
		m_hboxLayout1->setSpacing ( 5 );
		m_hboxLayout1->setMargin ( 5 );
		m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
	}// end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );
    _depura ( "END PluginPagos_FacturaProveedorView_FacturaProveedorView", 0 );
    return 0;
}

///
/**
\param l
\return
**/
int AlbaranProveedorView_AlbaranProveedorView ( AlbaranProveedorView *l )
{
    _depura ( "PluginPagos_AlbaranProveedorView_AlbaranProveedorView", 0 );
    GenPagoQToolButton *mui_exporta_efactura2 = new GenPagoQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

	if (!m_hboxLayout1) {
		m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
		m_hboxLayout1->setSpacing ( 5 );
		m_hboxLayout1->setMargin ( 5 );
		m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
	}// end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );
    _depura ( "END PluginPagos_AlbaranProveedorView_AlbaranProveedorView", 0 );
    return 0;
}

///
/**
\param l
\return
**/
int PedidoProveedorView_PedidoProveedorView ( PedidoProveedorView *l )
{
    _depura ( "PluginPagos_PedidoProveedorView_PedidoProveedorView", 0 );
    GenPagoQToolButton *mui_exporta_efactura2 = new GenPagoQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

	if (!m_hboxLayout1) {
		m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
		m_hboxLayout1->setSpacing ( 5 );
		m_hboxLayout1->setMargin ( 5 );
		m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
	}// end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );
    _depura ( "END PluginPagos_PedidoProveedorView_PedidoProveedorView", 0 );
    return 0;
}

