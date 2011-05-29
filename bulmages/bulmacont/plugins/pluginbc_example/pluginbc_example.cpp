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

#include <QMenu>
#include <QAction>
#include <QObject>
#include <QMessageBox>

#include <stdio.h>

#include "pluginbc_example.h"
#include "bccompany.h"
#include "blfunctions.h"


///
/**
**/
PluginBc_Example::PluginBc_Example()
{
    blDebug ( "PluginBc_Example::PluginBc_Example", 0 );
    blDebug ( "PluginBc_Example::PluginBc_Example", 0 );
}


///
/**
**/
PluginBc_Example::~PluginBc_Example()
{
    blDebug ( "PluginBc_Example::~PluginBc_Example", 0 );
    blDebug ( "PluginBc_Example::~PluginBc_Example", 0 );
}


///
/**
**/
void PluginBc_Example::elslot()
{
    blDebug ( "PluginBc_Example::elslot", 0 );
    fprintf ( stderr, "Sa ha activado el slot\n" );
    QMessageBox::warning ( 0,
                           _ ( "Titulo de la ventana" ),
                           ( "Mensaje." ),
                           QMessageBox::Ok,
                           QMessageBox::Cancel );
    blDebug ( "END PluginBc_Example::elslot", 0 );
}


///
/**
\param bcont
**/
void PluginBc_Example::inicializa ( BcBulmaCont *bcont )
{
    blDebug ( "PluginBc_Example::inicializa", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_example", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    /// Creamos el men&uacute;.
    QMenu *pPluginMenu = bcont->newMenu ( _ ( "&Plugin" ), "menuPlugin", "menuHerramientas" );
    QAction *accion = new QAction ( _ ( "&Prueba de plugin BcBulmaCont" ), 0 );
    accion->setStatusTip ( _ ( "Muestra statustip" ) );
    accion->setWhatsThis ( _ ( "Muestra que es esto" ) );
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    pPluginMenu->addAction ( accion );

    blDebug ( "END PluginBc_Example::inicializa", 0 );
}


///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    blDebug ( "Estoy dentro del plugin\n", 0 );
    PluginBc_Example *plug = new PluginBc_Example();
    plug->inicializa ( bcont );
    /// S&Oacute;LO A MODO DE EJEMPLO: se modifica el t&iacute;tulo de la ventana principal
    /// del programa para indicar que el plugin se ha cargado.
    bcont->setWindowTitle ( _ ( "Prueba de plugin BcBulmaCont." ) );
    return 0;
}

