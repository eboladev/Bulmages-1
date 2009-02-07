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

#include "busquedaperiodo.h"


///
/**
\return
**/
QString BusquedaPeriodo::periodo()
{
    _depura ( "BusquedaPeriodo::periodo", 0 );
    _depura ( "END BusquedaPeriodo::periodo", 0 );
    return m_valores[currentIndex() ];
}


///
/**
\return
**/
QString BusquedaPeriodo::valorCampo()
{
    _depura ( "BusquedaPeriodo::valorCampo", 0 );
    _depura ( "END BusquedaPeriodo::valorCampo", 0 );
    return m_valores[currentIndex() ];
}


///
/**
\param index
**/
void BusquedaPeriodo::s_activated ( int index )
{
    _depura ( "Activado el combo box.", 0 );
    if ( index > 0 ) {
        emit ( valueChanged ( m_valores[currentIndex() ] ) );
    } else {
        emit ( valueChanged ( "" ) );
    } // end if
}


///
/**
\param parent
**/
BusquedaPeriodo::BusquedaPeriodo ( QWidget *parent ) : BlComboBox ( parent )
{
    _depura ( "BusquedaPeriodo::BusquedaPeriodo", 0 );
    m_textos[0] = _( "Semanal" );
    m_textos[1] = _( "Quincenal" );
    m_textos[2] = _( "Mensual" );
    m_textos[3] = _( "Trimestral" );
    m_textos[4] = _( "Semestral" );
    m_textos[5] = _( "Anual" );
    m_textos[6] = _( "Bienal" );
    m_textos[7] = "";

    m_valores[0] = "@ 7 days";
    m_valores[1] = "@ 15 days";
    m_valores[2] = "@ 1 mon";
    m_valores[3] = "@ 3 mons";
    m_valores[4] = "@ 6 mons";
    m_valores[5] = "@ 1 year";
    m_valores[6] = "@ 2 year";
    m_valores[7] = "";

    int i = 0;
    while ( m_textos[i] != "" ) {
        /// Anyade un nuevo elemento al final del QComboBox.
        insertItem ( this->count() + 1, m_textos[i] );
        i++;
    } // end while

    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( s_activated ( int ) ) );
    _depura ( "END BusquedaPeriodo::BusquedaPeriodo", 0 );
}


///
/**
**/
BusquedaPeriodo::~BusquedaPeriodo()
{
    _depura ( "BusquedaPeriodo::~BusquedaPeriodo", 0 );
    _depura ( "END BusquedaPeriodo::~BusquedaPeriodo", 0 );
}


///
/**
\param periodo
**/
void BusquedaPeriodo::setperiodo ( QString periodo )
{
    _depura ( "BusquedaPeriodo::setperiodo", 0 );
    int i = 0;
    while ( m_valores[i] != periodo && m_valores[i] != "" ) {
// _depura(m_valores[i]+" "+periodo, 2);
        i++;
    } // end while
    if ( m_valores[i] != "" )
        setCurrentIndex ( i );
    else
        setCurrentIndex ( 0 );
    _depura ( "END BusquedaPeriodo::setperiodo", 0 );
}


///
/**
\param periodo
**/
void BusquedaPeriodo::setValorCampo ( QString periodo )
{
    _depura ( "BusquedaPeriodo::setValorCampo", 0 );
    setperiodo ( periodo );
    _depura ( "END BusquedaPeriodo::setValorCampo", 0 );
}




