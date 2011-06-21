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
    BL_FUNC_DEBUG
    companyact = comp;
    mdb_idregistroiva = "";
    
}


///
/**
**/
ListIva::ListIva()
{
    BL_FUNC_DEBUG
    companyact = NULL;
    mdb_idregistroiva = "";
    
}


///
/**
**/
ListIva::~ListIva()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param pos
\return
**/
Iva *ListIva::linpos ( int pos )
{
    BL_FUNC_DEBUG
    
    return m_lista.at ( pos );
}


/// Carga l&iacute;neas de presupuesto.
/**
\param idregistroiva
\return
**/
int ListIva::cargaListIva ( QString idregistroiva )
{
    BL_FUNC_DEBUG
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
	
        return 1;
    } // end if

    
    return 0;
}


///
/**
**/
void ListIva::guardaListIva()
{
    BL_FUNC_DEBUG
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

    
}


///
/**
\return
**/
BlFixed ListIva::calculabase()
{
    BL_FUNC_DEBUG
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
    
    return base;
}


///
/**
\return
**/
BlFixed ListIva::calculaiva()
{
    BL_FUNC_DEBUG
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
    
    return iva;
}


///
/**
**/
void ListIva::vaciar()
{
    BL_FUNC_DEBUG
    mdb_idregistroiva = "";
    m_lista.clear();
    
}


///
/**
\return
**/
void ListIva::remove()
{
    BL_FUNC_DEBUG
    if ( mdb_idregistroiva != "" )  {
        companyact->begin();
        int error = companyact->runQuery ( "DELETE FROM iva WHERE idregistroiva = " + mdb_idregistroiva );
        if ( error ) {
            companyact->rollback();
            return;
        } // end if
        companyact->commit();
    } // end if
    
}


///
/**
\param pos
**/
void ListIva::borraIva ( int pos )
{
    BL_FUNC_DEBUG
    Iva *linea;
    linea = m_lista.at ( pos );
    linea->remove();
    m_lista.removeAt ( pos );
    pintaListIva();
    
}


#endif