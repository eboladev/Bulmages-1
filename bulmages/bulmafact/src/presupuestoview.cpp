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
#include <Q3Table>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <Q3PopupMenu>
#include <QToolButton>
#include <QFile>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>
#include <QCloseEvent>
#include <fstream>
using namespace std;

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

PresupuestoView::PresupuestoView( company *comp , QWidget *parent, const char *name) : QWidget(parent, name, Qt::WDestructiveClose) , presupuesto(comp) ,dialogChanges(this) {
    _depura("Inicializacion de PresupuestoView\n",0);
    setupUi(this);
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto
    int res = g_plugins->lanza("PresupuestoView_PresupuestoView", this);
    if (res != 0)
        return;

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
    _depura("vamos a meter la ventana",0);
    comp->meteWindow(caption(),this);

    /// Disparamos los plugins por flanco descendente.
    g_plugins->lanza("PresupuestoView_PresupuestoView_Post", this);
    _depura("Fin de la inicializacion de PresupuestoView\n",0);
}// end PresupuestoView




void PresupuestoView::closeEvent(QCloseEvent *e) {
    _depura("closeEvent",0);
    if (dialogChanges_hayCambios())  {
        int val = QMessageBox::warning(this, "Guardar presupuesto",
                                        "Desea guardar los cambios?","Si","No","Cancelar",0,2);
        if (val == 0)
            guardar();
        if (val == 2)
            e->ignore();
    }// end if
}


PresupuestoView::~PresupuestoView() {
    companyact->refreshBudgets();
    companyact->sacaWindow(this);
}// end ~PresupuestoView


void PresupuestoView::inicialize() {

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



void PresupuestoView::on_mui_imprimir_clicked() {
    imprimirPresupuesto();
}//end s_printPresupuestoView




void PresupuestoView::on_mui_borrar_clicked() {
    int val = QMessageBox::warning( this, tr("Borrar presupuesto."),
                                    tr("ATENCION: Eliminar presupuestos a clientes puede disminuir la eficacia economica de la empresa. Continuar?"),tr("Si"),tr("No"),tr("Cancelar"),0,2);
    if (val == 0) {

        if (!borrar()) {
            dialogChanges_cargaInicial();
            _depura("Pedido borrado satisfactoriamente",2);
            close();
        }// end if
    }// end if
}

void PresupuestoView::pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc) {
    fprintf(stderr,"pintatotales()\n");
    m_totalBases->setText(QString(base.toQString()));
    m_totalTaxes->setText(QString(iva.toQString()));
    m_totalBudget->setText(QString(total.toQString()));
    m_totalDiscounts->setText(QString(desc.toQString()));
}// end pintatotales



/// Se encarga de generar un pedido a partir del presupuesto.
void PresupuestoView::generarPedidoCliente() {
	_depura("PresupuestoView::generarPedidoCliente",0);

    /// Comprobamos que existe el elemento, y en caso afirmativo lo mostramos y salimos
    QString SQLQuery = "SELECT * FROM pedidocliente WHERE refpedidocliente='"+DBvalue("refpresupuesto")+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        PedidoClienteView *bud = companyact->newPedidoClienteView();
        companyact->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idpedidocliente"));
        bud->show();
	delete cur;
        return;
    }// end if
    delete cur;


    /// Informamos de que no existe el pedido y a ver si lo queremos realizar. Si no salimos de la funcion.
    if (QMessageBox::question(
                this,
                tr("Pedido de cliente inexistente"),
                tr("No existe un pedido asociado a este presupuesto. Desea crearlo?"),
                tr("&Si"), tr("&No"),
                QString::null, 0, 1 ) )
        return;

    /// Creamos el pedido.
    PedidoClienteView *bud = companyact->newPedidoClienteView();
    bud->cargar("0");
    companyact->m_pWorkspace->addWindow(bud);

    /// Traspasamos toda la informacion del presupuesto al pedido.
    bud->setidcliente(DBvalue("idcliente"));
    bud->setcomentpedidocliente(DBvalue("comentpresupuesto"));
    bud->setdescpedidocliente(DBvalue("descpresupuesto"));
    bud->setfechapedidocliente(DBvalue("fpresupuesto"));
    bud->setidforma_pago(DBvalue("idforma_pago"));
    bud->setrefpedidocliente(DBvalue("refpresupuesto"));
    bud->setprocesadopedidocliente(DBvalue("procesadopresupuesto"));
    bud->setidalmacen(DBvalue("idalmacen"));
    bud->setcontactpedidocliente(DBvalue("contactpresupuesto"));
    bud->settelpedidocliente(DBvalue("telpresupuesto"));


    /// Traspasamos las lineas del presupuesto a lineas del pedido.
    SDBRecord *linea;
    SDBRecord *linea2;
    for ( linea = listalineas->lista()->first(); linea; linea = listalineas->lista()->next() ) {
        if (linea->DBvalue( "idarticulo") != "") {
            linea2 = bud->getlistalineas()->lista()->last();
            linea2->setDBvalue( "desclpedidocliente",linea->DBvalue("desclpresupuesto"));
            linea2->setDBvalue( "cantlpedidocliente",linea->DBvalue("cantlpresupuesto"));
            linea2->setDBvalue( "pvplpedidocliente",linea->DBvalue("pvplpresupuesto"));
            linea2->setDBvalue( "ivalpedidocliente",linea->DBvalue("ivalpresupuesto"));
            linea2->setDBvalue( "descuentolpedidocliente",linea->DBvalue("descuentolpresupuesto"));
            linea2->setDBvalue( "idarticulo",linea->DBvalue("idarticulo"));
            linea2->setDBvalue( "codigocompletoarticulo",linea->DBvalue("codigocompletoarticulo"));
            linea2->setDBvalue( "nomarticulo",linea->DBvalue("nomarticulo"));
            bud->getlistalineas()->nuevoRegistro();
        }// end if
    }// end for

    /// Traspasamos los descuentos del presupuesto a descuentos del pedido.
    SDBRecord *linea1;
    SDBRecord * linea3;
    for ( linea1 = listadescuentos->lista()->first(); linea1; linea1 = listadescuentos->lista()->next() ) {
        if (linea1->DBvalue( "proporciondpresupuesto") != "") {
            linea3 = bud->getlistadescuentos()->lista()->last();
            linea3->setDBvalue( "conceptdpedidocliente",linea1->DBvalue("conceptdpresupuesto"));
            linea3->setDBvalue( "proporciondpedidocliente",linea1->DBvalue("proporciondpresupuesto"));
            bud->getlistadescuentos()->nuevoRegistro();
        }// end if
    }// end for

    /// Pintamos el pedido y lo presentamos.
    bud->pintar();
    bud->show();
}




int PresupuestoView::cargar(QString id) {
    int error = 0;
    error = presupuesto::cargar(id);
    if (error)
        return -1;
    setCaption("presupuesto "+DBvalue("refpresupuesto"));
    if (companyact->meteWindow(caption(),this))
        return -1;
    dialogChanges_cargaInicial();
    return 0;
}



void PresupuestoView::on_mui_informereferencia_clicked() {
    InformeReferencia *inf = new InformeReferencia(companyact);
    inf->setreferencia(DBvalue("refpresupuesto"));
    inf->generarinforme();
    delete inf;

}


int PresupuestoView::guardar() {
    setcomentpresupuesto(m_comentpresupuesto->text());
    setnumpresupuesto(m_numpresupuesto->text());
    setidcliente(m_cliente->idcliente());
    setfpresupuesto(m_fpresupuesto->text());
    setvencpresupuesto(m_vencpresupuesto->text());
    setidalmacen(m_almacen->idalmacen());
    setidtrabajador(m_trabajador->idtrabajador());
    setidforma_pago(m_forma_pago->idforma_pago());
    setrefpresupuesto(m_refpresupuesto->text());
    setdescpresupuesto(m_descpresupuesto->text());
    setcontactpresupuesto(m_contactpresupuesto->text());
    settelpresupuesto(m_telpresupuesto->text());
    setprocesadopresupuesto(m_procesadopresupuesto->isChecked()?"TRUE":"FALSE");
    int err = presupuesto::guardar();
    dialogChanges_cargaInicial();
    return err;
}
