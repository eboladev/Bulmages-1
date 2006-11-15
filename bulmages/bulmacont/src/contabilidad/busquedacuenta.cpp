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

#include "busquedacuenta.h"
#include "../contabilidad/listcuentasview1.h"
#include "../contabilidad/empresa.h"


BusquedaCuenta::BusquedaCuenta(QWidget *parent)
        : QWidget(parent) {
    _depura("BusquedaCuenta::BusquedaCuenta", 0);
    setupUi(this);
    empresaactual = NULL;
    conexionbase = NULL;
    mdb_idcuenta = "";
    mdb_nomcuenta = "";
    mdb_codigocuenta = "";
    mdb_tipocuenta = "";
    connect(mui_codigocuenta, SIGNAL(editingFinished()), this, SLOT(s_lostFocus()));
    _depura("END BusquedaCuenta::BusquedaCuenta", 0);
}


BusquedaCuenta::~BusquedaCuenta() {
    _depura("END BusquedaCuenta::~BusquedaCuenta", 0);
}


void BusquedaCuenta::setempresa(empresa *comp) {
    _depura("BusquedaCuenta::setempresa", 0);
    empresaactual = comp;
    conexionbase = empresaactual->bdempresa();
    m_numdigitos = empresaactual->numdigitosempresa();
    _depura("END BusquedaCuenta::setempresa", 0);
}


void BusquedaCuenta::setidcuenta(QString val) {
    _depura("BusquedaCuenta::setidcuenta", 0);
    mdb_idcuenta=val;
    QString SQLQuery = "SELECT * FROM cuenta WHERE idcuenta = '" + mdb_idcuenta + "'";
    cursor2 *cur = conexionbase->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_codigocuenta = cur->valor("codigo");
        mdb_nomcuenta = cur->valor("descripcion");
        mdb_tipocuenta = cur->valor("tipocuenta");
    } else {
        mdb_idcuenta = "";
        mdb_nomcuenta = "";
        mdb_codigocuenta = "";
        mdb_tipocuenta = "";
    } // end if
    delete cur;
    mui_codigocuenta->setText(mdb_codigocuenta);
    mui_nomcuenta->setText(mdb_nomcuenta);
    _depura("END BusquedaCuenta::setidcuenta", 0);
}


void BusquedaCuenta::setcodigocuenta(QString val) {
    _depura("BusquedaCuenta::setcodigocuenta", 0);
    mdb_codigocuenta = val;
    QString SQLQuery = "SELECT * FROM cuenta WHERE codigo = '" + mdb_codigocuenta + "'";
    cursor2 *cur = conexionbase->cargacursor(SQLQuery);
    if (!cur->eof()) {
        mdb_idcuenta = cur->valor("idcuenta");
        mdb_nomcuenta = cur->valor("descripcion");
        mdb_tipocuenta = cur->valor("tipocuenta");
    } else {
        mdb_idcuenta = "";
        mdb_nomcuenta = "";
        mdb_tipocuenta = "";
    } // end if
    delete cur;
    mui_codigocuenta->setText(mdb_codigocuenta);
    mui_nomcuenta->setText(mdb_nomcuenta);
    _depura("END BusquedaCuenta::setcodigocuenta", 0);
}


/// B&uacute;squeda de cuentas.
void BusquedaCuenta::s_searchCuenta() {
    _depura("BusquedaCuenta::s_searchCuenta", 0);
    /// Generamos un di&aacute;logo.
    QDialog *diag = new QDialog(0);
    diag->setModal(true);

    /// Creamos una instancia del selector de cuentas.
    listcuentasview1 *listcuentas = new listcuentasview1(empresaactual, diag, 0, listcuentasview1::SelectMode);

    /// Hacemos la conexi&oacute;n del cerrar de las cuentas con el cerrar di&aacute;logo.
    connect(listcuentas, SIGNAL(selected(QString)), diag, SLOT(accept()));
    connect(listcuentas, SIGNAL(destroyed(QObject)), diag, SLOT(accept()));
    listcuentas->inicializa();

    /// Sacamos el di&aacute;logo.
    diag->exec();
    if (listcuentas->codcuenta() != "") {
        mdb_idcuenta = listcuentas->idcuenta();
        mdb_codigocuenta= listcuentas->codcuenta();
        mdb_nomcuenta = listcuentas->desccuenta();
        mui_codigocuenta->setText(mdb_codigocuenta);
        mui_nomcuenta->setText(mdb_nomcuenta);
    } // end if
    delete diag;
    _depura("END BusquedaCuenta::s_searchCuenta", 0);
}


void BusquedaCuenta::s_codigocuentatextChanged(const QString &val) {
    _depura("BusquedaCuenta::s_codigocuentatextChanged", 0, val);
    if (val == "+") {
        s_searchCuenta();
        emit(valueChanged(mui_codigocuenta->text()));
    } // end if
    _depura("END BusquedaCuenta::s_codigocuentatextChanged", 0);
}


void BusquedaCuenta::s_lostFocus() {
    _depura("BusquedaCuenta::s_lostFocus", 0);
    mdb_codigocuenta=mui_codigocuenta->text();
    QString cad = mdb_codigocuenta;
    if (cad != "") {
        cad = extiendecodigo(cad,m_numdigitos);
        cursor2 *cursorcta = conexionbase->cargacuenta(0, cad);
        int num = cursorcta->numregistros();
        if (num == 1) {
            mdb_codigocuenta = cursorcta->valor("codigo");
            mdb_idcuenta = cursorcta->valor("idcuenta");
            mdb_nomcuenta = cursorcta->valor("descripcion");
            mdb_tipocuenta = cursorcta->valor("tipocuenta");
            mui_codigocuenta->setText(mdb_codigocuenta);
            mui_nomcuenta->setText(mdb_nomcuenta);
        } else {
            mdb_idcuenta = "";
            mdb_codigocuenta = "";
            mdb_nomcuenta = "";
            mdb_tipocuenta = "";
            mui_nomcuenta->setText("");
        } // end if
        delete cursorcta;
    } // end if
    emit(valueChanged(mdb_idcuenta));
    _depura("END BusquedaCuenta::s_lostFocus", 0);
}

