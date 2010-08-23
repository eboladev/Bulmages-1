/***************************************************************************
 *   Copyright (C) 2009 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include <QLocale>

#include "bli18n.h"
#include "blfunctions.h"


QString BlI18n ( const char *cadena, int )
{
    /// Depuracion:
    QString salida = QString("BlI18n (") + QString(textdomain(NULL)) + QString(" - ") + QString(gettext(cadena)) + QString(")");
    blDebug(salida, 0);

#ifdef WIN32
    return ( cadena );
#else
    return QString ( gettext ( cadena ) );
#endif
}


QString BlI18n ( const char *domain, const char *cadena )
{
    /// Depuracion:
    QString salida = QString("BlI18n (") + QString(domain) + QString(" - ") + QString(dgettext(domain, cadena)) + QString(")");
    blDebug(salida, 0);

#ifdef WIN32
    return ( cadena );
#else
    return QString ( dgettext ( domain, cadena ) );
#endif
}



/// En windows de momento no vamos a usar GETTEXT. Aunque la macro deberia ser #ifdef USE_GETTEXT
#ifdef WIN32
void bindtextdomain ( const char *, const char * ) {};
void textdomain ( const char * ) {};
#endif

