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
#include <QObject>
#include <QMessageBox>
#include <QLineEdit>
#include <QTabWidget>

#include "company.h"
#include "plugincontratos.h"
#include "funcaux.h"
#include "contratoslist.h"



myplugincont::myplugincont() {}


myplugincont::~myplugincont() {}


void myplugincont::elslot() {
    ContratosList *vehiculoview = new ContratosList((company *) m_conexionbase);
    m_bulmafact->workspace()->addWindow(vehiculoview);
    vehiculoview->show();
}


void myplugincont::inicializa(bulmafact *bges) {
    /// Creamos el men&uacute;.
    m_conexionbase = bges->getcompany();
    m_bulmafact = bges;
    QAction *accion = new QAction("&Contratos", 0);
    accion->setStatusTip("Listado de Contratos");
    accion->setWhatsThis("Listado de Contratos");
    connect(accion, SIGNAL(activated()), this, SLOT(elslot()));
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuVentas->addSeparator();
    bges->menuVentas->addAction(accion);
}


int entryPoint(bulmafact *bges) {
    _depura("Punto de Entrada del plugin PluginContratos", 0);
    myplugincont *plug = new myplugincont();
    plug->inicializa(bges);
    _depura("END Punto de Entrada del plugin PluginContratos", 0);
    return 0;
}


int ClienteView_ClienteView(ClienteView *art) {
    _depura("ClienteView_ClienteView", 0);
    /// Para que funcione bien debemos iniciar con SelectMode y luego pasar a EditMode ya que si no se hace un meteWindow y no es deseable.
    ContratosList *l = new ContratosList(art->companyact(), art, 0, ContratosList::SelectMode);
    l->setObjectName(QString::fromUtf8("ccontratoslist"));
    art->mui_tab->addTab(l, "Contratos");
    l->editMode();
    _depura("END ClienteView_ClienteView", 0);
    return 0;
}

int ClienteView_cargar(ClienteView *art) {
    _depura("ClienteView_cargar", 0);
    ContratosList *l = art->findChild<ContratosList *>("ccontratoslist");
    l->mui_idcliente->setidcliente(art->DBvalue("idcliente"));
    l->on_mui_actualizar_clicked();
    _depura("END ClienteView_cargar", 0);
    return 0;
}


