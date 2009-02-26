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

#include "bcform.h"
#include "plugins.h"


class Fixed;


///
/**
\param comp
\param parent
\param f
**/
BcForm::BcForm ( BcCompany *comp, QWidget *parent, Qt::WFlags f )
        : BlForm ( comp, parent, f )
{
    _depura ( "BcForm::BcForm", 0 );
    m_listalineas = NULL;
    _depura ( "END BcForm::BcForm", 0 );
}


///
/**
**/
BcForm::~BcForm()
{
    _depura ( "BcForm::~BcForm", 0 );
    mainCompany() ->sacaWindow ( this );
    _depura ( "END BcForm::~BcForm", 0 );
}


///
/**
\param form
**/
void BcForm::setListaLineas ( BcSubForm * form )
{
    _depura ( "BcForm::setListaLineas", 0 );
    m_listalineas = form;
    _depura ( "END BcForm::setListaLineas", 0 );
}


///
/**
\return
**/
BcSubForm* BcForm::listalineas()
{
    _depura ( "BcForm::listalineas", 0 );
    _depura ( "END BcForm::listalineas", 0 );
    return m_listalineas;
}


///
/**
\return
**/
BcCompany * BcForm::mainCompany()
{
    _depura ( "BcForm::mainCompany", 0 );
    _depura ( "END BcForm::mainCompany", 0 );
    return ( BcCompany * ) BlForm::mainCompany();
}


