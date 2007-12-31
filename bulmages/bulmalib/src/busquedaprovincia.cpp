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

#include <QComboBox>

#include "busquedaprovincia.h"
#include "empresabase.h"
#include "funcaux.h"


/** Con este constructor inicializamos todos los componentes del Widget a NULL.
    De esta forma nos aseguramos que no haya problemas para determinar si un objeto
    ha sido creado o no.
*/
/**
\param parent
**/
BusquedaProvincia::BusquedaProvincia ( QWidget *parent )
        : QComboBox2 ( parent )
{
    _depura ( "BusquedaProvincia::BusquedaProvincia", 0 );
    m_cursorcombo = NULL;
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    _depura ( "END BusquedaProvincia::BusquedaProvincia", 0 );
}


/// El destructor libera memoria utilizada por el programa.
/**
**/
BusquedaProvincia::~BusquedaProvincia()
{
    _depura ( "BusquedaProvincia::~BusquedaProvincia", 0 );
    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;
    _depura ( "END BusquedaProvincia::~BusquedaProvincia", 0 );
}


/** Este metodo sirve para indicar al Widget cual es la provincia por defecto.
    Recarga todo el query de provincias y compone de nuevo el comboBox estableciendo como provincia
    por defecto aquella cuyo identificador coincide con el que se ha pasado por parametro.
*/
/**
\param idprovincia
**/
void BusquedaProvincia::setIdProvincia ( QString idprovincia )
{
    _depura ( "BusquedaProvincia::setIdProvincia", 0, idprovincia );
    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;
    m_cursorcombo = empresaBase() ->cargacursor ( "SELECT * FROM provincia LEFT JOIN pais ON provincia.idpais = pais.idpais ORDER BY descpais, provincia" );
    /// Si ha habido un problema con la base de datos salimos.
    if ( !m_cursorcombo ) return;

    int i = 0;
    int i1 = 0;
    clear();
    addItem ( "--" );

    while ( !m_cursorcombo->eof() ) {
        i ++;
        if ( m_cursorcombo->valor ( "idprovincia" ) == idprovincia )
            i1 = i;
        addItem ( m_cursorcombo->valor ( "provincia" ) + " (" + m_cursorcombo->valor ( "descpais" ) + ")" );
        m_cursorcombo->siguienteregistro();
    } // end while

    setCurrentIndex ( i1 );
    _depura ( "END BusquedaProvincia::setIdProvincia", 0 );
}


///
/**
\param idprovincia
**/
void BusquedaProvincia::setValorCampo ( QString idprovincia )
{
    _depura ( "BusquedaProvincia::setValorCampo", 0 );
    setIdProvincia ( idprovincia );
    _depura ( "END BusquedaProvincia::setValorCampo", 0 );
}


/// SLOT que se activa al activar un elemento del desplegable
/**
\param index
**/
void BusquedaProvincia::m_activated ( int index )
{
    _depura ( "BusquedaProvincia::m_activated", 0 );
    if ( index > 0 ) {
        emit ( valueChanged ( m_cursorcombo->valor ( "idprovincia", index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
    }
    _depura ( "END BusquedaProvincia::m_activated", 0 );
}


///
/**
\return
**/
QString BusquedaProvincia::idProvincia()
{
    _depura ( "BusquedaProvincia::idProvincia", 0 );
    if ( currentIndex() > 0 ) {
        return m_cursorcombo->valor ( "idprovincia", currentIndex() - 1 );
    } else {
        return "";
    } // end if
    _depura ( "END BusquedaProvincia::idProvincia", 0 );
}


///
/**
\return
**/
QString BusquedaProvincia::valorCampo()
{
    _depura ( "BusquedaProvincia::valorCampo", 0 );
    _depura ( "END BusquedaProvincia::valorCampo", 0 );
    return idProvincia();
}

