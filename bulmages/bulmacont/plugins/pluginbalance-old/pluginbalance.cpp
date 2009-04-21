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
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>
#include <QTextCodec>
#include <QLocale>

#include "pluginbalance.h"
#include "balanceview.h"
#include "bccompany.h"


///
/**
**/
pluginbalancemyplugin4::pluginbalancemyplugin4()
{
    _depura ( "pluginbalancemyplugin4::pluginbalancemyplugin4", 0 );
    _depura ( "END pluginbalancemyplugin4::pluginbalancemyplugin4", 0 );
}


///
/**
**/
pluginbalancemyplugin4::~pluginbalancemyplugin4()
{
    _depura ( "pluginbalancemyplugin4::~pluginbalancemyplugin4", 0 );
    _depura ( "END pluginbalancemyplugin4::~pluginbalancemyplugin4", 0 );
}


///
/**
**/
void pluginbalancemyplugin4::elslot()
{
    _depura ( "pluginbalancemyplugin4::elslot", 0 );
    BalanceView *cuad = new BalanceView ( ( BcCompany * ) mainCompany(), 0 );
    mainCompany() ->pWorkspace() ->addWindow ( cuad );
    cuad->show();
    _depura ( "END pluginbalancemyplugin4::elslot", 0 );
}


///
/**
\param bges
**/
void pluginbalancemyplugin4::inicializa ( BcBulmaCont *bges )
{
    _depura ( "pluginbalancemyplugin4::inicializa", 0 );
    /// Creamos el men&uacute;.
    setMainCompany ( bges->empresaactual() );
    m_bulmacont = bges;
    QMenu *pPluginMenu;
    /// Miramos si existe un menu Herramientas
    pPluginMenu = bges->menuBar() ->findChild<QMenu *> ( _ ( "menuVentana" ) );

    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
        pPluginMenu = new QMenu ( _ ( "&Ver" ), bges->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuVentana" ) );
    } // end if

    QAction *accion = new QAction ( _ ( "&Balance" ), 0 );
    accion->setStatusTip ( _ ( "Permite realizar balances" ) );
    accion->setWhatsThis ( _ ( "Podra disponer de la informacion del balance" ) );
    accion->setIcon ( QIcon ( QString::fromUtf8 ( ":/BulmaCont32x32/images/png/i_Balance.xpm" ) ) );
    bges->toolBar->addAction ( accion );
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    pPluginMenu->addAction ( accion );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar() ->insertMenu ( bges->menuVentana->menuAction(), pPluginMenu );
    _depura ( "END pluginbalancemyplugin4::inicializa", 0 );
}


///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    _depura ( "entryPoint::entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbalance-old", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    pluginbalancemyplugin4 *plug = new pluginbalancemyplugin4();
    plug->inicializa ( bcont );
    _depura ( "END entryPoint::entryPoint", 0 );
    return 0;
}

