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

#include <QtWidgets/QWidget>

#include "comercialclienteview.h"
#include "blfunctions.h"


///
/**
\param parent
**/
ComercialClienteView::ComercialClienteView ( QWidget *parent )
        : BfForm ( NULL, parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    blScript(this);
    
}


///
/**
**/
ComercialClienteView::~ComercialClienteView()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param comp
**/
void ComercialClienteView::setMainCompany ( BfCompany *comp )
{
    BL_FUNC_DEBUG
    BlMainCompanyPointer::setMainCompany ( comp );

    /// Datos por defecto.
    mui_idzonacomercial->setMainCompany ( comp );
    mui_idzonacomercial->setQuery ( "SELECT * FROM zonacomercial ORDER BY nomzonacomercial" );
    mui_idzonacomercial->setTableName ( "zonacomercial" );
    mui_idzonacomercial->setFieldId ( "idzonacomercial" );
//    mui_idzonacomercial->m_valores["idzonacomercial"] = "";
    mui_idzonacomercial->m_valores["nomzonacomercial"] = "";
    mui_idzonacomercial->setAllowNull ( true );
    mui_idzonacomercial->setId ( "" );






    
}
