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

#include "pluginbf_clienteticket.h"
#include "blplugins.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "ticketclienteview.h"
#include "ticketclientelist.h"

#include "genalbqtoolbutton.h"


TicketClienteList *g_albaranClienteList = NULL;
BfBulmaFact *g_pluginbf_clienteticket = NULL;



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
    blBindTextDomain ( "pluginbf_clienteticket", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );


    if ( bges->company()->hasTablePrivilege ( "albaran", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( _("&Ventas"), "menuVentas", "menuMaestro" );
        pPluginMenu->addSeparator();

        g_pluginbf_clienteticket = bges;
        BlAction *accionA = new BlAction ( _ ( "&Ticketes a clientes" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/client-delivery-note-list.png" ) ) );
        accionA->setStatusTip ( _ ( "Ticketes a clientes" ) );
        accionA->setWhatsThis ( _ ( "Ticketes a clientes" ) );
        accionA->setObjectName("mui_actionClienteTicket");
        pPluginMenu->addAction ( accionA );
        bges->Listados->addAction ( accionA );


        BlAction *accionB = new BlAction ( _ ( "&Nuevo ticket a cliente" ), 0 );
        accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/client-delivery-note.png" ) ) );
        accionB->setStatusTip ( _ ( "Nuevo ticket a cliente" ) );
        accionB->setWhatsThis ( _ ( "Nuevo ticket a cliente" ) );
        accionB->setObjectName("mui_actionClienteNuevoAlbaran");
        pPluginMenu->addAction ( accionB );
        bges->Fichas->addAction ( accionB );


    } // end if


    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionClienteTicket") {
        if ( g_albaranClienteList ) {
            g_albaranClienteList->hide();
            g_albaranClienteList->show();
        } // end if
    } // end if

    if (accion->objectName() == "mui_actionClienteNuevoAlbaran") {
        TicketClienteView * bud = new TicketClienteView ( g_pluginbf_clienteticket->company(), NULL );
        g_pluginbf_clienteticket->company()->m_pWorkspace->addSubWindow ( bud );
        bud->inicializar();
        bud->show();
    } // end if
    return 0;
}



int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "albaran", "SELECT" ) ) {
        g_albaranClienteList = new TicketClienteList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_albaranClienteList );
        g_albaranClienteList->hide();
    }// end if
    return 0;
}


int ClienteView_ClienteView_Post ( ClienteView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "albaran", "SELECT" ) ) {
        TicketClienteList *albaranesList = new TicketClienteList ( NULL, 0, BL_SELECT_MODE );
	albaranesList->setMainCompany(( BfCompany * ) prov->mainCompany());
        albaranesList->setEditMode();
        albaranesList->setObjectName ( "listalbaranes" );
        albaranesList->hideBusqueda();
        prov->mui_tab->addTab ( albaranesList, "Ticketes" );
    }// end if
    return 0;
}

int ClienteView_cargarPost_Post ( ClienteView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "albaran", "SELECT" ) ) {
        TicketClienteList *albaranesList = prov->findChild<TicketClienteList *> ( "listalbaranes" );
        albaranesList->setidcliente ( prov->dbValue ( "idcliente" ) );
        albaranesList->presentar();
    }// end if
    return 0;
}// end if


int BfBuscarReferencia_on_mui_abrirtodo_clicked_Post ( BfBuscarReferencia *ref )
{
    QString SQLQuery = "SELECT * FROM albaran WHERE refalbaran = '" + ref->mui_referencia->text() + "'";
    BlDbRecordSet *cur = ref->mainCompany() ->loadQuery ( SQLQuery );
    while ( !cur->eof() ) {
        TicketClienteView * bud = new TicketClienteView ( ( BfCompany * ) ref->mainCompany(), NULL );
        ref->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
        bud->load ( cur->value( "idfactura" ) );
        bud->show();
        cur->nextRecord();
    } // end while
    delete cur;
    return 0;
}// end if


/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewTicketClienteView ( BfCompany *v )
{
    TicketClienteView *h = new TicketClienteView ( v, 0 );
    g_plugParams = h;
    return 1;
}

