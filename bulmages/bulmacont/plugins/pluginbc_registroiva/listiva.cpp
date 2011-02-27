/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#ifdef TOMEU

#include "listiva.h"
#include "bccompany.h"
#include "blfunctions.h"


///
/**
\param comp
**/
ListIva::ListIva ( BcCompany *comp )
{
    blDebug ( "ListIva::ListIva", 0 );
    companyact = comp;
    mdb_idregistroiva = "";
    blDebug ( "END ListIva::ListIva", 0 );
}


///
/**
**/
ListIva::ListIva()
{
    blDebug ( "Constructor de ListIva\n", 0 );
    companyact = NULL;
    mdb_idregistroiva = "";
}


///
/**
**/
ListIva::~ListIva()
{
    blDebug ( "ListIva::~ListIva", 0 );
    blDebug ( "ListIva::~ListIva", 0 );
}


///
/**
\param pos
\return
**/
Iva *ListIva::linpos ( int pos )
{
    blDebug ( "ListIva::linpos", 0 );
    blDebug ( "END ListIva::linpos", 0 );
    return m_lista.at ( pos );
}


/// Carga l&iacute;neas de presupuesto.
/**
\param idregistroiva
\return
**/
int ListIva::cargaListIva ( QString idregistroiva )
{
    blDebug ( "ListIva::cargaListIva\n", 0 );
    int error = 0;
    vaciar();
    mdb_idregistroiva = idregistroiva;

    BlDbRecordSet * cur = companyact->loadQuery ( "SELECT * FROM  tipoiva LEFT JOIN (SELECT * FROM iva WHERE idregistroiva = " + idregistroiva + " ) AS t1 ON t1.idtipoiva = tipoiva.idtipoiva LEFT JOIN cuenta on tipoiva.idcuenta = cuenta.idcuenta  ORDER BY codigo" );
    if ( cur->error() ) {
        error = 1;
    } // end if
    while ( !cur->eof() ) {
        /// Creamos un elemento del tipo linpresupuesto y lo agregamos a la lista.
        Iva * lin = new Iva ( companyact, cur );
        m_lista.append ( lin );
        cur->nextRecord();
    } // end while
    delete cur;

    /// Tratamiento de excepciones.
    if ( error ) {
        blDebug ( "Error en la carga de Iva\n", 0 );
        return 1;
    } // end if

    blDebug ( "END ListIva::cargaListIva\n", 0 );
    return 0;
}


///
/**
**/
void ListIva::guardaListIva()
{
    blDebug ( "ListIva::guardaListIva", 0 );
    Iva *linea;

    QMutableListIterator<Iva*> m_ilista ( m_lista );
    /// Vamos delante del primer elemento de la lista.
    m_ilista.toFront();
    /// Comprobamos que el primer elemento y siguientes existan.
    while ( m_ilista.hasNext() ) {
        /// Si existe el elemento nos desplazamos a el moviendo el cursor.
        linea = m_ilista.next();
        if ( linea->baseiva() != "" ) {
            linea->guardaIva();
        } // end if
    } // end while

    blDebug ( "END ListIva::guardaListIva", 0 );
}


///
/**
\return
**/
BlFixed ListIva::calculabase()
{
    blDebug ( "ListIva::calculabase", 0 );
    BlFixed base ( "0.00" );
    Iva *linea;

    QMutableListIterator<Iva*> m_ilista ( m_lista );
    /// Vamos delante del primer elemento de la lista.
    m_ilista.toFront();
    /// Comprobamos que el primer elemento y siguientes existan.
    while ( m_ilista.hasNext() ) {
        /// Si existe el elemento nos desplazamos a el moviendo el cursor.
        linea = m_ilista.next();
        base = base + BlFixed ( linea->baseiva() );
    } // end while
    blDebug ( "END ListIva::calculabase", 0 );
    return base;
}


///
/**
\return
**/
BlFixed ListIva::calculaiva()
{
    blDebug ( "ListIva::calculaiva", 0 );
    BlFixed iva ( "0.00" );
    Iva *linea;

    QMutableListIterator<Iva*> m_ilista ( m_lista );
    /// Vamos delante del primer elemento de la lista.
    m_ilista.toFront();
    /// Comprobamos que el primer elemento y siguientes existan.
    while ( m_ilista.hasNext() ) {
        /// Si existe el elemento nos desplazamos a el moviendo el cursor.
        linea = m_ilista.next();
        iva = iva + BlFixed ( linea->ivaiva() );
    } // end while
    blDebug ( "END ListIva::calculaiva", 0 );
    return iva;
}


///
/**
**/
void ListIva::vaciar()
{
    blDebug ( "ListIva::vaciar", 0 );
    mdb_idregistroiva = "";
    m_lista.clear();
    blDebug ( "END ListIva::vaciar", 0 );
}


///
/**
\return
**/
void ListIva::borrar()
{
    blDebug ( "ListIva::borrar", 0 );
    if ( mdb_idregistroiva != "" )  {
        companyact->begin();
        int error = companyact->runQuery ( "DELETE FROM iva WHERE idregistroiva = " + mdb_idregistroiva );
        if ( error ) {
            companyact->rollback();
            return;
        } // end if
        companyact->commit();
    } // end if
    blDebug ( "END ListIva::borrar", 0 );
}


///
/**
\param pos
**/
void ListIva::borraIva ( int pos )
{
    blDebug ( "ListIva::borraIva", 0 );
    Iva *linea;
    linea = m_lista.at ( pos );
    linea->borrar();
    m_lista.removeAt ( pos );
    pintaListIva();
    blDebug ( "ListIva::borraIva", 0 );
}


#endif