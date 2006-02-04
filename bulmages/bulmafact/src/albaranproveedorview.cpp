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
#include "articleslist.h"
#include "configuracion.h"
#include "budgetslist.h"
#include "budget.h"
//#include "pedidosproveedorlist.h"
//#include "pedidoproveedorview.h"

#include <qlineedit.h>
#include <q3textedit.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <q3table.h>
#include <qwidget.h>
#include <qobject.h>
#include <qcombobox.h>
#include <q3popupmenu.h>
#include <qtoolbutton.h>
#include <QCloseEvent>

#include "funcaux.h"
#include "postgresiface2.h"
#include "listlinalbaranproveedorview.h"

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
	_depura("Funcion aun no implementada",2);
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
	setCaption("Albaran Proveedor  "+mdb_refalbaranp);
	if(companyact->meteWindow(caption(),this)) return -1;
	return 0;
}

