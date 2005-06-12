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
#include "factura.h"
#include "clientslist.h"

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


FacturaView::FacturaView(company *comp, QWidget *parent, const char *name)
: FacturaBase(parent, name, Qt::WDestructiveClose) , Factura (comp) {
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    subform2->setcompany(comp);
    m_almacen->setcompany(comp);
    m_forma_pago->setcompany(comp);
    m_cliente->setcompany(comp);    
    setListLinFactura(subform2);
    inicialize();
    comp->meteWindow(caption(),this);
    fprintf(stderr,"Fin de la inicialización de Factura\n");
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


void   FacturaView::pintatotales(float base, float iva) {
    m_totalBases->setText(QString::number(base));
    m_totalTaxes->setText(QString::number(iva));
    m_totalfactura->setText(QString::number(iva+base));
}// end pintatotales


