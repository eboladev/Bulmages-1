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

#include <QtWidgets/QAction>
#include <QtCore/QObject>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTabWidget>

#include "bfcompany.h"
#include "pluginbf_vehiculo.h"
#include "blfunctions.h"
#include "vehiculoview.h"


///
/**
**/
myplugin::myplugin()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
myplugin::~myplugin()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void myplugin::elslot()
{
    BL_FUNC_DEBUG
    VehiculoView *vehiculoview = new VehiculoView ( ( BfCompany * ) m_dbConnection );
    m_bulmafact->workspace() ->addSubWindow ( vehiculoview );
    vehiculoview->show();
    
}


///
/**
\param bges
**/
void myplugin::inicializa ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG
    /// Creamos el men&uacute;.
    m_dbConnection = bges->company();
    m_bulmafact = bges;
    QMenu *pPluginMenu = new QMenu ( _("&Vehiculos") );
    QAction *accion = new QAction ( _("&Nuevo vehiculo"), 0 );
    accion->setStatusTip ( _("Vehiculos") );
    accion->setWhatsThis ( _("Vehiculos") );
    connect ( accion, SIGNAL ( triggered(bool) ), this, SLOT ( elslot() ) );
    pPluginMenu->addAction ( accion );
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar() ->addMenu ( pPluginMenu );
    
}


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_Vehiculos", "0.5.9");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_vehiculo", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    myplugin *plug = new myplugin();
    plug->inicializa ( bges );
    
    return 0;
}

