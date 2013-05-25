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

#include <QtWidgets/QComboBox>

#include "busquedaestadoincidencia.h"
#include "blfunctions.h"


///
/**
\param parent
**/
BusquedaEstadoIncidencia::BusquedaEstadoIncidencia ( QWidget *parent )
        : QComboBox ( parent )
{
    BL_FUNC_DEBUG
    m_textos[0] = _ ( "Pendiente" );
    m_textos[1] = _ ( "Terminada" );
    m_textos[2] = _ ( "No quiere nada" );
    m_textos[3] = _ ( "No paga" );
    m_textos[4] = _ ( "Cambio datos" );
    m_textos[5] = _ ( "Revisar" );

    m_valores[0] = "1";
    m_valores[1] = "2";
    m_valores[2] = "3";
    m_valores[3] = "4";
    m_valores[4] = "5";
    m_valores[5] = "6";

    int i = 0;
    while ( i <= 5 ) {
        addItem ( m_textos[i] );
        i++;
    } // end while

    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( s_activated ( int ) ) );
    
}


///
/**
**/
BusquedaEstadoIncidencia::~BusquedaEstadoIncidencia()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param estado
**/
void BusquedaEstadoIncidencia::setestado ( QString estado )
{
    BL_FUNC_DEBUG
    int i = 0;
    while ( m_valores[i] != estado && i <= 5 ) {
        i ++;
    }
    if ( i <= 5 )
        setCurrentIndex ( i );
    else
        setCurrentIndex ( 0 );
    
}

