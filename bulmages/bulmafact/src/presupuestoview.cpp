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
#include <QFile>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>
#include <QCloseEvent>

#include "funcaux.h"
#include "listlinpresupuestoview.h"
#include "presupuestoview.h"
#include "company.h"
#include "clientslist.h"
#include "articulolist.h"
#include "configuracion.h"
#include "pedidoclienteview.h"
#include "albaranclienteview.h"
#include "busquedacliente.h"
#include "busquedaformapago.h"
#include "informereferencia.h"
#include "plugins.h"


PresupuestoView::PresupuestoView(company *comp, QWidget *parent)
        : Presupuesto(comp, parent) {
    _depura("Inicializacion de PresupuestoView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    try {
        setupUi(this);
        /// Disparamos los plugins.
        int res = g_plugins->lanza("PresupuestoView_PresupuestoView", this);
        if (res != 0)
            return;
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        subform2->setcompany(comp);
        m_descuentos->setcompany(comp);
        m_cliente->setcompany(comp);
        m_forma_pago->setcompany(comp);
        m_almacen->setcompany(comp);
        m_trabajador->setcompany(comp);
        m_refpresupuesto->setcompany(comp);

        setListaLineas(subform2);
        setListaDescuentos(m_descuentos);

        m_totalBases->setReadOnly(TRUE);
        m_totalBases->setAlignment(Qt::AlignRight);
        m_totalTaxes->setReadOnly(TRUE);
        m_totalTaxes->setAlignment(Qt::AlignRight);
        m_totalDiscounts->setReadOnly(TRUE);
        m_totalDiscounts->setAlignment(Qt::AlignRight);
        m_totalPresupuestoCliente->setReadOnly(TRUE);
        m_totalPresupuestoCliente->setAlignment(Qt::AlignRight);
        /// Inicializamos la forma de pago para que no se quede sin ser pintada.
        pintaidforma_pago("0");
        pintaidalmacen("0");
        pintaidtrabajador("0");
        comp->meteWindow(windowTitle(), this, FALSE);
        /// Disparamos los plugins por flanco descendente.
        g_plugins->lanza("PresupuestoView_PresupuestoView_Post", this);
    } catch (...) {
        mensajeInfo(tr("Error al crear el presupuesto"));
    } // end try
    _depura("Fin de la inicializacion de PresupuestoView", 0);
}


/// Este metodo es llamado cuando hacemos un nuevo registro, pero no hay carga desde la base de datos.
void PresupuestoView::inicializar() {
    _depura("PresupuestoView::inicializar", 0);
    subform2->inicializar();
    m_descuentos->inicializar();
    dialogChanges_cargaInicial();
    _depura("END PresupuestoView::inicializar", 0);
}


PresupuestoView::~PresupuestoView() {
    _depura("PresupuestoView::~PresupuestoView", 0);
    m_companyact->refreshPresupuestos();
    _depura("END PresupuestoView::~PresupuestoView", 0);
}


void PresupuestoView::pintaNumPresupuesto(QString id) {
    m_numpresupuesto->setText(id);
}


void PresupuestoView::pintaFPresupuesto(QString id) {
    m_fpresupuesto->setText(id);
}


void PresupuestoView::pintaVencPresupuesto(QString id) {
    m_vencpresupuesto->setText(id);
}


void PresupuestoView::pintaContractPresupuesto(QString id) {
    m_contactpresupuesto->setText(id);
}


void PresupuestoView::pintaTelPresupuesto(QString id) {
    m_telpresupuesto->setText(id);
}


void PresupuestoView::pintaComentPresupuesto(QString id) {
    m_comentpresupuesto->setPlainText(id);
}


void PresupuestoView::pintaidcliente(QString id) {
    m_cliente->setidcliente(id);
}


void PresupuestoView::pintarefPresupuesto(QString id) {
    m_refpresupuesto->setText(id);
}


void PresupuestoView::pintaidforma_pago(QString id) {
    m_forma_pago->setidforma_pago(id);
}


void PresupuestoView::pintaidalmacen(QString id) {
    m_almacen->setidalmacen(id);
}


void PresupuestoView::pintaidtrabajador(QString id) {
    m_trabajador->setidtrabajador(id);
}


void PresupuestoView::pintaprocesadoPresupuesto(QString id) {
    if (id == "t" || id == "TRUE") {
        m_procesadopresupuesto->setChecked(TRUE);
    } else {
        m_procesadopresupuesto->setChecked(FALSE);
    } // end if
}


void PresupuestoView::on_mui_guardar_clicked() {
    guardar();
}


void PresupuestoView::s_pintaTotales() {
    calculaypintatotales();
}


void PresupuestoView::on_mui_pasarapedido_clicked() {
    generarPedidoCliente();
}


void PresupuestoView::on_m_descuentos_editFinish(int, int) {
    calculaypintatotales();
}


void PresupuestoView::on_subform2_editFinish(int, int) {
    calculaypintatotales();
}


void PresupuestoView::pintadescPresupuesto(QString id) {
    m_descpresupuesto->setText(id);
}


void PresupuestoView::on_mui_imprimir_clicked() {
    _depura("PresupuestoView::on_mui_imprimir_clicked", 0);
    imprimir();
    _depura("END PresupuestoView::on_mui_imprimir_clicked", 0);
}


void PresupuestoView::pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc, Fixed irpf, Fixed reqeq) {
    _depura("PresupuestoView::pintatotales", 0);
    m_totalBases->setText(QString(base.toQString()));
    m_totalTaxes->setText(QString(iva.toQString()));
    m_totalPresupuestoCliente->setText(QString(total.toQString()));
    m_totalDiscounts->setText(QString(desc.toQString()));
    m_totalIRPF->setText(QString(irpf.toQString()));
    m_totalReqEq->setText(QString(reqeq.toQString()));
    _depura("END PresupuestoView::pintatotales", 0);
}


/// Se encarga de generar un pedido a partir del presupuesto.
void PresupuestoView::generarPedidoCliente() {
    _depura("PresupuestoView::generarPedidoCliente", 0);
    /// Comprobamos que existe el elemento, y en caso afirmativo lo mostramos y salimos.
    QString SQLQuery = "SELECT * FROM pedidocliente WHERE refpedidocliente = '" + DBvalue("refpresupuesto") + "' AND idcliente = "+DBvalue("idcliente");
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {


        /// Informamos que ya hay una factura y que la abriremos.
        /// Si no salimos de la funci&oacute;n.
        if (QMessageBox::question(this,
                                  tr("Pedido existente"),
                                  tr("Existe un pedido a este cliente con la misma referencia que este presupuesto. Desea abrirla para verificar?"),
                                  tr("&Si"), tr("&No"), QString::null, 0, 1)) {
            return;
        }
        PedidoClienteView *bud = m_companyact->newPedidoClienteView();
        m_companyact->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idpedidocliente"));
        bud->show();
        delete cur;
        return;
    } // end if
    delete cur;

    /// Informamos de que no existe el pedido y a ver si lo queremos realizar.
    /// Sino salimos de la funcion.
    //    if (QMessageBox::question(this,
    //                              tr("Pedido de cliente inexistente"),
    //                              tr("No existe un pedido asociado a este presupuesto. Desea crearlo?"),
    //                              tr("&Si"), tr("&No"),
    //                              QString::null, 0, 1))
    //        return;

    /// Creamos el pedido.
    PedidoClienteView *bud = m_companyact->newPedidoClienteView();
    bud->cargar("0");
    m_companyact->m_pWorkspace->addWindow(bud);

    /// Traspasamos toda la informacion del presupuesto al pedido.
    bud->setidcliente(DBvalue("idcliente"));
    bud->setcomentpedidocliente(DBvalue("comentpresupuesto"));
    bud->setdescpedidocliente(DBvalue("descpresupuesto"));
    bud->setfechapedidocliente(DBvalue("fpresupuesto"));
    bud->setidforma_pago(DBvalue("idforma_pago"));
    bud->setrefpedidocliente(DBvalue("refpresupuesto"));
    bud->setprocesadopedidocliente(DBvalue("procesadopresupuesto"));
    bud->setidalmacen(DBvalue("idalmacen"));
    bud->setcontactpedidocliente(DBvalue("contactpresupuesto"));
    bud->settelpedidocliente(DBvalue("telpresupuesto"));

    /// Traspasamos las lineas del presupuesto a lineas del pedido.
    SDBRecord *linea;
    SDBRecord *linea2;
    for (int i = 0; i < m_listalineas->rowCount(); i++) {
        linea = m_listalineas->lineaat(i);
        if (linea->DBvalue("idarticulo") != "") {
            linea2 = bud->getlistalineas()->lineaat(bud->getlistalineas()->rowCount() - 1);
            linea2->setDBvalue("desclpedidocliente", linea->DBvalue("desclpresupuesto"));
            linea2->setDBvalue("cantlpedidocliente", linea->DBvalue("cantlpresupuesto"));
            linea2->setDBvalue("pvplpedidocliente", linea->DBvalue("pvplpresupuesto"));
            linea2->setDBvalue("ivalpedidocliente", linea->DBvalue("ivalpresupuesto"));
            linea2->setDBvalue("descuentolpedidocliente", linea->DBvalue("descuentolpresupuesto"));
            linea2->setDBvalue("idarticulo", linea->DBvalue("idarticulo"));
            linea2->setDBvalue("codigocompletoarticulo", linea->DBvalue("codigocompletoarticulo"));
            linea2->setDBvalue("nomarticulo", linea->DBvalue("nomarticulo"));
            bud->getlistalineas()->nuevoRegistro();
        } // end if
    } // end for

    /// Traspasamos los descuentos del presupuesto a descuentos del pedido.
    SDBRecord *linea1;
    SDBRecord *linea3;
    for (int i = 0; i < m_listadescuentos->rowCount(); i++) {
        linea1 = m_listadescuentos->lineaat(i);
        if (linea1->DBvalue("proporciondpresupuesto") != "") {
            linea3 = bud->getlistadescuentos()->lineaat(bud->getlistadescuentos()->rowCount() - 1);
            linea3->setDBvalue("conceptdpedidocliente", linea1->DBvalue("conceptdpresupuesto"));
            linea3->setDBvalue("proporciondpedidocliente", linea1->DBvalue("proporciondpresupuesto"));
            bud->getlistadescuentos()->nuevoRegistro();
        } // end if
    } // end for

    /// Pintamos el pedido y lo presentamos.
    bud->pintar();
    bud->show();
    _depura("END PresupuestoView::generarPedidoCliente", 0);
}


int PresupuestoView::cargar(QString id) {
    _depura("PresupuestoView::cargar", 0);
    try {
        if (Presupuesto::cargar(id))
            throw -1;
        setWindowTitle(tr("Presupuesto") + " " + DBvalue("refpresupuesto") + " " + DBvalue("idpresupuesto"));
        m_companyact->meteWindow(windowTitle(), this);
        dialogChanges_cargaInicial();
        _depura("END PresupuestoView::cargar", 0);
    } catch(...) {
        return -1;
    } // end try
    return 0;
}


int PresupuestoView::guardar() {
    _depura("PresupuestoView::guardar", 0);
    try {
        /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
        int res = g_plugins->lanza("PresupuestoView_guardar", this);
        if (res != 0)
            return 0;
        setcomentPresupuesto(m_comentpresupuesto->toPlainText());
        setnumPresupuesto(m_numpresupuesto->text());
        setidcliente(m_cliente->idcliente());
        setfPresupuesto(m_fpresupuesto->text());
        setvencPresupuesto(m_vencpresupuesto->text());
        setidalmacen(m_almacen->idalmacen());
        setidtrabajador(m_trabajador->idtrabajador());
        setidforma_pago(m_forma_pago->idforma_pago());
        setrefPresupuesto(m_refpresupuesto->text());
        setdescPresupuesto(m_descpresupuesto->text());
        setcontactPresupuesto(m_contactpresupuesto->text());
        settelPresupuesto(m_telpresupuesto->text());
        setprocesadoPresupuesto(m_procesadopresupuesto->isChecked() ? "TRUE" : "FALSE");
        Presupuesto::guardar();
        dialogChanges_cargaInicial();
    } catch (...) {
        _depura("PresupuestoView::guardar error al guardar el presupuesto", 0);
        throw -1;
    } // end try
    _depura("END PresupuestoView::guardar", 0);
    return 0;
}


void PresupuestoView::on_m_cliente_valueChanged(QString id) {
    _depura("PresupuestoView::on_m_cliente_valueChanged", 0);
    subform2->setIdCliente(id);
    m_forma_pago->setIdCliente(id);
    _depura("END PresupuestoView::on_m_cliente_valueChanged", 0);
}

