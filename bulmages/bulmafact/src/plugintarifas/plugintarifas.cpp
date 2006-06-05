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
#include <QLineEdit>

#include <stdio.h>

#include "plugintarifas.h"
#include "company.h"
#include "funcaux.h"
#include "busquedatarifa.h"
#include "listltarifaview.h"
#include "tarifalistview.h"


myplugin1::myplugin1() {}


myplugin1::~myplugin1() {}


void myplugin1::elslot() {
    TarifaListView *tar = new TarifaListView(m_companyact, NULL);
    m_companyact->m_pWorkspace->addWindow(tar);
    tar->show();
}


void myplugin1::inicializa(bulmafact *bges) {
    /// El menu de Tarifas en la seccion de articulos
    m_bges = bges;
    m_companyact = bges->getcompany();
    QAction *planCuentas = new QAction("&Tarifas", 0);
    planCuentas->setStatusTip("Tarifas");
    planCuentas->setWhatsThis("Tarifas");
    bges->menuArticulos->addSeparator();
    planCuentas->addTo(bges->menuArticulos);
    connect(planCuentas, SIGNAL(activated()), this, SLOT(elslot()));
}


int entryPoint(bulmafact *bges) {
    _depura("Punto de Entrada del plugin de Tarifas\n", 0);
    myplugin1 *plug= new myplugin1();
    plug->inicializa(bges);
    return 0;
}


int ClienteView_ClienteView(ClienteView *cli) {
    _depura("dentro del plugin", 0);

    cli->addDBCampo("idtarifa", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Identificador", "plugintarifas"));

    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing(2);
    hboxLayout160->setMargin(0);
    hboxLayout160->setObjectName(QString::fromUtf8("hboxLayout16"));

    QLabel *textLabel2_9_26 = new QLabel(cli->m_frameplugin);
    textLabel2_9_26->setObjectName(QString::fromUtf8("textLabel2_9_2"));
    hboxLayout160->addWidget(textLabel2_9_26);
    textLabel2_9_26->setText("Tarifa");

    BusquedaTarifa *bus = new BusquedaTarifa(cli->m_frameplugin);
    bus->setObjectName(QString::fromUtf8("bus"));
    bus->setcompany(cli->companyact());
    bus->setidtarifa("");
    hboxLayout160->addWidget(bus);

    cli->vboxLayout2->addLayout(hboxLayout160);
    return 0;
}


int ClienteView_saveClient(ClienteView *cli) {
    BusquedaTarifa *bus = cli->findChild<BusquedaTarifa *>("bus");
    cli->setDBvalue("idtarifa", bus->idtarifa());
    return 0;
}


int Cliente_pintaCliente(Cliente *cli) {
    ClienteView *clo = (ClienteView *) cli;
    BusquedaTarifa *bus = clo->findChild<BusquedaTarifa *>("bus");
    bus->setidtarifa(clo->DBvalue("idtarifa"));
    return 0;
}


int ArticuloView_ArticuloView(ArticuloView *art) {
    _depura("ArticuloView_ArticuloView", 0);
    ListLTarifaView *l = new ListLTarifaView(art);
    l->setObjectName(QString::fromUtf8("ltarifas"));
    l->setcompany( art->companyact());
    l->cargar("0");
    art->mui_tab->addTab(l, "Tarifas");
    _depura("END ArticuloView_ArticuloView", 0);
    return 0;
}


int ArticuloView_cargar(ArticuloView *art) {
    _depura("ArticuloView_cargar", 0);
    ListLTarifaView *l = art->findChild<ListLTarifaView *>("ltarifas");
    l->cargar(art->DBvalue("idarticulo"));
    _depura("END ArticuloView_cargar", 0);
    return 0;
}


int ArticuloView_guardar_post(ArticuloView *art) {
    _depura("ArticuloView_guardar_post", 0);
    try {
        ListLTarifaView *l = art->findChild<ListLTarifaView *>("ltarifas");
        l->setColumnValue( "idarticulo", art->DBvalue("idarticulo"));
        l->guardar();
        return 0;
    } catch (...) {
        _depura("Hubo un al el guardar de las tarifas", 2);
        return 0;
    }
}


int ArticuloView_borrar(ArticuloView *art) {
    _depura("ArticuloView_borrar", 0);
    try {
        ListLTarifaView *l = art->findChild<ListLTarifaView *>("ltarifas");
        l->borrar();
        return 0;
    } catch (...) {
        _depura("Hubo un al el guardar de las tarifas", 0);
        throw -1;
    }
}

