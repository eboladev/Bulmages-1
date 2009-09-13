/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
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

#include "pluginbf_modificadores.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "listmodificadoresview.h"




///
/**
**/
MyPlugModificadores::MyPlugModificadores()
{
    _depura ( "MyPlugModificadores::MyPlugModificadores", 0 );
    _depura ( "END MyPlugModificadores::MyPlugModificadores", 0 );
}


///
/**
**/
MyPlugModificadores::~MyPlugModificadores()
{
    _depura ( "MyPlugModificadores::~MyPlugModificadores", 0 );
    _depura ( "END MyPlugModificadores::~MyPlugModificadores", 0 );
}


///
/**
**/
void MyPlugModificadores::elslot1()
{
    _depura ( "MyPlugModificadores::elslot1", 0 );
    ListModificadoresView * bud = new ListModificadoresView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    _depura ( "END MyPlugModificadores::elslot1", 0 );
}



///
/**
\param bges
**/
void MyPlugModificadores::inicializa ( BfBulmaFact *bges )
{
    _depura ( "MyPlugModificadores::inicializa", 0 );

    if ( bges->company()->hasTablePrivilege ( "serie_factura", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->menuMaestro;
        pPluginMenu->addSeparator();

        m_bges = bges;
        setMainCompany ( bges->company() );
        QAction *seriesFactura = new QAction ( _ ( "&Series de factura" ), 0 );
        seriesFactura->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/client-invoice-series.png" ) ));
        seriesFactura->setStatusTip ( _ ( "Series de factura" ) );
        seriesFactura->setWhatsThis ( _ ( "Series de factura" ) );
        pPluginMenu->addAction ( seriesFactura );
        bges->Listados->addAction ( seriesFactura );
        connect ( seriesFactura, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

    }// end if
    _depura ( "END MyPlugModificadores::inicializa", 0 );
}





///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    _depura ( "Punto de Entrada del plugin de Modificadores\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_modificadores", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    MyPlugModificadores *plug = new MyPlugModificadores();
    plug->inicializa ( bges );
    return 0;
}



