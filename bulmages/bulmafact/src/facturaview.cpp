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

#include <QCloseEvent>
#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QLayout>
#include <fstream>

using namespace std;

#include "facturaview.h"
#include "company.h"
#include "listlinfacturaview.h"
#include "listdescfacturaview.h"
#include "factura.h"
#include "clientslist.h"
#include "cobroview.h"
#include "funcaux.h"
#include "informereferencia.h"
#include "albaranclientelist.h"
#include "albaranclienteview.h"


FacturaView::FacturaView(company *comp, QWidget *parent, const char *name)
        : QWidget(parent, name, Qt::WDestructiveClose), Factura (comp), dialogChanges(this) {
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    _depura("FacturaView::FacturaView", 0);
    setupUi(this);
    subform2->setcompany(comp);
    m_almacen->setcompany(comp);
    m_forma_pago->setcompany(comp);
    m_cliente->setcompany(comp);
    m_descuentos->setcompany(comp);
    m_codigoserie_factura->setcompany(comp);
    setListLinFactura(subform2);
    setListDescuentoFactura(m_descuentos);
    inicialize();
    comp->meteWindow(caption(),this);
    /// Hacemos una carga inicial para que quede bien inicializada la clase (una chapucilla)
    cargar("0");
    _depura("END FacturaView::FacturaView");
}


FacturaView::~FacturaView() {
    companyact->sacaWindow(this);
}


void FacturaView::inicialize() {
    m_totalBases->setReadOnly(TRUE);
    m_totalBases->setAlignment(Qt::AlignRight);
    m_totalTaxes->setReadOnly(TRUE);
    m_totalTaxes->setAlignment(Qt::AlignRight);
    m_totalDiscounts->setReadOnly(TRUE);
    m_totalDiscounts->setAlignment(Qt::AlignRight);
    m_totalfactura->setReadOnly(TRUE);
    m_totalfactura->setAlignment(Qt::AlignRight);
}


void   FacturaView::pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc) {
    m_totalBases->setText(base.toQString());
    m_totalTaxes->setText(iva.toQString());
    m_totalfactura->setText(total.toQString());
    m_totalDiscounts->setText(desc.toQString());
}


/// Crea un nuevo cobro para la factura seleccionada.
void FacturaView::on_mui_cobrar_clicked() {
    CobroView *bud = companyact->newCobroView();
    bud->setidcliente(DBvalue("idcliente"));
    bud->setcantcobro(m_totalfactura->text());
    bud->setrefcobro(DBvalue("reffactura"));
    bud->setcomentcobro(DBvalue("descfactura"));
    bud->pintar();
    bud->show();
}


int FacturaView::cargar(QString id) {
    Factura::cargar(id);
    setCaption(tr("Factura") + " - " + DBvalue("reffactura"));
    if (companyact->meteWindow(caption(), this))
        return 1;
    dialogChanges_cargaInicial();
    return 0;
}


/// Imprime el informe de referencia.
void FacturaView::on_mui_informereferencia_clicked() {
    InformeReferencia *inf = new InformeReferencia(companyact);
    inf->setreferencia(DBvalue("reffactura"));
    inf->generarinforme();
    delete inf;
}


void FacturaView::on_mui_agregaralbaran_clicked() {
    _depura("FacturaView::on_mui_agregaralbaran_clicked\n", 0);
    QDialog *diag=new QDialog(0);
    diag->setModal(true);
    AlbaranClienteList *fac = new AlbaranClienteList(companyact, diag, tr("Seleccione albaran","company"), 0, AlbaranClienteList::SelectMode);
    connect(fac, SIGNAL(selected(QString)), diag, SLOT(accept()));

    /// Hacemos que las opciones de filtrado del listado ya esten bien.
    fac->m_cliente->setidcliente(DBvalue("idcliente"));
    fac->on_mui_actualizar_clicked();

    /// Lanzamos el dialogo.
    diag->exec();
    QString idalbaran = fac->idCliDelivNote();
    delete diag;
 
    /// Si no hay idfactura es que hemos abortado y por tanto cancelamos la operacion.
    if (idalbaran == "")
        return;

    /// Creamos la factura.
    AlbaranClienteView *bud = new AlbaranClienteView(companyact, NULL, tr("albaran que no se vera"));
    bud->cargar(idalbaran);

    /// Agregamos a comentarios que albaran se corresponde.
    QString comm = DBvalue("comentfactura") + "(" + tr("ALBARAN: Num ") + bud->numalbaran() + tr("Ref: ") + bud->refalbaran() + tr("Fecha: ") + bud->fechaalbaran() + ")\n";

    setDBvalue("comentfactura", comm);
    pintaComentFactura(comm);

    /// EN TEORIA SE DEBERIA COMPROBAR QUE LA FACTURA Y EL ALBARAN SON DEL MISMO CLIENTE, pero por ahora no lo hacemos.
    SDBRecord *linea, *linea1;
    for (int i = 0; i < bud->getlistalineas()->rowCount(); ++i) {
        linea = getlistalineas()->lineaat(i);
        /// Los registros vacios no se tienen en cuenta.
        if (linea->DBvalue( "idarticulo") != "") {
            linea1 = getlistalineas()->lineaat(getlistalineas()->rowCount() - 1);
            linea1->setDBvalue("desclfactura", linea->DBvalue("desclalbaran"));
            linea1->setDBvalue("cantlfactura", linea->DBvalue("cantlalbaran"));
            linea1->setDBvalue("pvplfactura", linea->DBvalue("pvplalbaran"));
            linea1->setDBvalue("descuentolfactura", linea->DBvalue("descontlalbaran"));
            linea1->setDBvalue("idarticulo", linea->DBvalue("idarticulo"));
            linea1->setDBvalue("codigocompletoarticulo", linea->DBvalue("codigocompletoarticulo"));
            linea1->setDBvalue("nomarticulo", linea->DBvalue("nomarticulo"));
            linea1->setDBvalue("ivalfactura", linea->DBvalue("ivalalbaran"));
            getlistalineas()->nuevoRegistro();
        } // end if
    } // end for

    /// Procesamos el albaran.
    bud->m_procesadoalbaran->setChecked(TRUE);
    bud->guardar();
    delete bud;

    /// Pintamos los totales.
    calculaypintatotales();
    _depura("END FacturaView::on_mui_agregaralbaran_clicked\n", 0);
}


void FacturaView::closeEvent(QCloseEvent *e) {
    _depura("closeEvent", 0);
    if (dialogChanges_hayCambios())  {
        int val = QMessageBox::warning(this,
                                       tr("Guardar factura"),
                                       tr("Desea guardar los cambios?"),
                                       tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
        if (val == 0)
            guardar();
        if (val == 2)
            e->ignore();
    } // end if
}


int FacturaView::guardar() {
    setcomentfactura(m_comentfactura->text());
    setidalmacen(m_almacen->idalmacen());
    setNumFactura(m_numfactura->text());
    setreffactura(m_reffactura->text());
    setidcliente(m_cliente->idcliente());
    setfechafactura(m_fechafactura->text());
    setdescfactura(m_descfactura->text());
    setidforma_pago(m_forma_pago->idforma_pago());
    setcodigoserie_factura(m_codigoserie_factura->codigoserie_factura());
    setprocesadafactura( m_procesadafactura->isChecked() ? "TRUE" : "FALSE");
    int err = Factura::guardar();
    dialogChanges_cargaInicial();
    return err;
}

