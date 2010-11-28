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

#include "pluginbc_mayor.h"
#include "bcextractoview.h"

BcExtractoView *g_mayor;





///
/**
**/
MyPluginMayor::MyPluginMayor()
{
    blDebug ( "MyPluginMayor::MyPluginMayor", 0 );
    blDebug ( "END MyPluginMayor::MyPluginMayor", 0 );
}


///
/**
**/
MyPluginMayor::~MyPluginMayor()
{
    blDebug ( "MyPluginMayor::~MyPluginMayor", 0 );
    blDebug ( "END MyPluginMayor::~MyPluginMayor", 0 );
}



///
/**
**/
void MyPluginMayor::elslot()
{
    blDebug ( "MyPluginMayor::elslot", 0 );
    if (g_mayor == NULL) {
      g_mayor = new BcExtractoView ( ( BcCompany * ) mainCompany(), 0 );
      mainCompany() ->pWorkspace() ->addSubWindow ( g_mayor );
    } // end if
    g_mayor->hide();
    g_mayor->show();
    blDebug ( "END MyPluginMayor::elslot", 0 );
}



///
/**
\param bges
**/
void MyPluginMayor::inicializa ( BcBulmaCont *bges )
{
    blDebug ( "MyPluginMayor::inicializa", 0 );

    /// Creamos el men&uacute;.
    setMainCompany ( (BlMainCompany *)bges->empresaactual() );
    m_bulmacont = bges;
    QMenu *pPluginMenu;
    /// Miramos si existe un menu Herramientas
    pPluginMenu = bges->menuBar() ->findChild<QMenu *> ( _ ( "menuVer" ) );

    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
        pPluginMenu = new QMenu ( _ ( "&Ver" ), bges->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuVer" ) );
    } // end if

    QAction *accion = new QAction ( _ ( "&Libro Mayor" ), 0 );
    accion->setStatusTip ( _ ( "Permite ver el mayor" ) );
    accion->setWhatsThis ( _ ( "Podra disponer de la informacion del mayor" ) );
    accion->setIcon(QIcon(QString::fromUtf8(":/Images/bank-statement.png")));
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    pPluginMenu->addAction ( accion );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar() ->insertMenu ( bges->menuMaestro->menuAction(), pPluginMenu );
    bges->toolBar->addAction ( accion );

    blDebug ( "END MyPluginMayor::inicializa", 0 );
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
    blBindTextDomain ( "pluginbc_mayor", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_mayor = NULL;

    MyPluginMayor *plug = new MyPluginMayor();
    plug->inicializa ( bcont );

    blDebug ( "END entryPoint::entryPoint", 0 );
    return 0;
}

