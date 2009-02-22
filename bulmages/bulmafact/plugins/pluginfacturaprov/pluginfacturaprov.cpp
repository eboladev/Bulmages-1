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

#include "pluginfacturaprov.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "facturapview.h"
#include "facturasplist.h"
#include "genfactqtoolbutton.h"


FacturasProveedorList *g_facturasProveedorList=NULL;

///
/**
**/
mypluginfactp::mypluginfactp()
{
    _depura ( "mypluginfactp::mypluginfactp", 0 );
    _depura ( "END mypluginfactp::mypluginfactp", 0 );
}


///
/**
**/
mypluginfactp::~mypluginfactp()
{
    _depura ( "mypluginfactp::~mypluginfactp", 0 );
    _depura ( "END mypluginfactp::~mypluginfactp", 0 );
}


///
/**
**/
void mypluginfactp::elslot()
{
    _depura ( "mypluginfactp::elslot", 0 );
    if (g_facturasProveedorList) {
	g_facturasProveedorList->hide();
	g_facturasProveedorList->show();
    }// end if
    _depura ( "END mypluginfactp::elslot", 0 );
}

///
/**
**/
void mypluginfactp::elslot1()
{
    _depura ( "mypluginfactp::elslot1", 0 );
        FacturaProveedorView * bud = new FacturaProveedorView((BfCompany *)mainCompany(), NULL);
        mainCompany() ->m_pWorkspace->addWindow ( bud );
        bud->show();
    _depura ( "END mypluginfactp::elslot1", 0 );
}



///
/**
\param bges
**/
void mypluginfactp::inicializa ( Bulmafact *bges )
{
    _depura ( "mypluginfactp::inicializa", 0 );

    if ( bges->getcompany()->hasTablePrivilege ( "facturap", "SELECT" ) ) {
    /// Miramos si existe un menu Compras
	QMenu *pPluginMenu = bges->newMenu("&Compras", "menuCompras", "menuMaestro");
	pPluginMenu->addSeparator();

	/// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
	m_bges = bges;
	setMainCompany ( bges->getcompany() );
	QAction *planCuentas = new QAction ( _( "&Facturas proveedores" ), 0 );
	planCuentas->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/supplier-invoice-list.svg" ) ));
	planCuentas->setStatusTip ( _( "Facturas de proveedores" ) );
	planCuentas->setWhatsThis ( _( "Facturas de proveedores" ) );
	pPluginMenu->addAction ( planCuentas );
	bges->Listados->addAction (planCuentas);
	connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

	QAction *npago = new QAction ( _( "&Nueva factura de proveedor" ), 0 );
	npago->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/supplier-invoice.svg" ) ));
	npago->setStatusTip ( _( "Nueva factura de proveedor" ) );
	npago->setWhatsThis ( _( "Nueva factura de proveedor" ) );
	pPluginMenu->addAction ( npago );
	bges->Fichas->addAction (npago);
	connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );


    }// end if
    _depura ( "END mypluginfactp::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Punto de Entrada del plugin de Facturas de Proveedor\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginfacturaprov", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    mypluginfactp *plug = new mypluginfactp();
    plug->inicializa ( bges );
    return 0;
}


int BfCompany_createMainWindows_Post(BfCompany *comp) {
    if ( comp->hasTablePrivilege ( "facturap", "SELECT" ) ) {
	g_facturasProveedorList = new FacturasProveedorList( comp, NULL );	
	comp->m_pWorkspace->addWindow ( g_facturasProveedorList );
	g_facturasProveedorList->hide();
    }// end if
    return 0;
}


int ProveedorView_ProveedorView_Post (ProveedorView *prov) {
    if ( prov->mainCompany()->hasTablePrivilege ( "facturap", "SELECT" ) ) {
	FacturasProveedorList *facturasProveedorList = new FacturasProveedorList( (BfCompany *)prov->mainCompany(), NULL, 0, BlFormList::SelectMode );
	facturasProveedorList->setModoEdicion();
	facturasProveedorList->setObjectName("listpagosproveedor");
	facturasProveedorList->hideBusqueda();
        prov->mui_tab->addTab ( facturasProveedorList, "Facturas" );
    }// end if
    return 0;
}

int ProveedorView_cargarPost_Post (ProveedorView *prov) {
    if ( prov->mainCompany()->hasTablePrivilege ( "facturap", "SELECT" ) ) {
	FacturasProveedorList *facturasProveedorList = prov->findChild<FacturasProveedorList *> ( "listpagosproveedor" );
        facturasProveedorList->setidproveedor ( prov->DBvalue ( "idproveedor" ) );
        facturasProveedorList->presentar();
    }// end if
    return 0;
}// end if


int BusquedaReferencia_on_mui_abrirtodo_clicked_Post (BusquedaReferencia *ref) {
    QString SQLQuery = "SELECT * FROM facturap WHERE reffacturap = '" + ref->mui_referencia->text() + "'";
    BlDbRecordSet *cur = ref->mainCompany() ->loadQuery ( SQLQuery );
    while ( !cur->eof() ) {
        FacturaProveedorView * bud = new FacturaProveedorView((BfCompany *)ref->mainCompany(), NULL);
        ref->mainCompany() ->m_pWorkspace->addWindow ( bud );
        bud->cargar ( cur->valor ( "idfacturap" ) );
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
int AlbaranProveedorView_AlbaranProveedorView ( AlbaranProveedorView *l )
{
    _depura ( "PluginPagos_AlbaranProveedorView_AlbaranProveedorView", 0 );
    GenFacProQToolButton *mui_exporta_efactura2 = new GenFacProQToolButton ( l, l->mui_plugbotones );

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

    GenFacProQToolButton *mui_exporta_efactura2 = new GenFacProQToolButton ( l, l->mui_plugbotones );

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

/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewFacturaProveedorView (BfCompany *v)
{
	FacturaProveedorView *h = new FacturaProveedorView(v, 0);
	g_plugParams = h;
	return 1;
}

