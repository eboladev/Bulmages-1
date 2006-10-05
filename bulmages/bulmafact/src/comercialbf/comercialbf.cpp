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
#include "myplugin.h"
#include "comercialbf.h"
#include "comercialclienteview.h"


int entryPoint(bulmafact *bges) {
    _depura("Punto de Entrada del plugin ComercialBF", 0);
    mypluginbf *plug = new mypluginbf();
    plug->inicializa(bges);
    _depura("END Punto de Entrada del plugin ComercialBF", 0);
    return 0;
}


int ClienteView_ClienteView_Post(ClienteView *cli) {
    _depura("ClienteView_ClienteView_Post",0);
    /// Agregamos los campos de registro que hacen falta.
    cli->addDBCampo("idzonacomercial", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("ClienteView_ClienteView_Post", "Zona comercial"));
    cli->addDBCampo("fechabasecomercialcliente", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("ClienteView_ClienteView_Post", "Fecha base"));
    cli->addDBCampo("periodocomercialcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("ClienteView_ClienteView_Post", "Periodo"));
    cli->addDBCampo("comentcomercialcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("ClienteView_ClienteView_Post", "Comentarios para el comercial"));
    /// Creamos la pesta&ntilde;a de comercial y la tratamos.
    ComercialClienteView *l = new ComercialClienteView(cli);
    l->setcompany(cli->companyact());
    l->setObjectName(QString::fromUtf8("lcomercial"));
    cli->mui_tab->addTab(l, QApplication::translate("ClienteView_ClienteView_Post", "&Comercial"));
    cli->mui_tab->removeTab(8);
    cli->mui_tab->removeTab(7);
    cli->mui_tab->removeTab(6);
    _depura("END ClienteView_ClienteView_Post", 0);
    return 0;
}


int ClienteView_Des_ClienteView_Post(ClienteView *cli) {
    _depura("ClienteView_Des_ClienteView", 0);
    /// Creamos la pesta&ntilde;a de comercial y la tratamos.
    ComercialClienteView *bus = cli->findChild<ComercialClienteView *>("lcomercial");
    delete bus;
    _depura("END ClienteView_Des_ClienteView", 0);
    return 0;
}


int ClienteView_saveClient(ClienteView *cli) {
    _depura("ClienteView_saveClient", 0);
    ComercialClienteView *bus = cli->findChild<ComercialClienteView *>("lcomercial");
    cli->setDBvalue("idzonacomercial", bus->mui_zonacomercial->idzonacomercial());
    cli->setDBvalue("fechabasecomercialcliente", bus->mui_fechabasecomercialcliente->fecha());
    cli->setDBvalue("periodocomercialcliente", bus->mui_periodocomercialcliente->periodo());
    cli->setDBvalue("comentcomercialcliente", bus->mui_comentcomercialcliente->toPlainText());

    _depura("END ClienteView_saveClient", 0);
    return 0;
}


int Cliente_pintaCliente(Cliente *cli) {
    _depura("Cliente_pintaCliente", 0);
    ClienteView *clo = (ClienteView *) cli;
    ComercialClienteView *bus = clo->findChild<ComercialClienteView *>("lcomercial");
    bus->mui_zonacomercial->setidzonacomercial(clo->DBvalue("idzonacomercial"));
    bus->mui_fechabasecomercialcliente->setfecha(clo->DBvalue("fechabasecomercialcliente"));
    bus->mui_periodocomercialcliente->setperiodo(clo->DBvalue("periodocomercialcliente"));
    bus->mui_comentcomercialcliente->setPlainText(clo->DBvalue("comentcomercialcliente"));
    _depura("END Cliente_pintaCliente", 0);
    return 0;
}

