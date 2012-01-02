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


#include <QApplication>
#include <QObject>
#include <QTextCodec>
#include <QLocale>

#include "pluginbc_balancearbol.h"
#include "balance1view.h"
#include "bccompany.h"


BcBulmaCont *g_pluginbc_balancearbol = NULL;


///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    BL_FUNC_DEBUG

    g_pluginbc_balancearbol = bcont;
    
    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "plugincc_balancearbol", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    QMenu *pPluginMenu = bcont->newMenu( _("&Ver"), "menuVer", "menuMaestro" );

    BlAction *accion = new BlAction ( _ ( "&Balance jerarquico" ), 0 );
    accion->setStatusTip ( _ ( "Permite realizar balances" ) );
    accion->setWhatsThis ( _ ( "Podra disponer de la informacion del balance" ) );
    accion->setIcon ( QIcon ( QString::fromUtf8 ( ":/BulmaCont32x32/images/png/i_arbolBalance.xpm" ) ) );
    accion->setObjectName("mui_actionBalanceJerarquico");
    bcont->toolBar->addAction ( accion );
    pPluginMenu->addAction ( accion );
    
    
    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    if (accion->objectName() == "mui_actionBalanceJerarquico") {
    	BalanceTreeView *cuad = new BalanceTreeView ( g_pluginbc_balancearbol->company(), 0 );
    	g_pluginbc_balancearbol -> company() ->pWorkspace() ->addSubWindow ( cuad );
    	cuad->show();
    } // end if
    
    return 0;    
} // end if
