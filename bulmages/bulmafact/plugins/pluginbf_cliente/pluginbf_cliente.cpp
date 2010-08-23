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

#include "pluginbf_cliente.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "clienteview.h"
#include "clientslist.h"



ClientsList *g_clientesList = NULL;

///
/**
**/
MyPlugCli::MyPlugCli()
{
    blDebug ( "MyPlugCli::MyPlugCli", 0 );
    blDebug ( "END MyPlugCli::MyPlugCli", 0 );
}


///
/**
**/
MyPlugCli::~MyPlugCli()
{
    blDebug ( "MyPlugCli::~MyPlugCli", 0 );
    blDebug ( "END MyPlugCli::~MyPlugCli", 0 );
}


///
/**
**/
void MyPlugCli::elslot()
{
    blDebug ( "MyPlugCli::elslot", 0 );
    if ( g_clientesList ) {
        g_clientesList->hide();
        g_clientesList->show();
    }// end if
    blDebug ( "END MyPlugCli::elslot", 0 );
}

///
/**
**/
void MyPlugCli::elslot1()
{
    blDebug ( "MyPlugCli::elslot1", 0 );
    ClienteView * bud = new ClienteView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    blDebug ( "END MyPlugCli::elslot1", 0 );
}



///
/**
\param bges
**/
void MyPlugCli::inicializa ( BfBulmaFact *bges )
{
    blDebug ( "MyPlugCli::inicializa", 0 );

    if ( bges->company()->hasTablePrivilege ( "cliente", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( "&Ventas", "menuVentas", "menuMaestro" );
        pPluginMenu->addSeparator();

        m_bges = bges;
        setMainCompany ( bges->company() );
        QAction *planCuentas = new QAction ( _ ( "&Clientes" ), 0 );
        planCuentas->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/client-list.png" ) ) );
        planCuentas->setStatusTip ( _ ( "Clientes" ) );
        planCuentas->setWhatsThis ( _ ( "Clientes" ) );
        pPluginMenu->addAction ( planCuentas );
        bges->Listados->addAction ( planCuentas );
        connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

        QAction *npago = new QAction ( _ ( "&Nuevo cliente" ), 0 );
        npago->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/client.png" ) ) );
        npago->setStatusTip ( _ ( "Nuevo cliente" ) );
        npago->setWhatsThis ( _ ( "Nuevo cliente" ) );
        pPluginMenu->addAction ( npago );
        bges->Fichas->addAction ( npago );
        connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

    }// end if
    blDebug ( "END MyPlugCli::inicializa", 0 );
}





///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de Entrada del plugin de Clientes\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_cliente", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    MyPlugCli *plug = new MyPlugCli();
    plug->inicializa ( bges );
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "cliente", "SELECT" ) ) {
        g_clientesList = new ClientsList ( comp, NULL );
        comp->m_pWorkspace->addWindow ( g_clientesList );
        g_clientesList->hide();
    }// end if
    return 0;
}


int Busqueda_on_mui_buscar_clicked ( BlSearchWidget *busq )
{
    if ( busq->tableName() == "cliente" ) {

        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        centrarEnPantalla ( diag );

        ClientsList *clients = new ClientsList ( ( BfCompany * ) busq->mainCompany(), diag, 0, BL_SELECT_MODE );
        busq->connect ( clients, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

        /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
        /// para que sea redimensionable y aparezca el titulo de la ventana.
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget ( clients );
        layout->setMargin ( 0 );
        layout->setSpacing ( 0 );
        diag->setLayout ( layout );
        diag->setWindowTitle ( clients->windowTitle() );

        diag->exec();

        if ( clients->idclient() != "" ) {
            busq->setId ( clients->idclient() );
        } // end if

        delete diag;
        return 1;
    } // end if

    return 0;
}


/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewClienteView ( BfCompany *v )
{
    ClienteView *h = new ClienteView ( v, 0 );
    g_plugParams = h;
    return 1;
}
