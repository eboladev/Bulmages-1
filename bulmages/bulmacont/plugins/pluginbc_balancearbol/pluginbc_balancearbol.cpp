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

#include "pluginbc_balancearbol.h"
#include "balance1view.h"
#include "bccompany.h"


///
/**
**/
myplugin4::myplugin4()
{
    blDebug ( "myplugin4::myplugin4", 0 );
    blDebug ( "END myplugin4::myplugin4", 0 );
}


///
/**
**/
myplugin4::~myplugin4()
{
    blDebug ( "myplugin4::~myplugin4", 0 );
    blDebug ( "END myplugin4::~myplugin4", 0 );
}


///
/**
**/
void myplugin4::elslot()
{
    blDebug ( "myplugin4::elslot", 0 );
    BalanceTreeView *cuad = new BalanceTreeView ( ( BcCompany * ) mainCompany(), 0 );
    mainCompany() ->pWorkspace() ->addSubWindow ( cuad );
    cuad->show();
    blDebug ( "END myplugin4::elslot", 0 );
}


///
/**
\param bges
**/
void myplugin4::inicializa ( BcBulmaCont *bges )
{
    blDebug ( "myplugin4::inicializa", 0 );

    /// Creamos el men&uacute;.
    setMainCompany ( bges->empresaactual() );
    m_bulmacont = bges;
    QMenu *pPluginMenu = bges->newMenu(_("&Ver"), "menuVer", "menuMaestro");

    QAction *accion = new QAction ( _ ( "&Balance jerarquico" ), 0 );
    accion->setStatusTip ( _ ( "Permite realizar balances" ) );
    accion->setWhatsThis ( _ ( "Podra disponer de la informacion del balance" ) );
    accion->setIcon ( QIcon ( QString::fromUtf8 ( ":/BulmaCont32x32/images/png/i_arbolBalance.xpm" ) ) );
    bges->toolBar->addAction ( accion );
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    pPluginMenu->addAction ( accion );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    blDebug ( "END myplugin4::inicializa", 0 );
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
    blBindTextDomain ( "plugincc_balancearbol", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    myplugin4 *plug = new myplugin4();
    plug->inicializa ( bcont );
    blDebug ( "END entryPoint::entryPoint", 0 );
    return 0;
}

