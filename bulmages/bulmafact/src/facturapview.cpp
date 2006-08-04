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

FacturaProveedorView::FacturaProveedorView(company *comp, QWidget *parent, const char *name)
        : QWidget(parent, name, Qt::WDestructiveClose), FacturaProveedor (comp), dialogChanges(this) {
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    setupUi(this);
    subform2->setcompany(comp);
    m_forma_pago->setcompany(comp);
    m_proveedor->setcompany(comp);
    m_descuentos->setcompany(comp);
    m_reffacturap->setcompany(comp);
    setListLinFacturaProveedor(subform2);
    setListDescuentoFacturaProv(m_descuentos);
    inicialize();
    comp->meteWindow(caption(), this);
    /// Hacemos una cargaa inicial falsa para que se inicializa bien la clase (una chapucilla).
    cargar("0");
    _depura("Fin de la inicializacion de FacturaProveedor\n");
}


FacturaProveedorView::~FacturaProveedorView() {
    companyact->sacaWindow(this);
}


void FacturaProveedorView::inicialize() {
    m_totalBases->setReadOnly(TRUE);
    m_totalBases->setAlignment(Qt::AlignRight);
    m_totalTaxes->setReadOnly(TRUE);
    m_totalTaxes->setAlignment(Qt::AlignRight);
    m_totalDiscounts->setReadOnly(TRUE);
    m_totalDiscounts->setAlignment(Qt::AlignRight);
    m_totalfacturap->setReadOnly(TRUE);
    m_totalfacturap->setAlignment(Qt::AlignRight);
}


void   FacturaProveedorView::pintatotales(Fixed base, Fixed iva) {
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
    FacturaProveedor::cargar(id);
    setCaption(tr("Factura de proveedor") + " - " + DBvalue("reffacturap"));
    if (companyact->meteWindow(caption(), this))
        return -1;
    dialogChanges_cargaInicial();
    return 0;
}


int FacturaProveedorView::guardar() {
    setidproveedor(m_proveedor->idproveedor());
    setnumfacturap(m_numfacturap->text());
    setfechafacturap(m_fechafacturap->text());
    setdescfacturap(m_descfacturap->text());
    setcomentfacturap(m_comentfacturap->text());
    setreffacturap(m_reffacturap->text());
    setidforma_pago(m_forma_pago->idforma_pago());
    setprocesadafacturap(m_procesadafacturap->isChecked() ? "TRUE" : "FALSE");
    int err = FacturaProveedor::guardar();
    dialogChanges_cargaInicial();
    return err;
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

