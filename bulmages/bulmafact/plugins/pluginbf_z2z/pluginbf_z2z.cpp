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

#include <stdio.h>

#include <QtWidgets/QAction>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtCore/QObject>

#include "pluginbf_z2z.h"
#include "blconfiguration.h"
#include "bfbulmafact.h"
#include "bfcompany.h"
#include "z2zview.h"


BfCompany *g_pluginbf_z2z;

///
/**
\param bcont
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG
    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_z2z", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    g_pluginbf_z2z = bges->company();

    /// Miramos si existe un menu Herramientas
    QMenu *pPluginMenu = bges->newMenu ( _("&Herramientas"), "menuHerramientas", "menuAcerca_de" );

    /// A&ntilde;ade en el men&uacute; del programa la opci&oacuteMn para
    /// acceder al corrector.
    BlAction *accionA = new BlAction ( _("&Traspaso de Z"), 0 );
    accionA->setStatusTip ( _("Traspaso de Z") );
    accionA->setWhatsThis ( _("Traspaso de Z") );
    accionA->setObjectName("mui_actionZ2Z");
    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( accionA );
    
    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, "PluginBf_Z2Z" );

    if (accion->objectName() == "mui_actionZ2Z") {
        Z2ZView * bud = new Z2ZView ( g_pluginbf_z2z, NULL );
        g_pluginbf_z2z->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if
    
    return 0;
}
