/***************************************************************************
 *   Copyright (C) 2010 by Tomeu Borras Riera                              *
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

/* Dedicat a na Mariona Palomo (filla de en Leo i na Laida) nascuda a Barcelona el dissabte 16/01/2010 */


#include <stdio.h>

#include "pluginbf_clientecobro.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "cobroview.h"
#include "cobroslist.h"
#include "impqtoolbutton.h"


CobrosList *g_cobrosList = NULL;
BfBulmaFact *g_bges = NULL;

///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de Entrada del plugin de Cobros\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_clientecobro", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
    
    if ( bges->company()->hasTablePrivilege ( "cobro", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( "&Ventas", "menuVentas", "menuMaestro" );
        pPluginMenu->addSeparator();

        /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
        g_bges = bges;

        BlAction *CobroCliente = new BlAction ( _ ( "&Cobros de clientes" ), 0 );
        CobroCliente->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/receive-list.png" ) ) );
        CobroCliente->setStatusTip ( _ ( "Cobros de clientes" ) );
        CobroCliente->setWhatsThis ( _ ( "Cobros de clientes" ) );
        CobroCliente->setObjectName("mui_actionCobroCliente");

        pPluginMenu->addAction ( CobroCliente );
        bges->Listados->addAction ( CobroCliente );

        BlAction *NuevoCobroCliente = new BlAction ( _ ( "&Nuevo cobro de cliente" ), 0 );
        NuevoCobroCliente->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/receive.png" ) ) );
        NuevoCobroCliente->setStatusTip ( _ ( "Nuevo cobro de cliente" ) );
        NuevoCobroCliente->setWhatsThis ( _ ( "Nuevo cobro de cliente" ) );
        NuevoCobroCliente->setObjectName("mui_actionNuevoCobroCliente");

        pPluginMenu->addAction ( NuevoCobroCliente );
        bges->Fichas->addAction ( NuevoCobroCliente );
    }// end if
    
    return 0;
}

int BlAction_triggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionCobroCliente") {
        if ( g_cobrosList ) {
            g_cobrosList->hide();
            g_cobrosList->show();
        }// end if
    } // end if

    if (accion->objectName() == "mui_actionNuevoCobroCliente") {
        CobroView * bud = new CobroView ( ( BfCompany * ) g_bges->company(), NULL );
        g_bges->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();                 
    } // end if


    return 0;
} // end if


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "cobro", "SELECT" ) ) {
        g_cobrosList = new CobrosList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_cobrosList );
        g_cobrosList->hide();
    }// end if
    return 0;
}


int ClienteView_ClienteView_Post ( ClienteView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "cobro", "SELECT" ) ) {
        CobrosList *pagosList = new CobrosList ( NULL, 0, BL_SELECT_MODE );
	pagosList->setMainCompany(( BfCompany * ) prov->mainCompany());
        pagosList->setModoEdicion();
        pagosList->setObjectName ( "listcobrosproveedor" );
        pagosList->hideBusqueda();
        prov->mui_tab->addTab ( pagosList, "Cobros" );
    }// end if
    return 0;
}

int ClienteView_cargarPost_Post ( ClienteView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "cobro", "SELECT" ) ) {
        CobrosList *pagosList = prov->findChild<CobrosList *> ( "listcobrosproveedor" );
        pagosList->setidcliente ( prov->dbValue ( "idcliente" ) );
        pagosList->presentar();
    }// end if
    return 0;
}// end if


int BfBuscarReferencia_on_mui_abrirtodo_clicked_Post ( BfBuscarReferencia *ref )
{
    QString SQLQuery = "SELECT * FROM cobro WHERE refcobro = '" + ref->mui_referencia->text() + "'";
    BlDbRecordSet *cur = ref->mainCompany() ->loadQuery ( SQLQuery );
    while ( !cur->eof() ) {
        CobroView * bud = new CobroView ( ( BfCompany * ) ref->mainCompany(), NULL );
        ref->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
        bud->cargar ( cur->valor ( "idcobro" ) );
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
int FacturaView_FacturaView ( FacturaView *l )
{
    blDebug ( "PluginCobros_FacturaView_FacturaView", 0 );
    GenCobroQToolButton *mui_exporta_efactura2 = new GenCobroQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }// end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );
    blDebug ( "END PluginCobros_FacturaView_FacturaView", 0 );
    return 0;
}

///
/**
\param l
\return
**/
int AlbaranClienteView_AlbaranClienteView ( AlbaranClienteView *l )
{
    blDebug ( "PluginCobros_AlbaranClienteView_AlbaranClienteView", 0 );
    GenCobroQToolButton *mui_exporta_efactura2 = new GenCobroQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }// end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );
    blDebug ( "END PluginCobros_AlbaranClienteView_AlbaranClienteView", 0 );
    return 0;
}

///
/**
\param l
\return
**/
int PedidoClienteView_PedidoClienteView ( PedidoClienteView *l )
{
    blDebug ( "PluginCobros_PedidoClienteView_PedidoClienteView", 0 );
    GenCobroQToolButton *mui_exporta_efactura2 = new GenCobroQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }// end if
    m_hboxLayout1->addWidget ( mui_exporta_efactura2 );
    blDebug ( "END PluginCobros_PedidoClienteView_PedidoClienteView", 0 );
    return 0;
}


/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewCobroView ( BfCompany *v )
{
    CobroView *h = new CobroView ( v, 0 );
    g_plugParams = h;
    return 1;
}
