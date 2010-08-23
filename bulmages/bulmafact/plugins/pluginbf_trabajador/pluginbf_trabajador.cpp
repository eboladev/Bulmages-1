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

#include "pluginbf_trabajador.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "trabajadorview.h"




///
/**
**/
MyPlugTrab::MyPlugTrab()
{
    blDebug ( "MyPlugTrab::MyPlugTrab", 0 );
    blDebug ( "END MyPlugTrab::MyPlugTrab", 0 );
}


///
/**
**/
MyPlugTrab::~MyPlugTrab()
{
    blDebug ( "MyPlugTrab::~MyPlugTrab", 0 );
    blDebug ( "END MyPlugTrab::~MyPlugTrab", 0 );
}


///
/**
**/
void MyPlugTrab::elslot1()
{
    blDebug ( "MyPlugTrab::elslot1", 0 );
    TrabajadorView * bud = new TrabajadorView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    blDebug ( "END MyPlugTrab::elslot1", 0 );
}



///
/**
\param bges
**/
void MyPlugTrab::inicializa ( BfBulmaFact *bges )
{
    blDebug ( "MyPlugTrab::inicializa", 0 );

    if ( bges->company()->hasTablePrivilege ( "trabajador", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->menuMaestro;
        pPluginMenu->addSeparator();

        /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
        m_bges = bges;
        setMainCompany ( bges->company() );
        QAction *planCuentas = new QAction ( _ ( "&Trabajadores" ), 0 );
        planCuentas->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/employee-list.png" ) ) );
        planCuentas->setStatusTip ( _ ( "Trabajadores" ) );
        planCuentas->setWhatsThis ( _ ( "Trabajadores" ) );
        pPluginMenu->addAction ( planCuentas );
        bges->Listados->addAction ( planCuentas );
        connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

    }// end if
    blDebug ( "END MyPlugTrab::inicializa", 0 );
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
    bindtextdomain ( "pluginbf_trabajador", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    MyPlugTrab *plug = new MyPlugTrab();
    plug->inicializa ( bges );
    return 0;
}



/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewTrabajadorView ( BfCompany *v )
{
    TrabajadorView *h = new TrabajadorView ( v, 0 );
    g_plugParams = h;
    return 1;
}
