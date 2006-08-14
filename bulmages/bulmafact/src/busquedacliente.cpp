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


BusquedaCliente::BusquedaCliente(QWidget *parent)
        : QWidget(parent) {
    _depura("BusquedaCliente::BusquedaCliente", 0);
    setupUi(this);
    companyact = NULL;
    mdb_idcliente = "";
    mdb_nomcliente = "";
    mdb_cifcliente = "";
    mdb_codcliente = "";
    m_semaforo = FALSE;
    _depura("END BusquedaCliente::BusquedaCliente", 0);
}


BusquedaCliente::~BusquedaCliente() {}


void BusquedaCliente::pinta() {
    _depura ("BusquedaCliente::pinta", 0);
    m_semaforo = TRUE;
    m_cifcliente->setText(mdb_cifcliente);
    m_nomcliente->setText(mdb_codcliente+".- "+mdb_nomcliente);
    m_semaforo = FALSE;
    _depura ("END BusquedaCliente::pinta", 0);

}

void BusquedaCliente::setidcliente(QString val) {
    _depura("BusquedaCliente::setidcliente", 0);
    mdb_idcliente = val;
    QString SQLQuery = "SELECT * FROM cliente WHERE idcliente='" + mdb_idcliente + "'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);

    if(!cur->eof()) {
        mdb_cifcliente = cur->valor("cifcliente");
        mdb_nomcliente = cur->valor("nomcliente");
        mdb_codcliente = cur->valor("codcliente");
    } else {
        mdb_idcliente = "";
        mdb_nomcliente = "";
        mdb_cifcliente = "";
        mdb_codcliente = "";
    } // end if

    delete cur;
    pinta();
    _depura("END BusquedaCliente::setidcliente", 0);
}


void BusquedaCliente::setcifcliente(QString val) {
    _depura("BusquedaCliente::setcifcliente", 0, val);
    mdb_cifcliente = val;
    QString SQLQuery = "SELECT * FROM cliente WHERE cifcliente='" + mdb_cifcliente + "'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);

    if(!cur->eof()) {
        mdb_idcliente = cur->valor("idcliente");
        mdb_nomcliente = cur->valor("nomcliente");
        mdb_codcliente = cur->valor("codcliente");
    } else {
        mdb_idcliente = "";
        mdb_nomcliente = "";
        mdb_codcliente = "";
    } // end if

    delete cur;
    pinta();
    _depura("END BusquedaCliente::setcifcliente", 0, val);
}


/// Busqueda de clientes.
void BusquedaCliente::on_mui_buscar_clicked() {
    _depura("BusquedaCliente::on_mui_buscar_clicked\n", 0);
    QDialog *diag = new QDialog(0);
    diag->setModal(true);

    ClientsList *clients = new ClientsList(companyact, diag, 0, ClientsList::SelectMode);
    connect(clients, SIGNAL(selected(QString)), diag, SLOT(accept()));
    diag->exec();
    if (clients->cifclient() != "") {
        setcifcliente(clients->cifclient());
	emit(valueChanged(mdb_idcliente));
    } // end if
    delete diag;
    _depura("END BusquedaCliente::on_mui_buscar_clicked\n", 0);

}


void BusquedaCliente::on_m_cifcliente_editingFinished() {
    _depura("BusquedaCliente::on_m_cifcliente_editingFinished", 0);
    pinta();
    emit(valueChanged(mdb_idcliente));
    _depura("END BusquedaCliente::on_m_cifcliente_editingFinished", 0);

}

void BusquedaCliente::on_m_cifcliente_textChanged(const QString &val) {
    _depura("BusquedaCliente::on_m_cifcliente_textChanged", 0);
    if (m_semaforo)
        return;

    bool encontrado = FALSE;
    QString SQLQuery = "SELECT * FROM cliente WHERE cifcliente='" + val + "'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idcliente = cur->valor("idcliente");
        mdb_nomcliente = cur->valor("nomcliente");
        mdb_cifcliente = cur->valor("cifcliente");
        mdb_codcliente = cur->valor("codcliente");
        encontrado = TRUE;
    }
    delete cur;

    if (! encontrado) {
        QString SQLQuery = "SELECT * FROM cliente WHERE codcliente='" + val + "'";
        cur = companyact->cargacursor(SQLQuery);
        if(!cur->eof()) {
            mdb_idcliente = cur->valor("idcliente");
            mdb_nomcliente = cur->valor("nomcliente");
            mdb_cifcliente = cur->valor("cifcliente");
            mdb_codcliente = cur->valor("codcliente");
            encontrado = TRUE;
        } // end if
        delete cur;
    } // end if


    if (! encontrado) {
        QString SQLQuery = "SELECT * FROM cliente WHERE upper(nomcliente) LIKE upper('%" + val + "%')";
        cur = companyact->cargacursor(SQLQuery);
        if(cur->numregistros() == 1) {
            mdb_idcliente = cur->valor("idcliente");
            mdb_nomcliente = cur->valor("nomcliente");
            mdb_cifcliente = cur->valor("cifcliente");
            mdb_codcliente = cur->valor("codcliente");
            encontrado = TRUE;
        } // end if
        delete cur;
    } // end if


    if(!encontrado) {
        m_nomcliente->setText("");
        mdb_idcliente = "";
        mdb_cifcliente = "";
        mdb_codcliente = "";
        mdb_nomcliente = "";
    } // end if

    if (encontrado) {
        m_nomcliente->setText(mdb_codcliente+".- "+mdb_nomcliente);
    } // end if
    _depura("END BusquedaCliente::on_m_cifcliente_textChanged", 0);

}

