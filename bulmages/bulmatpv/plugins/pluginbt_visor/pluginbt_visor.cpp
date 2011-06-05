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

#include "pluginbt_visor.h"
#include "blfunctions.h"
#include "btcompany.h"
#include "blplugins.h"
#include "btticket.h"
#include "blapplication.h"


/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;
QFile *g_file;


///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    blDebug ( "entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginvisor", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_file = new QFile ( g_confpr->value( CONF_TPV_VISOR_FILE ) );
    if ( g_file->open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        g_file->write ( "\x0Ch", 1 );
        g_file->write ( "Gracias por visitarnos.", 6 );
        g_file->flush();
        g_file->close();
    } // end if
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return 0;
}

///
/**
\return
**/
int exitPoint ( BtBulmaTPV *tpv )
{
    blDebug ( "pluginvisor::entryPoint", 0 );
    if ( g_file->open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        g_file->write ( "\x0Ch", 1 );
        g_file->close();
    } // end if
    delete g_file;
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return 0;
}

int BtTicket_insertarArticulo_Post ( BtTicket *tick )
{
    blDebug ( "pluginvisor::Ticket_insertarArticulo_Post", 0 );
    if ( g_file->open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        g_file->write ( "\x0Ch", 1 );
        g_file->write ( tick->lineaActBtTicket()->dbValue ( "codigocompletoarticulo" ).leftJustified ( 4, ' ', TRUE ).toAscii());
        g_file->write ( " " );	
        g_file->write ( tick->lineaActBtTicket()->dbValue ( "nomarticulo" ).leftJustified ( 15, ' ', TRUE ).toAscii());
        g_file->write ( "P.V.P. : ");
	g_file->write ( tick->lineaActBtTicket()->dbValue ( "pvpivainclalbaran" ).toAscii());
        g_file->flush();
        g_file->close();
    } // end if
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return 0;
}



/// Version para iva incluido.

int BtTicket_imprimir ( BtTicket *tick )
{
    if ( g_file->open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {

      
      
        base basesimp;
    base basesimpreqeq;
    BlDbRecord *linea;
    int precision=0;
    int maxprecision=0;
    
    /// Impresion de los contenidos.
    QString l;
    BlFixed total ( "0.00" );
    BlFixed descuentolinea ( "0.00" );
    for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
        if (linea = tick->listaLineas() ->at ( i ) ) {
            BlFixed cant ( linea->dbValue ( "cantlalbaran" ) );
            BlFixed pvpund ( linea->dbValue ( "pvpivainclalbaran" ) );
            BlFixed cantpvp = cant * pvpund;
            precision = cant.precision() > pvpund.precision() ? cant.precision() : pvpund.precision();
            maxprecision = precision > maxprecision ? precision : maxprecision;
            total = total + cantpvp;
        } // end if
    } // end for

      
      
      
      
      g_file->write ( "\x0Ch", 1 );
        QTextStream out ( g_file );
        out << "Total : " << total.toQString('0', maxprecision);
        g_file->flush();
        g_file->close();
    } // end if
    
    
    
    // ========================

    
    
    return 0;
}

