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

#include <QDialog>

#include "busquedaproveedor.h"
#include "providerslist.h"
#include "company.h"
#include "funcaux.h"


BusquedaProveedor::BusquedaProveedor(QWidget *parent, const char *name)
        : QWidget(parent, name) {
    setupUi(this);
    companyact = NULL;
    mdb_idproveedor = "";
    mdb_nomproveedor = "";
    mdb_cifproveedor = "";
}


BusquedaProveedor::~BusquedaProveedor() {}


void BusquedaProveedor::setidproveedor(QString val) {
    mdb_idproveedor = val;
    QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor='" + mdb_idproveedor + "'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        mdb_cifproveedor = cur->valor("cifproveedor");
        mdb_nomproveedor = cur->valor("nomproveedor");
    } else {
        mdb_idproveedor = "";
        mdb_nomproveedor = "";
        mdb_cifproveedor = "";
    } // end if
    delete cur;
    m_cifproveedor->setText(mdb_cifproveedor);
    m_nomproveedor->setText(mdb_nomproveedor);
}


void BusquedaProveedor::setcifproveedor(QString val) {
    mdb_cifproveedor = val;
    QString SQLQuery = "SELECT * FROM proveedor WHERE cifproveedor='" + mdb_cifproveedor + "'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);

    if (!cur->eof()) {
        mdb_idproveedor = cur->valor("idproveedor");
        mdb_nomproveedor = cur->valor("nomproveedor");
    } else {
        mdb_idproveedor = "";
        mdb_nomproveedor = "";
    } // end if

    delete cur;
    m_cifproveedor->setText(mdb_cifproveedor);
    m_nomproveedor->setText(mdb_nomproveedor);
}


/// Busqueda de proveedor.
void BusquedaProveedor::on_mui_buscar_clicked() {
    _depura("Busqueda de un provider\n", 0);

    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    QDialog *diag=new QDialog(0);
    diag->setModal(true);

    ProveedorList *providers = new ProveedorList(companyact, diag, tr("Seleccione proveedor","company"), 0, ProveedorList::SelectMode);

    connect(providers, SIGNAL(selected(QString)), diag, SLOT(accept()));
    diag->exec();

    if (providers->cifprovider() != "") {
        m_cifproveedor->setText(providers->cifprovider());
        mdb_cifproveedor = providers->cifprovider();
        m_nomproveedor->setText(providers->nomprovider());
        mdb_nomproveedor = providers->nomprovider();
        mdb_idproveedor = providers->idprovider();
    } // end if

    delete providers;
    delete diag;
}


void BusquedaProveedor::on_m_cifproveedor_textChanged(const QString &val) {
    mdb_cifproveedor = val;
    QString SQLQuery = "SELECT * FROM proveedor WHERE cifproveedor='" + mdb_cifproveedor + "'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);

    if (!cur->eof()) {
        mdb_idproveedor = cur->valor("idproveedor");
        mdb_nomproveedor = cur->valor("nomproveedor");
    } else {
        mdb_idproveedor = "";
        mdb_nomproveedor = "";
    } // end if

    delete cur;
    m_cifproveedor->setText(mdb_cifproveedor);
    m_nomproveedor->setText(mdb_nomproveedor);
    emit(valueChanged(mdb_idproveedor));
}

