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

#include "busquedaseriefactura.h"
#include "company.h"
#include "funcaux.h"


/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no. 
    Conecta el SIGNAL activated() con m_activated() para tratarlo.
*/
/**
\param parent
**/
BusquedaSerieFactura::BusquedaSerieFactura ( QWidget *parent )
        : QComboBox2 ( parent )
{
    _depura ( "BusquedaSerieFactura::BusquedaSerieFactura", 0 );
    m_cursorcombo = NULL;
    m_codigoserie_factura = "";
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    _depura ( "END BusquedaSerieFactura::BusquedaSerieFactura", 0 );
}


/** Libera la memoria reservada.
*/
/**
**/
BusquedaSerieFactura::~BusquedaSerieFactura()
{
    _depura ( "BusquedaSerieFactura::~BusquedaSerieFactura", 0 );
    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;
    _depura ( "END BusquedaSerieFactura::~BusquedaSerieFactura", 0 );
}


/** Permite indicar al Widget cual es la serie de factura seleccionada por defecto.
    Recarga cursor de serie_factura y cuando encuentra un registro cuyo codigoserie_factura coincide con el pasado
    como parametro lo establece como el registro activo por el comboBox.
*/
/**
\param codigo
\return
**/
void BusquedaSerieFactura::setcodigoserie_factura ( QString codigo )
{
    _depura ( "BusquedaSerieFactura::setcodigoserie_factura", 0 );
    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;
    m_cursorcombo = empresaBase() ->cargacursor ( "SELECT * FROM serie_factura" );
    if ( !m_cursorcombo ) return;
    int i = 0;
    int i1 = 0;
    int i2 = 0;
    clear();
    addItem ( "--" );
    while ( !m_cursorcombo->eof() ) {
        i ++;
        if ( m_cursorcombo->valor ( "codigoserie_factura" ) == m_codigoserie_factura )
            i2 = i;
        if ( m_cursorcombo->valor ( "codigoserie_factura" ) == codigo )
            i1 = i;
        addItem ( m_cursorcombo->valor ( "codigoserie_factura" ) + ".-" + m_cursorcombo->valor ( "descserie_factura" ) );
        m_cursorcombo->siguienteregistro();
    } // end if
    if ( i1 != 0 ) {
        setCurrentIndex ( i1 );
    } else {
        setCurrentIndex ( i2 );
    } // end if
    _depura ( "END BusquedaSerieFactura::setcodigoserie_factura", 0 );
}


///
/**
\param codigo
**/
void BusquedaSerieFactura::setValorCampo ( QString codigo )
{
    _depura ( "BusquedaSerieFactura::setValorCampo", 0 );
    setcodigoserie_factura ( codigo );
    _depura ( "END BusquedaSerieFactura::setValorCampo", 0 );
}


///
/**
\param index
**/
void BusquedaSerieFactura::m_activated ( int index )
{
    _depura ( "BusquedaSerieFactura::m_activated", 0 );
    if ( index > 0 ) {
        emit ( valueChanged ( m_cursorcombo->valor ( "codigoserie_factura", index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
    } // end if
    _depura ( "END BusquedaSerieFactura::m_activated", 0 );
}


///
/**
\return
**/
QString BusquedaSerieFactura::codigoserie_factura()
{
    _depura ( "BusquedaSerieFactura::codigoserie_factura", 0 );
    int index = currentIndex();
    if ( index > 0 ) {
        return ( m_cursorcombo->valor ( "codigoserie_factura", index - 1 ) );
    } else {
        return "";
    } // end if
    _depura ( "END BusquedaSerieFactura::codigoserie_factura", 0 );
}


///
/**
\return
**/
QString BusquedaSerieFactura::valorCampo()
{
    _depura ( "BusquedaSerieFactura::valorCampo", 0 );
    return codigoserie_factura();
    _depura ( "END BusquedaSerieFactura::valorCampo", 0 );
}


///
/**
\param comp
\return
**/
void BusquedaSerieFactura::setEmpresaBase ( Company *comp )
{
    _depura ( "BusquedaSerieFactura::setEmpresaBase", 0 );
    PEmpresaBase::setEmpresaBase ( comp );
    cursor2 *cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre ='SerieFacturaDefecto'" );
    if ( !cur ) return;
    if ( !cur->eof() ) {
        m_codigoserie_factura = cur->valor ( "valor" );
    } // end if
    delete cur;
    _depura ( "END BusquedaSerieFactura::setEmpresaBase", 0 );
}

