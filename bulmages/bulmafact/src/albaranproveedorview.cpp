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


#include "albaranproveedorview.h"


#include "company.h"
#include "division.h"
#include "articleslist.h"
#include "configuracion.h"
#include "budgetslist.h"
#include "budget.h"
//#include "pedidosproveedorlist.h"
//#include "pedidoproveedorview.h"

#include <qlineedit.h>
#include <qtextedit.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qtable.h>
#include <qwidget.h>
#include <qobjectlist.h>
#include <qcombobox.h>
#include <qpopupmenu.h>
#include <qtoolbutton.h>

#include "funcaux.h"
#include "postgresiface2.h"
#include "listlinalbaranproveedorview.h"

AlbaranProveedorView::AlbaranProveedorView(company *comp, QWidget *parent, const char *name) : AlbaranProveedorBase(parent, name, Qt::WDestructiveClose), AlbaranProveedor(comp) {
    subform2->setcompany(comp);
    m_almacen->setcompany(comp);
    m_forma_pago->setcompany(comp);
    m_proveedor->setcompany(comp);
    setListLinAlbaranProveedor(subform2);
    inicialize();
    comp->meteWindow(caption(),this);
    fprintf(stderr,"Fin de la inicialización de AlbaranProveedorView\n");
}// end ClientDelivNote


AlbaranProveedorView::~AlbaranProveedorView() {
	companyact->refreshAlbaranesProveedor();
	companyact->sacaWindow(this);
}// end ~ClientDelivNote


void AlbaranProveedorView::inicialize() {
	m_totalBases->setReadOnly(TRUE);
	m_totalBases->setAlignment(Qt::AlignRight);
	m_totalTaxes->setReadOnly(TRUE);
	m_totalTaxes->setAlignment(Qt::AlignRight);
	m_totalDiscounts->setReadOnly(TRUE);
	m_totalDiscounts->setAlignment(Qt::AlignRight);
	m_totalalbaranp->setReadOnly(TRUE);
	m_totalalbaranp->setAlignment(Qt::AlignRight);
}// end inicialize


void   AlbaranProveedorView::pintatotales(float base, float iva) {
    m_totalBases->setText(QString::number(base));
    m_totalTaxes->setText(QString::number(iva));
    m_totalalbaranp->setText(QString::number(iva+base));
}// end pintatotales









void AlbaranProveedorView::s_verpedidoproveedor() {
/*
    QString SQLQuery= "SELECT * FROM pedidoproveedor WHERE refpedidoproveedor='"+mdb_refalbaranp+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (cur->numregistros() > 1) {
        PedidosProveedorList *list = new PedidosProveedorList(companyact,NULL,theApp->translate("Edicion de Presupuestos", "company"));
        list->modoseleccion();
        list->show();
        while(!list->isHidden())
            theApp->processEvents();
        this->setEnabled(true);
        if (list->idpedidoproveedor() !="" && list->idpedidoproveedor() !=NULL) {
            PedidoProveedorView *bud = new PedidoProveedorView(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Presupuestos", "company"));
            bud->cargaPedidoProveedor(list->idpedidoproveedor());
            bud->show();
        }// end if
    } else if (!cur->eof()) {
        PedidoProveedorView *bud = new PedidoProveedorView(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Presupuestos", "company"));
        bud->cargaPedidoProveedor(cur->valor("idpedidoproveedor"));
        bud->show();
    }// end if
    delete cur;
*/
}// end s_verpedidoproveedor


#include "facturaview.h"
/// Se encarga de generar un pedido a partir del presupuesto.
void AlbaranProveedorView::generarFactura() {
/*
    /// Comprobamos que existe el elemento, y en caso afirmativo lo mostramos y salimos de la función.
    QString SQLQuery = "SELECT * FROM factura WHERE reffactura='"+mdb_refalbaranp+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
      FacturaView *bud = new FacturaView(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Facturas de Proveedors", "company"));
      bud->cargaFactura(cur->valor("idfactura"));
      bud->show();
      return;
    }
    delete cur;


    /// Informamos de que no existe el pedido y a ver si lo queremos realizar. Si no salimos de la función.
    if (QMessageBox::question(
            this,
            tr("Factura Inexistente"),
            tr("No existe una factura asociada a este albaranp."
                "Desea Crearla ?"),
            tr("&Yes"), tr("&No"),
            QString::null, 0, 1 ) )
        return;
    
	
    /// Creamos la factura.
    FacturaView *bud = new FacturaView(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Pedidos de Proveedors", "company"));
    bud->vaciaFactura();
//    bud->setcodigoalmacen(mdb_codigoalmacen);
    bud->setcomentfactura(mdb_comentalbaranp);
//    bud->setdescfactura(mdb_descalbaranp);
    bud->setfechafactura(mdb_fechaalbaranp);
    bud->setidforma_pago(mdb_idforma_pago);
    bud->setreffactura(mdb_refalbaranp);
    bud->setidproveedor(mdb_idproveedor);
    bud->setidalmacen(mdb_idalmacen);
    QString l;
    LinAlbaranProveedor *linea;
    
//    QString desclfactura, QString cantlfactura, QString pvplfactura, QString descuentolfactura, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString ivalfactura
    
    
    uint i = 0;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        bud->getlistalineas()->nuevalinea(linea->desclalbaranp(), linea->cantlalbaranp(), linea->pvplalbaranp(),"0",  linea->idarticulo(), linea->codigocompletoarticulo(), linea->nomarticulo(),"0");
        i++;
    }// end for
    bud->pintaFactura();
    bud->show();
*/
}// end generarAlbaran






