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
    blDebug ( "BusquedaTarifa::BusquedaTarifa", 0 );
    m_cursorcombo = NULL;
    setMouseTracking ( TRUE );
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    blDebug ( "END BusquedaTarifa::BusquedaTarifa", 0 );
}


///
/**
**/
BusquedaTarifa::~BusquedaTarifa()
{
    blDebug ( "BusquedaTarifa::~BusquedaTarifa", 0 );
    blDebug ( "END BusquedaTarifa::~BusquedaTarifa", 0 );
}


///
/**
\param idtarifa
**/
void BusquedaTarifa::setIdTarifa ( QString idtarifa )
{
    blDebug ( "BusquedaTarifa::setIdTarifa", 0 );
//    mdb_idtarifa = idtarifa;
    if ( m_cursorcombo != NULL ) {
        delete m_cursorcombo;
    } // end if

    m_cursorcombo = mainCompany() ->loadQuery ( "SELECT * FROM tarifa" );
    int i = 0;
    int i1 = 0;
    clear();
    addItem ( "--" );
    while ( !m_cursorcombo->eof() ) {
        i++;
        if ( m_cursorcombo->valor ( "idtarifa" ) == idtarifa )
            i1 = i;
        addItem ( m_cursorcombo->valor ( "nomtarifa" ) );
        m_cursorcombo->nextRecord();
    }
    setCurrentIndex ( i1 );
    blDebug ( "END BusquedaTarifa::setIdTarifa", 0 );
}


void BusquedaTarifa::m_activated ( int index )
{
    blDebug ( "BusquedaTarifa::m_activated", 0 );
    if ( index > 0 ) {
        emit ( valueChanged ( m_cursorcombo->valor ( "idtarifa", index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
    } // end if

    blDebug ( "END BusquedaTarifa::m_activated", 0 );

}


QString BusquedaTarifa::idtarifa()
{
    blDebug ( "BusquedaTarifa::idtarifa", 0 );

    int index = currentIndex();
    if ( index > 0 ) {
        blDebug ( "END BusquedaTarifa::idtarifa", 0, m_cursorcombo->valor ( "idtarifa", index - 1 ) );
        return ( m_cursorcombo->valor ( "idtarifa", index - 1 ) );
    } else {
        blDebug ( "END BusquedaTarifa::idtarifa", 0 );
        return "";
    } // end if

}


QString BusquedaTarifa::fieldValue()
{
    blDebug ( "BusquedaTarifa::fieldValue", 0 );
    blDebug ( "END BusquedaTarifa::fieldValue", 0 );
    return idtarifa();
}


void BusquedaTarifa::setFieldValue ( QString idtarifa )
{
    blDebug ( "BusquedaTarifa::setFieldValue", 0 );
    setIdTarifa ( idtarifa );
    blDebug ( "END BusquedaTarifa::setFieldValue", 0 );
}

