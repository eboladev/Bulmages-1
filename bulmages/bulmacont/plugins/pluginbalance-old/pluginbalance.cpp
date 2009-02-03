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
#include <QTranslator>
#include <QTextCodec>
#include <QLocale>

#include "pluginbalance.h"
#include "balanceview.h"
#include "empresa.h"







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
    BalanceView *cuad = new BalanceView ( ( Empresa * ) empresaBase(), 0 );
    empresaBase() ->pWorkspace() ->addWindow ( cuad );
    cuad->show();
    _depura ( "END pluginbalancemyplugin4::elslot", 0 );
}


///
/**
\param bges
**/
void pluginbalancemyplugin4::inicializa ( Bulmacont *bges )
{
    _depura ( "pluginbalancemyplugin4::inicializa", 0 );
    /// Creamos el men&uacute;.
    setEmpresaBase ( bges->empresaactual() );
    m_bulmacont = bges;
    QMenu *pPluginMenu;
    /// Miramos si existe un menu Herramientas
    pPluginMenu = bges->menuBar() ->findChild<QMenu *> ( _( "menuVentana" ) );

    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
        pPluginMenu = new QMenu ( _( "&Ver" ), bges->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuVentana" ) );
    } // end if

    QAction *accion = new QAction ( "&Balance", 0 );
    accion->setStatusTip ( "Permite realizar Balances" );
    accion->setWhatsThis ( "Podra disponer de la informacion del balance" );
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
void entryPoint ( Bulmacont *bcont )
{
    _depura ( "entryPoint::entryPoint", 0 );
    /// Cargamos el sistema de traducciones una vez pasado por las configuraciones generales
    QTranslator *traductor = new QTranslator ( 0 );
    if ( confpr->valor ( CONF_TRADUCCION ) == "locales" ) {
        traductor->load ( QString ( "pluginbalance_" ) + QLocale::system().name(),
                          confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
    } else {
        QString archivo = "pluginbalance_" + confpr->valor ( CONF_TRADUCCION );
        traductor->load ( archivo, confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
    } // end if
    theApp->installTranslator ( traductor );

    pluginbalancemyplugin4 *plug = new pluginbalancemyplugin4();
    plug->inicializa ( bcont );
    _depura ( "END entryPoint::entryPoint", 0 );
}

