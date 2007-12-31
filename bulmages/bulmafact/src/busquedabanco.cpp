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

#include "qcombobox2.h"

#include "busquedabanco.h"
#include "company.h"


/** Inicializa todos los componentes a NULL para que no haya confusiones sobre
    si un elemento ha sido inicializado o no.
    Hace la conexion del SIGNAL activated con m_activated para tratar el evento.
*/
/**
\param parent
**/
BusquedaBanco::BusquedaBanco ( QWidget *parent )
        : QComboBox2 ( parent )
{
    _depura ( "BusquedaBanco::BusquedaBanco", 0 );
    m_cursorcombo = NULL;
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    _depura ( "END BusquedaBanco::BusquedaBanco", 0 );
}


/** Libera la memoria dinamica que se estaba utilizando.
*/
/**
**/
BusquedaBanco::~BusquedaBanco()
{
    _depura ( "BusquedaBanco::~BusquedaBanco", 0 );
    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;
    _depura ( "END BusquedaBanco::~BusquedaBanco", 0 );
}


/** Con este metodo se puede indicar al Widget cual es el banco seleccionado.
    Recarga el cursor de bancoes y cuando encuentra uno con el mismo identificador
    que el que se ha pasado por parametro lo establece como elemento seleccionado.
*/
/**
\param idbanco
\return
**/
void BusquedaBanco::setidbanco ( QString idbanco )
{
    _depura ( "BusquedaBanco::setidbanco", 0 );
    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;
    m_cursorcombo = empresaBase() ->cargacursor ( "SELECT * FROM banco" );
    /// Tratamos el caso en que no se haya devuelto nada.
    if ( m_cursorcombo == NULL ) return;
    int i = 0;
    int i1 = 0;
    clear();
    addItem ( "--" );
    while ( !m_cursorcombo->eof() ) {
        i ++;
        if ( m_cursorcombo->valor ( "idbanco" ) == idbanco )
            i1 = i;
        addItem ( m_cursorcombo->valor ( "nombanco" ) );
        m_cursorcombo->siguienteregistro();
    } // end while
    setCurrentIndex ( i1 );
    _depura ( "END BusquedaBanco::setidbanco", 0 );
}


///
/**
\param idbanco
**/
void BusquedaBanco::setValorCampo ( QString idbanco )
{
    _depura ( "BusquedaBanco::setValorCampo", 0 );
    setidbanco ( idbanco );
    _depura ( "END BusquedaBanco::setValorCampo", 0 );
}

/** Devuelve el identificador del banco seleccionado
**/
/**
\return
**/
QString BusquedaBanco::idbanco()
{
    _depura ( "BusquedaBanco::idbanco", 0 );
    _depura ( "END BusquedaBanco::idbanco", 0 );
    /// Como puede haber habido un error con la base de datos debemos tratar dicho caso.
    if ( !m_cursorcombo ) return "0";
    return m_cursorcombo->valor ( "idbanco", currentIndex() - 1 );
}

/** Devuelve el identificador del banco seleccionado
**/
/**
\return
**/
QString BusquedaBanco::valorCampo()
{
    return idbanco();
}


/** SLOT que responde a la activacion de un elemento en el QComboBox y que hace que se emita el SIGNAL valueChanged
**/
/**
\param index
**/
void BusquedaBanco::m_activated ( int index )
{
    _depura ( "BusquedaBanco::m_activated", 0 );
    if ( index > 0 ) {
        emit ( valueChanged ( m_cursorcombo->valor ( "idbanco", index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
    }
    _depura ( "END BusquedaBanco::m_activated", 0 );
}


