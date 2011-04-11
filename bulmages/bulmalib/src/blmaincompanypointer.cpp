/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#include "blmaincompanypointer.h"


///
/**
\param emp
**/
BlMainCompanyPointer::BlMainCompanyPointer ( BlMainCompany *emp )
{
    blDebug ( "BlMainCompanyPointer::BlMainCompanyPointer", 0 );
    m_mainCompany = emp;
    blDebug ( "END BlMainCompanyPointer::BlMainCompanyPointer", 0 );
}


///
/**
**/
BlMainCompanyPointer::BlMainCompanyPointer()
{
    blDebug ( "BlMainCompanyPointer::BlMainCompanyPointers", 0 );
    m_mainCompany = NULL;
    blDebug ( "END BlMainCompanyPointer::BlMainCompanyPointers", 0 );
}


///
/**
**/
BlMainCompanyPointer::~BlMainCompanyPointer()
{
    blDebug ( "BlMainCompanyPointer::~BlMainCompanyPointer", 0 );
    blDebug ( "END BlMainCompanyPointer::~BlMainCompanyPointer", 0 );
}


///
/**
\param emp
**/
void BlMainCompanyPointer::setMainCompany ( BlMainCompany *emp )
{
    blDebug ( "BlMainCompanyPointer::setMainCompany", 0 );
    m_mainCompany = emp;
    blDebug ( "END BlMainCompanyPointer::setMainCompany", 0 );
}


///
/**
\return
**/
BlMainCompany *BlMainCompanyPointer::mainCompany()
{
    blDebug ( "BlMainCompanyPointer::mainCompany", 0 );
    blDebug ( "END BlMainCompanyPointer::mainCompany", 0 );
    return m_mainCompany;
}


///
/**
\return
**/
QString BlMainCompanyPointer::fieldValue()
{
    blDebug ( "BlMainCompanyPointer::fieldValue", 2, "debe ser derivado" );
    return "";
}


///
/**
**/
void BlMainCompanyPointer::setFieldValue ( QString )
{
    blDebug ( "BlMainCompanyPointer::setFieldValue", 2, "debe ser derivado" );
}

