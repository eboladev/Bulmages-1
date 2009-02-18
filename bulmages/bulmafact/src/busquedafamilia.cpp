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

#include "busquedafamilia.h"
#include "bfcompany.h"


/** Se encarga de poner todos los componentes a NULL para que no exista confusion
    en si los componentes han sido inicializados o no.
*/
/**
\param parent
**/
BusquedaFamilia::BusquedaFamilia ( QWidget *parent )
        : Busqueda ( parent )
{
    _depura ( "BusquedaFamilia::BusquedaFamilia", 0 );
		/// Establecemos los parametros de busqueda del Cliente
		setLabel ( _( "Familia:" ) );
		setTableName( "familia" );
		m_valores["codigocompletofamilia"] = "";
		m_valores["nombrefamilia"] = "";
    _depura ( "END BusquedaFamilia::BusquedaFamilia", 0 );
}


/** No requiere de acciones especiales en la destruccion del Widget
*/
/**
**/
BusquedaFamilia::~BusquedaFamilia()
{
    _depura ( "BusquedaFamilia::~BusquedaFamilia", 0 );
    _depura ( "END BusquedaFamilia::~BusquedaFamilia", 0 );
}


///
/**
\param val
**/
void BusquedaFamilia::setidfamilia ( QString val )
{
    _depura ( "BusquedaFamilia::setidfamilia", 0 );
    setId(val);
    _depura ( "END BusquedaFamilia::setidfamilia", 0 );
}

///
/**
\param val
**/
void BusquedaFamilia::setcodigocompletofamilia ( QString val )
{
    _depura ( "BusquedaFamilia::setcodigocompletofamilia", 0 );

    QString SQLQuery = "SELECT * FROM familia WHERE codigocompletofamilia='" + val + "'";
    BlDbRecordSet *cur = mainCompany() ->cargacursor ( SQLQuery );

    if ( !cur->eof() ) {
        setId(cur->valor ( "idfamilia" ));
    } else {
		setId("");
    } // end if

    delete cur;
    _depura ( "END BusquedaFamilia::setcodigocompletofamilia", 0 );
}




///
/**
\return
**/
QString BusquedaFamilia::codigocompletofamilia()
{
    _depura ( "BusquedaFamilia::codigocompletofamilia", 0 );
    _depura ( "END BusquedaFamilia::codigocompletofamilia", 0 );
    return m_valores["codigocompletofamilia"];
}


///
/**
\return
**/
QString BusquedaFamilia::idfamilia()
{
    _depura ( "BusquedaFamilia::idfamilia", 0 );
    _depura ( "END BusquedaFamilia::idfamilia", 0 );
    return id();
}



///
/**
\returns
**/
QString BusquedaFamilia::nombrefamilia()
{
    _depura ( "BusquedaFamilia::nombrefamilia", 0 );
    _depura ( "END BusquedaFamilia::nombrefamilia", 0 );
    return m_valores["nombrefamilia"];
}

