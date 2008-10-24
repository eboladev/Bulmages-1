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

#include "pluginalias1.h"
#include "funcaux.h"
#include "empresatpv.h"



int Ticket_agregarLinea_Post ( Ticket *tick, DBRecord * &item )
{
    return 0;
}



int Ticket_insertarArticuloCodigoNL_Post ( Ticket *tick )
{
	cursor2 *cur=0;

    QString query = "SELECT * FROM alias WHERE cadalias = '" + ( ( EmpresaTPV * ) tick->empresaBase() )->valorInput() + "'";

    cur = tick->empresaBase() ->cargacursor ( query );
    if (cur) {
	if ( !cur->eof() ) {
		tick->insertarArticulo ( cur->valor ( "idarticulo" ), Fixed ( "1" ) );
	} // end if
	
		
	delete cur;
    } // end if

    return 0;
}



int Ticket_insertarArticuloCodigo_Post ( Ticket *tick )
{
    int valor = -1;
    _depura ( "pluginalias1::Ticket_insertarArticulo_Post", 0 );
    static int semaforo = 0;
    if ( semaforo == 0 ) {
        semaforo = 1;
        QString query = "SELECT * FROM alias WHERE cadalias = '" + ( ( EmpresaTPV * ) tick->empresaBase() )->valorInput() + "'";
        cursor2 *cur = tick->empresaBase() ->cargacursor ( query );
        if ( !cur->eof() ) {
            tick->insertarArticulo ( cur->valor ( "idarticulo" ), Fixed ( "1" ) );
        } // end if
        delete cur;
        valor = 0;
        semaforo = 0;
    } // end if
    _depura ( "END pluginalias1::Ticket_insertarArticulo_Post", 0 );
    return valor;
}
