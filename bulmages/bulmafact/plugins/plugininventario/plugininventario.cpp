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

#include "plugininventario.h"
#include "company.h"
#include "funcaux.h"
#include "inventariosview.h"


myplugininv::myplugininv() {}


myplugininv::~myplugininv() {}


void myplugininv::elslot() {
    InventariosView *tar = new InventariosView((Company *)empresaBase(), NULL);
    empresaBase()->m_pWorkspace->addWindow(tar);
    tar->show();
}


void myplugininv::inicializa(Bulmafact *bges) {
    /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
    m_bges = bges;
    setEmpresaBase(bges->getcompany());
    QAction *planCuentas = new QAction(tr("&Inventarios"), 0);
    planCuentas->setStatusTip(tr("Inventarios"));
    planCuentas->setWhatsThis(tr("Inventarios"));
    bges->menuArticulos->addSeparator();
    bges->menuArticulos->addAction(planCuentas);
    connect(planCuentas, SIGNAL(activated()), this, SLOT(elslot()));
}


int entryPoint(Bulmafact *bges) {
    _depura("Punto de Entrada del plugin de Tarifas\n", 0);
    myplugininv *plug = new myplugininv();
    plug->inicializa(bges);
    return 0;
}

