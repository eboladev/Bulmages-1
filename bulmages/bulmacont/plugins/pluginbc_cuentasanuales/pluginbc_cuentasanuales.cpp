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
BcBulmaCont *g_pluginbc_cuentasanuales = NULL;

///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_cuentasanuales", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_cuentasAnuales = NULL;

    g_pluginbc_cuentasanuales = bcont;
    /// Creamos la ventana de asientos que va a ser fija.
    g_cuentasAnuales = NULL;

    /// Miramos si existe un menu Herramientas
    QMenu *pPluginMenu = bcont->newMenu ( _("&Cuentas Anuales"), "menuCAnuales", "menuHerramientas" );


    BlAction *accionA = new BlAction ( _ ( "&Cuentas Anuales" ), 0 );
    accionA->setStatusTip ( _ ( "Cuentas anuales" ) );
    accionA->setWhatsThis ( _ ( "Cuentas anuales" ) );
    accionA->setObjectName("mui_actionCuentasAnuales");
    pPluginMenu->addAction ( accionA );
    
    
    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionCuentasAnuales") {
        if (g_cuentasAnuales == NULL) {
            g_cuentasAnuales = new BcCuentasAnualesView ( g_pluginbc_cuentasanuales->company(), 0 );
            g_pluginbc_cuentasanuales->company() ->pWorkspace() -> addSubWindow ( g_cuentasAnuales );
        } // end if
        g_cuentasAnuales->hide();
        g_cuentasAnuales->show();
    } // end if
    return 0;
}
