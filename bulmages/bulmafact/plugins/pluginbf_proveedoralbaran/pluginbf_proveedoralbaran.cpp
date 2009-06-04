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

#include "pluginbf_proveedoralbaran.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "albaranproveedorview.h"
#include "albaranesproveedor.h"
#include "genalbpqtoolbutton.h"


AlbaranesProveedor *g_albaranesProveedor = NULL;

///
/**
**/
mypluginalbp::mypluginalbp()
{
    _depura ( "mypluginalbp::mypluginalbp", 0 );
    _depura ( "END mypluginalbp::mypluginalbp", 0 );
}


///
/**
**/
mypluginalbp::~mypluginalbp()
{
    _depura ( "mypluginalbp::~mypluginalbp", 0 );
    _depura ( "END mypluginalbp::~mypluginalbp", 0 );
}


///
/**
**/
void mypluginalbp::elslot()
{
    _depura ( "mypluginalbp::elslot", 0 );
    if ( g_albaranesProveedor ) {
        g_albaranesProveedor->hide();
        g_albaranesProveedor->show();
    }// end if
    _depura ( "END mypluginalbp::elslot", 0 );
}

///
/**
**/
void mypluginalbp::elslot1()
{
    _depura ( "mypluginalbp::elslot1", 0 );
    AlbaranProveedorView * bud = new AlbaranProveedorView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    _depura ( "END mypluginalbp::elslot1", 0 );
}



///
/**
\param bges
**/
void mypluginalbp::inicializa ( BfBulmaFact *bges )
{
    _depura ( "mypluginalbp::inicializa", 0 );

    if ( bges->getcompany()->hasTablePrivilege ( "albaranp", "SELECT" ) ) {
        /// Miramos si existe un menu Compras
        QMenu *pPluginMenu = bges->newMenu ( "&Compras", "menuCompras", "menuMaestro" );
        pPluginMenu->addSeparator();

        /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
        m_bges = bges;
        setMainCompany ( bges->getcompany() );
        QAction *planCuentas = new QAction ( _ ( "&Albaranes de proveedores" ), 0 );
        planCuentas->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier-delivery-note-list.png" ) ) );
        planCuentas->setStatusTip ( _ ( "Albaranes de proveedores" ) );
        planCuentas->setWhatsThis ( _ ( "Albaranes de proveedores" ) );
        pPluginMenu->addAction ( planCuentas );
        bges->Listados->addAction ( planCuentas );
        connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

        QAction *npago = new QAction ( _ ( "&Nuevo albaran de proveedor" ), 0 );
        npago->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier-delivery-note.png" ) ) );
        npago->setStatusTip ( _ ( "Nuevo albaran de proveedor" ) );
        npago->setWhatsThis ( _ ( "Nuevo albaran de proveedor" ) );
        pPluginMenu->addAction ( npago );
        bges->Fichas->addAction ( npago );
        connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );


    }// end if
    _depura ( "END mypluginalbp::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    _depura ( "Punto de entrada del plugin de facturas de proveedor\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_proveedoralbaran", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    mypluginalbp *plug = new mypluginalbp();
    plug->inicializa ( bges );
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "albaranp", "SELECT" ) ) {
        g_albaranesProveedor = new AlbaranesProveedor ( comp, NULL );
        comp->m_pWorkspace->addWindow ( g_albaranesProveedor );
        g_albaranesProveedor->hide();
    }// end if
    return 0;
}


int ProveedorView_ProveedorView_Post ( ProveedorView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "albaranp", "SELECT" ) ) {
        AlbaranesProveedor *albaranesProveedor = new AlbaranesProveedor ( ( BfCompany * ) prov->mainCompany(), NULL, 0, BL_SELECT_MODE );
        albaranesProveedor->setModoEdicion();
        albaranesProveedor->setObjectName ( "listalbaranesproveedor" );
        albaranesProveedor->hideBusqueda();
        prov->mui_tab->addTab ( albaranesProveedor, "Albaranes" );
    }// end if
    return 0;
}

int ProveedorView_cargarPost_Post ( ProveedorView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "albaranp", "SELECT" ) ) {
        AlbaranesProveedor *albaranesProveedor = prov->findChild<AlbaranesProveedor *> ( "listalbaranesproveedor" );
        albaranesProveedor->setidproveedor ( prov->dbValue ( "idproveedor" ) );
        albaranesProveedor->presentar();
    }// end if
    return 0;
}// end if


int BfBuscarReferencia_on_mui_abrirtodo_released_Post ( BfBuscarReferencia *ref )
{
    QString SQLQuery = "SELECT * FROM albaranp WHERE refalbaranp = '" + ref->mui_referencia->text() + "'";
    BlDbRecordSet *cur = ref->mainCompany() ->loadQuery ( SQLQuery );
    while ( !cur->eof() ) {
        AlbaranProveedorView * bud = new AlbaranProveedorView ( ( BfCompany * ) ref->mainCompany(), NULL );
        ref->mainCompany() ->m_pWorkspace->addWindow ( bud );
        bud->cargar ( cur->valor ( "idalbaranp" ) );
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
int PedidoProveedorView_PedidoProveedorView ( PedidoProveedorView *l )
{
    _depura ( "PluginPagos_PedidoProveedorView_PedidoProveedorView", 0 );

    GenAlbProQToolButton *mui_exporta_efactura2 = new GenAlbProQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }// end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );

    _depura ( "END PluginPagos_PedidoProveedorView_PedidoProveedorView", 0 );
    return 0;
}


/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewAlbaranProveedorView ( BfCompany *v )
{
    AlbaranProveedorView *h = new AlbaranProveedorView ( v, 0 );
    g_plugParams = h;
    return 1;
}


