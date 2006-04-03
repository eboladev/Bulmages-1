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


#include <QCloseEvent>
#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <Q3PopupMenu>
#include <QToolButton>
#include <QLayout>
#include <fstream>
using namespace std;


#include "pedidoproveedorview.h"
#include "company.h"
#include "listlinpedidoproveedorview.h"
#include "pedidoproveedor.h"
#include "providerslist.h"
#include "presupuestoview.h"
#include "presupuestolist.h"
#include "pagoview.h"
#include "albaranproveedorview.h"
#include "funcaux.h"


PedidoProveedorView::PedidoProveedorView(company *comp, QWidget *parent, const char *name)
        : QWidget(parent, name, Qt::WDestructiveClose) , PedidoProveedor (comp),dialogChanges(this) {
    setupUi(this);
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    subform3->setcompany(comp);
    m_proveedor->setcompany(comp);
    m_forma_pago->setcompany(comp);
    m_descuentos->setcompany(comp);
    m_almacen->setcompany(comp);
    m_trabajador->setcompany(comp);
    setListLinPedidoProveedor(subform3);
    setListDescuentoPedidoProveedor(m_descuentos);
    inicialize();
    comp->meteWindow(caption(),this);
    _depura("Fin de la inicializacion de PedidoProveedor\n");
}


PedidoProveedorView::~PedidoProveedorView() {
    companyact->refreshPedidosProveedor();
    companyact->sacaWindow(this);
}


void PedidoProveedorView::inicialize() {
    m_totalBases->setReadOnly(TRUE);
    m_totalBases->setAlignment(Qt::AlignRight);
    m_totalTaxes->setReadOnly(TRUE);
    m_totalTaxes->setAlignment(Qt::AlignRight);
    m_totalDiscounts->setReadOnly(TRUE);
    m_totalDiscounts->setAlignment(Qt::AlignRight);
    m_totalpedidoproveedor->setReadOnly(TRUE);
    m_totalpedidoproveedor->setAlignment(Qt::AlignRight);
    /// Inicializamos la forma de pago para que no se quede sin ser pintada.
    pintaidforma_pago("0");
    pintaidalmacen("0");
    pintaidtrabajador("0");
    subform3->pintar();
    m_descuentos->pintar();
}// end inicialize



int PedidoProveedorView::cargar(QString id) {
    PedidoProveedor::cargar(id);
    setCaption("Pedido Proveedor  "+DBvalue("refpedidoproveedor"));
    if (companyact->meteWindow(caption(),this))
        return -1;
    dialogChanges_cargaInicial();
    return 0;
}


int PedidoProveedorView::guardar() {
    setcomentpedidoproveedor(m_comentpedidoproveedor->text());
    setnumpedidoproveedor(m_numpedidoproveedor->text());
    setidproveedor(m_proveedor->idproveedor());
    setfechapedidoproveedor(m_fechapedidoproveedor->text());
    setidalmacen(m_almacen->idalmacen());
    setidtrabajador(m_trabajador->idtrabajador());
    setidforma_pago(m_forma_pago->idforma_pago());
    setrefpedidoproveedor(m_refpedidoproveedor->text());
    setdescpedidoproveedor(m_descpedidoproveedor->text());
    setcontactpedidoproveedor(m_contactpedidoproveedor->text());
    settelpedidoproveedor(m_telpedidoproveedor->text());
    setprocesadopedidoproveedor(m_procesadopedidoproveedor->isChecked()?"TRUE":"FALSE");
    int error = PedidoProveedor::guardar();
    dialogChanges_cargaInicial();
    return error;
}



void   PedidoProveedorView::pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc) {
    m_totalBases->setText(base.toQString());
    m_totalTaxes->setText(iva.toQString());
    m_totalpedidoproveedor->setText(total.toQString());
    m_totalDiscounts->setText(desc.toQString());
}// end pintatotales


void PedidoProveedorView::on_mui_pagar_clicked() {
    PagoView *bud = new PagoView(companyact,NULL,theApp->translate("Edicion de Pagos", "company"));
    bud->setidproveedor(DBvalue("idproveedor"));
    bud->setcantpago(m_totalpedidoproveedor->text());
    bud->setrefpago(DBvalue("refpedidoproveedor"));
    bud->setcomentpago(DBvalue("descpedidoproveedor"));
    bud->pintar();
    bud->show();
}// end s_verpresupuesto




/// Se encarga de generar un albaran a partir del pedido.
void PedidoProveedorView::generarAlbaran() {

    /// Comprobamos que existe el elemento, y en caso afirmativo lo mostramos y salimos de la funci�.
    QString SQLQuery = "SELECT * FROM albaranp WHERE refalbaranp='"+DBvalue("refpedidoproveedor")+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        AlbaranProveedorView *bud = new AlbaranProveedorView(companyact,NULL,theApp->translate("Edicion de Albaranes de Proveedores", "company"));
        companyact->m_pWorkspace->addWindow(bud);
        bud->cargar(cur->valor("idalbaranp"));
        bud->show();
        return;
    }
    delete cur;


    /// Informamos de que no existe el pedido y a ver si lo queremos realizar. Si no salimos de la funci�.
    if (QMessageBox::question(
                this,
                tr("Albaran Proveedor Inexistente"),
                tr("No existe un albaran asociado a este pedido."
                   "Desea Crearlo ?"),
                tr("&Yes"), tr("&No"),
                QString::null, 0, 1 ) )
        return;


    /// Creamos el pedido.
    AlbaranProveedorView *bud = new AlbaranProveedorView(companyact,NULL,theApp->translate("Edicion de Albaranes de Proveedores", "company"));
    companyact->m_pWorkspace->addWindow(bud);
    bud->vaciaAlbaranProveedor();

    bud->setcomentalbaranp(DBvalue("comentpedidoproveedor"));
    bud->setdescalbaranp(DBvalue("descpedidoproveedor"));
    bud->setidforma_pago(DBvalue("idforma_pago"));
    bud->setrefalbaranp(DBvalue("refpedidoproveedor"));
    bud->setidproveedor(DBvalue("idproveedor"));
    bud->setidalmacen(DBvalue("idalmacen"));

    QString l;
    SDBRecord *linea;
    uint i = 0;
    for ( linea = listalineas->lista()->first(); linea; linea = listalineas->lista()->next() ) {
        bud->getlistalineas()->nuevalinea(
            linea->DBvalue("desclpedidoproveedor"),
            linea->DBvalue("cantlpedidoproveedor"),
            linea->DBvalue("pvplpedidoproveedor"),
            linea->DBvalue("descuentolpedidoproveedor"),
            linea->DBvalue("idarticulo"),
            linea->DBvalue("codigocompletoarticulo"),
            linea->DBvalue("nomarticulo"),
            linea->DBvalue("ivalpedidoproveedor")
        );
        i++;
    }// end for



    for (int i=0; i < listadescuentos->rowCount()-1; i++) {
        Fixed propor(listadescuentos->DBvalue( "proporciondpedidoproveedor",i).ascii());
        bud->getlistadescuentos()->nuevalinea(listadescuentos->DBvalue( "conceptdpedidoproveedor",i), listadescuentos->DBvalue( "proporciondpedidoproveedor",i));
    }// end for

    bud->pintaAlbaranProveedor();
    bud->show();
}// end generarAlbaran



void PedidoProveedorView::closeEvent( QCloseEvent *e) {
    _depura("closeEvent",0);
    if (dialogChanges_hayCambios())  {
        int val = QMessageBox::warning( this, "Guardar Pedido Proveedor",
                                        "Desea guardar los cambios.","Si","No","Cancelar",0,2);
        if (val == 0)
            guardar();
        if (val == 2)
            e->ignore();
    }// end if
}
