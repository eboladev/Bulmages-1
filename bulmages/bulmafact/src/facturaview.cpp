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
#include "facturaview.h"
#include "company.h"
#include "listlinfacturaview.h"
#include "listdescfacturaview.h"
#include "factura.h"
#include "clientslist.h"
#include "cobroview.h"
#include "funcaux.h"
#include "informereferencia.h"

#include "clientdelivnoteslist.h"
#include "albarancliente.h"

#include <QCloseEvent>

#include <QMessageBox>
#include <Q3Table>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <Q3PopupMenu>
#include <QToolButton>
#include <QLayout>
#include <fstream>
using namespace std;




FacturaView::FacturaView(company *comp, QWidget *parent, const char *name)
        : FacturaBase(parent, name, Qt::WDestructiveClose) , Factura (comp) ,dialogChanges(this) {
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
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
    _depura("Fin de la inicializaci� de Factura\n");
}


FacturaView::~FacturaView() {
    companyact->refreshFacturas();
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
}// end inicialize


void   FacturaView::pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc) {
    m_totalBases->setText(base.toQString());
    m_totalTaxes->setText(iva.toQString());
    m_totalfactura->setText(total.toQString());
    m_totalDiscounts->setText(desc.toQString());
}// end pintatotales


/// Crea un nuevo cobro para la factura seleccionada.
void FacturaView::s_nuevoCobro() {
    CobroView *bud = new CobroView(companyact,NULL,theApp->translate("Edicion de Cobros", "company"));
    bud->setidcliente(DBvalue("idcliente"));
    bud->setcantcobro(m_totalfactura->text());
    bud->setrefcobro(DBvalue("reffactura"));
    bud->setcomentcobro(DBvalue("descfactura"));
    bud->pintar();
    bud->show();
}// end s_nuevoCobro


int FacturaView::cargaFactura(QString id) {
    Factura::cargaFactura(id);
    setCaption("Factura   "+DBvalue("reffactura"));
    if (companyact->meteWindow(caption(),this) )
        return 1;
    dialogChanges_cargaInicial();
    return 0;
}// end cargaFactura


/// Imprime el informe de referencia.
void FacturaView::s_informeReferencia() {
    InformeReferencia *inf = new InformeReferencia(companyact);
    inf->setreferencia(DBvalue("reffactura"));
    inf->generarinforme();
    delete inf;
}// end s_informeReferencia

void FacturaView::s_agregaAlbaran() {
    _depura("FacturaView::s_agregaAlbaran\n",0);
    /// Seleccionamos el albarán.FacturaView::s_agregaAlbaran
    // Pedimos la factura a la que agregar

    ClientDelivNotesList *fac = new ClientDelivNotesList(companyact, NULL, tr("Seleccione albaran","company"),0,ClientDelivNotesList::SelectMode);
    fac->setidcliente(DBvalue("idcliente"));
    fac->modoseleccion();

    // Esto es convertir un QWidget en un sistema modal de dialogo.
    this->setEnabled(false);
    fac->show();
    while(!fac->isHidden())
        theApp->processEvents();
    this->setEnabled(true);
    QString idalbaran = fac->idCliDelivNote();
    delete fac;

    /// Si no hay idfactura es que hemos abortado y por tanto cancelamos la operaci�
    if (idalbaran == "")
        return;

    /// Creamos la factura.
    AlbaranCliente *bud = new AlbaranCliente(companyact);
    bud->cargaAlbaranCliente(idalbaran);


    /// Agregamos a comentarios que albaran se corresponde.
 	QString comm = DBvalue("comentfactura")+"(ALBARAN: Num"+bud->numalbaran()+"Ref:"+bud->refalbaran()+"Fecha:"+bud->fechaalbaran()+")\n";

    setDBvalue("comentfactura", comm);

    /// EN TEORIA SE DEBARIA COMPROBAR QUE LA FACTURA ES DEL MISMO CLIENTE, pero por ahora pasamos de hacerlo.
    LinAlbaranCliente *linea;
    for (linea = bud->getlistalineas()->m_lista.first(); linea; linea = bud->getlistalineas()->m_lista.next() ) {
        //nuevalinea();
        listalineas->nuevalinea(linea->desclalbaran()
                                , linea->cantlalbaran()
                                , linea->pvplalbaran()
                                , linea->descontlalbaran()
                                , linea->idarticulo()
                                , linea->codigocompletoarticulo()
                                , linea->nomarticulo()
                                , linea->ivalalbaran()
                               );
    }// end for

    delete bud;

    pintaFactura();
    show();

    /// Comprobamos que el albaran se ajusta.

    /// Leemos lineas e insertamos lineas.

}// end agregaAlbaran


void FacturaView::closeEvent( QCloseEvent *e) {
    _depura("closeEvent",0);
    if (dialogChanges_hayCambios())  {
        int val = QMessageBox::warning( this, "Guardar Factura",
                                        "Desea guardar los cambios.","Si","No","Cancelar",0,2);
        if (val == 0)
            s_saveFactura();
        if (val == 2)
            e->ignore();
    }// end if
}

