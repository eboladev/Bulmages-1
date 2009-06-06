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

#include "pluginbf_proveedor.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "provedit.h"
#include "providerslist.h"



ProveedorList *g_providersList = NULL;

///
/**
**/
MyPlugPro::MyPlugPro()
{
    _depura ( "MyPlugPro::MyPlugPro", 0 );
    _depura ( "END MyPlugPro::MyPlugPro", 0 );
}


///
/**
**/
MyPlugPro::~MyPlugPro()
{
    _depura ( "MyPlugPro::~MyPlugPro", 0 );
    _depura ( "END MyPlugPro::~MyPlugPro", 0 );
}


///
/**
**/
void MyPlugPro::elslot()
{
    _depura ( "MyPlugPro::elslot", 0 );
    if ( g_providersList ) {
        g_providersList->hide();
        g_providersList->show();
    }// end if
    _depura ( "END MyPlugPro::elslot", 0 );
}

///
/**
**/
void MyPlugPro::elslot1()
{
    _depura ( "MyPlugPro::elslot1", 0 );
    ProveedorView * bud = new ProveedorView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    _depura ( "END MyPlugPro::elslot1", 0 );
}



///
/**
\param bges
**/
void MyPlugPro::inicializa ( BfBulmaFact *bges )
{
    _depura ( "MyPlugPro::inicializa", 0 );

    if ( bges->company()->hasTablePrivilege ( "proveedor", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( "&Compras", "menuCompras", "menuMaestro" );
        pPluginMenu->addSeparator();
        /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
        m_bges = bges;
        setMainCompany ( bges->company() );
        QAction *planCuentas = new QAction ( _ ( "&Proveedores" ), 0 );
        planCuentas->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier-list.png" ) ) );
        planCuentas->setStatusTip ( _ ( "Proveedores" ) );
        planCuentas->setWhatsThis ( _ ( "Proveedores" ) );
        pPluginMenu->addAction ( planCuentas );
        bges->Listados->addAction ( planCuentas );
        connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

        QAction *npago = new QAction ( _ ( "&Nuevo proveedor" ), 0 );
        npago->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier.png" ) ) );
        npago->setStatusTip ( _ ( "Nuevo proveedor" ) );
        npago->setWhatsThis ( _ ( "Nuevo proveedor" ) );
        pPluginMenu->addAction ( npago );
        bges->Fichas->addAction ( npago );
        connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

    }// end if
    _depura ( "END MyPlugPro::inicializa", 0 );
}



///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    _depura ( "Punto de Entrada del plugin de proveedores\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_proveedor", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    MyPlugPro *plug = new MyPlugPro();
    plug->inicializa ( bges );
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "proveedor", "SELECT" ) ) {
        g_providersList = new ProveedorList ( comp, NULL );
        comp->m_pWorkspace->addWindow ( g_providersList );
        g_providersList->hide();
    }// end if
    return 0;
}


int Busqueda_on_mui_buscar_released ( BlSearchWidget *busq )
{
    if ( busq->tableName() == "proveedor" ) {


        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        centrarEnPantalla ( diag );

        ProveedorList *clients = new ProveedorList ( ( BfCompany * ) busq->mainCompany(), diag, 0, BL_SELECT_MODE );
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
        if ( clients->idprovider() != "" ) {
            busq->setId ( clients->idprovider() );
        } // end if
        delete diag;


        return 1;
    } // end if
    return 0;

}


/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewProveedorView ( BfCompany *v )
{
    ProveedorView *h = new ProveedorView ( v, 0 );
    g_plugParams = h;
    return 1;
}
