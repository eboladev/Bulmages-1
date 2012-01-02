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
BcBulmaCont *g_pluginbc_diario = NULL;


///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_diario", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_diario = NULL;
    g_pluginbc_diario = bcont;

    QMenu *pPluginMenu = bcont->newMenu( _("&Ver"), "menuVer", "menuMaestro");

    BlAction *accionA = new BlAction ( _ ( "&Libro Diario" ), 0 );
    accionA->setStatusTip ( _ ( "Permite ver el Diario" ) );
    accionA->setWhatsThis ( _ ( "Podra disponer de la informacion del Diario" ) );
    accionA->setIcon(QIcon(QString::fromUtf8(":/Images/balance-sheet.png")));
    accionA->setObjectName("mui_actionDiario");
    pPluginMenu->addAction ( accionA );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bcont->toolBar->addAction ( accionA );

    
    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, _("PluginBc_Diario") );

    if (accion->objectName() == "mui_actionDiario") {
        if (g_diario == NULL) {
            g_diario = new BcDiarioView ( g_pluginbc_diario->company(), 0 );
            g_pluginbc_diario->company()->pWorkspace() ->addSubWindow ( g_diario );
        } // end if
        
	g_diario->hide();
        g_diario->show();
    } // end if

    
    return 0;
}
