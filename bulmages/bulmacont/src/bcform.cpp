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
#include "blplugins.h"


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
    blDebug ( "BcForm::BcForm", 0 );
    m_listalineas = NULL;
    blDebug ( "END BcForm::BcForm", 0 );
}


///
/**
**/
BcForm::~BcForm()
{
    blDebug ( "BcForm::~BcForm", 0 );
    mainCompany() ->sacaWindow ( this );
    blDebug ( "END BcForm::~BcForm", 0 );
}


///
/**
\param form
**/
void BcForm::setListaLineas ( BcSubForm * form )
{
    blDebug ( "BcForm::setListaLineas", 0 );
    m_listalineas = form;
    blDebug ( "END BcForm::setListaLineas", 0 );
}


///
/**
\return
**/
BcSubForm* BcForm::listalineas()
{
    blDebug ( "BcForm::listalineas", 0 );
    blDebug ( "END BcForm::listalineas", 0 );
    return m_listalineas;
}


///
/**
\return
**/
BcCompany * BcForm::mainCompany()
{
    blDebug ( "BcForm::mainCompany", 0 );
    blDebug ( "END BcForm::mainCompany", 0 );
    return ( BcCompany * ) BlForm::mainCompany();
}


