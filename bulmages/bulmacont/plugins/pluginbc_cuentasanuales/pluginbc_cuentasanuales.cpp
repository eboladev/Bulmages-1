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
PluginBc_CuentasAnuales::PluginBc_CuentasAnuales()
{
    blDebug ( "PluginBc_CuentasAnuales::PluginBc_CuentasAnuales", 0 );
    blDebug ( "END PluginBc_CuentasAnuales::PluginBc_CuentasAnuales", 0 );
}


///
/**
**/
PluginBc_CuentasAnuales::~PluginBc_CuentasAnuales()
{
    blDebug ( "PluginBc_CuentasAnuales::~PluginBc_CuentasAnuales", 0 );
    blDebug ( "END PluginBc_CuentasAnuales::~PluginBc_CuentasAnuales", 0 );
}


///
/**
**/
void PluginBc_CuentasAnuales::elslot()
{
    blDebug ( "PluginBc_CuentasAnuales::elslot", 0 );
    if (g_cuentasAnuales == NULL) {
      g_cuentasAnuales = new BcCuentasAnualesView ( ( BcCompany * ) mainCompany(), 0 );
      mainCompany() ->pWorkspace() -> addSubWindow ( g_cuentasAnuales );
    } // end if
    g_cuentasAnuales->hide();
    g_cuentasAnuales->show();
    blDebug ( "END PluginBc_CuentasAnuales::elslot", 0 );
}


///
/**
\param bges
**/
void PluginBc_CuentasAnuales::inicializa ( BcBulmaCont *bges )
{
    blDebug ( "PluginBc_CuentasAnuales::inicializa", 0 );

    /// Creamos el men&uacute;.
    setMainCompany ( bges->company() );

    /// Creamos la ventana de asientos que va a ser fija.
    g_cuentasAnuales = NULL;

    m_bulmacont = bges;
    /// Miramos si existe un menu Herramientas
    QMenu *pPluginMenu = bges->newMenu ( _("&Cuentas Anuales"), "menuCAnuales", "menuHerramientas" );


    QAction *accion = new QAction ( _ ( "&Cuentas Anuales" ), 0 );
    accion->setStatusTip ( _ ( "Cuentas anuales" ) );
    accion->setWhatsThis ( _ ( "Cuentas anuales" ) );
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    pPluginMenu->addAction ( accion );


    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
//    bges->menuBar() ->insertMenu ( bges->menuVentana->menuAction(), pPluginMenu );

    blDebug ( "END PluginBc_CuentasAnuales::inicializa", 0 );
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
    blBindTextDomain ( "pluginbc_cuentasanuales", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_cuentasAnuales = NULL;

    PluginBc_CuentasAnuales *plug = new PluginBc_CuentasAnuales();
    plug->inicializa ( bcont );
    blDebug ( "END entryPoint::entryPoint", 0 );
    return 0;
}

