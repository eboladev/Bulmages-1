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

// ALBARANES DE CLIENTES


#include "albaranclienteview.h"


#include "company.h"
#include "division.h"
#include "clientslist.h"
#include "articleslist.h"
#include "configuracion.h"
#include "budgetslist.h"
#include "budget.h"
#include "pedidosclientelist.h"
#include "pedidoclienteview.h"
#include "facturaview.h"
#include "informereferencia.h"


#include <qlineedit.h>
#include <q3textedit.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <q3table.h>
#include <qwidget.h>
#include <qobject.h>
#include <qcombobox.h>
#include <q3popupmenu.h>
#include <qtoolbutton.h>

#include "funcaux.h"
#include "postgresiface2.h"
#include "listlinalbaranclienteview.h"
#include "listdescalbaranclienteview.h"
#include "facturaslist.h"


AlbaranClienteView::AlbaranClienteView(company *comp, QWidget *parent, const char *name) : AlbaranClienteBase(parent, name, Qt::WDestructiveClose), AlbaranCliente(comp) ,dialogChanges(this) {
    subform2->setcompany(comp);
    m_descuentos->setcompany(comp);
    m_almacen->setcompany(comp);
    m_forma_pago->setcompany(comp);
    m_cliente->setcompany(comp);
    m_trabajador->setcompany(comp);
    setListLinAlbaranCliente(subform2);
    setListDescuentoAlbaranCliente(m_descuentos);
//    inicialize();
    comp->meteWindow(caption(),this);
    _depura("Fin de la inicializaciï¿½ de AlbaranClienteView\n");
}// end ClientDelivNote


bool AlbaranClienteView::close(bool fil) {
    if (dialogChanges_hayCambios())  {
        if ( QMessageBox::warning( this, tr("Guardar Albaran"),
                                   tr("Desea guardar los cambios."),
                                   QMessageBox::Ok ,
                                   QMessageBox::Cancel ) == QMessageBox::Ok)
            s_saveAlbaranCliente();
    }// end if
    return (QWidget::close(fil));
}

AlbaranClienteView::~AlbaranClienteView() {
    companyact->refreshAlbaranesCliente();
    companyact->sacaWindow(this);
}// end ~ClientDelivNote


/*
void AlbaranClienteView::inicialize() {
}// end inicialize
*/


void   AlbaranClienteView::pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc) {
    m_totalBases->setText(base.toQString());
    m_totalTaxes->setText(iva.toQString());
    m_totalalbaran->setText(total.toQString());
    m_totalDiscounts->setText(desc.toQString());
}// end pintatotales


void AlbaranClienteView::s_verpresupuesto() {
    QString SQLQuery= "SELECT * FROM presupuesto WHERE refpresupuesto='"+mdb_refalbaran+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (cur->numregistros() > 1) {
        BudgetsList *list = new BudgetsList(companyact,NULL,theApp->translate("Edicion de Presupuestos", "company"));
        list->modoseleccion();
        list->show();
        while(!list->isHidden())
            theApp->processEvents();
        this->setEnabled(true);
        if (list->idpresupuesto() !=QString("")) {
//&& list->idpresupuesto() !=NULL
            Budget *bud = new Budget(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Presupuestos", "company"));
            bud->chargeBudget(list->idpresupuesto());
            bud->show();
        }// end if
    } else if (!cur->eof()) {
        Budget *bud = new Budget(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Presupuestos", "company"));
        bud->chargeBudget(cur->valor("idpresupuesto"));
        bud->show();
    }// end if
    delete cur;
}// end s_verpresupuesto



void AlbaranClienteView::s_verpedidocliente() {
    QString SQLQuery= "SELECT * FROM pedidocliente WHERE refpedidocliente='"+mdb_refalbaran+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (cur->numregistros() > 1) {
        PedidosClienteList *list = new PedidosClienteList(companyact,NULL,theApp->translate("Edicion de Presupuestos", "company"));
        list->modoseleccion();
        list->show();
        while(!list->isHidden())
            theApp->processEvents();
        this->setEnabled(true);
        if (list->idpedidocliente() !="") {
// && list->idpedidocliente() !=NULL
            PedidoClienteView *bud = new PedidoClienteView(companyact,NULL,theApp->translate("Edicion de Presupuestos", "company"));
			companyact->m_pWorkspace->addWindow(bud);
            bud->cargaPedidoCliente(list->idpedidocliente());
            bud->show();
        }// end if
    } else if (!cur->eof()) {
        PedidoClienteView *bud = new PedidoClienteView(companyact,NULL,theApp->translate("Edicion de Presupuestos", "company"));
		companyact->m_pWorkspace->addWindow(bud);
        bud->cargaPedidoCliente(cur->valor("idpedidocliente"));
        bud->show();
    }// end if
    delete cur;
}// end s_verpedidocliente



/// Se encarga de generar una factura a partir de un albarï¿½
void AlbaranClienteView::generarFactura() {
    /// Comprobamos que existe el elemento, y en caso afirmativo lo mostramos y salimos de la funciï¿½.
    QString SQLQuery = "SELECT * FROM factura WHERE reffactura='"+mdb_refalbaran+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        FacturaView *bud = new FacturaView(companyact,NULL,theApp->translate("Edicion de Facturas de Clientes", "company"));
		companyact->m_pWorkspace->addWindow(bud);
        bud->cargaFactura(cur->valor("idfactura"));
        bud->show();
        return;
    }
    delete cur;
    /// Informamos de que no existe el pedido y a ver si lo queremos realizar. Si no salimos de la funciï¿½.
    if (QMessageBox::question(
                this,
                tr("Factura Inexistente"),
                tr("No existe una factura asociada a este albaran."
                   "Desea Crearla ?"),
                tr("&Yes"), tr("&No"),
                QString::null, 0, 1 ) )
        return;

    /// Creamos la factura.
    FacturaView *bud = new FacturaView(companyact,NULL,theApp->translate("Edicion de Pedidos de Clientes", "company"));
	companyact->m_pWorkspace->addWindow(bud);
    bud->vaciaFactura();
    //    bud->setcodigoalmacen(mdb_codigoalmacen);
    bud->setcomentfactura(mdb_comentalbaran);
    //    bud->setdescfactura(mdb_descalbaran);
    bud->setfechafactura(mdb_fechaalbaran);
    bud->setidforma_pago(mdb_idforma_pago);
    bud->setreffactura(mdb_refalbaran);
    bud->setidcliente(mdb_idcliente);
    bud->setidalmacen(mdb_idalmacen);
    QString l;
    LinAlbaranCliente *linea;
    uint i = 0;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        bud->getlistalineas()->nuevalinea(linea->desclalbaran(), linea->cantlalbaran(), linea->pvplalbaran(),linea->descontlalbaran() ,  linea->idarticulo(), linea->codigocompletoarticulo(), linea->nomarticulo(), linea->ivalalbaran());
        i++;
    }// end for
    bud->pintaFactura();
    bud->show();
}// end generarFactura




/// Se encarga de agregar un albarï¿½ a una factura
void AlbaranClienteView::agregarFactura() {
	
	// Pedimos la factura a la que agregar
	    _depura("Busqueda de una factura\n");
    FacturasList *fac = new FacturasList(companyact, NULL, tr("Seleccione factura","company"));
    // , WType_Dialog| WShowModal
    fac->modoseleccion();
    // Esto es convertir un QWidget en un sistema modal de dialogo.
    this->setEnabled(false);
    fac->show();
    while(!fac->isHidden())
        theApp->processEvents();
    this->setEnabled(true);
    QString idfactura = fac->idfactura();
    delete fac;

	/// Si no hay idfactura es que hemos abortado y por tanto cancelamos la operación
	if (idfactura == "") return;

    /// Creamos la factura.
    FacturaView *bud = new FacturaView(companyact,NULL,theApp->translate("Edicion de Pedidos de Clientes", "company"));
    bud->cargaFactura(idfactura);
	companyact->m_pWorkspace->addWindow(bud);
    /// EN TEORIA SE DEBARIA COMPROBAR QUE LA FACTURA ES DEL MISMO CLIENTE, pero por ahora pasamos de hacerlo.
    QString l;
    LinAlbaranCliente *linea;
    uint i = 0;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        bud->getlistalineas()->nuevalinea(linea->desclalbaran(), linea->cantlalbaran(), linea->pvplalbaran(),linea->descontlalbaran(),  linea->idarticulo(), linea->codigocompletoarticulo(), linea->nomarticulo(),linea->ivalalbaran());
        i++;
    }// end for
    bud->pintaFactura();
    bud->show();
}// end generarAlbaran


void AlbaranClienteView::cargaAlbaranCliente(QString id) {
    AlbaranCliente::cargaAlbaranCliente(id);
    setCaption("Albaran Cliente  "+mdb_refalbaran);
    companyact->meteWindow(caption(),this);
    dialogChanges_cargaInicial();
}



void AlbaranClienteView::s_informeReferencia() {
    InformeReferencia *inf = new InformeReferencia(companyact);
    inf->setreferencia(mdb_refalbaran);
    inf->generarinforme();
    delete inf;

}

