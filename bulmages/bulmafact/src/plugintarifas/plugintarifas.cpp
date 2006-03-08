#include <stdio.h>
#include <q3popupmenu.h>
#include <qaction.h>
#include <qobject.h>
#include <qmessagebox.h>
#include <QLineEdit>

#include "plugintarifas.h"
#include "company.h"
#include "funcaux.h"
#include "busquedatarifa.h"
#include "listltarifaview.h"




void entryPoint(bulmafact *bges) {
    _depura("Estoy dentro del plugin\n",0);
    bges->setCaption("Prueba de plugin Tarifas.");
}

int ClienteView_ClienteView(ClienteView *cli) {
    _depura("dentro del plugin",0);

    cli->addDBCampo("idtarifa", DBCampo::DBint, DBCampo::DBNothing, "Identificador");

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
    bus->setcompany( cli->companyact());
    bus->setidtarifa( "");
    hboxLayout160->addWidget(bus);

    cli->vboxLayout2->addLayout(hboxLayout160);
    return 0;
}

int ClienteView_saveClient(ClienteView *cli) {
    BusquedaTarifa *bus = cli->findChild<BusquedaTarifa *>("bus");
    cli->setDBvalue("idtarifa",bus->idtarifa());
    return 0;
}

int Cliente_pintaCliente(Cliente *cli) {
    ClienteView *clo = (ClienteView *) cli;
    BusquedaTarifa *bus = clo->findChild<BusquedaTarifa *>("bus");
    bus->setidtarifa( clo->DBvalue("idtarifa"));
    return 0;
}



int ArticuloView_ArticuloView(ArticuloView *art) {
    _depura("ArticuloView_ArticuloView",0);
    ListLTarifaView *l = new ListLTarifaView(art);
    l->setObjectName(QString::fromUtf8("ltarifas"));
    l->setcompany( art->companyact());
    art->tabWidget2->addTab(l, "Tarifas");
    return 0;
}


int ArticuloView_cargar(ArticuloView *art) {
    _depura("ArticuloView_cargar",0);
    ListLTarifaView *l = art->findChild<ListLTarifaView *>("ltarifas");
    l->cargarParaArticulo(art->idarticulo());
    l->pintar();

    return 0;
}
