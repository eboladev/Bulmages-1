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

#include  "bcbuscarcentrocoste.h"
#include  "blfunctions.h"
#include  "bccompany.h"


///
/**
\param parent
**/
BcBuscarCentroCoste::BcBuscarCentroCoste ( QWidget *parent, const char * )
        : BlComboBox ( parent )
{
    _depura ( "BcBuscarCentroCoste::BcBuscarCentroCoste", 0 );
    m_cursorcombo = NULL;
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    _depura ( "END BcBuscarCentroCoste::BcBuscarCentroCoste", 0 );
}


///
/**
**/
BcBuscarCentroCoste::~BcBuscarCentroCoste()
{
    _depura ( "BcBuscarCentroCoste::~BcBuscarCentroCoste", 0 );
    _depura ( "END BcBuscarCentroCoste::~BcBuscarCentroCoste", 0 );
}


///
/**
\return
**/
QString BcBuscarCentroCoste::idc_coste()
{
    _depura ( "BcBuscarCentroCoste::idc_coste", 0 );
    int index = currentIndex();
    if ( index > 0 ) {
        return ( m_cursorcombo->valor ( "idc_coste", index - 1 ) );
    } else {
        _depura ( "END BcBuscarCentroCoste::idc_coste", 0 );
        return "";
    } // end if
}


///
/**
\return
**/
QString BcBuscarCentroCoste::fieldValue()
{
    _depura ( "BcBuscarCentroCoste::fieldValue", 0 );
    _depura ( "END BcBuscarCentroCoste::fieldValue", 0 );
    return idc_coste();
}


///
/**
\param index
**/
void BcBuscarCentroCoste::m_activated ( int index )
{
    _depura ( "BcBuscarCentroCoste::m_activated", 0 );
    if ( index > 0 ) {
        emit ( valueChanged ( m_cursorcombo->valor ( "idc_coste", index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
        _depura ( "END BcBuscarCentroCoste::m_activated", 0 );
    } // end if
}


///
/**
\param idc_coste
**/
void BcBuscarCentroCoste::setidc_coste ( QString idc_coste )
{
    _depura ( "BcBuscarCentroCoste::setidc_coste", 0, idc_coste );
    if ( m_cursorcombo != NULL ) {
        delete m_cursorcombo;
    } // end if
    m_cursorcombo = mainCompany() ->loadQuery ( "SELECT * FROM c_coste" );
    int i = 0;
    int i1 = 0;
    clear();
    addItem ( "--" );
    while ( !m_cursorcombo->eof() ) {
        i ++;
        if ( m_cursorcombo->valor ( "idc_coste" ) == idc_coste ) {
            i1 = i;
        } // end if
        addItem ( m_cursorcombo->valor ( "nombre" ) );
        m_cursorcombo->nextRecord();
    } //end while
    setCurrentIndex ( i1 );
    _depura ( "END BcBuscarCentroCoste::setidc_coste", 0, idc_coste );
}


///
/**
\param idc_coste
**/
void BcBuscarCentroCoste::setFieldValue ( QString idc_coste )
{
    _depura ( "BcBuscarCentroCoste::setFieldValue", 0 );
    setidc_coste ( idc_coste );
    _depura ( "END BcBuscarCentroCoste::setFieldValue", 0 );
}

/// ===================================================================
/// Busqueda Cuenta Delegate para usar con los subforms
/// ===================================================================
/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el SIGNAL activated() con m_activated() para tratarlo.
*/
/**
\param parent
**/
BcBuscarCentroCosteDelegate::BcBuscarCentroCosteDelegate ( QWidget *parent )
        : BlComboBox ( parent )
{
    _depura ( "BcBuscarCentroCosteDelegate::BcBuscarCentroCosteDelegate", 0 );
    setEditable ( false );
    _depura ( "END BcBuscarCentroCosteDelegate::BcBuscarCentroCosteDelegate", 0 );
}


/// Libera la memoria reservada.
/**
**/
BcBuscarCentroCosteDelegate::~BcBuscarCentroCosteDelegate()
{
    _depura ( "BcBuscarCentroCosteDelegate::~BcBuscarCentroCosteDelegate", 10 );
    _depura ( "END BcBuscarCentroCosteDelegate::~BcBuscarCentroCosteDelegate", 0 );
}


/** Permite indicar al Widget cual es la serie de factura seleccionada por defecto.
    Recarga cursor de serie_factura y cuando encuentra un registro cuyo codigoserie_factura coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
/**
\param cod
\return
**/
void BcBuscarCentroCosteDelegate::set ( const QString &cod )
{
    _depura ( "BcBuscarCentroCosteDelegate::set", 0 );

    int index = 0;

    m_cursorcombo = mainCompany() ->loadQuery ( "SELECT nombre FROM c_coste ORDER BY nombre " );
    clear();

    addItem ( "--" );
    while ( !m_cursorcombo->eof() ) {
        addItem ( m_cursorcombo->valor ( "nombre" ) );
        if ( m_cursorcombo->valor ( "nombre" ) == cod )
            index = m_cursorcombo->currentRecord() + 1;
        m_cursorcombo->nextRecord();
    } // end while
    delete m_cursorcombo;
    setEditText ( cod );
    setCurrentIndex ( index );
    _depura ( "END BcBuscarCentroCosteDelegate::set", 0 );
}



