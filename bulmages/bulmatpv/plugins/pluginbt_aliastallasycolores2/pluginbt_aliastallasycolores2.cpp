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

#include <QDockWidget>

#include "pluginbt_aliastallasycolores2.h"
#include "blfunctions.h"
#include "btcompany.h"


int Ticket_agregarLinea_Post ( BtTicket *tick, BlDbRecord * &item )
{

    mensajeInfo ( "hola" );
    item->addDbField ( "idtc_talla", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Talla" ) );
    item->addDbField ( "idtc_color", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Color" ) );

    return 0;
}



int Ticket_insertarArticuloNL_Post ( BtTicket *tick )
{
    QString query = "SELECT * FROM tc_articulo_alias WHERE aliastc_articulo_tallacolor = '" + ( ( BtCompany * ) tick->mainCompany() )->valorBtInput() + "'";
    BlDbRecordSet *cur = tick->mainCompany() ->loadQuery ( query );
    if ( !cur->eof() ) {
        tick->insertarArticulo ( cur->valor ( "idarticulo" ), BlFixed ( "1" ) );
    } // end if
    delete cur;

    return 0;
}



int Ticket_insertarArticulo_Post ( BtTicket *tick )
{
    int valor = -1;
    _depura ( "PluginBt_AliasTallasYColores2::Ticket_insertarArticulo_Post", 0 );
    static int semaforo = 0;
    if ( semaforo == 0 ) {
        semaforo = 1;
        QString query = "SELECT * FROM tc_articulo_alias WHERE aliastc_articulo_tallacolor = '" + ( ( BtCompany * ) tick->mainCompany() )->valorBtInput() + "'";
        BlDbRecordSet *cur = tick->mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            tick->insertarArticulo ( cur->valor ( "idarticulo" ), BlFixed ( "1" ) );
        } // end if
        delete cur;
        valor = 0;
        semaforo = 0;
    } // end if
    _depura ( "END PluginBt_AliasTallasYColores2::Ticket_insertarArticulo_Post", 0 );
    return valor;
}