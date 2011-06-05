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

#include  "bcbuscarcanal.h"
#include  "bccompany.h"
#include  "blfunctions.h"


///
/**
**/
BcBuscarCanal::BcBuscarCanal ( QWidget *parent )
        : BlComboBox ( parent )
{
    blDebug ( Q_FUNC_INFO, 0 );
    setQuery ( "SELECT idcanal, nombre FROM canal ORDER BY nombre" );
    setTableName ( "canal" );
    setFieldId ( "idcanal" );
    m_valores["nombre"] = "";
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
BcBuscarCanal::~BcBuscarCanal()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


/// ===================================================================
/// Busqueda Cuenta Delegate para usar con los subforms
/// ===================================================================
/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no.
/**
\param parent
**/
BcBuscarCanalDelegate::BcBuscarCanalDelegate ( QWidget *parent )
        : BlComboBoxDelegate ( parent )
{
    blDebug ( Q_FUNC_INFO, 0 );
    setQuery ( "SELECT idcanal, nombre FROM canal ORDER BY nombre" );
    setTableName ( "canal" );
    setFieldId ( "idcanal" );
    m_valores["nombre"] = "";
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}



/// Libera la memoria reservada.
/**
**/
BcBuscarCanalDelegate::~BcBuscarCanalDelegate()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}



