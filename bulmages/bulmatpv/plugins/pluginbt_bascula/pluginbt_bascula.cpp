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

#include <QFile>
#include <QTextStream>

#include "pluginbt_bascula.h"
#include "blfunctions.h"
#include "btcompany.h"
#include "blplugins.h"
#include "btticket.h"
#include "blapplication.h"

#include "portlistener.h"

/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;
PortListener *g_listener = NULL;


///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    blDebug ( "entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbascula", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );


    QString portName = g_confpr->valor ( CONF_TPV_BASCULA_FILE );              // update this to use your port of choice
    g_listener = new PortListener(portName, tpv->empresaTPV());
    
    blDebug ( "END entryPoint", 0 );
    return 0;
}

///
/**
\return
**/
int exitPoint ( BtBulmaTPV *tpv )
{
    blDebug ( "pluginbascula::entryPoint", 0 );

    blDebug ( "END pluginbascula::entryPoint", 0 );
    return 0;
}