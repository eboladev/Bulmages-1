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
    m_comboRecordSet = NULL;
    m_codigotarifa = "";
    setTableName("tarifa");
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
void BusquedaTarifa::setId ( QString idtarifa )
{
    BL_FUNC_DEBUG

    try {
        /// Si no existe la mainCompany() pq aun no ha sido establecida salimos
        if ( !mainCompany() ) return;

	if ( m_comboRecordSet != NULL ) {
	    delete m_comboRecordSet;
	} // end if

	m_comboRecordSet = mainCompany() ->loadQuery ( "SELECT * FROM tarifa" );
	int i = 0;
	int i1 = 0;
	clear();
	addItem ( "--" );
	while ( !m_comboRecordSet->eof() ) {
	    i++;
	    if ( m_comboRecordSet->value( "idtarifa" ) == idtarifa )
		i1 = i;
	    addItem ( m_comboRecordSet->value( "nomtarifa" ) );
	    m_comboRecordSet->nextRecord();
	}

	setCurrentIndex ( i1 );
	
    } catch ( ... ) {
        BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error al establecer la tarifa.") );
        blMsgError(_("Error al establecer la tarifa."));
    } // end try

}


void BusquedaTarifa::m_activated ( int index )
{
    BL_FUNC_DEBUG
    if ( index > 0 ) {
        emit ( valueChanged ( m_comboRecordSet->value( "idtarifa", index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
    } // end if

    

}


QString BusquedaTarifa::id()
{
    BL_FUNC_DEBUG

    int index = currentIndex();
    if ( index > 0 ) {
        
        return ( m_comboRecordSet->value( "idtarifa", index - 1 ) );
    } else {
        
        return "";
    } // end if

}



