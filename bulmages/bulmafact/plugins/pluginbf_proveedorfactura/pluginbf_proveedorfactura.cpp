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

#include "pluginbf_proveedorfactura.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "facturapview.h"
#include "facturasplist.h"
#include "genfactqtoolbutton.h"


FacturasProveedorList *g_facturasProveedorList = NULL;
BfBulmaFact *g_pluginbf_proveedorfactura = NULL;


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_proveedorfactura", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );
    g_pluginbf_proveedorfactura = bges;

    if ( bges->company()->hasTablePrivilege ( "facturap", "SELECT" ) ) {
        /// Miramos si existe un menu Compras
        QMenu *pPluginMenu = bges->newMenu ( _("&Compras"), "menuCompras", "menuMaestro" );
        pPluginMenu->addSeparator();

        BlAction *accionA = new BlAction ( _ ( "&Facturas de proveedores" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier-invoice-list.png" ) ) );
        accionA->setStatusTip ( _ ( "Facturas de proveedores" ) );
        accionA->setWhatsThis ( _ ( "Facturas de proveedores" ) );
        accionA->setObjectName("mui_actionProveedoresFacturas");
        pPluginMenu->addAction ( accionA );
        bges->Listados->addAction ( accionA );

        BlAction *accionB = new BlAction ( _ ( "&Nueva factura de proveedor" ), 0 );
        accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier-invoice.png" ) ) );
        accionB->setStatusTip ( _ ( "Nueva factura de proveedor" ) );
        accionB->setWhatsThis ( _ ( "Nueva factura de proveedor" ) );
        accionB->setObjectName("mui_actionProveedorFacturaNueva");
        pPluginMenu->addAction ( accionB );
        bges->Fichas->addAction ( accionB );


    } // end if
    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, "PluginBf_ProveedorFactura" );
    if (accion->objectName() == "mui_actionProveedoresFacturas") {
        if ( g_facturasProveedorList ) {
            g_facturasProveedorList->hide();
            g_facturasProveedorList->show();
        } // end if
    }  // end if
    if (accion->objectName() == "mui_actionProveedorFacturaNueva") {
        FacturaProveedorView * bud = new FacturaProveedorView ( g_pluginbf_proveedorfactura->company(), NULL );
        g_pluginbf_proveedorfactura->company()->m_pWorkspace->addSubWindow ( bud );
        bud->inicializar();
        bud->show();
    }  // end if
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "facturap", "SELECT" ) ) {
        g_facturasProveedorList = new FacturasProveedorList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_facturasProveedorList );
        g_facturasProveedorList->hide();
    } // end if
    return 0;
}


int ProveedorView_ProveedorView_Post ( ProveedorView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "facturap", "SELECT" ) ) {
        FacturasProveedorList *facturasProveedorList = new FacturasProveedorList ( ( BfCompany * ) prov->mainCompany(), NULL, 0, BL_SELECT_MODE );
        facturasProveedorList->setEditMode();
        facturasProveedorList->setObjectName ( "listpagosproveedor" );
        facturasProveedorList->hideBusqueda();
        prov->mui_tab->addTab ( facturasProveedorList, "Facturas" );
    }// end if
    return 0;
}

int ProveedorView_cargarPost_Post ( ProveedorView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "facturap", "SELECT" ) ) {
        FacturasProveedorList *facturasProveedorList = prov->findChild<FacturasProveedorList *> ( "listpagosproveedor" );
        facturasProveedorList->setidproveedor ( prov->dbValue ( "idproveedor" ) );
        facturasProveedorList->presentar();
    } // end if
    return 0;
}


int BfBuscarReferencia_on_mui_abrirtodo_clicked_Post ( BfBuscarReferencia *ref )
{
    QString SQLQuery = "SELECT * FROM facturap WHERE reffacturap = '" + ref->mui_referencia->text() + "'";
    BlDbRecordSet *cur = ref->mainCompany() ->loadQuery ( SQLQuery );
    while ( !cur->eof() ) {
        FacturaProveedorView * bud = new FacturaProveedorView ( ( BfCompany * ) ref->mainCompany(), NULL );
        ref->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
        bud->load ( cur->value( "idfacturap" ) );
        bud->show();
        cur->nextRecord();
    } // end while
    delete cur;
    return 0;
}


///
/**
\param l
\return
**/
int AlbaranProveedorView_AlbaranProveedorView ( AlbaranProveedorView *l )
{
    BL_FUNC_DEBUG
    GenFacProQToolButton *mui_exporta_efactura2 = new GenFacProQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );

    
    return 0;
}

///
/**
\param l
\return
**/
int PedidoProveedorView_PedidoProveedorView ( PedidoProveedorView *l )
{
    BL_FUNC_DEBUG

    GenFacProQToolButton *mui_exporta_efactura2 = new GenFacProQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );

    
    return 0;
}

/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewFacturaProveedorView ( BfCompany *v )
{
    FacturaProveedorView *h = new FacturaProveedorView ( v, 0 );
    g_plugParams = h;
    return 1;
}

