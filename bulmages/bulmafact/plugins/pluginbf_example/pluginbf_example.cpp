/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtCore/QObject>
#include <QtWidgets/QMessageBox>

#include <cstdio>

#include "pluginbf_example.h"
#include "bfcompany.h"
#include "blfunctions.h"

///
/**
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG
    
    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_example", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );
    
    /// S&Oacute;LO A MODO DE EJEMPLO: se modifica el t&iacute;tulo de la ventana principal
    /// del programa para indicar que el plugin se ha cargado.
    bges->setWindowTitle ( _ ( "Prueba de plugin para BfBulmaFact." ) );

    QMenu *pPluginMenu;
    /// Miramos si existe un menu Herramientas
    pPluginMenu = bges->menuBar() ->findChild<QMenu *> ( "Herramientas" );

    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
        pPluginMenu = new QMenu ( _ ( "&Herramientas" ), bges->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "Herramientas" ) );
    } // end if
   
    /// Creamos el men&uacute;.
    BlAction *accionA = new BlAction ( _ ( "&Prueba de plugin" ), 0 );
    accionA->setStatusTip ( _ ( "Muestra statustip" ) );
    accionA->setWhatsThis ( _ ( "Muestra que es esto" ) );
    accionA->setObjectName("mui_actionExample");
    pPluginMenu->addAction ( accionA );
   
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar() ->insertMenu ( bges->menuVentana->menuAction(), pPluginMenu );
    return 0;
}

int BlAction_actionTriggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionExample") {
        fprintf ( stderr, "S'ha activado el slot\n" );
        QMessageBox::warning ( 0,
                _ ( "Titulo de la ventana" ),
                _ ( "Mensaje." ),
                QMessageBox::Ok,
                QMessageBox::Cancel );
        } // end if
    return 0;
}        


// A partir de esta linea, todo procede según el metodo anterior.
/***
 PluginBf_Example::PluginBf_Example()
{
    BL_FUNC_DEBUG
    
}

PluginBf_Example::~PluginBf_Example()
{
    BL_FUNC_DEBUG
    
}

void PluginBf_Example::elslot()
{
    BL_FUNC_DEBUG
    QMessageBox::warning ( 0,
                           _ ( "Titulo de la ventana" ),
                           _ ( "Mensaje." ),
                           QMessageBox::Ok,
                           QMessageBox::Cancel );
    
}


void PluginBf_Example::inicializa ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG
    QMenu *pPluginMenu;
    /// Miramos si existe un menu Herramientas
    pPluginMenu = bges->menuBar() ->findChild<QMenu *> ( "Herramientas" );

    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
        pPluginMenu = new QMenu ( _ ( "&Herramientas" ), bges->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "Herramientas" ) );
    } // end if
    /// Creamos el men&uacute;.

    QAction *accion = new QAction ( _ ( "&Prueba de plugin" ), 0 );
    accion->setStatusTip ( _ ( "Muestra statustip" ) );
    accion->setWhatsThis ( _ ( "Muestra que es esto" ) );
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    pPluginMenu->addAction ( accion );
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar() ->insertMenu ( bges->menuVentana->menuAction(), pPluginMenu );
    
}


int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_example", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    PluginBf_Example *plug = new PluginBf_Example();
    plug->inicializa ( bges );
    /// S&Oacute;LO A MODO DE EJEMPLO: se modifica el t&iacute;tulo de la ventana principal
    /// del programa para indicar que el plugin se ha cargado.
    bges->setWindowTitle ( _ ( "Prueba de plugin para BfBulmaFact." ) );
    return 0;
}
*/
