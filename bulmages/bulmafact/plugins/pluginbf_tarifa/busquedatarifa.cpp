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
    BL_FUNC_DEBUG
    m_cursorcombo = NULL;
    setMouseTracking ( TRUE );
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    
}


///
/**
**/
BusquedaTarifa::~BusquedaTarifa()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param idtarifa
**/
void BusquedaTarifa::setIdTarifa ( QString idtarifa )
{
    BL_FUNC_DEBUG
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
        if ( m_cursorcombo->value( "idtarifa" ) == idtarifa )
            i1 = i;
        addItem ( m_cursorcombo->value( "nomtarifa" ) );
        m_cursorcombo->nextRecord();
    }
    setCurrentIndex ( i1 );
    
}


void BusquedaTarifa::m_activated ( int index )
{
    BL_FUNC_DEBUG
    if ( index > 0 ) {
        emit ( valueChanged ( m_cursorcombo->value( "idtarifa", index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
    } // end if

    

}


QString BusquedaTarifa::idtarifa()
{
    BL_FUNC_DEBUG

    int index = currentIndex();
    if ( index > 0 ) {
        
        return ( m_cursorcombo->value( "idtarifa", index - 1 ) );
    } else {
        
        return "";
    } // end if

}


QString BusquedaTarifa::fieldValue()
{
    BL_FUNC_DEBUG
    
    return idtarifa();
}


void BusquedaTarifa::setFieldValue ( QString idtarifa )
{
    BL_FUNC_DEBUG
    setIdTarifa ( idtarifa );
    
}

