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

#include "pluginvisor.h"
#include "funcaux.h"
#include "empresatpv.h"
#include "plugins.h"
#include "ticket.h"
#include "blapplication.h"


/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;

QFile *g_file;

///
/**
\return
**/
int entryPoint ( BulmaTPV *tpv )
{
    _depura ( "entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginvisor", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    g_file = new QFile ( confpr->valor ( CONF_TPV_VISOR_FILE ) );
    if ( g_file->open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        g_file->write ( "\x0Ch", 1 );
        g_file->write ( "Ready.", 6 );
        g_file->flush();
        g_file->close();
    } // end if
    _depura ( "END entryPoint", 0 );
    return 0;
}

///
/**
\return
**/
int exitPoint ( BulmaTPV *tpv )
{
    _depura ( "pluginvisor::entryPoint", 0 );
    if ( g_file->open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        g_file->write ( "\x0Ch", 1 );
        g_file->close();
    } // end if
    delete g_file;
    _depura ( "END pluginvisor::entryPoint", 0 );
    return 0;
}

int Ticket_insertarArticulo_Post ( Ticket *tick )
{
    _depura ( "pluginvisor::Ticket_insertarArticulo_Post", 0 );
    if ( g_file->open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        g_file->write ( "\x0Ch", 1 );
        QTextStream out ( g_file );
        out <<   tick->lineaActTicket()->DBvalue ( "codigocompletoarticulo" ).left ( 5 );
        out << " " << tick->lineaActTicket()->DBvalue ( "nomarticulo" ).left ( 10 );
        out << "\n      P.V.P. : " << tick->lineaActTicket()->DBvalue ( "pvplalbaran" );
        g_file->flush();
        g_file->close();
    } // end if
    _depura ( "END pluginvisor::Ticket_insertarArticulo_Post", 0 );
}

int Ticket_total ( QString *total )
{
    if ( g_file->open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        g_file->write ( "\x0Ch", 1 );
        QTextStream out ( g_file );
        out << "Total : " << total;
        g_file->flush();
        g_file->close();
    } // end if
}

