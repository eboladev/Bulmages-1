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

#include <QWidget>

#include "blapplication.h"
#include "ficha.h"


///
/**
\param argc
\param argv
**/
BlApplication::BlApplication ( int &argc, char **argv ) : QApplication ( argc, argv )
{
    _depura ( "BlApplication::BlApplication", 0 );
    _depura ( "END BlApplication::BlApplication", 0 );
}


///
/**
**/
BlApplication::~BlApplication()
{
    _depura ( "BlApplication::~BlApplication", 0 );
    _depura ( "END BlApplication::~BlApplication", 0 );
}


///
/**
\param o
\param e
\return
**/
bool BlApplication::notify ( QObject *o, QEvent *e )
{
//    _depura ( "BlApplication::notify", 0 );
    try {
//        _depura ( "END BlApplication::notify", 0 );
        return QApplication::notify ( o, e );
    } catch ( ... ) {
//        _depura ( "END BlApplication::notify", 0, "Error inesperado en la aplicacion" );
        _depura ( "END BlApplication::notify", 2, "Error inesperado en la aplicacion" );
        return FALSE;
    } // end try
}

void BlApplication::tablaCambiada1(const QString &t) {
	emit tablaCambiada(t);
}

