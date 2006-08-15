/***************************************************************************
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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

#include <QLineEdit>
#include <QMessageBox>
#include <QLabel>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QCloseEvent>

#include "albaranproveedorview.h"
#include "company.h"
#include "articulolist.h"
#include "configuracion.h"
#include "presupuestolist.h"
#include "presupuestoview.h"
#include "funcaux.h"
#include "postgresiface2.h"
#include "listlinalbaranproveedorview.h"
#include "facturapview.h"
#include "facturaview.h"
#include "pagoview.h"
#include "pedidoproveedorview.h"


AlbaranProveedorView::AlbaranProveedorView(company *comp, QWidget *parent)
        : QWidget(parent, Qt::WDestructiveClose), AlbaranProveedor(comp), dialogChanges(this) {
    _depura("AlbaranProveedorView::AlbaranProveedorView", 0);
    try {
        setupUi(this);
        subform2->setcompany(comp);
        m_almacen->setcompany(comp);
        m_forma_pago->setcompany(comp);
        m_proveedor->setcompany(comp);
        m_descuentos->setcompany(comp);
        m_refalbaranp->setcompany(comp);
        setListLinAlbaranProveedor(subform2);
        setListDescuentoAlbaranProveedor(m_descuentos);
        m_totalBases->setReadOnly(TRUE);
        m_totalBases->setAlignment(Qt::AlignRight);
        m_totalTaxes->setReadOnly(TRUE);
        m_totalTaxes->setAlignment(Qt::AlignRight);
        m_totalDiscounts->setReadOnly(TRUE);
        m_totalDiscounts->setAlignment(Qt::AlignRight);
        m_totalalbaranp->setReadOnly(TRUE);
        m_totalalbaranp->setAlignment(Qt::AlignRight);
        pintaidforma_pago("0");
        pintaidalmacen("0");
        dialogChanges_cargaInicial();
        if (companyact != NULL)
            companyact->meteWindow(windowTitle(), this, FALSE);
    } catch (...) {
        mensajeInfo(tr("Error al crear el albaran proveedor"));
    } // end try
    _depura("END AlbaranProveedorView::AlbaranProveedorView", 0);
}


AlbaranProveedorView::~AlbaranProveedorView() {
    companyact->refreshAlbaranesProveedor();
    companyact->sacaWindow(this);
}


void AlbaranProveedorView::inicializar() {
    _depura("AlbaranProveedorView::inicializar", 0);
    subform2->inicializar();
    m_descuentos->inicializar();
    _depura("END AlbaranProveedorView::inicializar", 0);
}


void AlbaranProveedorView::pintatotales(Fixed base, Fixed iva) {
    _depura("AlbaranProveedorView::pintatotales", 0);
    m_totalBases->setText(base.toQString());
    m_totalTaxes->setText(iva.toQString());
    m_totalalbaranp->setText((iva + base).toQString());
    _depura("END AlbaranProveedorView::pintatotales", 0);
}


void AlbaranProveedorView::on_mui_borrar_clicked() {
    _depura("AlbaranProveedorView::on_mui_borrar_clicked", 0);
    int val = QMessageBox::warning(this,
                                   tr("Borrar albaran del proveedor."),
                                   tr("Desea eliminar el albaran del proveedor?"),
                                   tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
    if (val == 0) {
        if (!borrar()) {
            dialogChanges_cargaInicial();
            _depura("Albaran del proveedor borrado satisfactoriamente", 2);
            close();
        } // end if
    } // end if
    _depura("END AlbaranProveedorView::on_mui_borrar_clicked", 0);
}


void AlbaranProveedorView::s_verpedidoproveedor()  {
    _depura("Funcion aun no implementada", 2);
}


/// Se encarga de generar una facturap a partir del albaranp.
void AlbaranProveedorView::generarFactura()  {
    _depura("AlbaranProveedorView::generarFactura", 0);
    /// Comprobamos que existe el elemento, y en caso afirmativo lo mostramos
    /// y salimos de la funcion.
    QString SQLQuery = "SELECT * FROM facturap WHERE reffacturap = '" + DBvalue("refalbaranp") + "'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);

    if (!cur->eof()) {
        FacturaProveedorView *bud = companyact->newFacturaProveedorView();
        companyact->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idfacturap"));
        bud->show();
        return;
    } // end if
    delete cur;

    /// Informamos de que no existe el pedido y a ver si lo queremos realizar.
    /// Si no salimos de la funcion.
    if (QMessageBox::question(this,
                              tr("Factura inexistente"),
                              tr("No existe una factura asociada a este albaran."
                                 "Desea crearla?"),
                              tr("&Si"), tr("&No"), QString::null, 0, 1)) {
        return;
    } // end if

    /// Creamos la factura.
    FacturaProveedorView *bud = companyact->newFacturaProveedorView();
    companyact->m_pWorkspace->addWindow(bud);

    bud->setcomentfacturap(DBvalue("comentalbaranp"));
    bud->setidforma_pago(DBvalue("idforma_pago"));
    bud->setreffacturap(DBvalue("refalbaranp"));
    bud->setidproveedor(DBvalue("idproveedor"));
    SDBRecord *linea, *linea1;

    for (int i  = 0; i < listalineas->rowCount(); ++i) {
        linea = getlistalineas()->lineaat(i);
        linea1 = bud->getlistalineas()->newSDBRecord();
        linea1->setDBvalue("desclfacturap", linea->DBvalue("desclalbaranp"));
        linea1->setDBvalue("cantlfacturap", linea->DBvalue("cantlalbaranp"));
        linea1->setDBvalue("pvplfacturap", linea->DBvalue("pvplalbaranp"));
        linea1->setDBvalue("descuentolfacturap", linea->DBvalue("descontlalbaranp"));
        linea1->setDBvalue("idarticulo", linea->DBvalue("idarticulo"));
        linea1->setDBvalue("codigocompletoarticulo", linea->DBvalue("codigocompletoarticulo"));
        linea1->setDBvalue("nomarticulo", linea->DBvalue("nomarticulo"));
        linea1->setDBvalue("ivalfacturap", linea->DBvalue("ivalalbaranp"));
    } // end for

    bud->pintar();
    bud->show();
}


void AlbaranProveedorView::closeEvent(QCloseEvent *e) {
    _depura("AlbaranProveedorView::closeEvent", 0);
    if (dialogChanges_hayCambios()) {
        int val = QMessageBox::warning(this,
                                       tr("Guardar albaran"),
                                       tr("Desea guardar los cambios?"),
                                       tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
        if (val == 0)
            guardar();
        if (val == 2)
            e->ignore();
    } // end if
    _depura("END AlbaranProveedorView::closeEvent", 0);

}


int AlbaranProveedorView::cargar(QString id) {
    _depura("AlbaranProveedorView::cargar", 0);
    try {
        AlbaranProveedor::cargar(id);
        if(DBvalue("idalbaranp") != "") {
            setWindowTitle(tr("Albaran de proveedor") + " " + DBvalue("refalbaranp") +" " +DBvalue("idalbaranp"));
            companyact->meteWindow(windowTitle(), this);
            dialogChanges_cargaInicial();
        } // end if
    } catch(...) {
        return -1;
    } // end catch
    _depura("END AlbaranProveedorView::cargar", 0);
    return 0;
}


int AlbaranProveedorView::guardar() {
    _depura("AlbaranProveedorView::guardar", 0);
    try {
        setcomentalbaranp(m_comentalbaranp->text());
        setnumalbaranp(m_numalbaranp->text());
        setidproveedor(m_proveedor->idproveedor());
        setfechaalbaranp(m_fechaalbaranp->text());
        setidalmacen(m_almacen->idalmacen());
        setidforma_pago(m_forma_pago->idforma_pago());
        setrefalbaranp(m_refalbaranp->text());
        setdescalbaranp(m_descalbaranp->text());
        int err = AlbaranProveedor::guardar();
        dialogChanges_cargaInicial();
        _depura("END AlbaranProveedorView::guardar", 0);
        return err;
    } catch (...) {
        mensajeInfo( "Error inesperado al guardar");
        return -1;
    } // end catch
}


void AlbaranProveedorView::on_mui_guardar_clicked() {
    guardar();
    cargar(DBvalue("idalbaranp"));
}


void AlbaranProveedorView::on_mui_pagar_clicked() {
    _depura("AlbaranProveedorView::on_mui_pagar_clicked", 0);
    PagoView *bud = companyact->newPagoView();
    companyact->m_pWorkspace->addWindow(bud);
    bud->setidproveedor(DBvalue("idproveedor"));
    bud->setcantpago(m_totalalbaranp->text());
    bud->setrefpago(DBvalue("refalbaranp"));
    bud->setcomentpago(DBvalue("descalbaranp"));
    bud->pintar();
    bud->show();
    _depura("END AlbaranProveedorView::on_mui_pagar_clicked", 0);
}


void AlbaranProveedorView::on_mui_verpedidosproveedor_clicked() {
	_depura("AlbaranProveedorView::on_mui_verpedidos_clicked", 0);
	QString query = "SELECT * FROM pedidoproveedor WHERE refpedidoproveedor='"+DBvalue("refalbaranp")+"'";
	cursor2 *cur = companyact->cargacursor(query);
	while (!cur->eof()) {
		PedidoProveedorView *pedpro = companyact->newPedidoProveedorView();
		pedpro->cargar(cur->valor("idpedidoproveedor"));
		companyact->m_pWorkspace->addWindow( pedpro);
		pedpro->show();
		cur->siguienteregistro();
	} // end while
	delete cur;	
	_depura("END AlbaranProveedorView::on_mui_verpedidos_clicked", 0);
}