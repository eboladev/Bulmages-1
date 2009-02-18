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

#include <QFile>
#include <QTextStream>

#include "fichabc.h"
#include "plugins.h"


class Fixed;


///
/**
\param comp
\param parent
\param f
**/
FichaBc::FichaBc ( BcCompany *comp, QWidget *parent, Qt::WFlags f )
        : BlForm ( comp, parent, f )
{
    _depura ( "FichaBc::FichaBc", 0 );
    m_listalineas = NULL;
    _depura ( "END FichaBc::FichaBc", 0 );
}


///
/**
**/
FichaBc::~FichaBc()
{
    _depura ( "FichaBc::~FichaBc", 0 );
    mainCompany() ->sacaWindow ( this );
    _depura ( "END FichaBc::~FichaBc", 0 );
}


///
/**
\param form
**/
void FichaBc::setListaLineas ( BcSubForm * form )
{
    _depura ( "FichaBc::setListaLineas", 0 );
    m_listalineas = form;
    _depura ( "END FichaBc::setListaLineas", 0 );
}


///
/**
\return
**/
BcSubForm* FichaBc::listalineas()
{
    _depura ( "FichaBc::listalineas", 0 );
    _depura ( "END FichaBc::listalineas", 0 );
    return m_listalineas;
}


///
/**
\return
**/
BcCompany * FichaBc::mainCompany()
{
    _depura ( "FichaBc::mainCompany", 0 );
    _depura ( "END FichaBc::mainCompany", 0 );
    return ( BcCompany * ) BlForm::mainCompany();
}


