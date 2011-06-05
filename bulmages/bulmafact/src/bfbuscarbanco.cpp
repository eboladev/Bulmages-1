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

#include "blcombobox.h"

#include "bfbuscarbanco.h"
#include "bfcompany.h"


/** Inicializa todos los componentes a NULL para que no haya confusiones sobre
    si un elemento ha sido inicializado o no.
    Hace la conexion del SIGNAL activated con m_activated para tratar el evento.
*/
/**
\param parent
**/
BfBuscarBanco::BfBuscarBanco ( QWidget *parent )
        : BlComboBox ( parent )
{
    blDebug ( "BfBuscarBanco::BfBuscarBanco", 0 );
    m_cursorcombo = NULL;
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


/** Libera la memoria dinamica que se estaba utilizando.
*/
/**
**/
BfBuscarBanco::~BfBuscarBanco()
{
    blDebug ( "BfBuscarBanco::~BfBuscarBanco", 0 );
    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


/** Con este metodo se puede indicar al Widget cual es el banco seleccionado.
    Recarga el cursor de bancoes y cuando encuentra uno con el mismo identificador
    que el que se ha pasado por parametro lo establece como elemento seleccionado.
*/
/**
\param idbanco
\return
**/
void BfBuscarBanco::setidbanco ( QString idbanco )
{
    blDebug ( "BfBuscarBanco::setidbanco", 0 );
    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;
    m_cursorcombo = mainCompany() ->loadQuery ( "SELECT * FROM banco" );
    /// Tratamos el caso en que no se haya devuelto nada.
    if ( m_cursorcombo == NULL ) return;
    int i = 0;
    int i1 = 0;
    clear();
    addItem ( "--" );
    while ( !m_cursorcombo->eof() ) {
        i ++;
        if ( m_cursorcombo->value( "idbanco" ) == idbanco )
            i1 = i;
        addItem ( m_cursorcombo->value( "nombanco" ) );
        m_cursorcombo->nextRecord();
    } // end while
    setCurrentIndex ( i1 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param idbanco
**/
void BfBuscarBanco::setFieldValue ( QString idbanco )
{
    blDebug ( "BfBuscarBanco::setFieldValue", 0 );
    setidbanco ( idbanco );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}

/** Devuelve el identificador del banco seleccionado
**/
/**
\return
**/
QString BfBuscarBanco::idbanco()
{
    blDebug ( "BfBuscarBanco::idbanco", 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    /// Como puede haber habido un error con la base de datos debemos tratar dicho caso.
    if ( !m_cursorcombo ) return "0";
    if ( currentIndex() > 0 ) {
        return m_cursorcombo->value( "idbanco", currentIndex() - 1 );
    } // end if
    return "";
}

/** Devuelve el identificador del banco seleccionado
**/
/**
\return
**/
QString BfBuscarBanco::fieldValue()
{
    return idbanco();
}


/** SLOT que responde a la activacion de un elemento en el QComboBox y que hace que se emita el SIGNAL valueChanged
**/
/**
\param index
**/
void BfBuscarBanco::m_activated ( int index )
{
    blDebug ( "BfBuscarBanco::m_activated", 0 );
    if ( index > 0 ) {
        emit ( valueChanged ( m_cursorcombo->value( "idbanco", index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
    }
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


