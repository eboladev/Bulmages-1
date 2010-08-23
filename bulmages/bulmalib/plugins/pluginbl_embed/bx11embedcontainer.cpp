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

#ifndef WIN32

#include "bx11embedcontainer.h"


///
/**
\param parent
**/
BX11EmbedContainer::BX11EmbedContainer ( BlMainCompany *emp, QWidget *parent ) : QX11EmbedContainer ( parent ), BlMainCompanyPointer ( emp )
{
    blDebug ( "END BX11EmbedContainer::BX11EmbedContainer", 0 );
}


///
/**
**/
BX11EmbedContainer::~BX11EmbedContainer()
{
    blDebug ( "BX11EmbedContainer::~BX11EmbedContainer", 0, this->windowTitle() );
    sacaWindow();
    blDebug ( "END BX11EmbedContainer::~BX11EmbedContainer", 0 );
}


///
/**
\return
**/
int BX11EmbedContainer::sacaWindow()
{
    blDebug ( "BX11EmbedContainer::sacaWindow", 0 );
    discardClient();
    if ( mainCompany() != NULL ) {
        mainCompany() ->sacaWindow ( this );
    } // end if
    blDebug ( "END BX11EmbedContainer::sacaWindow", 0 );
    return 0;
}


#endif

