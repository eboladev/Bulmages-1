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


QString BlI18n(const char *cadena, int) {
//    QString salida = QString(textdomain(NULL)) + QString(" - ") + QString(gettext(cadena)) + QString("\n");
//    fprintf(stderr, salida.toAscii());
    return QString(gettext(cadena));
}


QString BlI18n(const char *domain, const char *cadena) {
//    QString salida = QString(domain) + QString(" - ") + QString(dgettext(domain, cadena)) + QString("\n");
//    fprintf(stderr, salida.toAscii());
    return QString(dgettext(domain, cadena));
}

