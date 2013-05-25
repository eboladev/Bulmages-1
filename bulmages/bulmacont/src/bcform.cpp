/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "bcform.h"
#include "blplugins.h"


class Fixed;


///
/**
\param comp
\param parent
\param f
**/
BcForm::BcForm ( BcCompany *comp, QWidget *parent, Qt::WindowFlags f )
        : BlForm ( comp, parent, f )
{
    BL_FUNC_DEBUG
    m_listalineas = NULL;
    
}


///
/**
**/
BcForm::~BcForm()
{
    BL_FUNC_DEBUG
    mainCompany() ->removeWindow ( this );
    
}


///
/**
\param form
**/
void BcForm::setListaLineas ( BcSubForm * form )
{
    BL_FUNC_DEBUG
    m_listalineas = form;
    
}


///
/**
\return
**/
BcSubForm* BcForm::listalineas()
{
    BL_FUNC_DEBUG
    
    return m_listalineas;
}


///
/**
\return
**/
BcCompany * BcForm::mainCompany()
{
    BL_FUNC_DEBUG
    
    return ( BcCompany * ) BlForm::mainCompany();
}


