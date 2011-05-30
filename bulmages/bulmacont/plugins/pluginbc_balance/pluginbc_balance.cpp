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

BcBulmaCont *g_bcont = NULL;

///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    blDebug ( "entryPoint", 0, "Punto de entrada de PluginBc_Balance" );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_balance", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    g_bcont = bcont;

    QMenu *pPluginMenu = bcont->newMenu( _("&Ver"), "menuVer", "menuMaestro");

    BlAction *accionA = new BlAction ( _ ( "&Balance jerarquico" ), 0 );
    accionA->setStatusTip ( _ ( "Permite realizar balances" ) );
    accionA->setWhatsThis ( _ ( "Podra disponer de la informacion del balance" ) );
    //connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    accionA->setObjectName("mui_actionBalance");
    pPluginMenu->addAction ( accionA );


    blDebug ( "END entryPoint", 0, "Punto de entrada de PluginBc_Balance" );
    return 0;
}


int BlAction_triggered(BlAction *accion) {
    blDebug ("PluginBc_Balance::BlAction_triggered");
    if (accion->objectName() == "mui_actionBalance") {
        BalanceView *cuad = new BalanceView ( ( BcCompany * ) g_bcont->company(), 0 );
        g_bcont->company()->pWorkspace() ->addSubWindow ( cuad );
        cuad->show();
    } // end if
    blDebug ("END PluginBc_Balance::BlAction_triggered");
    return 0;
}

