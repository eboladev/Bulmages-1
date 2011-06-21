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
#ifdef TOMEU

#include "listlinprevcobro.h"
#include "bccompany.h"
#include "linprevcobro.h"


///
/**
**/
void ListLinPrevCobro::inicializaVariables()
{
    BL_FUNC_DEBUG
    m_companyact = NULL;
    mdb_idregistroiva = "";
    mfilt_idregistroiva = "";
    mfilt_tipoprevcobro = "";
    mfilt_codigocuentaprevcobro = "";
    mfilt_finprevcobro = "";
    mfilt_ffiprevcobro = "";
    
}


///
/**
\param comp
**/
ListLinPrevCobro::ListLinPrevCobro ( BcCompany *comp )
{
    BL_FUNC_DEBUG
    inicializaVariables();
    m_companyact = comp;
    
}

///
/**
**/
ListLinPrevCobro::ListLinPrevCobro()
{
    BL_FUNC_DEBUG
    inicializaVariables();
    
}


///
/**
**/
ListLinPrevCobro::~ListLinPrevCobro()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param desc
\param cantl
\param pvpl
\param descl
\param idart
\param codart
\param nomart
\param ivapres
\param k
\param l
\param m
\param idctacliente
\param codigoctacliente
\param nomctacliente
**/
void ListLinPrevCobro::nuevalinea ( QString desc, QString cantl, QString pvpl, QString descl, QString idart, QString codart, QString nomart, QString ivapres, QString k, QString l, QString m, QString idctacliente, QString codigoctacliente, QString nomctacliente )
{
    linprevcobro * lin = new linprevcobro ( m_companyact, "", desc,  cantl, pvpl,
                                            descl, idart, codart, nomart,
                                            mdb_idregistroiva, ivapres, k,
                                            l, m, idctacliente, codigoctacliente, nomctacliente );
    m_lista.append ( lin );
}


///
/**
\param pos
\return
**/
linprevcobro *ListLinPrevCobro::linpos ( int pos )
{
    BL_FUNC_DEBUG
    
    return m_lista.at ( pos );
}


/// Carga l&iacute;neas de una factura.
/**
\return
**/
int ListLinPrevCobro::chargeBudgetLines()
{
    QString cadwhere = "";
    vaciar();
    fprintf ( stderr, "ListLinPrevCobro::chargeBudgetLines\n" );
    fprintf ( stderr, "Hacemos la carga del cursor\n" );
    if ( mfilt_idregistroiva != "" )
        cadwhere = " AND idregistroiva = " + mfilt_idregistroiva;
    if ( mfilt_finprevcobro != "" )
        cadwhere += " AND fcobroprevcobro >= '" + mfilt_finprevcobro + "'";
    if ( mfilt_codigocuentaprevcobro != "" )
        cadwhere += " AND idcuenta = id_cuenta('" + mfilt_codigocuentaprevcobro + "')";
    if ( mfilt_tipoprevcobro != "" )
        cadwhere += " AND tipoprevcobro = '" + mfilt_tipoprevcobro + "'";
    if ( mfilt_procesado == "PROCESADO" )
        cadwhere += " AND idasiento IS NOT NULL ";
    if ( mfilt_procesado == "NO PROCESADO" )
        cadwhere += " AND idasiento IS NULL ";

    BlDbRecordSet *cur = m_companyact->loadQuery ( "SELECT * FROM prevcobro "
                         " LEFT JOIN cuenta ON cuenta.idcuenta = prevcobro.idcuenta "
                         " LEFT JOIN (SELECT idcuenta AS idctacliente, codigo AS codigoctacliente, descripcion AS nomctacliente FROM cuenta) AS T1 ON t1.idctacliente = prevcobro.idctacliente "
                         " WHERE 1 = 1 " + cadwhere + " ORDER BY fcobroprevcobro " );

    int i = 0;
    while ( !cur->eof() ) {
        /// Creamos un elemento del tipo linprevcobro y lo agregamos a la lista.
        linprevcobro * lin = new linprevcobro ( m_companyact,
                                                cur->value( "idprevcobro" ),
                                                cur->value( "fprevistaprevcobro" ),
                                                cur->value( "fcobroprevcobro" ),
                                                cur->value( "idfpago" ),
                                                cur->value( "idcuenta" ),
                                                cur->value( "idasiento" ),
                                                cur->value( "cantidadprevistaprevcobro" ),
                                                cur->value( "cantidadprevcobro" ),
                                                cur->value( "idregistroiva" ),
                                                cur->value( "tipoprevcobro" ),
                                                cur->value( "docprevcobro" ),
                                                cur->value( "codigo" ),
                                                cur->value( "descripcion" ),
                                                cur->value( "idctacliente" ),
                                                cur->value( "codigoctacliente" ),
                                                cur->value( "nomctacliente" ) );
        m_lista.append ( lin );
        i++;
        cur->nextRecord();
    } // end while
    delete cur;
    
    return 0;
}


///
/**
**/
void ListLinPrevCobro::guardaListLinPrevCobro()
{
    BL_FUNC_DEBUG
    linprevcobro *linea;

    QMutableListIterator<linprevcobro*> m_ilista ( m_lista );
    /// Vamos delante del primer elemento de la lista.
    m_ilista.toFront();
    /// Comprobamos que el primer elemento y siguientes existan.
    while ( m_ilista.hasNext() ) {
        /// Si existe el elemento nos desplazamos a el moviendo el cursor.
        linea = m_ilista.next();
        linea->guardalinprevcobro();
    } // end while
}


///
/**
**/
void ListLinPrevCobro::vaciar()
{
    BL_FUNC_DEBUG
    m_lista.clear();
    
}


///
/**
**/
void ListLinPrevCobro::remove()
{
    BL_FUNC_DEBUG
    if ( mdb_idregistroiva != "" )  {
        m_companyact->begin();
        m_companyact->runQuery ( "DELETE FROM prevcobro WHERE idregistroiva = " + mdb_idregistroiva );
        m_companyact->commit();
    } // end if
    
}

///
/**
**/
void ListLinPrevCobro::borralinprevcobro ( int pos )
{
    BL_FUNC_DEBUG
    linprevcobro *linea;
    linea = m_lista.at ( pos );
    linea->remove();
    m_lista.removeAt ( pos );
    pintaListLinPrevCobro();
    
}


///
/**
\return
**/
BlFixed ListLinPrevCobro::totalCobro()
{
    BL_FUNC_DEBUG
    linprevcobro *linea;
    BlFixed tcobro ( "0" );

    QMutableListIterator<linprevcobro*> m_ilista ( m_lista );
    /// Vamos delante del primer elemento de la lista.
    m_ilista.toFront();
    /// Comprobamos que el primer elemento y siguientes existan.
    while ( m_ilista.hasNext() ) {
        /// Si existe el elemento nos desplazamos a el moviendo el cursor.
        linea = m_ilista.next();
        if ( linea->tipoprevcobro() == "t" ) {
            tcobro = tcobro + BlFixed ( linea->cantidadprevcobro() );
        } // end if
    } // end while
    
    return tcobro;
}


///
/**
\return
**/
BlFixed ListLinPrevCobro::totalPago()
{
    BL_FUNC_DEBUG
    linprevcobro *linea;
    BlFixed tpago ( "0" );

    QMutableListIterator<linprevcobro*> m_ilista ( m_lista );
    /// Vamos delante del primer elemento de la lista.
    m_ilista.toFront();
    /// Comprobamos que el primer elemento y siguientes existan.
    while ( m_ilista.hasNext() ) {
        /// Si existe el elemento nos desplazamos a el moviendo el cursor.
        linea = m_ilista.next();
        if ( linea->tipoprevcobro() == "f" ) {
            tpago = tpago + BlFixed ( linea->cantidadprevcobro() );
        } // end if
    } // end while
    
    return tpago;
}


#endif