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

#include  "bcbuscarcentrocoste.h"
#include  "blfunctions.h"
#include  "bfcompany.h"


///
/**
\param parent
**/
BcBuscarCentroCoste::BcBuscarCentroCoste ( QWidget *parent, const char * )
        : BlComboBox ( parent )
{
    BL_FUNC_DEBUG
    setQuery ( "SELECT idc_coste, nombre FROM c_coste ORDER BY nombre" );
    setTableName ( "c_coste" );
    setFieldId ( "idc_coste" );
    m_valores["nombre"] = "";
    
}


///
/**
**/
BcBuscarCentroCoste::~BcBuscarCentroCoste()
{
    BL_FUNC_DEBUG
    
}



/// ===================================================================
/// Busqueda Cuenta Delegate para usar con los subforms
/// ===================================================================
/** Inicializa todos los componentes del Widget a NULL para que no haya posibles confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el SIGNAL activated() con m_activated() para tratarlo.
*/
/**
\param parent
**/
BcBuscarCentroCosteDelegate::BcBuscarCentroCosteDelegate ( QWidget *parent )
        : BlComboBoxDelegate ( parent )
{
    BL_FUNC_DEBUG
    setQuery ( "SELECT idc_coste, nombre FROM c_coste ORDER BY nombre" );
    setTableName ( "c_coste" );
    setFieldId ( "idc_coste" );
    m_valores["nombre"] = "";
    
}


/// Libera la memoria reservada.
/**
**/
BcBuscarCentroCosteDelegate::~BcBuscarCentroCosteDelegate()
{
    BL_FUNC_DEBUG
    
}


