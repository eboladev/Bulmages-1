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

#include "pluginbf_clientefactura.h"
#include "blplugins.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "facturaview.h"
#include "facturaslist.h"

#include "genfacqtoolbutton.h"


FacturasList *g_facturasList = NULL;

///
/**
**/
mypluginfact::mypluginfact()
{
    _depura ( "mypluginfact::mypluginfact", 0 );
    _depura ( "END mypluginfact::mypluginfact", 0 );
}


///
/**
**/
mypluginfact::~mypluginfact()
{
    _depura ( "mypluginfact::~mypluginfact", 0 );
    _depura ( "END mypluginfact::~mypluginfact", 0 );
}


///
/**
**/
void mypluginfact::elslot()
{
    _depura ( "mypluginfact::elslot", 0 );
    if ( g_facturasList ) {
        g_facturasList->hide();
        g_facturasList->show();
    }// end if
    _depura ( "END mypluginfact::elslot", 0 );
}

///
/**
**/
void mypluginfact::elslot1()
{
    _depura ( "mypluginfact::elslot1", 0 );
    FacturaView * bud = new FacturaView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    _depura ( "END mypluginfact::elslot1", 0 );
}



///
/**
\param bges
**/
void mypluginfact::inicializa ( Bulmafact *bges )
{
    _depura ( "mypluginfact::inicializa", 0 );

    if ( bges->getcompany()->hasTablePrivilege ( "factura", "SELECT" ) ) {
        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( "&Ventas", "menuVentas", "menuMaestro" );
        pPluginMenu->addSeparator();

        /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
        m_bges = bges;
        setMainCompany ( bges->getcompany() );
        QAction *planCuentas = new QAction ( _ ( "&Facturas a clientes" ), 0 );
        planCuentas->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/client-invoice-list.svg" ) ) );
        planCuentas->setStatusTip ( _ ( "Facturas a clientes" ) );
        planCuentas->setWhatsThis ( _ ( "Facturas a clientes" ) );
        pPluginMenu->addAction ( planCuentas );
        bges->Listados->addAction ( planCuentas );
        connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

        QAction *npago = new QAction ( _ ( "&Nueva factura a cliente" ), 0 );
        npago->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/client-invoice.svg" ) ) );
        npago->setStatusTip ( _ ( "Nueva factura a cliente" ) );
        npago->setWhatsThis ( _ ( "Nueva factura a cliente" ) );
        pPluginMenu->addAction ( npago );
        bges->Fichas->addAction ( npago );
        connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );


    }// end if
    _depura ( "END mypluginfact::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Punto de Entrada del plugin de Facturas a Clientes\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_clientefactura", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    mypluginfact *plug = new mypluginfact();
    plug->inicializa ( bges );
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "factura", "SELECT" ) ) {
        g_facturasList = new FacturasList ( comp, NULL );
        comp->m_pWorkspace->addWindow ( g_facturasList );
        g_facturasList->hide();
    }// end if
    return 0;
}


int ClienteView_ClienteView_Post ( ClienteView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "factura", "SELECT" ) ) {
        FacturasList *facturasList = new FacturasList ( ( BfCompany * ) prov->mainCompany(), NULL, 0, BL_SELECT_MODE );
        facturasList->setModoEdicion();
        facturasList->setObjectName ( "listfacturas" );
        facturasList->hideBusqueda();
        prov->mui_tab->addTab ( facturasList, "Facturas" );
    }// end if
    return 0;
}

int ClienteView_cargarPost_Post ( ClienteView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "factura", "SELECT" ) ) {
        FacturasList *facturasList = prov->findChild<FacturasList *> ( "listfacturas" );
        facturasList->setidcliente ( prov->dbValue ( "idcliente" ) );
        facturasList->presentar();
    }// end if
    return 0;
}// end if


int BusquedaReferencia_on_mui_abrirtodo_clicked_Post ( BusquedaReferencia *ref )
{
    QString SQLQuery = "SELECT * FROM factura WHERE reffactura = '" + ref->mui_referencia->text() + "'";
    BlDbRecordSet *cur = ref->mainCompany() ->loadQuery ( SQLQuery );
    while ( !cur->eof() ) {
        FacturaView * bud = new FacturaView ( ( BfCompany * ) ref->mainCompany(), NULL );
        ref->mainCompany() ->m_pWorkspace->addWindow ( bud );
        bud->cargar ( cur->valor ( "idfactura" ) );
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
int AlbaranClienteView_AlbaranClienteView ( AlbaranClienteView *l )
{

    _depura ( "PluginFactura_AlbaranClienteView_AlbaranClienteView", 0 );
    GenFacQToolButton *mui_exporta_efactura2 = new GenFacQToolButton ( l, l->mui_plugbotones );
    AgFacQToolButton *mui_exporta_efactura = new AgFacQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }// end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );
    m_hboxLayout1->addWidget ( mui_exporta_efactura );

    _depura ( "END PluginFactura_AlbaranClienteView_AlbaranClienteView", 0 );

    return 0;
}

///
/**
\param l
\return
**/
int PedidoClienteView_PedidoClienteView ( PedidoClienteView *l )
{

    _depura ( "PluginFactura_PedidoClienteView_PedidoClienteView", 0 );

    GenFacQToolButton *mui_exporta_efactura2 = new GenFacQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }// end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );

    _depura ( "END PluginFactura_PedidoClienteView_PedidoClienteView", 0 );

    return 0;
}


///
/**
\param l
\return
**/
int PresupuestoView_PresupuestoView ( PresupuestoView *l )
{

    _depura ( "PluginFactura_PresupuestoView_PresupuestoView", 0 );

    GenFacQToolButton *mui_exporta_efactura2 = new GenFacQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }// end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );

    _depura ( "END PluginFactura_PresupuestoView_PresupuestoView", 0 );

    return 0;
}

/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewFacturaView ( BfCompany *v )
{
    FacturaView *h = new FacturaView ( v, 0 );
    g_plugParams = h;
    return 1;
}

