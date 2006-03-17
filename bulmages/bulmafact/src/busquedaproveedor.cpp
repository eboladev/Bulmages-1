//
// C++ Implementation: busquedaproveedor
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QDialog>

#include "busquedaproveedor.h"
#include "providerslist.h"
#include "company.h"
#include "funcaux.h"

BusquedaProveedor::BusquedaProveedor(QWidget *parent, const char *name)
: BusquedaProveedorBase(parent, name) {
    companyact=NULL;
    mdb_idproveedor="";
    mdb_nomproveedor="";
    mdb_cifproveedor="";
}


BusquedaProveedor::~BusquedaProveedor() {}

void BusquedaProveedor::setidproveedor(QString val) {
    mdb_idproveedor=val;
    QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor='"+mdb_idproveedor+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_cifproveedor = cur->valor("cifproveedor");
        mdb_nomproveedor = cur->valor("nomproveedor");
    } else {
        mdb_idproveedor="";
        mdb_nomproveedor="";
        mdb_cifproveedor="";
    }// end if
    delete cur;
    m_cifproveedor->setText(mdb_cifproveedor);
    m_nomproveedor->setText(mdb_nomproveedor);

}


void BusquedaProveedor::setcifproveedor(QString val) {
    mdb_cifproveedor=val;
    QString SQLQuery = "SELECT * FROM proveedor WHERE cifproveedor='"+mdb_cifproveedor+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idproveedor = cur->valor("idproveedor");
        mdb_nomproveedor = cur->valor("nomproveedor");
    } else {
        mdb_idproveedor="";
        mdb_nomproveedor="";
    }// end if
    delete cur;
    m_cifproveedor->setText(mdb_cifproveedor);
    m_nomproveedor->setText(mdb_nomproveedor);
}


// Bsqueda de Proveedors.
void BusquedaProveedor::s_searchProveedor() {
    _depura("Busqueda de un provider\n",0);

    // Esto es convertir un QWidget en un sistema modal de dialogo.

	QDialog *diag=new QDialog(0);
	diag->setModal(true);

    ProveedorList *providers = new ProveedorList(companyact, diag, tr("Seleccione proveedor","company"),0,ProveedorList::SelectMode);

	connect(providers, SIGNAL(selected(QString)), diag, SLOT(accept()));
	diag->exec();



    if (providers->cifprovider() !="") {
        m_cifproveedor->setText(providers->cifprovider());
        mdb_cifproveedor = providers->cifprovider();
        m_nomproveedor->setText(providers->nomprovider());
        mdb_nomproveedor = providers->nomprovider();
        mdb_idproveedor = providers->idprovider();
    }// end if
    delete providers;
	delete diag;
}// end searchClient


void BusquedaProveedor::s_cifproveedortextChanged(const QString &val) {
    mdb_cifproveedor=val;
    QString SQLQuery = "SELECT * FROM proveedor WHERE cifproveedor='"+mdb_cifproveedor+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_idproveedor = cur->valor("idproveedor");
        mdb_nomproveedor = cur->valor("nomproveedor");
    } else {
        mdb_idproveedor="";
        mdb_nomproveedor="";
    }// end if
    delete cur;
    m_cifproveedor->setText(mdb_cifproveedor);
    m_nomproveedor->setText(mdb_nomproveedor);
    emit(valueChanged(mdb_idproveedor));
}// end setCifClient
