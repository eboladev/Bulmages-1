/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borrï¿½ Riera                              *
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

// PRESUPUESTOS
/*
-- Entendemos que un presupuesto es una relaciï¿½ de materiales y trabajos cuantificada que
-- hacemos a peticiï¿½ de un cliente determinado
-- Numero
-- Data: Data d'emisiï¿½del presupost.
-- PersContacte: Nom de persona de contacte (si cal).
-- TelfContacte: Telï¿½on.
-- Venciment: Data mï¿½ima de validesa del presupost.
-- Comentaris
--  Pressupost a clients.
CREATE TABLE presupuesto (
   idpresupuesto serial PRIMARY KEY,
   numpresupuesto integer,
   fpresupuesto date,
   contactpresupuesto character varying(90),
   telpresupuesto character varying(20),
   vencpresupuesto date,
   comentpresupuesto character varying(3000),
   idusuari integer,
   
   idcliente integer REFERENCES cliente(idcliente)
);
*/

/*
-- Linea de presupuesto
-- Numero
-- Descripcio: Descripciï¿½de l'article en el moment de ser presupostat.
-- Quantitat
-- PVP: Preu de l'article en el moment de ser pressupostat
-- Descompte: Percentatge de descompte en lï¿½ia.
-- Linia de pressupost a clients.
CREATE TABLE lpresupuesto (
   idlpresupuesto integer PRIMARY KEY,
   desclpresupuesto character varying(150),
   cantlpresupuesto float,
   pvplpresupuesto float,
   descuentolpresupuesto float,
   
   idpresupuesto integer NOT NULL REFERENCES presupuesto(idpresupuesto),
   idarituclo integer REFERENCES articulo(idarticulo)
);
*/

#include "listlinpresupuestoview.h"

#include "budget.h"
#include "company.h"
#include "division.h"
#include "clientslist.h"
#include "articleslist.h"
#include "configuracion.h"


#include <qmessagebox.h>

#include <qtable.h>
#include <qwidget.h>
#include <qobjectlist.h>
#include <qcombobox.h>
#include <qpopupmenu.h>
#include <qtoolbutton.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qlayout.h>

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

Budget::Budget( company *comp , QWidget *parent, const char *name) : BudgetBase(parent, name, Qt::WDestructiveClose) , presupuesto(comp) {
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    subform2->setcompany(comp);
    setlislinpresupuesto(subform2);
    inicialize();
    comp->meteWindow(caption(),this);
    fprintf(stderr,"Fin de la inicialización de Budget\n");
}// end Budget


Budget::~Budget() {
    companyact->refreshBudgets();
    companyact->sacaWindow(this);
}// end ~Budget


void Budget::inicialize() {

    m_nomalmacen->setText("");


    // Inicializamos la tabla de descuentos del presupuesto
    m_listDiscounts->setNumRows( 0 );
    m_listDiscounts->setNumCols( 0 );
    m_listDiscounts->setSelectionMode( QTable::SingleRow );
    m_listDiscounts->setSorting( TRUE );
    m_listDiscounts->setSelectionMode( QTable::SingleRow );
    m_listDiscounts->setColumnMovingEnabled( TRUE );
    m_listDiscounts->setNumCols(4);
    m_listDiscounts->horizontalHeader()->setLabel( COL_DESCUENTO_IDDPRESUPUESTO, tr( "id" ) );
    m_listDiscounts->horizontalHeader()->setLabel( COL_DESCUENTO_CONCEPTDPRESUPUESTO, tr( "Concepto" ) );
    m_listDiscounts->horizontalHeader()->setLabel( COL_DESCUENTO_PROPORCIONDPRESUPUESTO, tr( "Proporciï¿½" ) );

    m_listDiscounts->setColumnWidth(COL_DESCUENTO_IDDPRESUPUESTO,100);
    m_listDiscounts->setColumnWidth(COL_DESCUENTO_CONCEPTDPRESUPUESTO,400);
    m_listDiscounts->setColumnWidth(COL_DESCUENTO_PROPORCIONDPRESUPUESTO,100);
    m_listDiscounts->hideColumn(COL_DESCUENTO_IDDPRESUPUESTO);
    m_listDiscounts->hideColumn(COL_DESCUENTO_REMOVE);

    m_listDiscounts->setNumRows(10);

    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    m_listDiscounts->setPaletteBackgroundColor("#AFFAFA");
    m_listDiscounts->setReadOnly(FALSE);

    m_totalBases->setReadOnly(TRUE);
    m_totalBases->setAlignment(Qt::AlignRight);
    m_totalTaxes->setReadOnly(TRUE);
    m_totalTaxes->setAlignment(Qt::AlignRight);
    m_totalDiscounts->setReadOnly(TRUE);
    m_totalDiscounts->setAlignment(Qt::AlignRight);
    m_totalBudget->setReadOnly(TRUE);
    m_totalBudget->setAlignment(Qt::AlignRight);

    if (mdb_idpresupuesto=="0") {
        cursor2 * cur0= companyact->cargacursor("SELECT * FROM configuracion where nombre='AlmacenDefecto'","queryconfig");
        if (!cur0->eof()) {
            if (cur0->valor("valor")!="") {
                m_codigoalmacen->setText(cur0->valor("valor"));
            }
        }
        delete cur0;
    }
    /*
    	if (confpr->valor(CONF_MOSTRAR_ALMACEN)!="YES") {
    		m_codigoalmacen->setDisabled(true);
    		m_buscaralmacen->setDisabled(true);
    		m_lblalmacen->setDisabled(true);
    		m_nomalmacen->setDisabled(true);
    	}
    	
    	if (confpr->valor(CONF_NUM_AUT_PRESUP)=="YES") {
    		m_numpresupuesto->setReadOnly(true);
    		if (m_idpresupuesto!="0") {
    		  	m_codigoalmacen->setReadOnly(true);
    		}
    	}
    */
    // Inicializamos la forma de pago para que no se quede sin ser pintada.
    pintaidforma_pago("0");
}// end inicialize



void Budget::pintaidforma_pago(QString idformapago) {
    m_cursorcombo = NULL;
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_cursorcombo = companyact->cargacursor("SELECT * FROM forma_pago","unquery");
    int i = 0;
    int i1 = 0;
    while (!m_cursorcombo->eof()) {
        i ++;
        if (m_cursorcombo->valor("idforma_pago") == idformapago)
            i1 = i;
        m_comboformapago->insertItem(m_cursorcombo->valor("descforma_pago"));
        m_cursorcombo->siguienteregistro();
    }
    if (i1 != 0 )
        m_comboformapago->setCurrentItem(i1-1);
} //end cargarcombodformapago






// Bsqueda de Clientes.
void Budget::searchClient() {
    fprintf(stderr,"Busqueda de un client\n");
    ClientsList *clients = new ClientsList(companyact, NULL, theApp->translate("Seleccione cliente","company"));

    // , WType_Dialog| WShowModal
    clients->selectMode();

    // Esto es convertir un QWidget en un sistema modal de dialogo.
    this->setEnabled(false);
    clients->show();
    while(!clients->isHidden())
        theApp->processEvents();
    this->setEnabled(true);

    if (clients->cifclient() !="" && clients->cifclient() !=NULL) {
    // HAY QUE VIGILAR QUE ESTO ESTE BIEN
//        setIdcliente( clients->idclient());
        m_cifclient->setText(clients->cifclient());
        m_nomclient->setText(clients->nomclient());
    }
    delete clients;
}// end searchClient


void Budget::budgetDateLostFocus() {
    m_fpresupuesto->setText(normalizafecha(m_fpresupuesto->text()).toString("dd/MM/yyyy"));
    setFPresupuesto(m_fpresupuesto->text());
}


void Budget::budgetExpiryLostFocus() {
    m_vencpresupuesto->setText(normalizafecha(m_vencpresupuesto->text()).toString("dd/MM/yyyy"));
    setVencPresupuesto(m_vencpresupuesto->text());
}


void Budget::s_almacenLostFocus() {
    setCodigoAlmacen(m_codigoalmacen->text());
}//end s_almacenLostFocus


void Budget::s_printBudget() {
    imprimirPresupuesto();
}//end s_printBudget


void Budget::buscarAlmacen() {
    QMessageBox::warning( this, "BulmaFact - Presupuestos", "Aun no estï¿½implementado.", "Si", "No");
    cursor2 * cur= companyact->cargacursor("SELECT * FROM almacen where codigoalmacen ="+ m_codigoalmacen->text(),"unquery");
    delete cur;
} // end buscarAlmacen



void Budget::s_removeBudget() {
    fprintf(stderr,"Iniciamos el boton_borrar\n");
    if (QMessageBox::warning( this, "BulmaFact - Presupuestos", "Desea borrar este presupuesto", "Si", "No") == 0) {
        borraPresupuesto();
    }// end if
}// end boton_borrar

void   Budget::pintatotales(float base, float iva) {
    m_totalBases->setText(QString::number(base));
    m_totalTaxes->setText(QString::number(iva));
    m_totalBudget->setText(QString::number(iva+base));
}
;// end pintatotales


#include "pedidoclienteview.h"
/// Se encarga de generar un pedido a partir del presupuesto.
void Budget::generarPedidoCliente() {
    PedidoClienteView *bud = new PedidoClienteView(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Pedidos de Clientes", "company"));
    bud->vaciaPedidoCliente();
    bud->setcifcliente(mdb_cifcliente);
    bud->setcodigoalmacen(mdb_codigoalmacen);
    bud->setcomentpedidocliente(mdb_comentpresupuesto);
    bud->setdescpedidocliente(mdb_descpresupuesto);
    bud->setfechapedidocliente(mdb_fpresupuesto);
    bud->setidforma_pago(mdb_idforma_pago);
    bud->setrefpedidocliente(mdb_refpresupuesto);
    bud->setidpresupuesto(mdb_idpresupuesto);
    QString l;
    linpresupuesto *linea;
    uint i = 0;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        bud->getlistalineas()->nuevalinea(linea->desclpresupuesto(), linea->cantlpresupuesto(), linea->pvplpresupuesto(), "", linea->ivalpresupuesto(), linea->descuentolpresupuesto(), linea->idarticulo(), linea->codigocompletoarticulo(), linea->nomarticulo());
        i++;
    }// end for
    bud->pintaPedidoCliente();
    bud->show();
}// end generaPedidoCliente




