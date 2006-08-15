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

#include <QCloseEvent>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QToolButton>
#include <QWidget>

#include "albaranclienteview.h"
#include "articulolist.h"
#include "clientslist.h"
#include "cobroview.h"
#include "company.h"
#include "configuracion.h"
#include "facturaslist.h"
#include "facturaview.h"
#include "funcaux.h"
#include "informereferencia.h"
#include "listdescalbaranclienteview.h"
#include "listlinalbaranclienteview.h"
#include "pedidoclienteview.h"
#include "pedidosclientelist.h"
#include "postgresiface2.h"
#include "presupuestolist.h"
#include "presupuestoview.h"


AlbaranClienteView::AlbaranClienteView(company *comp, QWidget *parent)
        : QWidget(parent, Qt::WDestructiveClose), AlbaranCliente(comp), dialogChanges(this) {
    _depura("AlbaranClienteView::AlbaranClienteView", 0);
    try {
        setupUi(this);
        subform2->setcompany(comp);
        m_descuentos->setcompany(comp);
        m_almacen->setcompany(comp);
        m_forma_pago->setcompany(comp);
        m_cliente->setcompany(comp);
        m_trabajador->setcompany(comp);
        m_refalbaran->setcompany(comp);

        setListLinAlbaranCliente(subform2);
        setListDescuentoAlbaranCliente(m_descuentos);

        comp->meteWindow(windowTitle(), this, FALSE);
        dialogChanges_cargaInicial();
    } catch(...) {
        mensajeInfo(tr("Error al crear el albaran cliente"));
    } // end try
    _depura("END AlbaranClienteView::AlbaranClienteView\n", 0);
}


AlbaranClienteView::~AlbaranClienteView() {
    companyact->refreshAlbaranesCliente();
    companyact->sacaWindow(this);
}

void AlbaranClienteView::inicializar() {
    _depura("AlbaranClienteView::inicializar", 0);
    subform2->inicializar();
    m_descuentos->inicializar();
    _depura("END AlbaranClienteView::inicializar", 0);
}

void AlbaranClienteView::pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc) {
    m_totalBases->setText(base.toQString());
    m_totalTaxes->setText(iva.toQString());
    m_totalalbaran->setText(total.toQString());
    m_totalDiscounts->setText(desc.toQString());
}


void AlbaranClienteView::s_verpresupuesto() {
    QString SQLQuery = "SELECT * FROM presupuesto WHERE refpresupuesto = '" +
                       DBvalue("refalbaran") + "'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);

    if (cur->numregistros() > 1) {
        PresupuestoList *list = new PresupuestoList(companyact, NULL);
        list->modoseleccion();
        list->show();

        while (!list->isHidden())  {
            theApp->processEvents();
        } // end while

        this->setEnabled(true);

        if (list->idpresupuesto() != QString("")) {
            PresupuestoView *bud = companyact->newBudget();
            bud->cargar(list->idpresupuesto());
            bud->show();
        } // end if
    } else if (!cur->eof()) {
        PresupuestoView *bud = companyact->newBudget();
        bud->cargar(cur->valor("idpresupuesto"));
        bud->show();
    } // end if
    delete cur;
}


void AlbaranClienteView::on_mui_verpedidocliente_clicked() {
    _depura("on_mui_verpedidocliente_clicked", 0);
    QString SQLQuery = "SELECT * FROM pedidocliente WHERE refpedidocliente = '" + DBvalue("refalbaran") + "'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        while (!cur->eof()) {
            PedidoClienteView *bud = companyact->newPedidoClienteView();
            companyact->m_pWorkspace->addWindow(bud);
            bud->cargar(cur->valor("idpedidocliente"));
            bud->show();
            cur->siguienteregistro();
        } // end while
    } else {
        _depura("no hay pedidos con esta referencia", 2);
    } // end if
    delete cur;
}


/// Se encarga de generar una factura a partir de un albaran.
void AlbaranClienteView::generarFactura() {
    /// Comprobamos que existe el elemento, y en caso afirmativo lo mostramos
    /// y salimos de la funcion.
    QString SQLQuery = "SELECT * FROM factura WHERE reffactura = '" + DBvalue("refalbaran") + "'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);

    if (!cur->eof()) {
        FacturaView *bud = companyact->newFacturaView();
        companyact->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idfactura"));
        bud->show();
        return;
    } // end if
    delete cur;

    /// Informamos de que no existe el pedido y a ver si lo queremos realizar.
    /// Si no salimos de la funcion.
    if (QMessageBox::question(this,
                              tr("Factura inexistente"),
                              tr("No existe una factura asociada a este albaran. Desea crearla?"),
                              tr("&Si"), tr("&No"), QString::null, 0, 1)) {
        return;
    }

    /// Creamos la factura.
    FacturaView *bud = companyact->newFacturaView();
    companyact->m_pWorkspace->addWindow(bud);

    /// Cargamos un elemento que no existe para inicializar bien la clase.
    bud->cargar("0");
    bud->setcomentfactura(DBvalue("comentalbaran"));
    bud->setidforma_pago(DBvalue("idforma_pago"));
    bud->setreffactura(DBvalue("refalbaran"));
    bud->setidcliente(DBvalue("idcliente"));
    bud->setidalmacen(DBvalue("idalmacen"));
    bud->pintaFactura();
    bud->show();

    QString l;
    SDBRecord *linea, *linea1;
    for (int i = 0; i < listalineas->rowCount(); ++i) {
        linea = listalineas->lineaat(i);
        if (linea->DBvalue( "idarticulo") != "") {
            linea1 = bud->getlistalineas()->lineaat(bud->getlistalineas()->rowCount() - 1);
            linea1->setDBvalue("desclfactura", linea->DBvalue("desclalbaran"));
            linea1->setDBvalue("cantlfactura", linea->DBvalue("cantlalbaran"));
            linea1->setDBvalue("pvplfactura", linea->DBvalue("pvplalbaran"));
            linea1->setDBvalue("descuentolfactura", linea->DBvalue("descontlalbaran"));
            linea1->setDBvalue("idarticulo", linea->DBvalue("idarticulo"));
            linea1->setDBvalue("codigocompletoarticulo", linea->DBvalue("codigocompletoarticulo"));
            linea1->setDBvalue("nomarticulo", linea->DBvalue("nomarticulo"));
            linea1->setDBvalue("ivalfactura", linea->DBvalue("ivalalbaran"));
            bud->getlistalineas()->nuevoRegistro();
        } // end if
    } // end for
    bud->calculaypintatotales();
    m_procesadoalbaran->setChecked(TRUE);
}


/// Se encarga de agregar un albaran a una factura.
void AlbaranClienteView::agregarFactura() {
    /// Pedimos la factura a la que agregar.
    _depura("AlbaranClienteView::agregarFactura", 0);
    QDialog *diag = new QDialog(0);
    diag->setModal(true);
    FacturasList *fac = new FacturasList(companyact, diag, 0, FacturasList::SelectMode);
    connect(fac, SIGNAL(selected(QString)), diag, SLOT(accept()));

    /// Hacemos que las opciones de filtrado del listado ya esten bien.
    fac->m_cliente->setidcliente(DBvalue("idcliente"));
    fac->on_mui_actualizar_clicked();

    /// Lanzamos el dialogo.
    diag->exec();
    QString idfactura = fac->idfactura();
    delete diag;

    /// Si no hay idfactura es que hemos abortado y por tanto cancelamos la operacion.
    if (idfactura == "")
        return;

    /// Creamos la factura.
    FacturaView *bud = companyact->newFacturaView();
    bud->cargar(idfactura);

    /// Agregamos en los comentarios que se ha añadido este albaran.
    bud->pintaComentFactura(bud->DBvalue("comentfactura") + tr("Num. albaran") + DBvalue("numalbaran") + "\n" );

    companyact->m_pWorkspace->addWindow(bud);
    /// EN TEORIA SE DEBARIA COMPROBAR QUE LA FACTURA ES DEL MISMO CLIENTE,
    /// pero por ahora pasamos de hacerlo.
    QString l;
    SDBRecord *linea, *linea1;
    for (int i = 0; i < listalineas->rowCount(); ++i) {
        linea = listalineas->lineaat(i);
        if (linea->DBvalue("idarticulo") != "") {
            linea1 = bud->getlistalineas()->lineaat(bud->getlistalineas()->rowCount() - 1);
            linea1->setDBvalue("desclfactura", linea->DBvalue("desclalbaran"));
            linea1->setDBvalue("cantlfactura", linea->DBvalue("cantlalbaran"));
            linea1->setDBvalue("pvplfactura", linea->DBvalue("pvplalbaran"));
            linea1->setDBvalue("descuentolfactura", linea->DBvalue("descontlalbaran"));
            linea1->setDBvalue("idarticulo", linea->DBvalue("idarticulo"));
            linea1->setDBvalue("codigocompletoarticulo", linea->DBvalue("codigocompletoarticulo"));
            linea1->setDBvalue("nomarticulo", linea->DBvalue("nomarticulo"));
            linea1->setDBvalue("ivalfactura", linea->DBvalue("ivalalbaran"));
            bud->getlistalineas()->nuevoRegistro();
        } // end if
    } // end for
    bud->calculaypintatotales();
    bud->show();
    m_procesadoalbaran->setChecked(TRUE);
    _depura("END AlbaranClienteView::agregarFactura", 0);
}


int AlbaranClienteView::cargar(QString id) {
    _depura("AlbaranClienteView::cargar", 0);
    try {
        if (AlbaranCliente::cargar(id))
            throw -1;
        setWindowTitle(tr("Albaran a cliente") + " " + DBvalue("refalbaran") + " " + DBvalue("idalbaran"));
        companyact->meteWindow(windowTitle(), this);
        dialogChanges_cargaInicial();
    } catch(...) {
        return -1;
    } // end try
    _depura("AlbaranClienteView::cargar", 0);
    return 0;
}


void AlbaranClienteView::closeEvent(QCloseEvent *e) {
    _depura("closeEvent", 0);
    if (dialogChanges_hayCambios())	{
        int val = QMessageBox::warning(this,
                                       tr("Guardar albaran"),
                                       tr("Desea guardar los cambios?"),
                                       tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
        if (val == 0)
            guardar();
        if (val == 2)
            e->ignore();
    } // end if
}


int AlbaranClienteView::guardar() {
    _depura("AlbaranClienteView::guardar", 0);
    /// Cogemos todos los valores del formulario y actualizamos la clase.
    setcomentalbaran(m_comentalbaran->text());
    setcomentprivalbaran(m_comentprivalbaran->text());
    setidalmacen(m_almacen->idalmacen());
    setNumAlbaran(m_numalbaran->text());
    setidcliente(m_cliente->idcliente());
    setprocesadoalbaran(m_procesadoalbaran->isChecked() ? "TRUE" : "FALSE");
    setcontactalbaran(m_contactalbaran->text());
    settelalbaran(m_telalbaran->text());
    setfechaalbaran(m_fechaalbaran->text());
    setidforma_pago(m_forma_pago->idforma_pago());
    setidtrabajador(m_trabajador->idtrabajador());
    setrefalbaran(m_refalbaran->text());
    setdescalbaran(m_descalbaran->text());

    /// Hacemos el guardado.
    int err = AlbaranCliente::guardar();
    dialogChanges_cargaInicial();
    _depura("END AlbaranClienteView::guardar", 0);
    return err;
}


/// Crea un nuevo cobro para el albaran seleccionado.
void AlbaranClienteView::on_mui_cobrar_clicked() {
    CobroView *bud = companyact->newCobroView();
    bud->setidcliente(DBvalue("idcliente"));
    bud->setcantcobro(m_totalalbaran->text());
    bud->setrefcobro(DBvalue("refalbaran"));
    bud->setcomentcobro(DBvalue("descalbaran"));
    bud->pintar();
    bud->show();
}

