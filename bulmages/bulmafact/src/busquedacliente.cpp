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

#include "busquedacliente.h"
#include "clientslist.h"
#include "company.h"
#include "funcaux.h"


BusquedaCliente::BusquedaCliente(QWidget *parent, const char *name)
        : QWidget(parent, name) {
    setupUi(this);
    companyact = NULL;
    mdb_idcliente = "";
    mdb_nomcliente = "";
    mdb_cifcliente = "";
}


BusquedaCliente::~BusquedaCliente() {}


void BusquedaCliente::setidcliente(QString val) {
    _depura("BusquedaCliente::setidcliente", 0);
    mdb_idcliente = val;
    QString SQLQuery = "SELECT * FROM cliente WHERE idcliente='" + mdb_idcliente + "'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);

    if(!cur->eof()) {
        mdb_cifcliente = cur->valor("cifcliente");
        mdb_nomcliente = cur->valor("nomcliente");
    } else {
        mdb_idcliente = "";
        mdb_nomcliente = "";
        mdb_cifcliente = "";
    } // end if

    delete cur;
    m_cifcliente->setText(mdb_cifcliente);
    m_nomcliente->setText(mdb_nomcliente);
    _depura("END BusquedaCliente::setidcliente", 0);
}


void BusquedaCliente::setcifcliente(QString val) {
    mdb_cifcliente = val;
    QString SQLQuery = "SELECT * FROM cliente WHERE cifcliente='" + mdb_cifcliente + "'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);

    if(!cur->eof()) {
        mdb_idcliente = cur->valor("idcliente");
        mdb_nomcliente = cur->valor("nomcliente");
    } else {
        mdb_idcliente = "";
        mdb_nomcliente = "";
    } // end if

    delete cur;
    m_cifcliente->setText(mdb_cifcliente);
    m_nomcliente->setText(mdb_nomcliente);
}


/// Busqueda de clientes.
void BusquedaCliente::on_mui_buscar_clicked() {
    _depura("BusquedaCliente::on_mui_buscar_clicked\n", 0);
    QDialog *diag = new QDialog(0);
    diag->setModal(true);

    ClientsList *clients = new ClientsList(companyact, diag, tr("Seleccione cliente", "company"), 0, ClientsList::SelectMode);
    connect(clients, SIGNAL(selected(QString)), diag, SLOT(accept()));
    diag->exec();
    if (clients->cifclient() != "") {
        m_cifcliente->setText(clients->cifclient());
        mdb_cifcliente = clients->cifclient();
        m_nomcliente->setText(clients->nomclient());
        mdb_nomcliente = clients->nomclient();
        mdb_idcliente = clients->idclient();
    } // end if
    delete diag;
}


void BusquedaCliente::on_m_cifcliente_textChanged(const QString &val) {
    mdb_cifcliente = val;
    QString SQLQuery = "SELECT * FROM cliente WHERE cifcliente='" + mdb_cifcliente + "'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);

    if(!cur->eof()) {
        mdb_idcliente = cur->valor("idcliente");
        mdb_nomcliente = cur->valor("nomcliente");
    } else {
        mdb_idcliente = "";
        mdb_nomcliente = "";
    } // end if

    delete cur;
    m_cifcliente->setText(mdb_cifcliente);
    m_nomcliente->setText(mdb_nomcliente);
    emit(valueChanged(mdb_idcliente));
}

