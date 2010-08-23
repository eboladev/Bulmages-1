/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include  "bcbuscarcanal.h"
#include  "bccompany.h"
#include  "blfunctions.h"


///
/**
**/
BcBuscarCanal::BcBuscarCanal ( QWidget *parent )
        : BlComboBox ( parent )
{
    blDebug ( "BcBuscarCanal::BcBuscarCanal", 0 );
    m_cursorcombo = NULL;
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    blDebug ( "END BcBuscarCanal::BcBuscarCanal", 0 );
}


///
/**
**/
BcBuscarCanal::~BcBuscarCanal()
{
    blDebug ( "BcBuscarCanal::~BcBuscarCanal", 0 );
    blDebug ( "END BcBuscarCanal::~BcBuscarCanal", 0 );
}


///
/**
\param idcanal
**/
void BcBuscarCanal::setidcanal ( QString idcanal )
{
    blDebug ( "BcBuscarCanal::setidcanal", 0, idcanal );
    if ( m_cursorcombo != NULL ) {
        delete m_cursorcombo;
    } // end if
    m_cursorcombo = mainCompany() ->loadQuery ( "SELECT * FROM canal" );
    int i = 0;
    int i1 = 0;
    clear();
    addItem ( "--" );
    while ( !m_cursorcombo->eof() ) {
        i ++;
        if ( m_cursorcombo->valor ( "idcanal" ) == idcanal ) {
            i1 = i;
        } // end if
        addItem ( m_cursorcombo->valor ( "nombre" ) );
        m_cursorcombo->nextRecord();
    } //end while
    setCurrentIndex ( i1 );
    blDebug ( "END BcBuscarCanal::setidcanal", 0, idcanal );
}


///
/**
\param idcanal
**/
void BcBuscarCanal::setFieldValue ( QString idcanal )
{
    blDebug ( "BcBuscarCanal::setFieldValue", 0 );
    setidcanal ( idcanal );
    blDebug ( "END BcBuscarCanal::setFieldValue", 0 );
}


///
/**
\param index
**/
void BcBuscarCanal::m_activated ( int index )
{
    blDebug ( "BcBuscarCanal::m_activated", 0 );
    if ( index > 0 ) {
        emit ( valueChanged ( m_cursorcombo->valor ( "idcanal", index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
    } // end if
    blDebug ( "END BcBuscarCanal::m_activated", 0 );
}


///
/**
\return
**/
QString BcBuscarCanal::idcanal()
{
    blDebug ( "BcBuscarCanal::idcanal", 0 );
    int index = currentIndex();
    if ( index > 0 ) {
        return ( m_cursorcombo->valor ( "idcanal", index - 1 ) );
    } else {
        return "";
    } // end if
    blDebug ( "END BcBuscarCanal::idcanal", 0 );
}


///
/**
\return
**/
QString BcBuscarCanal::fieldValue()
{
    return idcanal();
}

/// ===================================================================
/// Busqueda Cuenta Delegate para usar con los subforms
/// ===================================================================
/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el SIGNAL activated() con m_activated() para tratarlo. */
/**
\param parent
**/
BcBuscarCanalDelegate::BcBuscarCanalDelegate ( QWidget *parent )
        : BlComboBox ( parent )
{
    blDebug ( "BcBuscarCanalDelegate::BcBuscarCanalDelegate", 10 );
    setEditable ( false );
    blDebug ( "END BcBuscarCanalDelegate::BcBuscarCanalDelegate", 0 );
}



/// Libera la memoria reservada.
/**
**/
BcBuscarCanalDelegate::~BcBuscarCanalDelegate()
{
    blDebug ( "BcBuscarCanalDelegate::~BcBuscarCanalDelegate", 10 );
    blDebug ( "END BcBuscarCanalDelegate::~BcBuscarCanalDelegate", 0 );
}


/** Permite indicar al Widget cual es la serie de factura seleccionada por defecto.
    Recarga cursor de serie_factura y cuando encuentra un registro cuyo codigoserie_factura coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
/**
\param cod
\return
**/
void BcBuscarCanalDelegate::set ( const QString &codigo )
{
    blDebug ( "BcBuscarCanalDelegate::set", 0 );
    int index = 0;

    m_cursorcombo = mainCompany() ->loadQuery ( "SELECT nombre FROM canal ORDER BY nombre" );
    clear();

    addItem ( "--" );
    ///TODO: La idea es que salga en el desplegable del combobox el listado de cuentas que
    /// coincidan con el texto escrito para poder elegirlo.
    while ( !m_cursorcombo->eof() ) {
        addItem ( m_cursorcombo->valor ( "nombre" ) );
        if ( m_cursorcombo->valor ( "nombre" ) == codigo )
            index = m_cursorcombo->currentRecord() + 1;
        m_cursorcombo->nextRecord();
    }
    delete m_cursorcombo;
    setEditText ( codigo );
    setCurrentIndex ( index );

    blDebug ( "END BcBuscarCanalDelegate::set", 0 );
}

