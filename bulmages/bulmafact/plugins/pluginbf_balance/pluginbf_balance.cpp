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

#include <QtWidgets/QAction>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtCore/QObject>
#include <QtCore/QTextCodec>
#include <QtCore/QLocale>

#include "pluginbf_balance.h"
#include "balanceview.h"
#include "bfcompany.h"


BfBulmaFact *g_pluginbf_balance = NULL;


///
/**
\param bcont
**/
int entryPoint ( BfBulmaFact *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_balance", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );
    g_pluginbf_balance = bcont;

    QMenu *pPluginMenu = bcont->newMenu( _("&Ver"), "menuVer", "menuMaestro");

    BlAction *accionA = new BlAction ( _ ( "&Balance" ), 0 );
    accionA->setStatusTip ( _ ( "Permite realizar balances" ) );
    accionA->setWhatsThis ( _ ( "Podra disponer de la informacion del balance" ) );
    accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/BulmaCont32x32/images/png/i_balance.xpm" ) ) );
    accionA->setObjectName("mui_actionBalance");
    pPluginMenu->addAction ( accionA );
    
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    /// Usamos un toolBox especial para meter los botones de contabilidad.
    QToolBar *toolCont =  bcont->findChild<QToolBar *> ( "contabilidad" );
    if ( !toolCont) {
	toolCont = new QToolBar(bcont);
	toolCont->setObjectName("contabilidad");
	toolCont->setFocusPolicy(Qt::TabFocus);
	toolCont->setOrientation(Qt::Horizontal);
	toolCont->setIconSize(QSize(32, 32));
        toolCont->setWindowTitle(N_("Contabilidad", 0));
        toolCont->setToolTip(N_("Contabilidad", 0));
        toolCont->setStatusTip(N_("Contabilidad", 0));
        toolCont->setWhatsThis(N_("Contabilidad", 0));
        toolCont->setAccessibleName(N_("Contabilidad", 0));
        toolCont->setAccessibleDescription(N_("Contabilidad", 0));
	bcont->addToolBar(Qt::TopToolBarArea, toolCont);
    } // end if
    toolCont->addAction(accionA);
    
    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG

    if (accion->objectName() == "mui_actionBalance") {

	if (!g_pluginbf_balance->company()->showWindow("BalanceView")) {
	    BalanceView *balance = new BalanceView ( g_pluginbf_balance->company(), 0 );
	    balance->setObjectName("BalanceView");
	    g_pluginbf_balance->company()->pWorkspace() ->addSubWindow ( balance );
	    balance->show();
	} // end if

    } // end if

    return 0;
}

