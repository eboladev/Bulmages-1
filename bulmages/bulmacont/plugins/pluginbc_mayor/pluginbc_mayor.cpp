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
BcBulmaCont *g_pluginbc_mayor = NULL;

///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_mayor", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_mayor = NULL;

    /// Creamos el men&uacute;.
    g_pluginbc_mayor = bcont;
    QMenu *pPluginMenu = bcont->newMenu( _("&Ver"), "menuVer", "menuMaestro");

    BlAction *accionA = new BlAction ( _ ( "&Libro Mayor" ), 0 );
    accionA->setStatusTip ( _ ( "Permite ver el mayor" ) );
    accionA->setWhatsThis ( _ ( "Podra disponer de la informacion del mayor" ) );
    accionA->setObjectName("mui_actionLibro");
    accionA->setIcon(QIcon(QString::fromUtf8(":/Images/bank-statement.png")));
    bcont->toolBar->addAction ( accionA );
    //connect ( accionA, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    pPluginMenu->addAction ( accionA );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.


    
    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionLibro") {
        if (g_mayor == NULL) {
            g_mayor = new BcExtractoView ( g_pluginbc_mayor->company(), 0 );
            g_pluginbc_mayor->company() ->pWorkspace() ->addSubWindow ( g_mayor );
        } // end if
        g_mayor->hide();
        g_mayor->show();
    } // end if
    return 0;
}
