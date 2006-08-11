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

#include <QCloseEvent>
#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QLayout>
#include <fstream>

using namespace std;

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


PedidoProveedorView::PedidoProveedorView(company *comp, QWidget *parent, const char *name)
        : QWidget(parent, name, Qt::WDestructiveClose), PedidoProveedor (comp), dialogChanges(this) {
    _depura("PedidoProveedorView::PedidoProveedorView", 0);
    try {
        setupUi(this);
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        subform3->setcompany(comp);
        m_proveedor->setcompany(comp);
        m_forma_pago->setcompany(comp);
        m_descuentos->setcompany(comp);
        m_almacen->setcompany(comp);
        m_trabajador->setcompany(comp);
        m_refpedidoproveedor->setcompany(comp);
        setListLinPedidoProveedor(subform3);
        setListDescuentoPedidoProveedor(m_descuentos);
        inicialize();
        dialogChanges_cargaInicial();
        comp->meteWindow(caption(), this, FALSE);
    } catch(...) {
        mensajeInfo(tr("Error al crear el pedido proveedor"));
    } // end try
    _depura("END PedidoProveedorView::PedidoProveedorView", 0);
}


PedidoProveedorView::~PedidoProveedorView() {
    _depura("PedidoProveedorView::~PedidoProveedorView", 0);
    companyact->refreshPedidosProveedor();
    companyact->sacaWindow(this);
    _depura("END PedidoProveedorView::~PedidoProveedorView", 0);
}


void PedidoProveedorView::inicialize() {
    m_totalBases->setReadOnly(TRUE);
    m_totalBases->setAlignment(Qt::AlignRight);
    m_totalTaxes->setReadOnly(TRUE);
    m_totalTaxes->setAlignment(Qt::AlignRight);
    m_totalDiscounts->setReadOnly(TRUE);
    m_totalDiscounts->setAlignment(Qt::AlignRight);
    m_totalpedidoproveedor->setReadOnly(TRUE);
    m_totalpedidoproveedor->setAlignment(Qt::AlignRight);
    /// Inicializamos la forma de pago para que no se quede sin ser pintada.
    pintaidforma_pago("0");
    pintaidalmacen("0");
    pintaidtrabajador("0");
    subform3->pintar();
    m_descuentos->pintar();
}


int PedidoProveedorView::cargar(QString id) {
    _depura("PedidoProveedorView::cargar", 0);
    try {
        if (PedidoProveedor::cargar(id))
            throw -1;
        setWindowTitle(tr("Pedido a proveedor") + " " + DBvalue("refpedidoproveedor"));
        companyact->meteWindow(caption(), this);
        dialogChanges_cargaInicial();
        _depura("END PedidoProveedorView::cargar", 0);
    } catch (...) {
        return -1;
    } // end try
    return 0;
}


int PedidoProveedorView::guardar() {
    setcomentpedidoproveedor(m_comentpedidoproveedor->text());
    setnumpedidoproveedor(m_numpedidoproveedor->text());
    setidproveedor(m_proveedor->idproveedor());
    setfechapedidoproveedor(m_fechapedidoproveedor->text());
    setidalmacen(m_almacen->idalmacen());
    setidtrabajador(m_trabajador->idtrabajador());
    setidforma_pago(m_forma_pago->idforma_pago());
    setrefpedidoproveedor(m_refpedidoproveedor->text());
    setdescpedidoproveedor(m_descpedidoproveedor->text());
    setcontactpedidoproveedor(m_contactpedidoproveedor->text());
    settelpedidoproveedor(m_telpedidoproveedor->text());
    setprocesadopedidoproveedor(m_procesadopedidoproveedor->isChecked() ? "TRUE" : "FALSE");
    int error = PedidoProveedor::guardar();
    if (error == 0) {
        dialogChanges_cargaInicial();
    } //endif
    return error;
}


void PedidoProveedorView::pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc) {
    m_totalBases->setText(base.toQString());
    m_totalTaxes->setText(iva.toQString());
    m_totalpedidoproveedor->setText(total.toQString());
    m_totalDiscounts->setText(desc.toQString());
}


void PedidoProveedorView::on_mui_pagar_clicked() {
    PagoView *bud = new PagoView(companyact, NULL, tr("Edicion de pagos"));
    bud->setidproveedor(DBvalue("idproveedor"));
    bud->setcantpago(m_totalpedidoproveedor->text());
    bud->setrefpago(DBvalue("refpedidoproveedor"));
    bud->setcomentpago(DBvalue("descpedidoproveedor"));
    bud->pintar();
    bud->show();
}


/// Se encarga de generar un albaran a partir del pedido.
void PedidoProveedorView::generarAlbaran() {
    /// Comprobamos que existe el elemento, y en caso afirmativo lo mostramos y salimos de la funcion.
    QString SQLQuery = "SELECT * FROM albaranp WHERE refalbaranp = '" + DBvalue("refpedidoproveedor") + "'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        AlbaranProveedorView *bud = new AlbaranProveedorView(companyact, NULL, tr("Edicion de albaranes de proveedores"));
        companyact->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idalbaranp"));
        bud->show();
        return;
    } // end if
    delete cur;

    /// Informamos de que no existe el pedido y a ver si lo queremos realizar.
    /// Si no salimos de la funcion.
    if (QMessageBox::question(this,
                              tr("Albaran de proveedor inexistente"),
                              tr("No existe un albaran asociado a este pedido.\n Desea crearlo?"),
                              tr("&Si"), tr("&No"),
                              QString::null, 0, 1))
        return;

    /// Creamos el pedido.
    AlbaranProveedorView *bud = new AlbaranProveedorView(companyact, NULL, QApplication::translate("PedidoProveedorView", "Edicion de albaranes de proveedores"));
    companyact->m_pWorkspace->addWindow(bud);
    bud->vaciaAlbaranProveedor();

    bud->setcomentalbaranp(DBvalue("comentpedidoproveedor"));
    bud->setdescalbaranp(DBvalue("descpedidoproveedor"));
    bud->setidforma_pago(DBvalue("idforma_pago"));
    bud->setrefalbaranp(DBvalue("refpedidoproveedor"));
    bud->setidproveedor(DBvalue("idproveedor"));
    bud->setidalmacen(DBvalue("idalmacen"));

    QString l;
    SDBRecord *linea, *linea1;
    for (int i = 0; i < listalineas->rowCount(); ++i) {
        linea = listalineas->lineaat(i);
        linea1 = bud->getlistalineas()->newSDBRecord();
        linea1->setDBvalue("desclalbaranp", linea->DBvalue(tr("Descripcion del albaran")));
        linea1->setDBvalue("cantlalbaranp", linea->DBvalue(tr("Cantidad del pedido")));
        linea1->setDBvalue("pvplalbaranp", linea->DBvalue(tr("P.V.P.")));
        linea1->setDBvalue("descontlalbaranp", linea->DBvalue(tr("Descuento")));
        linea1->setDBvalue("idarticulo", linea->DBvalue(tr("ID articulo")));
        linea1->setDBvalue("codigocompletoarticulo", linea->DBvalue(tr("Codigo completo articulo")));
        linea1->setDBvalue("nomarticulo", linea->DBvalue(tr("Nombre del articulo")));
        linea1->setDBvalue("ivalalbaranp", linea->DBvalue(tr("I.V.A.")));
        i++;
    } // end for

    for (int i = 0; i < listadescuentos->rowCount() - 1; i++) {
        linea1 = bud->getlistadescuentos()->newSDBRecord();
        linea1->setDBvalue("conceptdalbaranp", listadescuentos->DBvalue("conceptdpedidoproveedor", i));
        linea1->setDBvalue("proporciondalbaranp", listadescuentos->DBvalue("proporciondpedidoproveedor", i));
    } // end for
    bud->show();
}


void PedidoProveedorView::closeEvent(QCloseEvent *e) {
    _depura("closeEvent", 0);
    if (dialogChanges_hayCambios()) {
        int val = QMessageBox::warning(this,
                                       tr("Guardar pedido a proveedor"),
                                       tr("Desea guardar los cambios?"),
                                       tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
        if (val == 0)
            guardar();
        if (val == 2)
            e->ignore();
    } // end if
}

