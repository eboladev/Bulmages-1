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

#include <Q3PopupMenu>
#include <QAction>
#include <QObject>
#include <QMessageBox>

#include <stdio.h>

#include "plugin.h"
#include "company.h"
#include "funcaux.h"


myplugin:: myplugin() {}

myplugin::~myplugin() {}


void myplugin::elslot() {
    fprintf(stderr,"SE ha activado el slot\n");
    QMessageBox::warning(0,
                         "Guardar Familia",
                         "Desea guardar los cambios.",
                         QMessageBox::Ok,
                         QMessageBox::Cancel);
}


void myplugin::inicializa(bulmafact *bges) {
    /// El menu de empresa.
    Q3PopupMenu *pPluginMenu = new Q3PopupMenu();
    pPluginMenu->setCheckable(true);
    bges->menuBar()->insertItem("&Plugin", pPluginMenu);
    QAction *planCuentas = new QAction("&Prueba de Plugin", 0);
    planCuentas->setStatusTip("Muestra el plan contable");
    planCuentas->setWhatsThis("Muestra el plan contable");
    planCuentas->addTo(pPluginMenu);
    connect(planCuentas, SIGNAL(activated()), this, SLOT(elslot()));
}


void entryPoint(bulmafact *bges) {
    _depura("Estoy dentro del plugin\n", 0);
    myplugin *plug = new myplugin();
    plug->inicializa(bges);
    bges->setCaption("Prueba de plugin.");
}

