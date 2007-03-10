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

#include "qapplication2.h"
#include "funcaux.h"


QApplication2::QApplication2(int &argc, char **argv) : QApplication(argc, argv) {
    _depura("QApplication2::QApplication2", 0);
}


QApplication2::~QApplication2() {
    _depura("QApplication2::~QApplication2", 0);
}


bool QApplication2::notify(QObject *o, QEvent *e) {
    try {
        return QApplication::notify(o, e);
    } catch (...) {
        mensajeInfo(tr("Error inesperado en BulmaFact"));
        return FALSE;
    } // end try
}

