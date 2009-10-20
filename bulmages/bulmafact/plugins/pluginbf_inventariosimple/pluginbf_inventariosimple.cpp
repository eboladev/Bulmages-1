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

#include "pluginbf_inventariosimple.h"
#include "listinventariosimpleview.h"
#include "bfcompany.h"
#include "blfunctions.h"



///
/**
**/
myplugininvsimple::myplugininvsimple()
{
    _depura ( "myplugininvsimple::myplugininvsimple", 0 );
    _depura ( "END myplugininvsimple::myplugininvsimple", 0 );
}


///
/**
**/
myplugininvsimple::~myplugininvsimple()
{
    _depura ( "myplugininvsimple::~myplugininvsimple", 0 );
    _depura ( "END myplugininvsimple::~myplugininvsimple", 0 );
}


///
/**
**/
void myplugininvsimple::elslot()
{
    _depura ( "myplugininvsimple::elslot", 0 );

    ListInventarioSimpleView *tar = new ListInventarioSimpleView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addWindow ( tar );
    tar->show();

    _depura ( "END myplugininvsimple::elslot", 0 );
}


///
/**
\param bges
**/
void myplugininvsimple::inicializa ( BfBulmaFact *bges )
{
    _depura ( "myplugininvsimple::inicializa", 0 );
    /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
    m_bges = bges;
    setMainCompany ( bges->company() );

    /// Miramos si existe un menu Articulos
    QMenu *pPluginMenu = bges->newMenu ( _("&Articulos"), "menuArticulos", "menuMaestro" );
    pPluginMenu->addSeparator();


    QAction *planCuentas = new QAction ( _ ( "&Inventarios" ), 0 );
    planCuentas->setStatusTip ( _ ( "Inventarios" ) );
    planCuentas->setWhatsThis ( _ ( "Inventarios" ) );

    pPluginMenu->addAction ( planCuentas );
    connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    _depura ( "END myplugininvsimple::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    _depura ( "Punto de Entrada del plugin de Inventario Simple\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_inventariosimple", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    myplugininvsimple *plug = new myplugininvsimple();
    plug->inicializa ( bges );
    return 0;
}



