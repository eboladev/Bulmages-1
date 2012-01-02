/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   http://www.iglues.org                                                 *
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
#include "blform.h"
#include "blfunctions.h"


/// Definimos la variable global g_theApp.
BlApplication *g_theApp = NULL;


///
/**
\param argc
\param argv
**/
BlApplication::BlApplication ( int &argc, char **argv ) : QApplication ( argc, argv )
{
  BL_FUNC_DEBUG
}


///
/**
**/
BlApplication::~BlApplication()
{
  BL_FUNC_DEBUG
}


#if CONFIG_DEBUG == TRUE
///
/**
\param object
\param event
\return
**/
bool BlApplication::notify ( QObject *object, QEvent *event )
{
    ///   Esta funcion es llamada continuamente y por eso no la depuramos (BL_FUNC_DEBUG).

    try {
        return QApplication::notify ( object, event );
    } catch ( ... ) {
        fprintf(stderr, _("Error inesperado en el sistema de notificaciones.\n").toAscii());
        return FALSE;
    } // end try

}
#endif


///
/**
\param table
**/
void BlApplication::emitDbTableChanged ( const QString &table )
{
  BL_FUNC_DEBUG
  emit dbTableChanged ( table );
}

