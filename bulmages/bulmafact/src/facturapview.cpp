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

#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QLayout>
#include <fstream>
#include <QCloseEvent>

using namespace std;

#include "facturapview.h"
#include "company.h"
#include "listlinfacturapview.h"
#include "facturap.h"
#include "providerslist.h"
#include "pagoview.h"
#include "funcaux.h"

FacturaProveedorView::FacturaProveedorView(company *comp, QWidget *parent)
        : QWidget(parent, Qt::WDestructiveClose), FacturaProveedor(comp), dialogChanges(this) {
    _depura("FacturaProveedorView::FacturaProveedorView", 0);
    try {
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        setupUi(this);
        subform2->setcompany(comp);
        m_forma_pago->setcompany(comp);
        m_proveedor->setcompany(comp);
        m_descuentos->setcompany(comp);
        m_reffacturap->setcompany(comp);
        setListLinFacturaProveedor(subform2);
        setListDescuentoFacturaProv(m_descuentos);
        m_totalBases->setReadOnly(TRUE);
        m_totalBases->setAlignment(Qt::AlignRight);
        m_totalTaxes->setReadOnly(TRUE);
        m_totalTaxes->setAlignment(Qt::AlignRight);
        m_totalDiscounts->setReadOnly(TRUE);
        m_totalDiscounts->setAlignment(Qt::AlignRight);
        m_totalfacturap->setReadOnly(TRUE);
        m_totalfacturap->setAlignment(Qt::AlignRight);
        comp->meteWindow(windowTitle(), this, FALSE);
    } catch (...) {
        mensajeInfo(tr("Error al crear la factura proveedor"));
    } // end try
    _depura("Fin de la inicializacion de FacturaProveedor\n");
}


FacturaProveedorView::~FacturaProveedorView() {
    _depura("FacturaProveedorView::~FacturaProveedorView", 0);
    companyact->refreshFacturasProveedor();
    companyact->sacaWindow(this);
    _depura("END FacturaProveedorView::~FacturaProveedorView", 0);

}


/// inicializar debe ser invocado cuando se crea una nueva ficha sin cargar ningun date de la base de datos (por ejemplo una nueva ficha).
/// Sirve para inicializar los componenetes sin necesidad de query alguno
void FacturaProveedorView::inicializar() {
    _depura("FacturaProveedorView::inicializar", 0);
    subform2->inicializar();
    m_descuentos->inicializar();
    _depura("END FacturaProveedorView::inicializar", 0);
}


void FacturaProveedorView::pintatotales(Fixed base, Fixed iva) {
    m_totalBases->setText(base.toQString());
    m_totalTaxes->setText(iva.toQString());
    m_totalfacturap->setText((iva + base).toQString());
}


void FacturaProveedorView::s_nuevoPago() {
    _depura("FacturaProveedorView::s_nuevoPago", 0);
    PagoView *bud = companyact->newPagoView();
    companyact->m_pWorkspace->addWindow(bud);
    bud->setidproveedor(DBvalue("idproveedor"));
    bud->setcantpago(m_totalfacturap->text());
    bud->setrefpago(DBvalue("reffacturap"));
    bud->setcomentpago(DBvalue("descfacturap"));
    bud->pintar();
    bud->show();
    _depura("END FacturaProveedorView::s_nuevoPago", 0);

}


void FacturaProveedorView::closeEvent(QCloseEvent *e) {
    _depura("closeEvent", 0);
    if (dialogChanges_hayCambios())  {
        int val = QMessageBox::warning(this,
                                       tr("Guardar factura de proveedor."),
                                       tr("Desea guardar los cambios?"),
                                       tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
        if (val == 0)
            guardar();
        if (val == 2)
            e->ignore();
    } // end if
}


int FacturaProveedorView::cargar(QString id) {
    _depura("FacturaProveedorView::cargar", 0);
    try {
        FacturaProveedor::cargar(id);
        setWindowTitle(tr("Factura de proveedor") + " " + DBvalue("reffacturap") + " - " + DBvalue("numfacturap"));
        companyact->meteWindow(windowTitle(), this);
        dialogChanges_cargaInicial();
    } catch (...) {
        return -1;
    } // end try
    _depura("END FacturaProveedorView::cargar");
    return 0;
}


int FacturaProveedorView::guardar() {
    _depura("FacturaProveedorView::guardar", 0);
    try {
        setidproveedor(m_proveedor->idproveedor());
        setnumfacturap(m_numfacturap->text());
        setfechafacturap(m_fechafacturap->text());
        setdescfacturap(m_descfacturap->text());
        setcomentfacturap(m_comentfacturap->text());
        setreffacturap(m_reffacturap->text());
        setidforma_pago(m_forma_pago->idforma_pago());
        setprocesadafacturap(m_procesadafacturap->isChecked() ? "TRUE" : "FALSE");
        FacturaProveedor::guardar();
        dialogChanges_cargaInicial();
    } catch (...) {
        mensajeInfo(tr("Error al guardar la factura proveedor"));
        return -1;
    } // end try
    _depura("END FacturaProveedorView::guardar", 0);
    return 0;
}


void FacturaProveedorView::on_mui_borrar_clicked() {
    int val = QMessageBox::warning(this,
                                   tr("Borrar factura de proveedor."),
                                   tr("Desea eliminar la factura?"),
                                   tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
    if (val == 0) {
        if (!borrar()) {
            dialogChanges_cargaInicial();
            _depura("Factura de Proveedor borrado satisfactoriamente.", 2);
            close();
        } // end if
    } // end if
}

