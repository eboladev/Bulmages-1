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

#include <QComboBox>

#include "busquedaprovincia.h"
#include "empresabase.h"
#include "funcaux.h"


/** Con este constructor inicializamos todos los componentes del Widget a NULL.
    De esta forma nos aseguramos que no haya problemas para determinar si un objeto
    ha sido creado o no.
*/
/**
\param parent
**/
BusquedaProvincia::BusquedaProvincia ( QWidget *parent )
        : QComboBox2 ( parent )
{
    _depura ( "BusquedaProvincia::BusquedaProvincia", 0 );
    setQuery("SELECT * FROM provincia LEFT JOIN pais ON provincia.idpais = pais.idpais ORDER BY descpais, provincia");
    setTableName ("provincia");
    setCampoId ("idprovincia");
    m_valores["nomprovincia"] = "";
    m_valores["descpais"] = "";
    _depura ( "END BusquedaProvincia::BusquedaProvincia", 0 );
}


/// El destructor libera memoria utilizada por el programa.
/**
**/
BusquedaProvincia::~BusquedaProvincia()
{
    _depura ( "BusquedaProvincia::~BusquedaProvincia", 0 );
    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;
    _depura ( "END BusquedaProvincia::~BusquedaProvincia", 0 );
}

