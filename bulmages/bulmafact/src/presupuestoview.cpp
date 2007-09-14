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


PresupuestoView::PresupuestoView(Company *comp, QWidget *parent)
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
        subform2->setEmpresaBase(comp);
        m_descuentos->setEmpresaBase(comp);
        mui_idcliente->setEmpresaBase(comp);
        mui_idforma_pago->setEmpresaBase(comp);
        mui_idalmacen->setEmpresaBase(comp);
        mui_idtrabajador->setEmpresaBase(comp);
        mui_refpresupuesto->setEmpresaBase(comp);

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
	
	mui_idforma_pago->setValorCampo("0");
	mui_idalmacen->setValorCampo("0");
	mui_idtrabajador->setValorCampo("0");

        meteWindow(windowTitle(), this, FALSE);
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
    ((Company *)empresaBase())->refreshPresupuestos();

    /// Disparamos los plugins.
    g_plugins->lanza("PresupuestoView_DesPresupuestoView", this);
    _depura("END PresupuestoView::~PresupuestoView", 0);
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
    cursor2 *cur = empresaBase()->cargacursor(SQLQuery);
    if(!cur->eof()) {


        /// Informamos que ya hay una factura y que la abriremos.
        /// Si no salimos de la funci&oacute;n.
        if (QMessageBox::question(this,
                                  tr("Pedido existente"),
                                  tr("Existe un pedido a este cliente con la misma referencia que este presupuesto. Desea abrirla para verificar?"),
                                  tr("&Si"), tr("&No"), QString::null, 0, 1)) {
            return;
        }
        PedidoClienteView *bud = empresaBase()->newPedidoClienteView();
        empresaBase()->m_pWorkspace->addWindow(bud);
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
    PedidoClienteView *bud = empresaBase()->newPedidoClienteView();
    bud->cargar("0");
    empresaBase()->m_pWorkspace->addWindow(bud);

    /// Traspasamos toda la informacion del presupuesto al pedido.
    recogeValores();
    bud->setDBvalue("idcliente", DBvalue("idcliente"));
    bud->setDBvalue("comentpedidocliente", DBvalue("comentpresupuesto"));
    bud->setDBvalue("descpedidocliente", DBvalue("descpresupuesto"));
    bud->setDBvalue("fechapedidocliente", DBvalue("fpresupuesto"));
    bud->setDBvalue("idforma_pago", DBvalue("idforma_pago"));
    bud->setDBvalue("refpedidocliente", DBvalue("refpresupuesto"));
    bud->setDBvalue("procesadopedidocliente", DBvalue("procesadopresupuesto"));
    bud->setDBvalue("idalmacen", DBvalue("idalmacen"));
    bud->setDBvalue("contactpedidocliente", DBvalue("contactpresupuesto"));
    bud->setDBvalue("telpedidocliente", DBvalue("telpresupuesto"));

    /// Traspasamos las lineas del presupuesto a lineas del pedido.
    SDBRecord *linea;
    SDBRecord *linea2;


    for (int i = 0; i < m_listalineas->rowCount(); i++) {
        linea = m_listalineas->lineaat(i);
        if (linea->DBvalue("idarticulo") != "") {
            linea2 = bud->getlistalineas()->lineaat(bud->getlistalineas()->rowCount() - 1);
            bud->getlistalineas()->nuevoRegistro();
            bud->getlistalineas()->setProcesarCambios(FALSE);
            linea2->setDBvalue("desclpedidocliente", linea->DBvalue("desclpresupuesto"));
            linea2->setDBvalue("cantlpedidocliente", linea->DBvalue("cantlpresupuesto"));
            linea2->setDBvalue("pvplpedidocliente", linea->DBvalue("pvplpresupuesto"));
            linea2->setDBvalue("ivalpedidocliente", linea->DBvalue("ivalpresupuesto"));
            linea2->setDBvalue("descuentolpedidocliente", linea->DBvalue("descuentolpresupuesto"));
            linea2->setDBvalue("idarticulo", linea->DBvalue("idarticulo"));
            linea2->setDBvalue("codigocompletoarticulo", linea->DBvalue("codigocompletoarticulo"));
            linea2->setDBvalue("nomarticulo", linea->DBvalue("nomarticulo"));
	    bud->getlistalineas()->setProcesarCambios(TRUE);
        } // end if
    } // end for



    /// Traspasamos los descuentos del presupuesto a descuentos del pedido.
    SDBRecord *linea1;
    SDBRecord *linea3;
    for (int i = 0; i < m_listadescuentos->rowCount(); i++) {
        linea1 = m_listadescuentos->lineaat(i);
        if (linea1->DBvalue("proporciondpresupuesto") != "") {
            linea3 = bud->getlistadescuentos()->lineaat(bud->getlistadescuentos()->rowCount() - 1);
	    bud->getlistadescuentos()->setProcesarCambios(FALSE);
            linea3->setDBvalue("conceptdpedidocliente", linea1->DBvalue("conceptdpresupuesto"));
            linea3->setDBvalue("proporciondpedidocliente", linea1->DBvalue("proporciondpresupuesto"));
	    bud->getlistadescuentos()->setProcesarCambios(TRUE);
            bud->getlistadescuentos()->nuevoRegistro();
        } // end if
    } // end for


    /// Pintamos el pedido y lo presentamos.
    bud->pintar();
    bud->show();
    _depura("END PresupuestoView::generarPedidoCliente", 0);
}



int PresupuestoView::guardar() {
    _depura("PresupuestoView::guardar", 0);
    try {
        /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
        int res = g_plugins->lanza("PresupuestoView_guardar", this);
        if (res != 0)
            return 0;
        recogeValores();
        Presupuesto::guardar();
        /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
        g_plugins->lanza("PresupuestoView_guardar_Post", this);
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
    mui_idforma_pago->setIdCliente(id);
    _depura("END PresupuestoView::on_m_cliente_valueChanged", 0);
}

