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
#include <Q3Table>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <Q3PopupMenu>
#include <QToolButton>
#include <QLayout>
#include <fstream>
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

PedidoClienteView::PedidoClienteView(company *comp, QWidget *parent, const char *name) : QWidget(parent, name, Qt::WDestructiveClose) , PedidoCliente (comp),dialogChanges(this) {
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    setupUi(this);
    subform3->setcompany(comp);
    m_cliente->setcompany(comp);
    m_forma_pago->setcompany(comp);
    m_descuentos->setcompany(comp);
    m_almacen->setcompany(comp);
    m_trabajador->setcompany(comp);
    setListLinPedidoCliente(subform3);
    setListDescuentoPedidoCliente(m_descuentos);
    comp->meteWindow(caption(),this);
}


PedidoClienteView::~PedidoClienteView() {
    companyact->refreshPedidosCliente();
    companyact->sacaWindow(this);
}


void   PedidoClienteView::pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc) {
    m_totalBases->setText(base.toQString());
    m_totalTaxes->setText(iva.toQString());
    m_totalpedidocliente->setText(total.toQString());
    m_totalDiscounts->setText(desc.toQString());
}


void PedidoClienteView::on_mui_verpresupuesto_clicked() {
    QString SQLQuery= "SELECT * FROM presupuesto WHERE refpresupuesto='"+DBvalue("refpedidocliente")+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (!cur->eof()) {
        while (!cur->eof()) {
            PresupuestoView *bud = companyact->newBudget();
            companyact->m_pWorkspace->addWindow(bud);
            bud->cargar(cur->valor("idpresupuesto"));
            bud->show();
            cur->siguienteregistro();
        }// end while
    } else {
        _depura("No hay presupuestos con la misma referencia.",2);
    }// end if
    delete cur;
}


/// Se encarga de generar un albaran a partir del pedido.
void PedidoClienteView::generarAlbaran() {
    _depura("PedidoClienteView::generarAlbaran",0);
    /// Comprobamos que existe el elemento, y en caso afirmativo lo mostramos y salimos de la funci�.
    QString SQLQuery = "SELECT * FROM albaran WHERE refalbaran='"+DBvalue("refpedidocliente")+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        AlbaranClienteView *bud = new AlbaranClienteView(companyact,NULL,theApp->translate("Edicion de Albaranes de Clientes", "company"));
        companyact->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idalbaran"));
        bud->show();
        return;
    }// end if
    delete cur;


    /// Informamos de que no existe el pedido y a ver si lo queremos realizar. Si no salimos de la funci�.
    if (QMessageBox::question(
                this,
                tr("No existe albaran de cliente."),
                tr("No existe un albaran asociado a este pedido."
                   "Desea crearlo?"),
                tr("&Si"), tr("&No"),
                QString::null, 0, 1 ) )
        return;


    /// Creamos el albaran.
    AlbaranClienteView *bud =companyact->newAlbaranClienteView();
    companyact->m_pWorkspace->addWindow(bud);
    bud->cargar("0");

    /// Traspasamos los datos del albaran
    bud->setcomentalbaran(DBvalue("comentpedidocliente"));
    bud->setdescalbaran(DBvalue("descpedidocliente"));
    bud->setidforma_pago(DBvalue("idforma_pago"));
    bud->setrefalbaran(DBvalue("refpedidocliente"));
    bud->setidcliente(DBvalue("idcliente"));
    bud->setidalmacen(DBvalue("idalmacen"));
    bud->pintar();
    bud->show();

    /// Traspasamos las lineas del albaran
    SDBRecord *linea, *linea1;
	for (int i = 0; i < listalineas->rowCount(); ++i) {
	linea = listalineas->lineaat(i);
        if (linea->DBvalue( "idarticulo") != "") {
            linea1 = bud->getlistalineas()->lineaat(bud->getlistalineas()->rowCount()-1);
            linea1->setDBvalue( "desclalbaran",linea->DBvalue("desclpedidocliente"));
            linea1->setDBvalue( "cantlalbaran",linea->DBvalue("cantlpedidocliente"));
            linea1->setDBvalue( "pvplalbaran",linea->DBvalue("pvplpedidocliente"));
            linea1->setDBvalue( "descontlalbaran",linea->DBvalue("descuentolpedidocliente"));
            linea1->setDBvalue( "idarticulo",linea->DBvalue("idarticulo"));
            linea1->setDBvalue( "codigocompletoarticulo",linea->DBvalue("codigocompletoarticulo"));
            linea1->setDBvalue( "nomarticulo",linea->DBvalue("nomarticulo"));
            linea1->setDBvalue( "ivalalbaran",linea->DBvalue("ivalpedidocliente"));
            bud->getlistalineas()->nuevoRegistro();
        }// end if
    }// end for

    /// Traspasamos los descuentos.
    for ( int i = 0; i < listadescuentos->rowCount(); ++i) {
	linea1 = listadescuentos->lineaat(i);
        if (linea1->DBvalue( "proporciondpedidocliente") != "") {
            linea = bud->getlistadescuentos()->lineaat(bud->getlistadescuentos()->rowCount()-1);
            linea->setDBvalue( "conceptdalbaran",linea1->DBvalue("conceptdpedidocliente"));
            linea->setDBvalue( "proporciondalbaran",linea1->DBvalue("proporciondpedidocliente"));
            bud->getlistadescuentos()->nuevoRegistro();
        }// end if
    }// end for

}// end generarAlbaran


void PedidoClienteView::on_mui_cobrar_clicked() {
    _depura("PedidoClienteView::on_mui_cobrar_clicked",0);
    CobroView *bud = companyact->newCobroView();
    bud->setidcliente(DBvalue("idcliente"));
    bud->setcantcobro(m_totalpedidocliente->text());
    bud->setrefcobro(DBvalue("refpedidocliente"));
    bud->setcomentcobro(DBvalue("descpedidocliente"));
    bud->pintar();
    bud->show();
}// end s_nuevoCobro

int PedidoClienteView::cargar(QString id) {
    PedidoCliente::cargar(id);
    setCaption("Pedido Cliente  "+DBvalue("refpedidocliente"));
    if (companyact->meteWindow(caption(),this))
        return 1;
    dialogChanges_cargaInicial();
    return 0;
}

int PedidoClienteView::guardar() {
    setcomentpedidocliente(m_comentpedidocliente->text());
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
    setprocesadopedidocliente(m_procesadopedidocliente->isChecked()?"TRUE":"FALSE");
    int err = PedidoCliente::guardar();
    dialogChanges_cargaInicial();
    return err;
}


/// Imprime el informe de referencia.
void PedidoClienteView::on_mui_informereferencia_clicked() {
    _depura("PedidoClienteView::on_mui_informereferencia_clicked",0);
    InformeReferencia *inf = new InformeReferencia(companyact);
    inf->setreferencia(DBvalue("refpedidocliente"));
    inf->generarinforme();
    delete inf;
    _depura("END PedidoClienteView::on_mui_informereferencia_clicked",0);
}

void PedidoClienteView::closeEvent( QCloseEvent *e) {
    _depura("closeEvent",0);
    if (dialogChanges_hayCambios())  {
        int val = QMessageBox::warning(this, tr("Guardar pedido de cliente"),
						tr("Desea guardar los cambios?"),
						tr("&Si"),tr("&No"),tr("&Cancelar"),0,2);
        if (val == 0)
            guardar();
        if (val == 2)
            e->ignore();
    }// end if
}

