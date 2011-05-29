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
#include "pluginbf_vehiculo.h"
#include "blfunctions.h"
#include "vehiculoview.h"


///
/**
**/
myplugin::myplugin()
{
    blDebug ( "myplugin::myplugin", 0 );
    blDebug ( "END myplugin::myplugin", 0 );
}


///
/**
**/
myplugin::~myplugin()
{
    blDebug ( "myplugin::~myplugin", 0 );
    blDebug ( "END myplugin::~myplugin", 0 );
}


///
/**
**/
void myplugin::elslot()
{
    blDebug ( "myplugin::elslot", 0 );
    VehiculoView *vehiculoview = new VehiculoView ( ( BfCompany * ) m_dbConnection );
    m_bulmafact->workspace() ->addSubWindow ( vehiculoview );
    vehiculoview->show();
    blDebug ( "END myplugin::elslot", 0 );
}


///
/**
\param bges
**/
void myplugin::inicializa ( BfBulmaFact *bges )
{
    blDebug ( "myplugin::inicializa", 0 );
    /// Creamos el men&uacute;.
    m_dbConnection = bges->company();
    m_bulmafact = bges;
    QMenu *pPluginMenu = new QMenu ( _("&Vehiculos") );
    QAction *accion = new QAction ( _("&Nuevo vehiculo"), 0 );
    accion->setStatusTip ( _("Vehiculos") );
    accion->setWhatsThis ( _("Vehiculos") );
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    pPluginMenu->addAction ( accion );
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar() ->addMenu ( pPluginMenu );
    blDebug ( "END myplugin::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "entryPoint", 0, "Punto de Entrada del plugin PluginVehiculosBF" );

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_Vehiculos", "0.5.9");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_vehiculo", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    myplugin *plug = new myplugin();
    plug->inicializa ( bges );
    blDebug ( "END entryPoint", 0, "Punto de Entrada del plugin PluginVehiculosBF" );
    return 0;
}

