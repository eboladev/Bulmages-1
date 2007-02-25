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
#include <QCloseEvent>

#include "pedidoclienteview.h"
#include "company.h"
#include "listlinpedidoclienteview.h"
#include "pedidocliente.h"
#include "clientslist.h"
#include "presupuestoview.h"
#include "presupuestolist.h"
#include "cobroview.h"
#include "albaranclienteview.h"
#include "informereferencia.h"
#include "funcaux.h"
#include "plugins.h"


PedidoClienteView::PedidoClienteView(company *comp, QWidget *parent)
        : PedidoCliente(comp, parent) {
    _depura("PedidoClienteView::PedidoClienteView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    try {
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        setupUi(this);
        /// Disparamos los plugins.
        int res = g_plugins->lanza("PedidoClienteView_PedidoClienteView", this);
        if (res != 0)
            return;
        subform3->setcompany(comp);
        m_cliente->setcompany(comp);
        m_forma_pago->setcompany(comp);
        m_descuentos->setcompany(comp);
        m_almacen->setcompany(comp);
        m_trabajador->setcompany(comp);
        m_refpedidocliente->setcompany(comp);
        setListaLineas(subform3);
        setListaDescuentos(m_descuentos);
        comp->meteWindow(windowTitle(), this, FALSE);
    } catch (...) {
        mensajeInfo(tr("Error al crear el pedido cliente"));
    } // end try
    _depura("END PedidoClienteView::PedidoClienteView", 0);
}


PedidoClienteView::~PedidoClienteView() {
    _depura("PedidoClienteView::~PedidoClienteView", 0);
    m_companyact->refreshPedidosCliente();
    _depura("END PedidoClienteView::~PedidoClienteView", 0);
}


void PedidoClienteView::inicializar() {
    _depura("PedidoClienteView::inicializar", 0);
    subform3->inicializar();
    m_descuentos->inicializar();
    dialogChanges_cargaInicial();
    _depura("END PedidoClienteView::inicializar", 0);
}

void PedidoClienteView::pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc, Fixed irpf, Fixed reqeq) {
    _depura("PedidoClienteView::pintatotales", 0);
    m_totalBases->setText(QString(base.toQString()));
    m_totalTaxes->setText(QString(iva.toQString()));
    m_totalDiscounts->setText(QString(desc.toQString()));
    m_totalIRPF->setText(QString(irpf.toQString()));
    m_totalReqEq->setText(QString(reqeq.toQString()));
    m_totalpedidocliente->setText(total.toQString());
    _depura("END PedidoClienteView::pintatotales", 0);
}


void PedidoClienteView::on_mui_verpresupuesto_clicked() {
    _depura("PedidoClienteView::on_mui_verpresupuesto_clicked", 0);
    QString SQLQuery = "SELECT * FROM presupuesto WHERE refpresupuesto = '" + DBvalue("refpedidocliente") + "' AND idcliente = " + DBvalue("idcliente");
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        while (!cur->eof()) {
            PresupuestoView *bud = m_companyact->nuevoPresupuestoView();
            m_companyact->m_pWorkspace->addWindow(bud);
            if (bud->cargar(cur->valor("idpresupuesto"))) {
                delete bud;
                return;
            } // end if
            bud->show();
            cur->siguienteregistro();
        } // end while
    } else {
        _depura("No hay presupuestos con la misma referencia.", 2);
    } // end if
    delete cur;
    _depura("END PedidoClienteView::on_mui_verpresupuesto_clicked", 0);
}


/** Genera un Albaran a Cliente a partir de los datos
    que tiene el pedido.
*/
void PedidoClienteView::generarAlbaran() {
    _depura("PedidoClienteView::generarAlbaran", 0);
    /// Comprobamos que existe el elemento, y en caso afirmativo lo mostramos y salimos de la funcion.
    QString SQLQuery = "SELECT * FROM albaran WHERE refalbaran = '" + DBvalue("refpedidocliente") + "' AND idcliente = " + DBvalue("idcliente");
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
	   /// Informamos que ya hay un albaran y que la abriremos.
	   /// Si no salimos de la funci&oacute;n.
	   if (QMessageBox::question(this,
				    tr("Albaran ya existe"),
				    tr("Existe un albaran a este cliente con la misma referencia que este pedido. Desea abrirlo para verificar?"),
				    tr("&Si"), tr("&No"), QString::null, 0, 1)) {
		  return;
	   }
       AlbaranClienteView *bud = new AlbaranClienteView(m_companyact, NULL);
       m_companyact->m_pWorkspace->addWindow(bud);
       bud->cargar(cur->valor("idalbaran"));
       bud->show();
       return;
    } // end if
    delete cur;

    /// Creamos el albaran.
    AlbaranClienteView *bud = m_companyact->newAlbaranClienteView();
    m_companyact->m_pWorkspace->addWindow(bud);
    bud->cargar("0");

    /// Traspasamos los datos al albaran.
    bud->setcomentalbaran(DBvalue("comentpedidocliente"));
    bud->setdescalbaran(DBvalue("descpedidocliente"));
    bud->setidforma_pago(DBvalue("idforma_pago"));
    bud->setrefalbaran(DBvalue("refpedidocliente"));
    bud->setidcliente(DBvalue("idcliente"));
    bud->setidalmacen(DBvalue("idalmacen"));
    bud->setcontactalbaran(DBvalue("contactpedidocliente"));
    bud->settelalbaran(DBvalue("telpedidocliente"));
    bud->setidtrabajador(DBvalue("idtrabajador"));
    bud->pintar();
    bud->show();

    /// Traspasamos las lineas al albaran.
    SDBRecord *linea, *linea1;
    for (int i = 0; i < m_listalineas->rowCount(); ++i) {
        linea = m_listalineas->lineaat(i);
        if (linea->DBvalue("idarticulo") != "") {
            linea1 = bud->getlistalineas()->lineaat(bud->getlistalineas()->rowCount() - 1);
            linea1->setDBvalue("desclalbaran", linea->DBvalue("desclpedidocliente"));
            linea1->setDBvalue("cantlalbaran", linea->DBvalue("cantlpedidocliente"));
            linea1->setDBvalue("pvplalbaran", linea->DBvalue("pvplpedidocliente"));
            linea1->setDBvalue("descuentolalbaran", linea->DBvalue("descuentolpedidocliente"));
            linea1->setDBvalue("idarticulo", linea->DBvalue("idarticulo"));
            linea1->setDBvalue("codigocompletoarticulo", linea->DBvalue("codigocompletoarticulo"));
            linea1->setDBvalue("nomarticulo", linea->DBvalue("nomarticulo"));
            linea1->setDBvalue("ivalalbaran", linea->DBvalue("ivalpedidocliente"));
            bud->getlistalineas()->nuevoRegistro();
        } // end if
    } // end for

    /// Traspasamos los descuentos.
    for (int i = 0; i < m_listadescuentos->rowCount(); ++i) {
        linea1 = m_listadescuentos->lineaat(i);
        if (linea1->DBvalue("proporciondpedidocliente") != "") {
            linea = bud->getlistadescuentos()->lineaat(bud->getlistadescuentos()->rowCount() - 1);
            linea->setDBvalue("conceptdalbaran", linea1->DBvalue("conceptdpedidocliente"));
            linea->setDBvalue("proporciondalbaran", linea1->DBvalue("proporciondpedidocliente"));
            bud->getlistadescuentos()->nuevoRegistro();
        } // end if
    } // end for
    _depura("END PedidoClienteView::generarAlbaran", 0);
}


void PedidoClienteView::on_mui_cobrar_clicked() {
    _depura("PedidoClienteView::on_mui_cobrar_clicked", 0);
    CobroView *bud = m_companyact->newCobroView();
    bud->setidcliente(DBvalue("idcliente"));
    bud->setcantcobro(m_totalpedidocliente->text());
    bud->setrefcobro(DBvalue("refpedidocliente"));
    bud->setcomentcobro(DBvalue("descpedidocliente"));
    bud->pintar();
    bud->show();
    _depura("END PedidoClienteView::on_mui_cobrar_clicked", 0);
}


int PedidoClienteView::cargar(QString id) {
    _depura("PedidoClienteView::cargar", 0);
    try {
        if (PedidoCliente::cargar(id)) {
            throw -1;
        } // end if
        setWindowTitle(tr("Pedido de cliente") + " " + DBvalue("refpedidocliente") + " " + DBvalue("idpedidocliente"));
        m_companyact->meteWindow(windowTitle(), this);
        dialogChanges_cargaInicial();
    } catch (...) {
        return -1;
    } // end try
    _depura("END PedidoClienteView::cargar", 0);
    return 0;
}


int PedidoClienteView::guardar() {
    _depura("PedidoClienteView::guardar", 0);
    try {
        setcomentpedidocliente(m_comentpedidocliente->toPlainText());
        setnumpedidocliente(m_numpedidocliente->text());
        setidcliente(m_cliente->idcliente());
        setfechapedidocliente(m_fechapedidocliente->text());
        setidalmacen(m_almacen->idalmacen());
        setidtrabajador(m_trabajador->idtrabajador());
        setidforma_pago(m_forma_pago->idforma_pago());
        setrefpedidocliente(m_refpedidocliente->text());
        setdescpedidocliente(m_descpedidocliente->text());
        setcontactpedidocliente(m_contactpedidocliente->text());
        settelpedidocliente(m_telpedidocliente->text());
        setprocesadopedidocliente(m_procesadopedidocliente->isChecked() ? "TRUE" : "FALSE");
        PedidoCliente::guardar();
        dialogChanges_cargaInicial();
    } catch (...) {
        _depura("", 0);
        throw -1;
    } // end try
    _depura("END PedidoClienteView::guardar", 0);
    return 0;
}

void PedidoClienteView::on_m_cliente_valueChanged(QString id) {
	_depura("PedidoClienteView::on_m_cliente_valueChanged", 0);
	subform3->setIdCliente(id);
	m_forma_pago->setIdCliente(id);
	_depura("END PedidoClienteView::on_m_cliente_valueChanged", 0);
}

