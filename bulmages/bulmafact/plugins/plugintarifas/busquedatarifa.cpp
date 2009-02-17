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

#include "busquedatarifa.h"
#include "bfcompany.h"
#include "blfunctions.h"


///
/**
\param parent
**/
BusquedaTarifa::BusquedaTarifa ( QWidget *parent )
        : BlComboBox ( parent )
{
    _depura ( "BusquedaTarifa::BusquedaTarifa", 0 );
    m_cursorcombo = NULL;
    setMouseTracking(TRUE);
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    _depura ( "END BusquedaTarifa::BusquedaTarifa", 0 );
}


///
/**
**/
BusquedaTarifa::~BusquedaTarifa()
{
    _depura ( "BusquedaTarifa::~BusquedaTarifa", 0 );
    _depura ( "END BusquedaTarifa::~BusquedaTarifa", 0 );
}


///
/**
\param idtarifa
**/
void BusquedaTarifa::setIdTarifa ( QString idtarifa )
{
    _depura ( "BusquedaTarifa::setIdTarifa", 0 );
//    mdb_idtarifa = idtarifa;
    if ( m_cursorcombo != NULL ) {
        delete m_cursorcombo;
    } // end if

    m_cursorcombo = empresaBase() ->cargacursor ( "SELECT * FROM tarifa" );
    int i = 0;
    int i1 = 0;
    clear();
    addItem ( "--" );
    while ( !m_cursorcombo->eof() ) {
        i++;
        if ( m_cursorcombo->valor ( "idtarifa" ) == idtarifa )
            i1 = i;
        addItem ( m_cursorcombo->valor ( "nomtarifa" ) );
        m_cursorcombo->siguienteregistro();
    }
    setCurrentIndex ( i1 );
    _depura ( "END BusquedaTarifa::setIdTarifa", 0 );
}


void BusquedaTarifa::m_activated(int index)
{
    _depura("BusquedaTarifa::m_activated", 0);
    if ( index > 0 ) {
	emit (valueChanged ( m_cursorcombo->valor("idtarifa", index -1) ));
    } else {
	emit (valueChanged ( "" ));
    } // end if
    
    _depura("END BusquedaTarifa::m_activated", 0);
    
}


QString BusquedaTarifa::idtarifa()
{
    _depura("BusquedaTarifa::idtarifa", 0);
    
    int index = currentIndex();
    if ( index > 0 ) {
        _depura("END BusquedaTarifa::idtarifa", 0, m_cursorcombo->valor("idtarifa", index -1));
	return ( m_cursorcombo->valor("idtarifa", index -1));
    } else {
        _depura("END BusquedaTarifa::idtarifa", 0);
	return "";
    } // end if
    
}


QString BusquedaTarifa::valorCampo()
{
    _depura("BusquedaTarifa::valorCampo", 0);
    _depura("END BusquedaTarifa::valorCampo", 0);
    return idtarifa();
}


void BusquedaTarifa::setValorCampo(QString idtarifa)
{
    _depura("BusquedaTarifa::setValorCampo", 0);
    setIdTarifa(idtarifa);
    _depura("END BusquedaTarifa::setValorCampo", 0);
}

