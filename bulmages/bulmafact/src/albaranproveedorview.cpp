/***************************************************************************
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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

 // ALBARANES DE CLIENTES


#include "albaranproveedorview.h"


#include "company.h"
#include "division.h"
#include "articulolist.h"
#include "configuracion.h"
#include "presupuestolist.h"
#include "presupuestoview.h"
//#include "pedidosproveedorlist.h"
//#include "pedidoproveedorview.h"

#include <QLineEdit>
#include <Q3TextEdit>
#include <QMessageBox>
#include <QLabel>
#include <Q3Table>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <Q3PopupMenu>
#include <QToolButton>
#include <QCloseEvent>

#include "funcaux.h"
#include "postgresiface2.h"
#include "listlinalbaranproveedorview.h"
#include "facturapview.h"

AlbaranProveedorView::AlbaranProveedorView(company *comp, QWidget *parent, const char *name) : AlbaranProveedorBase(parent, name, Qt::WDestructiveClose), AlbaranProveedor(comp) ,dialogChanges(this) {
    subform2->setcompany(comp);
    m_almacen->setcompany(comp);
    m_forma_pago->setcompany(comp);
    m_proveedor->setcompany(comp);
    m_descuentos->setcompany(comp);
    setListLinAlbaranProveedor(subform2);
    setListDescuentoAlbaranProveedor(m_descuentos);
    inicialize();
    comp->meteWindow(caption(),this);
    _depura("Fin de la inicializacion de AlbaranProveedorView\n");
}// end ClientDelivNote


AlbaranProveedorView::~AlbaranProveedorView() {
	companyact->refreshAlbaranesProveedor();
	companyact->sacaWindow(this);
}// end ~ClientDelivNote


void AlbaranProveedorView::inicialize() {
	m_totalBases->setReadOnly(TRUE);
	m_totalBases->setAlignment(Qt::AlignRight);
	m_totalTaxes->setReadOnly(TRUE);
	m_totalTaxes->setAlignment(Qt::AlignRight);
	m_totalDiscounts->setReadOnly(TRUE);
	m_totalDiscounts->setAlignment(Qt::AlignRight);
	m_totalalbaranp->setReadOnly(TRUE);
	m_totalalbaranp->setAlignment(Qt::AlignRight);
}// end inicialize


void   AlbaranProveedorView::pintatotales(float base, float iva) {
    m_totalBases->setText(QString::number(base));
    m_totalTaxes->setText(QString::number(iva));
    m_totalalbaranp->setText(QString::number(iva+base));
}// end pintatotales


void AlbaranProveedorView::s_verpedidoproveedor() {
	_depura("Funcion aun no implementada",2);
}// end s_verpedidoproveedor


#include "facturaview.h"
/// Se encarga de generar una facturap a partir del albaranp
void AlbaranProveedorView::generarFactura() {
	_depura("AlbaranProveedorView::generarFactura",0);

    /// Comprobamos que existe el elemento, y en caso afirmativo lo mostramos y salimos de la funci�.
    QString SQLQuery = "SELECT * FROM facturap WHERE reffacturap='"+DBvalue("refalbaranp")+"'";
    cursor2 *cur = companyact->cargacursor(SQLQuery);
    if(!cur->eof()) {
        FacturaProveedorView *bud = companyact->newFacturaProveedorView();
		companyact->m_pWorkspace->addWindow(bud);
        bud->cargaFacturaProveedor(cur->valor("idfacturap"));
        bud->show();
        return;
    }
    delete cur;
    /// Informamos de que no existe el pedido y a ver si lo queremos realizar. Si no salimos de la funci�.
    if (QMessageBox::question(
                this,
                tr("Factura Inexistente"),
                tr("No existe una factura asociada a este albaran."
                   "Desea Crearla ?"),
                tr("&Yes"), tr("&No"),
                QString::null, 0, 1 ) )
        return;

    /// Creamos la factura.
    FacturaProveedorView *bud = companyact->newFacturaProveedorView();
	companyact->m_pWorkspace->addWindow(bud);
    bud->vaciaFacturaProveedor();
    bud->setcomentfacturap(DBvalue("comentalbaranp"));
    bud->setidforma_pago(DBvalue("idforma_pago"));
    bud->setreffacturap(DBvalue("refalbaranp"));
    bud->setidproveedor(DBvalue("idproveedor"));
    LinAlbaranProveedor *linea;
    uint i = 0;
    for ( linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next() ) {
        bud->getlistalineas()->nuevalinea(
		linea->desclalbaranp(),
		linea->cantlalbaranp(),
		linea->pvplalbaranp(),
		linea->descontlalbaranp(),
		linea->idarticulo(),
		linea->codigocompletoarticulo(),
		linea->nomarticulo(),
		linea->ivalalbaranp()
	);
        i++;
    }// end for
    bud->pintaFacturaProveedor();
    bud->show();

}// end generarAlbaran




void AlbaranProveedorView::closeEvent( QCloseEvent *e) {
	_depura("closeEvent",0);
    if (dialogChanges_hayCambios())  {
        int val = QMessageBox::warning( this, "Guardar Albaran",
                                   "Desea guardar los cambios.","Si","No","Cancelar",0,2);
	if (val == 0) 
            guardaAlbaranProveedor();
	if (val == 2)
	    e->ignore();
    }// end if	
}


int AlbaranProveedorView::cargaAlbaranProveedor(QString id) {
	AlbaranProveedor::cargaAlbaranProveedor(id);
	setCaption("Albaran Proveedor  "+DBvalue("refalbaranp"));
	if(companyact->meteWindow(caption(),this)) return -1;
	return 0;
}

