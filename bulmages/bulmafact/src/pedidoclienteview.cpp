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
#include "cobroview.h"
#include "albaranclienteview.h"
#include "informereferencia.h"

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

#include <QCloseEvent>

PedidoClienteView::PedidoClienteView(company *comp, QWidget *parent, const char *name)
        : PedidoClienteBase(parent, name, Qt::WDestructiveClose) , PedidoCliente (comp),dialogChanges(this) {
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
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

bool PedidoClienteView::close(bool fil) {
    if (dialogChanges_hayCambios())  {
        if ( QMessageBox::warning( this, "Guardar Pedido Cliente",
                                   "Desea guardar los cambios.",
                                   QMessageBox::Ok ,
                                   QMessageBox::Cancel ) == QMessageBox::Ok)
            s_savePedidoCliente();
    }// end if
    return (QWidget::close(fil));
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
        if (list->idpresupuesto() !="") {
            Budget *bud = companyact->newBudget();
            companyact->m_pWorkspace->addWindow(bud);
            bud->chargeBudget(list->idpresupuesto());
            bud->show();
        }// end if
    } else if (!cur->eof()) {
        Budget *bud = companyact->newBudget();
        companyact->m_pWorkspace->addWindow(bud);
        bud->chargeBudget(cur->valor("idpresupuesto"));
        bud->show();
    }// end if
    delete cur;
}// end s_verpresupuesto


/// Se encarga de generar un albaran a partir del pedido.
void PedidoClienteView::generarAlbaran() {
    /// Comprobamos que existe el elemento, y en caso afirmativo lo mostramos y salimos de la funci�.
    QString SQLQuery = "SELECT * FROM albaran WHERE refalbaran='"+mdb_refpedidocliente+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        AlbaranClienteView *bud = new AlbaranClienteView(companyact,NULL,theApp->translate("Edicion de Albaranes de Clientes", "company"));
        companyact->m_pWorkspace->addWindow(bud);
        bud->cargaAlbaranCliente(cur->valor("idalbaran"));
        bud->show();
        return;
    }
    delete cur;


    /// Informamos de que no existe el pedido y a ver si lo queremos realizar. Si no salimos de la funci�.
    if (QMessageBox::question(
                this,
                tr("Albaran Cliente Inexistente"),
                tr("No existe un albaran asociado a este pedido."
                   "Desea Crearlo ?"),
                tr("&Yes"), tr("&No"),
                QString::null, 0, 1 ) )
        return;


    /// Creamos el pedido.
    AlbaranClienteView *bud = new AlbaranClienteView(companyact,NULL,theApp->translate("Edicion de Pedidos de Clientes", "company"));
    companyact->m_pWorkspace->addWindow(bud);
    bud->vaciaAlbaranCliente();
    bud->setcomentalbaran(mdb_comentpedidocliente);
    bud->setdescalbaran(mdb_descpedidocliente);
    bud->setidforma_pago(mdb_idforma_pago);
    bud->setrefalbaran(mdb_refpedidocliente);
    bud->setidcliente(mdb_idcliente);
    bud->setidalmacen(mdb_idalmacen);
    QString l;
    LinPedidoCliente *linea;
    uint i = 0;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        bud->getlistalineas()->nuevalinea(linea->desclpedidocliente(), linea->cantlpedidocliente(), linea->pvplpedidocliente(),linea->descuentolpedidocliente(),  linea->idarticulo(), linea->codigocompletoarticulo(), linea->nomarticulo(), linea->ivalpedidocliente());
        i++;
    }// end for

    DescuentoPedidoCliente *linea1;
    i = 0;
    for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
        bud->getlistadescuentos()->nuevalinea(linea1->conceptdpedidocliente(), linea1->proporciondpedidocliente());
        i++;
    }// end for

    bud->pintaAlbaranCliente();
    bud->show();
}// end generarAlbaran


void PedidoClienteView::s_nuevoCobro() {
    CobroView *bud = new CobroView(companyact,NULL,theApp->translate("Edicion de Cobros", "company"));
    bud->setidcliente(mdb_idcliente);
    bud->setcantcobro(m_totalpedidocliente->text());
    bud->setrefcobro(mdb_refpedidocliente);
    bud->setcomentcobro(mdb_descpedidocliente);
    bud->pintaCobro();
    bud->show();
}// end s_nuevoCobro

int PedidoClienteView::cargaPedidoCliente(QString id) {
    PedidoCliente::cargaPedidoCliente(id);
    setCaption("Pedido Cliente  "+mdb_refpedidocliente);
    if (companyact->meteWindow(caption(),this))
        return 1;
    dialogChanges_cargaInicial();
    return 0;
}

/// Imprime el informe de referencia.
void PedidoClienteView::s_informeReferencia() {
    InformeReferencia *inf = new InformeReferencia(companyact);
    inf->setreferencia(mdb_refpedidocliente);
    inf->generarinforme();
    delete inf;
}// end s_informeReferencia

void PedidoClienteView::closeEvent( QCloseEvent *e) {
    _depura("closeEvent",0);
    if (dialogChanges_hayCambios())  {
        int val = QMessageBox::warning( this, "Guardar Pedido Cliente",
                                        "Desea guardar los cambios.","Si","No","Cancelar",0,2);
        if (val == 0)
            guardaPedidoCliente();
        if (val == 2)
            e->ignore();
    }// end if
}

