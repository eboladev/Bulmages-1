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




FacturaView::FacturaView(company *comp, QWidget *parent, const char *name)
: FacturaBase(parent, name, Qt::WDestructiveClose) , Factura (comp) ,dialogChanges(this) {
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    subform2->setcompany(comp);
    m_almacen->setcompany(comp);
    m_forma_pago->setcompany(comp);
    m_cliente->setcompany(comp);
    m_descuentos->setcompany(comp);
    setListLinFactura(subform2);
    setListDescuentoFactura(m_descuentos);
    inicialize();
    comp->meteWindow(caption(),this);
    fprintf(stderr,"Fin de la inicialización de Factura\n");
}


FacturaView::~FacturaView() {
    companyact->refreshFacturas();
    companyact->sacaWindow(this);
}



bool FacturaView::close(bool fil) {
    if (dialogChanges_hayCambios())  {
        if ( QMessageBox::warning( this, "Guardar Factura",
                                   "Desea guardar los cambios.",
                                   QMessageBox::Ok ,
                                   QMessageBox::Cancel ) == QMessageBox::Ok)
            s_saveFactura();
    }// end if
    return (QWidget::close(fil));
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


void   FacturaView::pintatotales(float base, float iva, float total, float desc) {
    m_totalBases->setText(QString::number(base));
    m_totalTaxes->setText(QString::number(iva));
    m_totalDiscounts->setText(QString::number(desc));
    m_totalfactura->setText(QString::number(total));
}// end pintatotales


void FacturaView::s_nuevoCobro() {
    CobroView *bud = new CobroView(companyact,NULL,theApp->translate("Edicion de Cobros", "company"));
    bud->setidcliente(mdb_idcliente);
    bud->setcantcobro(m_totalfactura->text());
    bud->setrefcobro(mdb_reffactura);
    bud->setcomentcobro(mdb_descfactura);
    bud->pintaCobro();
    bud->show();
}// end s_nuevoCobro


void FacturaView::cargaFactura(QString id) {
    Factura::cargaFactura(id);
    setCaption("Factura   "+mdb_reffactura);
    companyact->meteWindow(caption(),this);
    dialogChanges_cargaInicial();

}


void FacturaView::s_informeReferencia() {
    InformeReferencia *inf = new InformeReferencia(companyact);
    inf->setreferencia(mdb_reffactura);
    inf->generarinforme();
    delete inf;

}

