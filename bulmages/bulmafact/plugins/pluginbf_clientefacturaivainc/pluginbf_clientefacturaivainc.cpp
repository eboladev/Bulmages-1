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

#include "pluginbf_clientefacturaivainc.h"
#include "blplugins.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "facturaivaincclienteview.h"
#include "facturaivaincclientelist.h"

#include "genfacqtoolbutton.h"


FacturaIVAIncClienteList *g_facturaClienteList = NULL;
BfBulmaFact *g_pluginbf_clientefacturaivainc = NULL;



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
    blBindTextDomain ( "pluginbf_clientefacturaivainc", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );


    if ( bges->company()->hasTablePrivilege ( "factura", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( _("&Ventas"), "menuVentas", "menuMaestro" );
        pPluginMenu->addSeparator();

        g_pluginbf_clientefacturaivainc = bges;
        BlAction *accionA = new BlAction ( _ ( "&FacturaIVAInces a clientes" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/client-delivery-note-list.png" ) ) );
        accionA->setStatusTip ( _ ( "FacturaIVAInces a clientes" ) );
        accionA->setWhatsThis ( _ ( "FacturaIVAInces a clientes" ) );
        accionA->setObjectName("mui_actionClienteFacturaIVAInc");
        pPluginMenu->addAction ( accionA );
        bges->Listados->addAction ( accionA );


        BlAction *accionB = new BlAction ( _ ( "&Nuevo facturaivainc a cliente" ), 0 );
        accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/client-delivery-note.png" ) ) );
        accionB->setStatusTip ( _ ( "Nuevo facturaivainc a cliente" ) );
        accionB->setWhatsThis ( _ ( "Nuevo facturaivainc a cliente" ) );
        accionB->setObjectName("mui_actionClienteNuevoAlbaran");
        pPluginMenu->addAction ( accionB );
        bges->Fichas->addAction ( accionB );


    } // end if


    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionClienteFacturaIVAInc") {
        if ( g_facturaClienteList ) {
            g_facturaClienteList->hide();
            g_facturaClienteList->show();
        } // end if
    } // end if

    if (accion->objectName() == "mui_actionClienteNuevoAlbaran") {
        FacturaIVAIncClienteView * bud = new FacturaIVAIncClienteView ( g_pluginbf_clientefacturaivainc->company(), NULL );
        g_pluginbf_clientefacturaivainc->company()->m_pWorkspace->addSubWindow ( bud );
        bud->inicializar();
        bud->show();
    } // end if
    return 0;
}



int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "factura", "SELECT" ) ) {
        g_facturaClienteList = new FacturaIVAIncClienteList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_facturaClienteList );
        g_facturaClienteList->hide();
    }// end if
    return 0;
}


int ClienteView_ClienteView_Post ( ClienteView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "factura", "SELECT" ) ) {
        FacturaIVAIncClienteList *facturaesList = new FacturaIVAIncClienteList ( NULL, 0, BL_SELECT_MODE );
	facturaesList->setMainCompany(( BfCompany * ) prov->mainCompany());
        facturaesList->setEditMode();
        facturaesList->setObjectName ( "listfacturaes" );
        facturaesList->hideBusqueda();
        prov->mui_tab->addTab ( facturaesList, "FacturaIVAInces" );
    }// end if
    return 0;
}

int ClienteView_cargarPost_Post ( ClienteView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "factura", "SELECT" ) ) {
        FacturaIVAIncClienteList *facturaesList = prov->findChild<FacturaIVAIncClienteList *> ( "listfacturaes" );
        facturaesList->setidcliente ( prov->dbValue ( "idcliente" ) );
        facturaesList->presentar();
    }// end if
    return 0;
}// end if


int BfBuscarReferencia_on_mui_abrirtodo_clicked_Post ( BfBuscarReferencia *ref )
{
    QString SQLQuery = "SELECT * FROM factura WHERE reffactura = '" + ref->mui_referencia->text() + "'";
    BlDbRecordSet *cur = ref->mainCompany() ->loadQuery ( SQLQuery );
    while ( !cur->eof() ) {
        FacturaIVAIncClienteView * bud = new FacturaIVAIncClienteView ( ( BfCompany * ) ref->mainCompany(), NULL );
        ref->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
        bud->load ( cur->value( "idfactura" ) );
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
int TicketClienteView_TicketClienteView ( TicketClienteView *l )
{

    BL_FUNC_DEBUG
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

    return 0;
}


/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewFacturaIVAIncClienteView ( BfCompany *v )
{
    FacturaIVAIncClienteView *h = new FacturaIVAIncClienteView ( v, 0 );
    g_plugParams = h;
    return 1;
}

