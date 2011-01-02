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
#include "albaranclienteview.h"
#include "albaranclientelist.h"

#include "genalbqtoolbutton.h"


TicketClienteList *g_albaranClienteList = NULL;

///
/**
**/
mypluginalb::mypluginalb()
{
    blDebug ( "mypluginalb::mypluginalb", 0 );
    blDebug ( "END mypluginalb::mypluginalb", 0 );
}


///
/**
**/
mypluginalb::~mypluginalb()
{
    blDebug ( "mypluginalb::~mypluginalb", 0 );
    blDebug ( "END mypluginalb::~mypluginalb", 0 );
}


///
/**
**/
void mypluginalb::elslot()
{
    blDebug ( "mypluginalb::elslot", 0 );
    if ( g_albaranClienteList ) {
        g_albaranClienteList->hide();
        g_albaranClienteList->show();
    }// end if
    blDebug ( "END mypluginalb::elslot", 0 );
}

///
/**
**/
void mypluginalb::elslot1()
{
    blDebug ( "mypluginalb::elslot1", 0 );
    TicketClienteView * bud = new TicketClienteView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->inicializar();
    bud->show();
    blDebug ( "END mypluginalb::elslot1", 0 );
}



///
/**
\param bges
**/
void mypluginalb::inicializa ( BfBulmaFact *bges )
{
    blDebug ( "mypluginalb::inicializa", 0 );

    if ( bges->company()->hasTablePrivilege ( "albaran", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( "&Ventas", "menuVentas", "menuMaestro" );
        pPluginMenu->addSeparator();

        m_bges = bges;
        setMainCompany ( bges->company() );
        QAction *planCuentas = new QAction ( _ ( "&Ticketes a clientes" ), 0 );
        planCuentas->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/client-delivery-note-list.png" ) ) );
        planCuentas->setStatusTip ( _ ( "Ticketes a clientes" ) );
        planCuentas->setWhatsThis ( _ ( "Ticketes a clientes" ) );
        pPluginMenu->addAction ( planCuentas );
        bges->Listados->addAction ( planCuentas );
        connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

        QAction *npago = new QAction ( _ ( "&Nuevo albaran a cliente" ), 0 );
        npago->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/client-delivery-note.png" ) ) );
        npago->setStatusTip ( _ ( "Nuevo albaran a cliente" ) );
        npago->setWhatsThis ( _ ( "Nuevo albaran a cliente" ) );
        pPluginMenu->addAction ( npago );
        bges->Fichas->addAction ( npago );
        connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );


    }// end if
    blDebug ( "END mypluginalb::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de Entrada del plugin de Ticketes a Clientes\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_clienteticket", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    mypluginalb *plug = new mypluginalb();
    plug->inicializa ( bges );
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
        albaranesList->setModoEdicion();
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
        bud->cargar ( cur->valor ( "idfactura" ) );
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

