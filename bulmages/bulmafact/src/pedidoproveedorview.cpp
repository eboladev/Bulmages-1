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

#include <fstream>

#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QLayout>

#include "pedidoproveedorview.h"
#include "company.h"
#include "listlinpedidoproveedorview.h"
#include "pedidoproveedor.h"
#include "providerslist.h"
#include "presupuestoview.h"
#include "presupuestolist.h"
#include "pagoview.h"
#include "albaranproveedorview.h"
#include "funcaux.h"


PedidoProveedorView::PedidoProveedorView(company *comp, QWidget *parent)
        : PedidoProveedor(comp, parent) {
    _depura("PedidoProveedorView::PedidoProveedorView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    try {
        setupUi(this);
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        mui_lineasDetalle->setEmpresaBase(comp);
        mui_proveedor->setEmpresaBase(comp);
        mui_formaPago->setEmpresaBase(comp);
        mui_descuentos->setEmpresaBase(comp);
        mui_almacenDestino->setEmpresaBase(comp);
        mui_trabajador->setEmpresaBase(comp);
        mui_referenciaPedido->setEmpresaBase(comp);

        setListaLineas(mui_lineasDetalle);
        setListaDescuentos(mui_descuentos);

        inicialize();
        dialogChanges_cargaInicial();
        comp->meteWindow(windowTitle(), this, FALSE);
    } catch (...) {
        mensajeInfo(tr("Error al crear el pedido a proveedor"));
    } // end try
    _depura("END PedidoProveedorView::PedidoProveedorView", 0);
}


PedidoProveedorView::~PedidoProveedorView() {
    _depura("PedidoProveedorView::~PedidoProveedorView", 0);
    empresaBase()->refreshPedidosProveedor();
    _depura("END PedidoProveedorView::~PedidoProveedorView", 0);
}


void PedidoProveedorView::inicialize() {
    _depura("PedidoProveedorView::inicialize", 0);
    mui_totalBaseImponible->setReadOnly(TRUE);
    mui_totalBaseImponible->setAlignment(Qt::AlignRight);
    QPalette p1 = mui_totalBaseImponible->palette();
    p1.setBrush(QPalette::Base, this->palette().color(QPalette::Window));
    mui_totalBaseImponible->setPalette(p1);

    mui_totalImpuestos->setReadOnly(TRUE);
    mui_totalImpuestos->setAlignment(Qt::AlignRight);
    QPalette p2 = mui_totalImpuestos->palette();
    p2.setBrush(QPalette::Base, this->palette().color(QPalette::Window));
    mui_totalImpuestos->setPalette(p2);

    mui_totalDescuentos->setReadOnly(TRUE);
    mui_totalDescuentos->setAlignment(Qt::AlignRight);
    QPalette p3 = mui_totalDescuentos->palette();
    p3.setBrush(QPalette::Base, this->palette().color(QPalette::Window));
    mui_totalDescuentos->setPalette(p3);

    mui_totalPedido->setReadOnly(TRUE);
    mui_totalPedido->setAlignment(Qt::AlignRight);
    QPalette p4 = mui_totalPedido->palette();
    p4.setBrush(QPalette::Base, this->palette().color(QPalette::Window));
    mui_totalPedido->setPalette(p4);

    /// Inicializamos los desplegables.
    pintaidforma_pago("0");
    pintaidalmacen("0");
    pintaidtrabajador("0");
    mui_lineasDetalle->pintar();
    mui_descuentos->pintar();
    _depura("END PedidoProveedorView::inicialize", 0);
}


int PedidoProveedorView::cargar(QString id) {
    _depura("PedidoProveedorView::cargar", 0);
    try {
        if (PedidoProveedor::cargar(id))
            throw -1;
        setWindowTitle(tr("Pedido a proveedor") + " " + DBvalue("refpedidoproveedor") +" "+ DBvalue("idpedidoproveedor"));
        empresaBase()->meteWindow(windowTitle(), this);
        dialogChanges_cargaInicial();
        _depura("END PedidoProveedorView::cargar", 0);
    } catch (...) {
        return -1;
    } // end try
    return 0;
}


int PedidoProveedorView::guardar() {
    _depura("PedidoProveedorView::guardar", 0);
    try {
        setcomentpedidoproveedor(mui_comentarios->toPlainText());
        setnumpedidoproveedor(mui_numeroPedido->text());
        setidproveedor(mui_proveedor->idproveedor());
        setfechapedidoproveedor(mui_fechaPedido->text());
        setidalmacen(mui_almacenDestino->idalmacen());
        setidtrabajador(mui_trabajador->idtrabajador());
        setidforma_pago(mui_formaPago->idforma_pago());
        setrefpedidoproveedor(mui_referenciaPedido->text());
        setdescpedidoproveedor(mui_descripcionPedido->text());
        setcontactpedidoproveedor(mui_personaContacto->text());
        settelpedidoproveedor(mui_telefonoContacto->text());
        setprocesadopedidoproveedor(mui_tramitadoPedido->isChecked() ? "TRUE" : "FALSE");
        PedidoProveedor::guardar();
        dialogChanges_cargaInicial();
    } catch (...) {
        _depura("PedidoProveedorView::guardar Error al guardar el Pedido Proveedor", 0);
        throw -1;
    } // end try
    _depura("END PedidoProveedorView::guardar", 0);
    return 0;
}


void PedidoProveedorView::pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc, Fixed irpf, Fixed reqeq) {
    _depura("PedidoProveedorView::pintatotales", 0);
    mui_totalBaseImponible->setText(base.toQString());
    mui_totalImpuestos->setText(iva.toQString());
    mui_totalPedido->setText(total.toQString());
    mui_totalDescuentos->setText(desc.toQString());
    mui_totalIRPF->setText(QString(irpf.toQString()));
    mui_totalRecargo->setText(QString(reqeq.toQString()));
    _depura("END PedidoProveedorView::pintatotales", 0);
}


void PedidoProveedorView::on_mui_pagar_clicked() {
    _depura("PedidoProveedorView::on_mui_pagar_clicked", 0);
    PagoView *bud = empresaBase()->newPagoView();
    empresaBase()->m_pWorkspace->addWindow(bud);
    bud->setidproveedor(DBvalue("idproveedor"));
    bud->setcantpago(mui_totalPedido->text());
    bud->setrefpago(DBvalue("refpedidoproveedor"));
    bud->setcomentpago(DBvalue("descpedidoproveedor"));
    bud->pintar();
    bud->show();
    _depura("END PedidoProveedorView::on_mui_pagar_clicked", 0);
}


/// Se encarga de generar un albaran a partir del pedido.
void PedidoProveedorView::generarAlbaran() {
    _depura("PedidoProveedorView::generarAlbaran", 0);
    /// Comprobamos que existe el elemento, y en caso afirmativo lo mostramos y salimos de la funcion.
    QString SQLQuery = "SELECT * FROM albaranp WHERE refalbaranp = '" + DBvalue("refpedidoproveedor") + "'";
    cursor2 *cur = empresaBase()->cargacursor(SQLQuery);
    if (!cur->eof()) {
        AlbaranProveedorView *bud = new AlbaranProveedorView(empresaBase(), NULL);
        empresaBase()->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idalbaranp"));
        bud->show();
        return;
    } // end if
    delete cur;

    /// Informamos de que no existe el pedido y a ver si lo queremos realizar.
    /// Si no salimos de la funcion.
    if (QMessageBox::question(this,
                              tr("El albaran de proveedor no existe"),
                              tr("No existe un albaran asociado a este pedido.\nDesea crearlo?"),
                              tr("&Si"), tr("&No"),
                              QString::null, 0, 1))
        return;

    /// Creamos el pedido.
    AlbaranProveedorView *bud = empresaBase()->newAlbaranProveedorView();
    empresaBase()->m_pWorkspace->addWindow(bud);
    bud->inicializar();

    bud->setcomentalbaranp(DBvalue("comentpedidoproveedor"));
    bud->setdescalbaranp(DBvalue("descpedidoproveedor"));
    bud->setidforma_pago(DBvalue("idforma_pago"));
    bud->setrefalbaranp(DBvalue("refpedidoproveedor"));
    bud->setidproveedor(DBvalue("idproveedor"));
    bud->setidalmacen(DBvalue("idalmacen"));

    /// Traspasamos las lineas del presupuesto a lineas del pedido.
    SDBRecord *linea;
    SDBRecord *linea2;
    for (int i = 0; i < m_listalineas->rowCount(); i++) {
        linea = m_listalineas->lineaat(i);
        if (linea->DBvalue("idarticulo") != "") {
            linea2 = bud->getlistalineas()->lineaat(bud->getlistalineas()->rowCount() - 1);
            bud->getlistalineas()->nuevoRegistro();
            linea2->setDBvalue("desclalbaranp", linea->DBvalue("desclpedidoproveedor"));
            linea2->setDBvalue("cantlalbaranp", linea->DBvalue("cantlpedidoproveedor"));
            linea2->setDBvalue("pvplalbaranp", linea->DBvalue("pvplpedidoproveedor"));
            linea2->setDBvalue("descuentolalbaranp", linea->DBvalue("descuentolpedidoproveedor"));
            linea2->setDBvalue("idarticulo", linea->DBvalue("idarticulo"));
            linea2->setDBvalue("codigocompletoarticulo", linea->DBvalue("codigocompletoarticulo"));
            linea2->setDBvalue("nomarticulo", linea->DBvalue("nomarticulo"));
            linea2->setDBvalue("ivalalbaranp", linea->DBvalue("ivalpedidoproveedor"));
        } // end if
    } // end for

    /// Traspasamos los descuentos del pedido a descuentos del albaran.
    SDBRecord *linea1;
    SDBRecord *linea3;
    for (int i = 0; i < m_listadescuentos->rowCount(); i++) {
        linea1 = m_listadescuentos->lineaat(i);
        if (linea1->DBvalue("proporciondpedidoproveedor") != "") {
            linea3 = bud->getlistadescuentos()->lineaat(bud->getlistadescuentos()->rowCount() - 1);
            linea3->setDBvalue("conceptdalbaranp", linea1->DBvalue("conceptdpedidoproveedor"));
            linea3->setDBvalue("proporciondalbaranp", linea1->DBvalue("proporciondpedidoproveedor"));
            bud->getlistadescuentos()->nuevoRegistro();
        } // end if
    } // end for

    bud->pintar();
    bud->show();
    _depura("END PedidoProveedorView::generarAlbaran", 0);
}

void PedidoProveedorView::on_mui_proveedor_valueChanged(QString id) {
    _depura("PedidoProveedorView::on_m_proveedor_valueChanged", 0);
    mui_lineasDetalle->setIdProveedor(id);
    mui_formaPago->setIdProveedor(id);
    _depura("END PedidoProveedorView::on_m_proveedor_valueChanged", 0);
}

