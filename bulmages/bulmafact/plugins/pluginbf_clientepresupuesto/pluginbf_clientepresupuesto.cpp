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

#include "pluginbf_clientepresupuesto.h"
#include "blplugins.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "presupuestoview.h"
#include "presupuestolist.h"

#include "genpreqtoolbutton.h"


PresupuestoList *g_presupuestosList = NULL;

///
/**
**/
mypluginpres::mypluginpres()
{
    blDebug ( "mypluginpres::mypluginpres", 0 );
    blDebug ( "END mypluginpres::mypluginpres", 0 );
}


///
/**
**/
mypluginpres::~mypluginpres()
{
    blDebug ( "mypluginpres::~mypluginpres", 0 );
    blDebug ( "END mypluginpres::~mypluginpres", 0 );
}


///
/**
**/
void mypluginpres::elslot()
{
    blDebug ( "mypluginpres::elslot", 0 );
    if ( g_presupuestosList ) {
        g_presupuestosList->hide();
        g_presupuestosList->show();
    }// end if
    blDebug ( "END mypluginpres::elslot", 0 );
}

///
/**
**/
void mypluginpres::elslot1()
{
    blDebug ( "mypluginpres::elslot1", 0 );
    PresupuestoView * bud = new PresupuestoView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->inicializar();
    bud->show();
    blDebug ( "END mypluginpres::elslot1", 0 );
}



///
/**
\param bges
**/
void mypluginpres::inicializa ( BfBulmaFact *bges )
{
    blDebug ( "mypluginpres::inicializa", 0 );

    if ( bges->company()->hasTablePrivilege ( "presupuesto", "SELECT" ) ) {


        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( "&Ventas", "menuVentas", "menuMaestro" );
        pPluginMenu->addSeparator();

        /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
        m_bges = bges;
        setMainCompany ( bges->company() );
        QAction *planCuentas = new QAction ( _ ( "&Presupuestos a clientes" ), 0 );
        planCuentas->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/client-quote-list.png" ) ) );
        planCuentas->setStatusTip ( _ ( "Presupuestos a clientes" ) );
        planCuentas->setWhatsThis ( _ ( "Presupuestos a clientes" ) );
        pPluginMenu->addAction ( planCuentas );
        bges->Listados->addAction ( planCuentas );
        connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

        QAction *npago = new QAction ( _ ( "&Nuevo presupuesto a cliente" ), 0 );
        npago->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/client-quote.png" ) ) );
        npago->setStatusTip ( _ ( "Nuevo presupuesto a cliente" ) );
        npago->setWhatsThis ( _ ( "Nuevo presupuesto a cliente" ) );
        pPluginMenu->addAction ( npago );
        bges->Fichas->addAction ( npago );
        connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );
    }// end if
    blDebug ( "END mypluginpres::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de entrada del plugin de presupuestos a clientes\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginpresupuesto", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    mypluginpres *plug = new mypluginpres();
    plug->inicializa ( bges );
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "presupuesto", "SELECT" ) ) {
        g_presupuestosList = new PresupuestoList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_presupuestosList );
        g_presupuestosList->hide();
    }// end if
    return 0;
}


int ClienteView_ClienteView_Post ( ClienteView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "presupuesto", "SELECT" ) ) {
        PresupuestoList *presupuestosList = new PresupuestoList ( NULL, 0, BL_SELECT_MODE );
	presupuestosList->setMainCompany(( BfCompany * ) prov->mainCompany());
        presupuestosList->setModoEdicion();
        presupuestosList->setObjectName ( "listpresupuestos" );
        presupuestosList->hideBusqueda();
        prov->mui_tab->addTab ( presupuestosList, "Presupuestos" );
    }// end if
    return 0;
}

int ClienteView_cargarPost_Post ( ClienteView *prov )
{
    if ( prov->mainCompany()->hasTablePrivilege ( "presupuesto", "SELECT" ) ) {
        PresupuestoList *presupuestosList = prov->findChild<PresupuestoList *> ( "listpresupuestos" );
        presupuestosList->setidcliente ( prov->dbValue ( "idcliente" ) );
        presupuestosList->presentar();
    }// end if
    return 0;
}// end if


int BfBuscarReferencia_on_mui_abrirtodo_clicked_Post ( BfBuscarReferencia *ref )
{
    QString SQLQuery = "SELECT * FROM presupuesto WHERE refpresupuesto = '" + ref->mui_referencia->text() + "'";
    BlDbRecordSet *cur = ref->mainCompany() ->loadQuery ( SQLQuery );
    while ( !cur->eof() ) {
        PresupuestoView * bud = new PresupuestoView ( ( BfCompany * ) ref->mainCompany(), NULL );
        ref->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
        bud->cargar ( cur->valor ( "idpresupuesto" ) );
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
int PedidoClienteView_PedidoClienteView ( PedidoClienteView *l )
{

    blDebug ( "PedidoClienteView_PedidoClienteView", 0 );

    VerPreQToolButton *mui_exporta_epresupuesto2 = new VerPreQToolButton ( l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );

    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    }// end if
    m_hboxLayout1->addWidget ( mui_exporta_epresupuesto2 );

    blDebug ( "END PedidoClienteView_PedidoClienteView", 0 );

    return 0;
}




/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewPresupuestoView ( BfCompany *v )
{
    PresupuestoView *h = new PresupuestoView ( v, 0 );
    g_plugParams = h;
    return 1;
}

