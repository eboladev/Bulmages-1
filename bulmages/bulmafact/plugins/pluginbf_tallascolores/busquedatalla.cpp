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


#include "busquedatalla.h"
#include "blfunctions.h"


/** Constructor del Widget conecta las se&ntilde;ales y prepara los componentes.
*/
/**
\param parent
**/
BusquedaTalla::BusquedaTalla ( QWidget *parent, const char * )
        : BlComboBox ( parent )
{
    blDebug ( "BusquedaTalla::BusquedaTalla", 0 );

    setQuery ( "SELECT idtc_talla, nomtc_talla FROM tc_talla ORDER BY nomtc_talla" );
    setTableName ( "tc_talla" );
    setFieldId ( "idtc_talla" );
    m_valores["nomtc_talla"] = "";
    blDebug ( "END BusquedaTalla::BusquedaTalla", 0 );
}


/** El destructor no precisa de operaciones especiales
*/
/**
**/
BusquedaTalla::~BusquedaTalla()
{
    blDebug ( "BusquedaTalla::~BusquedaTalla", 0 );
    blDebug ( "END BusquedaTalla::~BusquedaTalla", 0 );
}



/// ========================= ITEM DELEGATE ===============================0

/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el SIGNAL activated() con m_activated() para tratarlo.
*/
/**
\param parent
**/
BusquedaTallaDelegate::BusquedaTallaDelegate ( QWidget *parent )
        : BlComboBoxDelegate ( parent )
{
    blDebug ( "BusquedaTallaDelegate::BusquedaTallaDelegate", 0 );
    setSizeAdjustPolicy ( QComboBox::AdjustToContents );
    setQuery ( "SELECT idtc_talla, nomtc_talla FROM tc_talla ORDER BY nomtc_talla" );
    setTableName ( "tc_talla" );
    setFieldId ( "idtc_talla" );
    m_valores["nomtc_talla"] = "";
    setAllowNull(FALSE);
    blDebug ( "END BusquedaTallaDelegate::BusquedaTallaDelegate", 0 );
}


/** Libera la memoria reservada.
*/
/**
**/
BusquedaTallaDelegate::~BusquedaTallaDelegate()
{
    blDebug ( "BusquedaTallaDelegate::~BusquedaTallaDelegate", 0 );
    blDebug ( "END BusquedaTallaDelegate::~BusquedaTallaDelegate", 0 );
}

