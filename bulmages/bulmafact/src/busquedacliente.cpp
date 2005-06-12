//
// C++ Implementation: busquedacliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "busquedacliente.h"
#include "clientslist.h"
#include "company.h"

BusquedaCliente::BusquedaCliente(QWidget *parent, const char *name)
: BusquedaClienteBase(parent, name) {
    companyact=NULL;
    mdb_idcliente="";
    mdb_nomcliente="";
    mdb_cifcliente="";
}


BusquedaCliente::~BusquedaCliente() {}

void BusquedaCliente::setidcliente(QString val) {
    mdb_idcliente=val;
    QString SQLQuery = "SELECT * FROM cliente WHERE idcliente='"+mdb_idcliente+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_cifcliente = cur->valor("cifcliente");
        mdb_nomcliente = cur->valor("nomcliente");
    } else {
        mdb_idcliente="";
        mdb_nomcliente="";
        mdb_cifcliente="";
    }// end if
    delete cur;
    m_cifcliente->setText(mdb_cifcliente);
    m_nomcliente->setText(mdb_nomcliente);

}


void BusquedaCliente::setcifcliente(QString val) {
    mdb_cifcliente=val;
    QString SQLQuery = "SELECT * FROM cliente WHERE cifcliente='"+mdb_cifcliente+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idcliente = cur->valor("idcliente");
        mdb_nomcliente = cur->valor("nomcliente");
    } else {
        mdb_idcliente="";
        mdb_nomcliente="";
    }// end if
    delete cur;
    m_cifcliente->setText(mdb_cifcliente);
    m_nomcliente->setText(mdb_nomcliente);
}


// Bsqueda de Clientes.
void BusquedaCliente::s_searchCliente() {
    fprintf(stderr,"Busqueda de un client\n");
    ClientsList *clients = new ClientsList(companyact, NULL, tr("Seleccione cliente","company"));
    // , WType_Dialog| WShowModal
    clients->selectMode();
    // Esto es convertir un QWidget en un sistema modal de dialogo.
    this->setEnabled(false);
    clients->show();
    while(!clients->isHidden())
        theApp->processEvents();
    this->setEnabled(true);
    if (clients->cifclient() !="" && clients->cifclient() !=NULL) {
        m_cifcliente->setText(clients->cifclient());
        mdb_cifcliente = clients->cifclient();
        m_nomcliente->setText(clients->nomclient());
        mdb_nomcliente = clients->nomclient();
        mdb_idcliente = clients->idclient();
    }// end if
    delete clients;
//    emit(valueChanged(mdb_idcliente));
}// end searchClient


void BusquedaCliente::s_cifclientetextChanged(const QString &val) {
    mdb_cifcliente=val;
    QString SQLQuery = "SELECT * FROM cliente WHERE cifcliente='"+mdb_cifcliente+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idcliente = cur->valor("idcliente");
        mdb_nomcliente = cur->valor("nomcliente");
    } else {
        mdb_idcliente="";
        mdb_nomcliente="";
    }// end if
    delete cur;
    m_cifcliente->setText(mdb_cifcliente);
    m_nomcliente->setText(mdb_nomcliente);
    emit(valueChanged(mdb_idcliente));
}// end setCifClient
