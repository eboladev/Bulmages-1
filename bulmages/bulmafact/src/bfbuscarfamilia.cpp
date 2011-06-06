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

#include "bfbuscarfamilia.h"
#include "bfcompany.h"


/** Se encarga de poner todos los componentes a NULL para que no exista confusion
    en si los componentes han sido inicializados o no.
*/
/**
\param parent
**/
BfBuscarFamilia::BfBuscarFamilia ( QWidget *parent )
        : BlSearchWidget ( parent )
{
    blDebug ( Q_FUNC_INFO, 0 );
    /// Establecemos los parametros de busqueda del Cliente
    setLabel ( _ ( "Familia:" ) );
    setTableName ( "familia" );
    m_valores["codigocompletofamilia"] = "";
    m_valores["nombrefamilia"] = "";
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


/** No requiere de acciones especiales en la destruccion del Widget
*/
/**
**/
BfBuscarFamilia::~BfBuscarFamilia()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param val
**/
void BfBuscarFamilia::setidfamilia ( QString val )
{
    blDebug ( Q_FUNC_INFO, 0 );
    setId ( val );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}

///
/**
\param val
**/
void BfBuscarFamilia::setcodigocompletofamilia ( QString val )
{
    blDebug ( Q_FUNC_INFO, 0 );

    QString SQLQuery = "SELECT * FROM familia WHERE codigocompletofamilia='" + val + "'";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery );

    if ( !cur->eof() ) {
        setId ( cur->value( "idfamilia" ) );
    } else {
        setId ( "" );
    } // end if

    delete cur;
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}




///
/**
\return
**/
QString BfBuscarFamilia::codigocompletofamilia()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return m_valores["codigocompletofamilia"];
}


///
/**
\return
**/
QString BfBuscarFamilia::idfamilia()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return id();
}



///
/**
\returns
**/
QString BfBuscarFamilia::nombrefamilia()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return m_valores["nombrefamilia"];
}

