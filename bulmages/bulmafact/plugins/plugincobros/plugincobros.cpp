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

#include "plugincobros.h"
#include "company.h"
#include "funcaux.h"
#include "cobroview.h"
#include "cobroslist.h"
#include "impqtoolbutton.h"


CobrosList *g_cobrosList=NULL;

///
/**
**/
myplugincob::myplugincob()
{
    _depura ( "myplugincob::myplugincob", 0 );
    _depura ( "END myplugincob::myplugincob", 0 );
}


///
/**
**/
myplugincob::~myplugincob()
{
    _depura ( "myplugincob::~myplugincob", 0 );
    _depura ( "END myplugincob::~myplugincob", 0 );
}


///
/**
**/
void myplugincob::elslot()
{
    _depura ( "myplugincob::elslot", 0 );
    if (g_cobrosList) {
	g_cobrosList->hide();
	g_cobrosList->show();
    }// end if
    _depura ( "END myplugincob::elslot", 0 );
}

///
/**
**/
void myplugincob::elslot1()
{
    _depura ( "myplugincob::elslot1", 0 );
        CobroView * bud = new CobroView((Company *)empresaBase(), NULL);
        empresaBase() ->m_pWorkspace->addWindow ( bud );
        bud->show();
    _depura ( "END myplugincob::elslot1", 0 );
}



///
/**
\param bges
**/
void myplugincob::inicializa ( Bulmafact *bges )
{
    _depura ( "myplugincob::inicializa", 0 );

    if ( bges->getcompany()->has_table_privilege ( "cobro", "SELECT" ) ) {

    /// Miramos si existe un menu Ventas
	QMenu *pPluginMenu = bges->newMenu("&Ventas", "menuVentas", "menuMaestro");
	pPluginMenu->addSeparator();

	/// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
	m_bges = bges;
	setEmpresaBase ( bges->getcompany() );
	QAction *planCuentas = new QAction ( _( "&Cobros a clientes" ), 0 );
	planCuentas->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/receive-list.svg" ) ));
	planCuentas->setStatusTip ( _( "Cobros a clientes" ) );
	planCuentas->setWhatsThis ( _( "Cobros a clientes" ) );
	pPluginMenu->addAction ( planCuentas );
	bges->Listados->addAction (planCuentas);
	connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

	QAction *npago = new QAction ( _( "&Nuevo cobro a cliente" ), 0 );
	npago->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/receive.svg" ) ));
	npago->setStatusTip ( _( "Nuevo cobro a cliente" ) );
	npago->setWhatsThis ( _( "Nuevo cobro a cliente" ) );
	pPluginMenu->addAction ( npago );
	bges->Fichas->addAction (npago);
	connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );
    }// end if
    _depura ( "END myplugincob::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Punto de Entrada del plugin de Cobros\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("plugincobros", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    myplugincob *plug = new myplugincob();
    plug->inicializa ( bges );
    return 0;
}


int Company_createMainWindows_Post(Company *comp) {
    if ( comp->has_table_privilege ( "cobro", "SELECT" ) ) {
	g_cobrosList = new CobrosList( comp, NULL );	
	comp->m_pWorkspace->addWindow ( g_cobrosList );
	g_cobrosList->hide();
    }// end if
    return 0;
}


int ClienteView_ClienteView_Post (ClienteView *prov) {
    if ( prov->empresaBase()->has_table_privilege ( "cobro", "SELECT" ) ) {
		CobrosList *pagosList = new CobrosList( (Company *)prov->empresaBase(), NULL, 0, BlFormList::SelectMode );
		pagosList->setModoEdicion();
		pagosList->setObjectName("listcobrosproveedor");
		pagosList->hideBusqueda();
        prov->mui_tab->addTab ( pagosList, "Cobros" );
    }// end if
    return 0;
}

int ClienteView_cargarPost_Post (ClienteView *prov) {
    if ( prov->empresaBase()->has_table_privilege ( "cobro", "SELECT" ) ) {
		CobrosList *pagosList = prov->findChild<CobrosList *> ( "listcobrosproveedor" );
        pagosList->setidcliente ( prov->DBvalue ( "idcliente" ) );
        pagosList->presentar();
    }// end if
    return 0;
}// end if


int BusquedaReferencia_on_mui_abrirtodo_clicked_Post (BusquedaReferencia *ref) {
    QString SQLQuery = "SELECT * FROM cobro WHERE refcobro = '" + ref->mui_referencia->text() + "'";
    cursor2 *cur = ref->empresaBase() ->cargacursor ( SQLQuery );
    while ( !cur->eof() ) {
        CobroView * bud = new CobroView((Company *)ref->empresaBase(), NULL);
        ref->empresaBase() ->m_pWorkspace->addWindow ( bud );
        bud->cargar ( cur->valor ( "idcobro" ) );
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
int FacturaView_FacturaView ( FacturaView *l )
{
    _depura ( "PluginCobros_FacturaView_FacturaView", 0 );
    GenCobroQToolButton *mui_exporta_efactura2 = new GenCobroQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

	if (!m_hboxLayout1) {
		m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
		m_hboxLayout1->setSpacing ( 5 );
		m_hboxLayout1->setMargin ( 5 );
		m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
	}// end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );
    _depura ( "END PluginPagos_FacturaView_FacturaView", 0 );
    return 0;
}

///
/**
\param l
\return
**/
int AlbaranClienteView_AlbaranClienteView ( AlbaranClienteView *l )
{
    _depura ( "PluginCobros_AlbaranClienteView_AlbaranClienteView", 0 );
    GenCobroQToolButton *mui_exporta_efactura2 = new GenCobroQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

	if (!m_hboxLayout1) {
		m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
		m_hboxLayout1->setSpacing ( 5 );
		m_hboxLayout1->setMargin ( 5 );
		m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
	}// end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );
    _depura ( "END PluginPagos_AlbaranClienteView_AlbaranClienteView", 0 );
    return 0;
}

///
/**
\param l
\return
**/
int PedidoClienteView_PedidoClienteView ( PedidoClienteView *l )
{
    _depura ( "PluginPagos_PedidoClienteView_PedidoClienteView", 0 );
    GenCobroQToolButton *mui_exporta_efactura2 = new GenCobroQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

	if (!m_hboxLayout1) {
		m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
		m_hboxLayout1->setSpacing ( 5 );
		m_hboxLayout1->setMargin ( 5 );
		m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
	}// end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );
    _depura ( "END PluginPagos_PedidoClienteView_PedidoClienteView", 0 );
    return 0;
}

