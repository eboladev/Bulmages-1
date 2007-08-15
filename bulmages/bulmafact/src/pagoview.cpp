/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>
#include <QCloseEvent>

#include <fstream>

#include "pagoview.h"
#include "company.h"
#include "configuracion.h"
#include "busquedaproveedor.h"
#include "busquedafecha.h"

#include "funcaux.h"


PagoView::PagoView(Company *comp, QWidget *parent)
        :  Pago(comp, parent) {
    try {
        setAttribute(Qt::WA_DeleteOnClose);
        setupUi(this);
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        mui_proveedor->setEmpresaBase(comp);
        mui_refpago->setEmpresaBase(comp);
        mui_idbanco->setEmpresaBase(comp);
        meteWindow(windowTitle(), this, FALSE);
	pintar();
        dialogChanges_cargaInicial();
    } catch (...) {
        mensajeInfo(tr("Error al crear el pago"));
    } // end try
    _depura("Fin de la inicializacion de PagoView\n", 0);
}


PagoView::~PagoView() {
    _depura("PagoView::~PagoView", 0);
    _depura("END PagoView::~PagoView", 0);
}


int PagoView::cargar(QString id) {
    _depura("PagoView::cargar", 0);
    try {
        if (Pago::cargar(id))
            throw -1;
        setWindowTitle(tr("Pago") + " " + DBvalue("refpago")+" "+ DBvalue("idpago"));
        dialogChanges_cargaInicial();
        meteWindow(windowTitle(), this);
    } catch (...) {
        return -1;
    } // end try
    _depura("END PagoView::cargar", 0);
    return 0;
}


void PagoView::pintafechapago(QString id) {
    mui_fechapago->setText(id);
}
void PagoView::pintacomentpago(QString id) {
    mui_comentpago->setText(id);
}
void PagoView::pintaidproveedor(QString id) {
    mui_proveedor->setidproveedor(id);
}
void PagoView::pintarefpago(QString id) {
    mui_refpago->setText(id);
}
void PagoView::pintacantpago(QString id) {
    mui_cantpago->setText(id);
}
void PagoView::pintaidbanco(QString id) {
    mui_idbanco->setidbanco(id);
}
void PagoView::pintaprevisionpago(QString id) {
    if (id == "t" || id == "TRUE") {
        mui_previsionpago->setChecked(TRUE);
    } else {
        mui_previsionpago->setChecked(FALSE);
    } // end if
}

void PagoView::on_mui_comentpago_textChanged(const QString &str) {
    setcomentpago(str);
}
void PagoView::on_mui_refpago_valueChanged(const QString &str) {
    setrefpago(str);
}
void PagoView::on_mui_cantpago_textChanged(const QString &str) {
    setcantpago(str);
}
void PagoView::on_mui_previsionpago_stateChanged(int i) {
    if (i) {
        setprevisionpago("TRUE");
    } else {
        setprevisionpago("FALSE");
    } // end if
}
void PagoView::on_mui_proveedor_valueChanged(QString id) {
    setidproveedor(id);
}
void PagoView::on_mui_fechapago_valueChanged(QString id) {
    setfechapago(id);
}
void PagoView::on_mui_idbanco_valueChanged(QString id) {
    setidbanco(id);
}
