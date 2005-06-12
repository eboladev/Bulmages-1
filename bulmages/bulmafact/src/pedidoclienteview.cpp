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
#include "pedidoclienteview.h"
#include "company.h"
#include "listlinpedidoclienteview.h"
#include "pedidocliente.h"
#include "clientslist.h"
#include "budget.h"
#include "budgetslist.h"

#include <qmessagebox.h>
#include <qtable.h>
#include <qwidget.h>
#include <qobjectlist.h>
#include <qcombobox.h>
#include <qpopupmenu.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <fstream>
using namespace std;
#include "funcaux.h"


PedidoClienteView::PedidoClienteView(company *comp, QWidget *parent, const char *name)
: PedidoClienteBase(parent, name, Qt::WDestructiveClose) , PedidoCliente (comp) {
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    subform3->setcompany(comp);
    m_cliente->setcompany(comp);
    m_forma_pago->setcompany(comp);
    m_almacen->setcompany(comp);
    setListLinPedidoCliente(subform3);
    inicialize();
    comp->meteWindow(caption(),this);
    fprintf(stderr,"Fin de la inicialización de PedidoCliente\n");
}


PedidoClienteView::~PedidoClienteView() {
    companyact->refreshPedidosCliente();
    companyact->sacaWindow(this);
}

void PedidoClienteView::inicialize() {
    m_totalBases->setReadOnly(TRUE);
    m_totalBases->setAlignment(Qt::AlignRight);
    m_totalTaxes->setReadOnly(TRUE);
    m_totalTaxes->setAlignment(Qt::AlignRight);
    m_totalDiscounts->setReadOnly(TRUE);
    m_totalDiscounts->setAlignment(Qt::AlignRight);
    m_totalpedidocliente->setReadOnly(TRUE);
    m_totalpedidocliente->setAlignment(Qt::AlignRight);
}// end inicialize



void   PedidoClienteView::pintatotales(float base, float iva) {
    m_totalBases->setText(QString::number(base));
    m_totalTaxes->setText(QString::number(iva));
    m_totalpedidocliente->setText(QString::number(iva+base));
}// end pintatotales



void PedidoClienteView::s_verpresupuesto() {
    QString SQLQuery= "SELECT * FROM presupuesto WHERE refpresupuesto='"+mdb_refpedidocliente+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if (cur->numregistros() > 1) {
        BudgetsList *list = new BudgetsList(companyact,NULL,theApp->translate("Edicion de Presupuestos", "company"));
        list->modoseleccion();
        list->show();
        while(!list->isHidden())
            theApp->processEvents();
        this->setEnabled(true);
        if (list->idpresupuesto() !="" && list->idpresupuesto() !=NULL) {
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



#include "albaranclienteview.h"
/// Se encarga de generar un pedido a partir del presupuesto.
void PedidoClienteView::generarAlbaran() {
    /// Comprobamos que existe el elemento, y en caso afirmativo lo mostramos y salimos de la función.
    QString SQLQuery = "SELECT * FROM albaran WHERE refalbaran='"+mdb_refpedidocliente+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
      AlbaranClienteView *bud = new AlbaranClienteView(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Albaranes de Clientes", "company"));
      bud->cargaAlbaranCliente(cur->valor("idalbaran"));
      bud->show();
      return;
    }
    delete cur;


    /// Informamos de que no existe el pedido y a ver si lo queremos realizar. Si no salimos de la función.
    if (QMessageBox::question(
            this,
            tr("Albaran Cliente Inexistente"),
            tr("No existe un albaran asociado a este pedido."
                "Desea Crearlo ?"),
            tr("&Yes"), tr("&No"),
            QString::null, 0, 1 ) )
        return;
    
	
    /// Creamos el pedido.
    AlbaranClienteView *bud = new AlbaranClienteView(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Pedidos de Clientes", "company"));
    bud->vaciaAlbaranCliente();
//    bud->setcodigoalmacen(mdb_codigoalmacen);
    bud->setcomentalbaran(mdb_comentpedidocliente);
    bud->setdescalbaran(mdb_descpedidocliente);
    bud->setfechaalbaran(mdb_fechapedidocliente);
    bud->setidforma_pago(mdb_idforma_pago);
    bud->setrefalbaran(mdb_refpedidocliente);
    bud->setidcliente(mdb_idcliente);
    bud->setidalmacen(mdb_idalmacen);
    QString l;
    LinPedidoCliente *linea;
    uint i = 0;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        bud->getlistalineas()->nuevalinea(linea->desclpedidocliente(), linea->cantlpedidocliente(), linea->pvplpedidocliente(),linea->descuentolpedidocliente(),  linea->idarticulo(), linea->codigocompletoarticulo(), linea->nomarticulo());
        i++;
    }// end for
    bud->pintaAlbaranCliente();
    bud->show();
}// end generarAlbaran


