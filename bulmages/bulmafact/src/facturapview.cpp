//
// C++ Implementation: facturap
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "facturapview.h"
#include "company.h"
#include "listlinfacturapview.h"
#include "facturap.h"
#include "providerslist.h"
#include "cobroview.h"
#include "funcaux.h"

#include <qmessagebox.h>
#include <q3table.h>
#include <qwidget.h>
#include <qobject.h>
#include <qcombobox.h>
#include <q3popupmenu.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <fstream>

#include <QCloseEvent>
using namespace std;




FacturaProveedorView::FacturaProveedorView(company *comp, QWidget *parent, const char *name)
        : FacturaProveedorBase(parent, name, Qt::WDestructiveClose) , FacturaProveedor (comp) ,dialogChanges(this) {
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    subform2->setcompany(comp);
    m_forma_pago->setcompany(comp);
    m_proveedor->setcompany(comp);
    m_descuentos->setcompany(comp);
    setListLinFacturaProveedor(subform2);
    setListDescuentoFacturaProv(m_descuentos);
    inicialize();
    comp->meteWindow(caption(),this);
    _depura("Fin de la inicializaci� de FacturaProveedor\n");
}


FacturaProveedorView::~FacturaProveedorView() {
    //    companyact->refreshFacturaProveedors();
    companyact->sacaWindow(this);
}

void FacturaProveedorView::inicialize() {
    m_totalBases->setReadOnly(TRUE);
    m_totalBases->setAlignment(Qt::AlignRight);
    m_totalTaxes->setReadOnly(TRUE);
    m_totalTaxes->setAlignment(Qt::AlignRight);
    m_totalDiscounts->setReadOnly(TRUE);
    m_totalDiscounts->setAlignment(Qt::AlignRight);
    m_totalfacturap->setReadOnly(TRUE);
    m_totalfacturap->setAlignment(Qt::AlignRight);
}// end inicialize


void   FacturaProveedorView::pintatotales(float base, float iva) {
    m_totalBases->setText(QString::number(base));
    m_totalTaxes->setText(QString::number(iva));
    m_totalfacturap->setText(QString::number(iva+base));
}// end pintatotales


void FacturaProveedorView::s_nuevoCobro() {
    /*
        CobroView *bud = new CobroView(companyact,NULL,theApp->translate("Edicion de Cobros", "company"));
        bud->setidproveedor(mdb_idproveedor);
        bud->setcantcobro(m_totalfacturap->text());
        bud->setrefcobro(mdb_reffacturap);
        bud->setcomentcobro(mdb_descfacturap);
        bud->pintaCobro();
        bud->show();
    */
}// end s_nuevoCobro


void FacturaProveedorView::closeEvent( QCloseEvent *e) {
    _depura("closeEvent",0);
    if (dialogChanges_hayCambios())  {
        int val = QMessageBox::warning( this, "Guardar Factura Proveedor",
                                        "Desea guardar los cambios.","Si","No","Cancelar",0,2);
        if (val == 0)
            guardaFacturaProveedor();
        if (val == 2)
            e->ignore();
    }// end if
}

int FacturaProveedorView::cargaFacturaProveedor(QString id) {
    FacturaProveedor::cargaFacturaProveedor(id);
    setCaption("FacturaProveedor   "+DBvalue("reffacturap"));
    if (companyact->meteWindow(caption(),this))
        return -1;
    return 0;
}



