/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include <stdio.h>

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>

#include "plugindebugbc.h"
#include "configuracion.h"

///
/**
**/
myplugin::myplugin() {
    _depura("myplugin::myplugin", 0);
    _depura("END myplugin::myplugin", 0);
}


///
/**
**/
myplugin::~myplugin() {
    _depura("myplugin::~myplugin", 0);
    _depura("END myplugin::~myplugin", 0);
}


///
/**
\return
**/
void myplugin::cambia(bool valor) {
    _depura("myplugin::cambia", 0);
	confpr->setValor(CONF_DEBUG, (valor?"TRUE":"FALSE"));
    _depura("END myplugin::cambia", 0);
}



///
/**
\param bcont
**/
void entryPoint(Bulmacont *bcont) {
    _depura("Entrada del plugin Corrector", 10);

    myplugin *corr = new myplugin();

    /// A&ntilde;ade en el men&uacute; del programa la opci&oacuteMn para
    /// acceder al corrector.
    QAction *viewCorrector = new QAction("&Modo Debug", 0);
    viewCorrector->setCheckable(TRUE);

    if(confpr->valor(CONF_DEBUG) == "TRUE") {
    	viewCorrector->setChecked(TRUE);
    } else {
	viewCorrector->setChecked(FALSE);
    }

    viewCorrector->setStatusTip("Activa/Desactiva el Modo Debug");
    viewCorrector->setWhatsThis("Debug.\n\nActiva/Desactiva el Modo Debug");
    QObject::connect(viewCorrector, SIGNAL(toggled(bool)), corr, SLOT(cambia(bool)));
    bcont->mui_MenuVer()->addSeparator();
    bcont->mui_MenuVer()->addAction(viewCorrector);
    _depura("Iniciado correctamente el plugin Corrector", 10);
}

