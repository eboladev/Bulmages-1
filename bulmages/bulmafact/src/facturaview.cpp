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
#include <QComboBox>
#include <QLayout>
#include <QMessageBox>
#include <QObject>
#include <QToolButton>
#include <QWidget>

#include <fstream>

using namespace std;

#include "albaranclientelist.h"
#include "albaranclienteview.h"
#include "clientslist.h"
#include "cobroview.h"
#include "company.h"
#include "factura.h"
#include "facturaview.h"
#include "funcaux.h"
#include "informereferencia.h"
#include "listdescfacturaview.h"
#include "listlinfacturaview.h"


FacturaView::FacturaView(company *comp, QWidget *parent)
        : Ficha(parent), Factura(comp) {
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    _depura("FacturaView::FacturaView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    try {
        setupUi(this);
        subform2->setcompany(comp);
        m_almacen->setcompany(comp);
        m_forma_pago->setcompany(comp);
        m_cliente->setcompany(comp);
        m_descuentos->setcompany(comp);
        m_codigoserie_factura->setcompany(comp);
        m_reffactura->setcompany(comp);
        setListLinFactura(subform2);
        setListDescuentoFactura(m_descuentos);
        m_totalBases->setReadOnly(TRUE);
        m_totalBases->setAlignment(Qt::AlignRight);
        m_totalTaxes->setReadOnly(TRUE);
        m_totalTaxes->setAlignment(Qt::AlignRight);
        m_totalDiscounts->setReadOnly(TRUE);
        m_totalDiscounts->setAlignment(Qt::AlignRight);
        m_totalfactura->setReadOnly(TRUE);
        m_totalfactura->setAlignment(Qt::AlignRight);
        comp->meteWindow(windowTitle(), this, FALSE);
    } catch (...) {
        mensajeInfo(tr("Error al crear la factura"));
    }
    _depura("END FacturaView::FacturaView");
}


FacturaView::~FacturaView() {
    _depura("FacturaView::~FacturaView", 0);
    companyact->refreshFacturas();
    companyact->sacaWindow(this);
    _depura("END FacturaView::~FacturaView", 0);
}


void FacturaView::inicializar() {
    _depura("FacturaView::inicializar", 0);
    subform2->inicializar();
    m_descuentos->inicializar();
    dialogChanges_cargaInicial();
    _depura("END FacturaView::inicializar", 0);
}


void   FacturaView::pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc) {
    m_totalBases->setText(base.toQString());
    m_totalTaxes->setText(iva.toQString());
    m_totalfactura->setText(total.toQString());
    m_totalDiscounts->setText(desc.toQString());
}


/// Crea un nuevo cobro para la factura seleccionada.
void FacturaView::on_mui_cobrar_clicked() {
    _depura("FacturaView::on_mui_cobrar_clicked", 0);
    CobroView *bud = companyact->newCobroView();
    companyact->m_pWorkspace->addWindow(bud);
    bud->setidcliente(DBvalue("idcliente"));
    bud->setcantcobro(m_totalfactura->text());
    bud->setrefcobro(DBvalue("reffactura"));
    bud->setcomentcobro(DBvalue("descfactura"));
    bud->pintar();
    bud->show();
    _depura("END FacturaView::on_mui_cobrar_clicked", 0);
}


int FacturaView::cargar(QString id) {
    _depura("FacturaView::cargar", 0);
    try {
        Factura::cargar(id);
        if (DBvalue("idfactura") != "") {
            setWindowTitle(tr("Factura") + " " + DBvalue("reffactura") + " " + DBvalue("idfactura"));
            companyact->meteWindow(windowTitle(), this);
        } // end if
        dialogChanges_cargaInicial();
    } catch (...) {
        return -1;
    } // end try
    _depura("END FacturaView::cargar", 0);
    return 0;
}


void FacturaView::on_mui_agregaralbaran_clicked() {
    _depura("FacturaView::on_mui_agregaralbaran_clicked\n", 0);
    QDialog *diag = new QDialog(0);
    diag->setModal(true);
    AlbaranClienteList *fac = new AlbaranClienteList(companyact, diag, 0, AlbaranClienteList::SelectMode);
    connect(fac, SIGNAL(selected(QString)), diag, SLOT(accept()));

    /// Hacemos que las opciones de filtrado del listado ya esten bien.
    fac->m_cliente->setidcliente(DBvalue("idcliente"));
    fac->on_mui_actualizar_clicked();

    /// Lanzamos el dialogo.
    diag->exec();
    QString idalbaran = fac->idCliDelivNote();
    delete diag;

    /// Si no hay idfactura es que hemos abortado y por tanto cancelamos la operacion.
    if (idalbaran == "")
        return;

    /// Creamos la factura.
    AlbaranClienteView *bud = new AlbaranClienteView(companyact, NULL);
    bud->cargar(idalbaran);

    /// Agregamos a comentarios que albaran se corresponde.
    QString comm = DBvalue("comentfactura") + "(" + tr("ALBARAN: Num ") + bud->numalbaran() + tr("Ref:") + " " + bud->refalbaran() + tr("Fecha:") + " " + bud->fechaalbaran() + ")\n";

    setDBvalue("comentfactura", comm);
    pintaComentFactura(comm);

    /// EN TEORIA SE DEBERIA COMPROBAR QUE LA FACTURA Y EL ALBARAN SON DEL MISMO CLIENTE, pero por ahora no lo hacemos.
    SDBRecord *linea, *linea1;
    for (int i = 0; i < bud->getlistalineas()->rowCount(); ++i) {
        linea = bud->getlistalineas()->lineaat(i);
        /// Los registros vacios no se tienen en cuenta.
        if (linea->DBvalue("idarticulo") != "") {
            linea1 = getlistalineas()->lineaat(getlistalineas()->rowCount() - 1);
            linea1->setDBvalue("desclfactura", linea->DBvalue("desclalbaran"));
            linea1->setDBvalue("cantlfactura", linea->DBvalue("cantlalbaran"));
            linea1->setDBvalue("pvplfactura", linea->DBvalue("pvplalbaran"));
            linea1->setDBvalue("descuentolfactura", linea->DBvalue("descontlalbaran"));
            linea1->setDBvalue("idarticulo", linea->DBvalue("idarticulo"));
            linea1->setDBvalue("codigocompletoarticulo", linea->DBvalue("codigocompletoarticulo"));
            linea1->setDBvalue("nomarticulo", linea->DBvalue("nomarticulo"));
            linea1->setDBvalue("ivalfactura", linea->DBvalue("ivalalbaran"));
            getlistalineas()->nuevoRegistro();
        } // end if
    } // end for

    /// Procesamos el albaran.
    bud->m_procesadoalbaran->setChecked(TRUE);
    bud->guardar();
    delete bud;

    /// Pintamos los totales.
    calculaypintatotales();
    _depura("END FacturaView::on_mui_agregaralbaran_clicked\n", 0);
}



int FacturaView::guardar() {
    _depura("FacturaView::guardar", 0);
    try {
        setcomentfactura(m_comentfactura->toPlainText());
        setidalmacen(m_almacen->idalmacen());
        setNumFactura(m_numfactura->text());
        setreffactura(m_reffactura->text());
        setidcliente(m_cliente->idcliente());
        setfechafactura(m_fechafactura->text());
        setdescfactura(m_descfactura->text());
        setidforma_pago(m_forma_pago->idforma_pago());
        setcodigoserie_factura(m_codigoserie_factura->codigoserie_factura());
        setprocesadafactura( m_procesadafactura->isChecked() ? "TRUE" : "FALSE");
        Factura::guardar();
        dialogChanges_cargaInicial();
    } catch(...) {
        _depura("FacturaView::guardar error al guardar", 0);
        throw -1;
    } // end try
    _depura("END FacturaView::guardar", 0);
    return 0;
}


void FacturaView::on_mui_veralbaranes_clicked() {
    _depura("FacturaView::on_mui_veralbaranes_clicked", 0);
    QString SQLQuery = "SELECT * FROM albaran WHERE refalbaran = '" + DBvalue("reffactura") + "'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        while (!cur->eof()) {
            AlbaranClienteView *bud = new AlbaranClienteView(companyact, NULL);
            companyact->m_pWorkspace->addWindow(bud);
            bud->cargar(cur->valor("idalbaran"));
            bud->show();
            cur->siguienteregistro();
        } // end while
    } else {
        _depura("no hay albaranes con esta referencia", 2);
    } // end if
    delete cur;
}
