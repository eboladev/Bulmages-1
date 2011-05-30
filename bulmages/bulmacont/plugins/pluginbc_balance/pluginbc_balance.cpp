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

#include <cstdio>

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>
#include <QTextCodec>
#include <QLocale>

#include "pluginbc_balance.h"
#include "balanceview.h"
#include "bccompany.h"


///
/**
**/
PluginBc_Balance::PluginBc_Balance()
{
    blDebug ( "PluginBc_Balance::PluginBc_Balance", 0 );
    blDebug ( "END PluginBc_Balance::PluginBc_Balance", 0 );
}


///
/**
**/
PluginBc_Balance::~PluginBc_Balance()
{
    blDebug ( "PluginBc_Balance::~PluginBc_Balance", 0 );
    blDebug ( "END PluginBc_Balance::~PluginBc_Balance", 0 );
}


///
/**
**/
void PluginBc_Balance::elslot()
{
    blDebug ( "PluginBc_Balance::elslot", 0 );
    BalanceView *cuad = new BalanceView ( ( BcCompany * ) mainCompany(), 0 );
    mainCompany() ->pWorkspace() ->addSubWindow ( cuad );
    cuad->show();
    blDebug ( "END PluginBc_Balance::elslot", 0 );
}


///
/**
\param bges
**/
void PluginBc_Balance::inicializa ( BcBulmaCont *bges )
{
    blDebug ( "PluginBc_Balance::inicializa", 0 );
    /// Creamos el men&uacute;.
    setMainCompany ( bges->company() );
    m_bulmacont = bges;
    QMenu *pPluginMenu = bges->newMenu("&Ver", "menuVer", "menuMaestro");

    QAction *accion = new QAction ( _ ( "&Balance jerarquico" ), 0 );
    accion->setStatusTip ( _ ( "Permite realizar balances" ) );
    accion->setWhatsThis ( _ ( "Podra disponer de la informacion del balance" ) );
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    pPluginMenu->addAction ( accion );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
//    bges->toolBar->addAction ( accion );
    blDebug ( "END PluginBc_Balance::inicializa", 0 );
}



///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    blDebug ( "entryPoint::entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_balance", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    PluginBc_Balance *plug = new PluginBc_Balance();
    plug->inicializa ( bcont );
    blDebug ( "END entryPoint::entryPoint", 0 );
    return 0;
}

