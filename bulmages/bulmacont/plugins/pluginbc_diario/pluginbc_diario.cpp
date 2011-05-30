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

#include "pluginbc_diario.h"
#include "bcdiarioview.h"
#include "bccompany.h"


BcDiarioView *g_diario;

///
/**
**/
PluginBc_Diario::PluginBc_Diario()
{
    blDebug ( "PluginBc_Diario::PluginBc_Diario", 0 );
    blDebug ( "END PluginBc_Diario::PluginBc_Diario", 0 );
}


///
/**
**/
PluginBc_Diario::~PluginBc_Diario()
{
    blDebug ( "PluginBc_Diario::~PluginBc_Diario", 0 );
    blDebug ( "END PluginBc_Diario::~PluginBc_Diario", 0 );
}


///
/**
**/
void PluginBc_Diario::elslot()
{
    blDebug ( "PluginBc_Diario::elslot", 0 );
    if (g_diario == NULL) {
      g_diario = new BcDiarioView ( ( BcCompany * ) mainCompany(), 0 );
      mainCompany() ->pWorkspace() ->addSubWindow ( g_diario );
    } // end if
    g_diario->hide();
    g_diario->show();
    blDebug ( "END PluginBc_Diario::elslot", 0 );
}


///
/**
\param bges
**/
void PluginBc_Diario::inicializa ( BcBulmaCont *bges )
{
    blDebug ( "PluginBc_Diario::inicializa", 0 );

    /// Creamos el men&uacute;.
    setMainCompany ( bges->company() );
    m_bulmacont = bges;
    QMenu *pPluginMenu = bges->newMenu(_("&Ver"), "menuVer", "menuMaestro");

    QAction *accion = new QAction ( _ ( "&Libro Diario" ), 0 );
    accion->setStatusTip ( _ ( "Permite ver el diario" ) );
    accion->setWhatsThis ( _ ( "Podra disponer de la informacion del diario" ) );
    accion->setIcon(QIcon(QString::fromUtf8(":/Images/balance-sheet.png")));
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    pPluginMenu->addAction ( accion );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->toolBar->addAction ( accion );

    blDebug ( "END PluginBc_Diario::inicializa", 0 );
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
    blBindTextDomain ( "pluginbc_diario", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_diario = NULL;

    PluginBc_Diario *plug = new PluginBc_Diario();
    plug->inicializa ( bcont );
    blDebug ( "END entryPoint::entryPoint", 0 );
    return 0;
}

