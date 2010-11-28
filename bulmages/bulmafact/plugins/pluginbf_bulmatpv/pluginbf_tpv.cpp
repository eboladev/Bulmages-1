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

#include <QAction>
#include <QObject>
#include <QMessageBox>
#include <QLineEdit>
#include <QTabWidget>

#include "bfcompany.h"
#include "pluginbf_tpv.h"
#include "blfunctions.h"
#include "zlist.h"
#include "blform.h"

///
/**
**/
myplugincont::myplugincont()
{
    blDebug ( "myplugincont::myplugincont", 0 );
    blDebug ( "END myplugincont::myplugincont", 0 );
}

///
/**
**/
myplugincont::~myplugincont()
{
    blDebug ( "myplugincont::~myplugincont", 0 );
    blDebug ( "END myplugincont::~myplugincont", 0 );
}


///
/**
**/
void myplugincont::elslot()
{
    blDebug ( "myplugincont::elslot", 0 );
    ZList *vehiculoview = new ZList ( ( BfCompany * ) m_dbConnection );
    m_bulmafact->workspace() ->addSubWindow ( vehiculoview );
    vehiculoview->show();
    blDebug ( "END myplugincont::elslot", 0 );
}


///
/**
\param bges
**/
void myplugincont::inicializa ( BfBulmaFact *bges )
{
    blDebug ( "myplugincont::inicializa", 0 );

    /// Miramos si existe un menu Ventas
    QMenu *pPluginMenu = NULL;
    pPluginMenu = bges->menuBar() ->findChild<QMenu *> ( "menuVentas" );
    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
        QMenu *pPluginVentas = NULL;
        pPluginVentas = bges->menuBar() ->findChild<QMenu *> ( "menuMaestro" );
        pPluginMenu = new QMenu ( "&Ventas", bges->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuVentas" ) );
        bges->menuBar()->insertMenu ( pPluginVentas->menuAction(), pPluginMenu );
    } // end if

    /// Creamos el men&uacute;.
    m_dbConnection = bges->company();
    m_bulmafact = bges;
    QAction *accion = new QAction ( "&Cuadres de Caja", 0 );
    accion->setStatusTip ( "Listado de Cuadres de Caja" );
    accion->setWhatsThis ( "Listado de Cuadres de Caja" );
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( accion );
    blDebug ( "END myplugincont::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "entryPoint", 0, "Punto de Entrada del plugin PluginContratos" );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_tpv", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    myplugincont *plug = new myplugincont();
    plug->inicializa ( bges );
    blDebug ( "END entryPoint", 0, "Punto de Entrada del plugin PluginContratos" );
    return 0;
}

