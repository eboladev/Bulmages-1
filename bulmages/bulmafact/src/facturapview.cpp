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
#include <QMessageBox>
#include <Q3Table>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <Q3PopupMenu>
#include <QToolButton>
#include <QLayout>
#include <fstream>
#include <QCloseEvent>
using namespace std;

#include "facturapview.h"
#include "company.h"
#include "listlinfacturapview.h"
#include "facturap.h"
#include "providerslist.h"
#include "cobroview.h"
#include "funcaux.h"

FacturaProveedorView::FacturaProveedorView(company *comp, QWidget *parent, const char *name)
        : QWidget(parent, name, Qt::WDestructiveClose) , FacturaProveedor (comp) ,dialogChanges(this) {
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
	setupUi(this);
    subform2->setcompany(comp);
    m_forma_pago->setcompany(comp);
    m_proveedor->setcompany(comp);
    m_descuentos->setcompany(comp);
    setListLinFacturaProveedor(subform2);
    setListDescuentoFacturaProv(m_descuentos);
    inicialize();
    comp->meteWindow(caption(),this);
    dialogChanges_cargaInicial();
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


void   FacturaProveedorView::pintatotales(Fixed base, Fixed iva) {
    m_totalBases->setText(base.toQString());
    m_totalTaxes->setText(iva.toQString());
    m_totalfacturap->setText((iva+base).toQString());
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
            guardar();
        if (val == 2)
            e->ignore();
    }// end if
}

int FacturaProveedorView::cargar(QString id) {
    FacturaProveedor::cargar(id);
    setCaption("FacturaProveedor   "+DBvalue("reffacturap"));
    if (companyact->meteWindow(caption(),this))
        return -1;
    dialogChanges_cargaInicial();
    return 0;
}




int FacturaProveedorView::guardar() {
    setidproveedor(m_proveedor->idproveedor());
    setnumfacturap(m_numfacturap->text());
    setfechafacturap(m_fechafacturap->text());
    setdescfacturap(m_descfacturap->text());
    setcomentfacturap(m_comentfacturap->text());
    setreffacturap(m_reffacturap->text());
    setidforma_pago(m_forma_pago->idforma_pago());
    setprocesadafacturap(m_procesadafacturap->isChecked()?"TRUE":"FALSE");

    int err = FacturaProveedor::guardar();
    dialogChanges_cargaInicial();
    return err;
}
