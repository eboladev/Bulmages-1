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

#include "pluginbf_proveedorpago.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "pagoview.h"
#include "pagoslist.h"
#include "impqtoolbutton.h"


PagosList *g_pagosList = NULL;
BfBulmaFact *g_pluginbf_proveedorpago = NULL;


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
    blBindTextDomain ( "pluginbf_proveedorpago", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );
    g_pluginbf_proveedorpago = bges;


    if ( bges->company()->hasTablePrivilege ( "pago", "SELECT" ) ) {
        /// Miramos si existe un menu Compras
        QMenu *pPluginMenu = bges->newMenu ( _("&Compras"), "menuCompras", "menuMaestro" );
        pPluginMenu->addSeparator();

        
        BlAction *accionA = new BlAction ( _ ( "&Pagos a proveedores" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/pay-list.png" ) ) );
        accionA->setStatusTip ( _ ( "Pagos a proveedores" ) );
        accionA->setWhatsThis ( _ ( "Pagos a proveedores" ) );
        accionA->setObjectName("mui_actionProveedoresPagos");
        pPluginMenu->addAction ( accionA );
        bges->Listados->addAction ( accionA );

        BlAction *accionB = new BlAction ( _ ( "&Nuevo pago a proveedor" ), 0 );
        accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/pay.png" ) ) );
        accionB->setStatusTip ( _ ( "Nuevo pago a proveedor" ) );
        accionB->setWhatsThis ( _ ( "Nuevo pago a proveedor" ) );
        accionB->setObjectName("mui_actionProveedorPagoNuevo");
        pPluginMenu->addAction ( accionB );
        bges->Fichas->addAction ( accionB );


    } // end if


    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, "PluginBf_ProveedorPago" );

    if (accion->objectName() == "mui_actionProveedoresPagos") {
        if ( g_pagosList ) {
            g_pagosList->hide();
            g_pagosList->show();
        } // end if
    } // end if

    if (accion->objectName() == "mui_actionProveedorPagoNuevo") {
        PagoView * bud = new PagoView ( g_pluginbf_proveedorpago->company(), NULL );
        g_pluginbf_proveedorpago->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "pago", "SELECT" ) ) {
        g_pagosList = new PagosList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_pagosList );
        g_pagosList->hide();
    }// end if
    return 0;
}


int ProveedorView_ProveedorView_Post ( ProveedorView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "pago", "SELECT" ) ) {
        PagosList *pagosList = new PagosList ( ( BfCompany * ) prov->mainCompany(), NULL, 0, BL_SELECT_MODE );
        pagosList->setEditMode();
        pagosList->setObjectName ( "listpagosproveedor" );
        pagosList->hideBusqueda();
        prov->mui_tab->addTab ( pagosList, "Pagos" );
    }// end if
    return 0;
}

int ProveedorView_cargarPost_Post ( ProveedorView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "pago", "SELECT" ) ) {
        PagosList *pagosList = prov->findChild<PagosList *> ( "listpagosproveedor" );
        pagosList->setidproveedor ( prov->dbValue ( "idproveedor" ) );
        pagosList->presentar();
    }// end if
    return 0;
}// end if


int BfBuscarReferencia_on_mui_abrirtodo_clicked_Post ( BfBuscarReferencia *ref )
{
    QString SQLQuery = "SELECT * FROM pago WHERE refpago = '" + ref->mui_referencia->text() + "'";
    BlDbRecordSet *cur = ref->mainCompany() ->loadQuery ( SQLQuery );
    while ( !cur->eof() ) {
        PagoView * bud = new PagoView ( ( BfCompany * ) ref->mainCompany(), NULL );
        ref->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
        bud->load ( cur->value( "idpago" ) );
        bud->show();
        cur->nextRecord();
    } // end while
    delete cur;
    return 0;
}// end if




///
/**
\param l
\return
**/
int FacturaProveedorView_FacturaProveedorView ( FacturaProveedorView *l )
{
    BL_FUNC_DEBUG
    GenPagoQToolButton *mui_exporta_efactura2 = new GenPagoQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }// end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );
    
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
    GenPagoQToolButton *mui_exporta_efactura2 = new GenPagoQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }// end if
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
    GenPagoQToolButton *mui_exporta_efactura2 = new GenPagoQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }// end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );
    
    return 0;
}


/// Apertura de un elemento controlado a partir del parametro g_plugParams tabla_identificador
int Plugin_open(BfCompany * comp) {
  BL_FUNC_DEBUG
  QString cad = *((QString*)g_plugParams);
  QStringList args = cad.split("_");
  if (args[0] == "actividad") {
	PagoView * bud = new PagoView ( comp, 0 );
        comp->m_pWorkspace->addSubWindow ( bud );
	QString id =  args[1];
	bud->load(id);
        bud->show();

  } // end if
  return 0;
}
