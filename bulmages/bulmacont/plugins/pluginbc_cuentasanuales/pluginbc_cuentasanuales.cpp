/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
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
#include <QInputDialog>

#include "pluginbc_cuentasanuales.h"
#include "bccompany.h"
#include "bccuentasanualesview.h"

BcCuentasAnualesView *g_cuentasAnuales;



///
/**
**/
MyPluginCuentasAnuales::MyPluginCuentasAnuales()
{
    blDebug ( "MyPluginCuentasAnuales::MyPluginCuentasAnuales", 0 );
    blDebug ( "END MyPluginCuentasAnuales::MyPluginCuentasAnuales", 0 );
}


///
/**
**/
MyPluginCuentasAnuales::~MyPluginCuentasAnuales()
{
    blDebug ( "MyPluginCuentasAnuales::~MyPluginCuentasAnuales", 0 );
    blDebug ( "END MyPluginCuentasAnuales::~MyPluginCuentasAnuales", 0 );
}


///
/**
**/
void MyPluginCuentasAnuales::elslot()
{
    blDebug ( "MyPluginCuentasAnuales::elslot", 0 );
    if (g_cuentasAnuales == NULL) {
      g_cuentasAnuales = new BcCuentasAnualesView ( ( BcCompany * ) mainCompany(), 0 );
      mainCompany() ->pWorkspace() -> addWindow ( g_cuentasAnuales );
    } // end if
    g_cuentasAnuales->hide();
    g_cuentasAnuales->show();
    blDebug ( "END MyPluginCuentasAnuales::elslot", 0 );
}


///
/**
\param bges
**/
void MyPluginCuentasAnuales::inicializa ( BcBulmaCont *bges )
{
    blDebug ( "MyPluginCuentasAnuales::inicializa", 0 );

    /// Creamos el men&uacute;.
    setMainCompany ( bges->empresaactual() );

    /// Creamos la ventana de asientos que va a ser fija.
    g_cuentasAnuales = NULL;

    m_bulmacont = bges;
    QMenu *pPluginMenu;
    /// Miramos si existe un menu Herramientas
    pPluginMenu = bges->menuBar() ->findChild<QMenu *> ( _ ( "menuListados" ) );

    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
        pPluginMenu = new QMenu ( _ ( "&Listados" ), bges->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuListados" ) );
    } // end if

    QAction *accion = new QAction ( _ ( "&Cuentas Anuales" ), 0 );
    accion->setStatusTip ( _ ( "Cuentas anuales" ) );
    accion->setWhatsThis ( _ ( "Cuentas anuales" ) );
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    pPluginMenu->addAction ( accion );


    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar() ->insertMenu ( bges->menuVentana->menuAction(), pPluginMenu );

    blDebug ( "END MyPluginCuentasAnuales::inicializa", 0 );
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
    bindtextdomain ( "pluginbc_cuentasanuales", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_cuentasAnuales = NULL;

    MyPluginCuentasAnuales *plug = new MyPluginCuentasAnuales();
    plug->inicializa ( bcont );
    blDebug ( "END entryPoint::entryPoint", 0 );
    return 0;
}

