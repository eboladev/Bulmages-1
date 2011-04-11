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

#include "pluginbf_almacen.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "listalmacenview.h"




///
/**
**/
MyPlugAlm::MyPlugAlm()
{
    blDebug ( "MyPlugAlm::MyPlugAlm", 0 );
    blDebug ( "END MyPlugAlm::MyPlugAlm", 0 );
}


///
/**
**/
MyPlugAlm::~MyPlugAlm()
{
    blDebug ( "MyPlugAlm::~MyPlugAlm", 0 );
    blDebug ( "END MyPlugAlm::~MyPlugAlm", 0 );
}


///
/**
**/
void MyPlugAlm::elslot1()
{
    blDebug ( "MyPlugAlm::elslot1", 0 );
    ListAlmacenView * bud = new ListAlmacenView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->show();
    blDebug ( "END MyPlugAlm::elslot1", 0 );
}



///
/**
\param bges
**/
void MyPlugAlm::inicializa ( BfBulmaFact *bges )
{
    blDebug ( "MyPlugAlm::inicializa", 0 );

    if ( bges->company()->hasTablePrivilege ( "almacen", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->menuMaestro;
        pPluginMenu->addSeparator();

        /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
        m_bges = bges;
        setMainCompany ( bges->company() );
        QAction *planCuentas = new QAction ( _ ( "&Almacenes" ), 0 );
        planCuentas->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/warehouse-list.png" ) ) );
        planCuentas->setStatusTip ( _ ( "Almacenes" ) );
        planCuentas->setWhatsThis ( _ ( "Almacenes" ) );
        pPluginMenu->addAction ( planCuentas );
        bges->Listados->addAction ( planCuentas );
        connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

    }// end if
    blDebug ( "END MyPlugAlm::inicializa", 0 );
}





///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de Entrada del plugin de Trabajadors\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_almacen", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    MyPlugAlm *plug = new MyPlugAlm();
    plug->inicializa ( bges );
    return 0;
}



/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewAlmacenView ( BfCompany *v )
{
    ListAlmacenView *h = new ListAlmacenView ( v, 0 );
    g_plugParams = h;
    return 1;
}
