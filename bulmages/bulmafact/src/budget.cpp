/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borr� Riera                              *
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

#include "listlinpresupuestoview.h"

#include "budget.h"
#include "company.h"
#include "division.h"
#include "clientslist.h"
#include "articleslist.h"
#include "configuracion.h"
#include "pedidoclienteview.h"
#include "albaranclienteview.h"
#include "busquedacliente.h"
#include "busquedaformapago.h"

#include "informereferencia.h"

#include <qmessagebox.h>

#include <q3table.h>
#include <qwidget.h>
#include <qobject.h>
#include <qcombobox.h>
#include <q3popupmenu.h>
#include <qtoolbutton.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <QCloseEvent>
#include <fstream>
using namespace std;

#include "funcaux.h"

#define COL_IDLPRESUPUESTO 0
#define COL_IDARTICULO 1
#define COL_CODARTICULO 2
#define COL_NOMARTICULO 3
#define COL_DESCLPRESUPUESTO 4
#define COL_CANTLPRESUPUESTO 5
#define COL_PVPLPRESUPUESTO 6
#define COL_DESCUENTOLPRESUPUESTO 7
#define COL_IDPRESUPUESTO 8
#define COL_REMOVE 9
#define COL_TASATIPO_IVA 10
#define COL_TIPO_IVA 11

#define COL_DESCUENTO_IDDPRESUPUESTO 0
#define COL_DESCUENTO_CONCEPTDPRESUPUESTO 1
#define COL_DESCUENTO_PROPORCIONDPRESUPUESTO 2
#define COL_DESCUENTO_REMOVE 3

#define coma "'"

Budget::Budget( company *comp , QWidget *parent, const char *name) : BudgetBase(parent, name, Qt::WDestructiveClose) , presupuesto(comp) ,dialogChanges(this) {
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    subform2->setcompany(comp);
    m_descuentos->setcompany(comp);
    m_cliente->setcompany(comp);
    m_forma_pago->setcompany(comp);
    m_almacen->setcompany(comp);
    m_trabajador->setcompany(comp);
    setlislinpresupuesto(subform2);
    setlisdescpresupuesto(m_descuentos);
    inicialize();
    comp->meteWindow(caption(),this);
    _depura("Fin de la inicializacion de Budget\n",0);
}// end Budget




void Budget::closeEvent( QCloseEvent *e) {
	_depura("closeEvent",0);
    if (dialogChanges_hayCambios())  {
        int val = QMessageBox::warning( this, "Guardar Presupuesto",
                                   "Desea guardar los cambios.","Si","No","Cancelar",0,2);
	if (val == 0) 
            s_saveBudget();
	if (val == 2)
	    e->ignore();
    }// end if	
}


Budget::~Budget() {
    companyact->refreshBudgets();
    companyact->sacaWindow(this);
}// end ~Budget


void Budget::inicialize() {

    m_totalBases->setReadOnly(TRUE);
    m_totalBases->setAlignment(Qt::AlignRight);
    m_totalTaxes->setReadOnly(TRUE);
    m_totalTaxes->setAlignment(Qt::AlignRight);
    m_totalDiscounts->setReadOnly(TRUE);
    m_totalDiscounts->setAlignment(Qt::AlignRight);
    m_totalBudget->setReadOnly(TRUE);
    m_totalBudget->setAlignment(Qt::AlignRight);
    // Inicializamos la forma de pago para que no se quede sin ser pintada.
    pintaidforma_pago("0");
    pintaidalmacen("0");
    pintaidtrabajador("0");
}// end inicialize



void Budget::s_printBudget() {
    imprimirPresupuesto();
}//end s_printBudget




void Budget::s_removeBudget() {
    fprintf(stderr,"Iniciamos el boton_borrar\n");
    if (QMessageBox::warning( this, "BulmaFact - Presupuestos", "Desea borrar este presupuesto", "Si", "No") == 0) {
        borraPresupuesto();
    }// end if
}// end boton_borrar

void   Budget::pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc) {
    fprintf(stderr,"pintatotales()\n");
    m_totalBases->setText(QString(base.toQString()));
    m_totalTaxes->setText(QString(iva.toQString()));
    m_totalBudget->setText(QString(total.toQString()));
    m_totalDiscounts->setText(QString(desc.toQString()));
}// end pintatotales



/// Se encarga de generar un pedido a partir del presupuesto.
void Budget::generarPedidoCliente() {
    /// Comprobamos que existe el elemento, y en caso afirmativo lo mostramos y salimos de la funci�.
    QString SQLQuery = "SELECT * FROM pedidocliente WHERE refpedidocliente='"+mdb_refpresupuesto+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        PedidoClienteView *bud = new PedidoClienteView(companyact,NULL,theApp->translate("Edicion de Pedidos de Clientes", "company"));
	companyact->m_pWorkspace->addWindow(bud);
        bud->cargaPedidoCliente(cur->valor("idpedidocliente"));
        bud->show();
        return;
    }
    delete cur;


    /// Informamos de que no existe el pedido y a ver si lo queremos realizar. Si no salimos de la funci�.
    if (QMessageBox::question(
                this,
                tr("Pedido Cliente Inexistente"),
                tr("No existe un pedido asociado a este presupuesto."
                   "Desea Crearlo ?"),
                tr("&Yes"), tr("&No"),
                QString::null, 0, 1 ) )
        return;

    /// Creamos el pedido.
    PedidoClienteView *bud = new PedidoClienteView(companyact,0,theApp->translate("Edicion de Pedidos de Clientes", "company"));
    bud->vaciaPedidoCliente();
	companyact->m_pWorkspace->addWindow(bud);
    bud->setidcliente(mdb_idcliente);
    bud->setcomentpedidocliente(mdb_comentpresupuesto);
    bud->setdescpedidocliente(mdb_descpresupuesto);
    bud->setfechapedidocliente(mdb_fpresupuesto);
    bud->setidforma_pago(mdb_idforma_pago);
    bud->setrefpedidocliente(mdb_refpresupuesto);
    bud->setprocesadopedidocliente(mdb_procesadopresupuesto);
    bud->setidalmacen(mdb_idalmacen);
    bud->setcontactpedidocliente(mdb_contactpresupuesto);
    bud->settelpedidocliente(mdb_telpresupuesto);
    QString l;
    linpresupuesto *linea;
    uint i = 0;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        bud->getlistalineas()->nuevalinea(linea->desclpresupuesto(), linea->cantlpresupuesto(), linea->pvplpresupuesto(), "", linea->ivalpresupuesto(), linea->descuentolpresupuesto(), linea->idarticulo(), linea->codigocompletoarticulo(), linea->nomarticulo(),"FALSE");
        i++;
    }// end for

    DescuentoPresupuesto *linea1;
    i = 0;
    for ( linea1 = listadescuentos->m_lista.first(); linea1; linea1 = listadescuentos->m_lista.next() ) {
        bud->getlistadescuentos()->nuevalinea(linea1->conceptdpresupuesto(), linea1->proporciondpresupuesto());
        i++;
    }// end for

    bud->pintaPedidoCliente();
    bud->show();
}// end generaPedidoCliente




int Budget::chargeBudget(QString id) {
    int error = 0;
    error = presupuesto::chargeBudget(id);
	if (error) return -1;
    setCaption("presupuesto "+mdb_refpresupuesto);
    if (companyact->meteWindow(caption(),this))
	return -1;
    dialogChanges_cargaInicial();
    return 0;
}



void Budget::s_informeReferencia() {
    InformeReferencia *inf = new InformeReferencia(companyact);
    inf->setreferencia(mdb_refpresupuesto);
    inf->generarinforme();
    delete inf;

}



