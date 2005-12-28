//
// C++ Implementation: factura
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pedidoproveedorview.h"
#include "company.h"
#include "listlinpedidoproveedorview.h"
#include "pedidoproveedor.h"
#include "providerslist.h"
#include "budget.h"
#include "budgetslist.h"
#include "pagoview.h"

#include <qmessagebox.h>
#include <q3table.h>
#include <qwidget.h>
#include <qobject.h>
#include <qcombobox.h>
#include <q3popupmenu.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <fstream>
using namespace std;
#include "funcaux.h"


PedidoProveedorView::PedidoProveedorView(company *comp, QWidget *parent, const char *name)
: PedidoProveedorBase(parent, name, Qt::WDestructiveClose) , PedidoProveedor (comp),dialogChanges(this) {
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    subform3->setcompany(comp);
    m_proveedor->setcompany(comp);
    m_forma_pago->setcompany(comp);
    m_descuentos->setcompany(comp);
    m_almacen->setcompany(comp);
    m_trabajador->setcompany(comp);
    setListLinPedidoProveedor(subform3);
    setListDescuentoPedidoProveedor(m_descuentos);
    inicialize();
    comp->meteWindow(caption(),this);
    _depura("Fin de la inicializacion de PedidoProveedor\n");
}

bool PedidoProveedorView::close(bool fil) {
    if (dialogChanges_hayCambios())  {
        if ( QMessageBox::warning( this, "Guardar Pedido Proveedor",
                                   "Desea guardar los cambios.",
                                   QMessageBox::Ok ,
                                   QMessageBox::Cancel ) == QMessageBox::Ok)
            s_savePedidoProveedor();
    }// end if
    return (QWidget::close(fil));
}


PedidoProveedorView::~PedidoProveedorView() {
    companyact->refreshPedidosProveedor();
    companyact->sacaWindow(this);
}

void PedidoProveedorView::inicialize() {
    m_totalBases->setReadOnly(TRUE);
    m_totalBases->setAlignment(Qt::AlignRight);
    m_totalTaxes->setReadOnly(TRUE);
    m_totalTaxes->setAlignment(Qt::AlignRight);
    m_totalDiscounts->setReadOnly(TRUE);
    m_totalDiscounts->setAlignment(Qt::AlignRight);
    m_totalpedidoproveedor->setReadOnly(TRUE);
    m_totalpedidoproveedor->setAlignment(Qt::AlignRight);
    /// Inicializamos la forma de pago para que no se quede sin ser pintada.
    pintaidforma_pago("0");
    pintaidalmacen("0");
    pintaidtrabajador("0");
}// end inicialize



void   PedidoProveedorView::pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc) {
    m_totalBases->setText(base.toQString());
    m_totalTaxes->setText(iva.toQString());
    m_totalpedidoproveedor->setText(total.toQString());
    m_totalDiscounts->setText(desc.toQString());
}// end pintatotales


void PedidoProveedorView::s_verpresupuesto() {
    QString SQLQuery= "SELECT * FROM presupuesto WHERE refpresupuesto='"+mdb_refpedidoproveedor+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (cur->numregistros() > 1) {
        BudgetsList *list = new BudgetsList(companyact,NULL,theApp->translate("Edicion de Presupuestos", "company"));
        list->modoseleccion();
        list->show();
        while(!list->isHidden())
            theApp->processEvents();
        this->setEnabled(true);
        if (list->idpresupuesto() !="") {
// && list->idpresupuesto() !=NULL
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



#include "albaranproveedorview.h"
/// Se encarga de generar un pedido a partir del presupuesto.
void PedidoProveedorView::generarAlbaran() {
/*
    /// Comprobamos que existe el elemento, y en caso afirmativo lo mostramos y salimos de la funci�.
    QString SQLQuery = "SELECT * FROM albaran WHERE refalbaran='"+mdb_refpedidoproveedor+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        AlbaranProveedorView *bud = new AlbaranProveedorView(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Albaranes de Proveedors", "company"));
        bud->cargaAlbaranProveedor(cur->valor("idalbaran"));
        bud->show();
        return;
    }
    delete cur;


    /// Informamos de que no existe el pedido y a ver si lo queremos realizar. Si no salimos de la funci�.
    if (QMessageBox::question(
                this,
                tr("Albaran Proveedor Inexistente"),
                tr("No existe un albaran asociado a este pedido."
                   "Desea Crearlo ?"),
                tr("&Yes"), tr("&No"),
                QString::null, 0, 1 ) )
        return;


    /// Creamos el pedido.
    AlbaranProveedorView *bud = new AlbaranProveedorView(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Pedidos de Proveedors", "company"));
    bud->vaciaAlbaranProveedor();
    bud->setcomentalbaran(mdb_comentpedidoproveedor);
    bud->setdescalbaran(mdb_descpedidoproveedor);
    bud->setfechaalbaran(mdb_fechapedidoproveedor);
    bud->setidforma_pago(mdb_idforma_pago);
    bud->setrefalbaran(mdb_refpedidoproveedor);
    bud->setidproveedor(mdb_idproveedor);
    bud->setidalmacen(mdb_idalmacen);
    QString l;
    LinPedidoProveedor *linea;
    uint i = 0;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        bud->getlistalineas()->nuevalinea(linea->desclpedidoproveedor(), linea->cantlpedidoproveedor(), linea->pvplpedidoproveedor(),linea->descuentolpedidoproveedor(),  linea->idarticulo(), linea->codigocompletoarticulo(), linea->nomarticulo(), linea->ivalpedidoproveedor());
        i++;
    }// end for

    DescuentoPedidoProveedor *linea1;
    i = 0;
    for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
        bud->getlistadescuentos()->nuevalinea(linea1->conceptdpedidoproveedor(), linea1->proporciondpedidoproveedor());
        i++;
    }// end for

    bud->pintaAlbaranProveedor();
    bud->show();
    
*/    
}// end generarAlbaran


void PedidoProveedorView::s_nuevoCobro() {
    PagoView *bud = new PagoView(companyact,NULL,theApp->translate("Edicion de Pagos", "company"));
    bud->setidproveedor(mdb_idproveedor);
    bud->setcantpago(m_totalpedidoproveedor->text());
    bud->setrefpago(mdb_refpedidoproveedor);
    bud->setcomentpago(mdb_descpedidoproveedor);
    bud->pintaPago();
    bud->show();
}// end s_nuevoCobro

void PedidoProveedorView::cargaPedidoProveedor(QString id) {
    PedidoProveedor::cargaPedidoProveedor(id);
    setCaption("Pedido Proveedor  "+mdb_refpedidoproveedor);
    companyact->meteWindow(caption(),this);
    dialogChanges_cargaInicial();
}
