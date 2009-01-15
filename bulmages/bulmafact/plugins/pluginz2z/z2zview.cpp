/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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
#include <QComboBox>
#include <QLayout>
#include <QMessageBox>
#include <QObject>
#include <QToolButton>
#include <QWidget>
#include <fstream>

#include "z2zview.h"
#include "company.h"
#include "funcaux.h"
#include "plugins.h"


/** Inicializa la pantalla.
    Inicializa todos los componentes y mete la pantalla en el workSpace.
*/
/**
\param comp
\param parent
**/
Z2ZView::Z2ZView ( Company *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "Z2ZView::Z2ZView", 0 );
    setupUi ( this );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {

        meteWindow ( windowTitle(), this, FALSE );
    } catch ( ... ) {
        mensajeInfo ( tr ( "Error al crear la factura" ), this );
    }
    _depura ( "END Z2ZView::Z2ZView" );
}


/** Al destruir la clase provocamos que se refresque el listado de facturas.
    Para que salga actualizado.
*/
/**
**/
Z2ZView::~Z2ZView()
{
    _depura ( "Z2ZView::~Z2ZView", 0 );
    _depura ( "END Z2ZView::~Z2ZView", 0 );
}


void Z2ZView::on_mui_referencia_returnPressed() {
	new QListWidgetItem(mui_referencia->text(), mui_listarefs);
	calculaTotalTickets();
	mui_referencia->setText("");
	mui_referencia->setFocus();
}

void 	Z2ZView::on_mui_listarefs_itemDoubleClicked(QListWidgetItem *it) {
	delete it;
	calculaTotalTickets();
}


void Z2ZView::calculaTotalTickets() {
	QString listarefs;
	if (mui_listarefs->count() > 0) {
		listarefs = "upper('" + mui_listarefs->item(0)->text() + "')";
		for (int i=1; i < mui_listarefs->count(); i++) {
			listarefs += ", upper('" + mui_listarefs->item(i)->text() + "')";
		} // end for
	} else {
		mensajeInfo("lista vacia");
		return;
	} // end if


	QString query = "SELECT sum(pvpivainclalbaran*cantlalbaran)::NUMERIC(12,2) AS tot FROM lalbaran LEFT JOIN albaran ON albaran.idalbaran= lalbaran.idalbaran WHERE upper(albaran.refalbaran) IN ("+listarefs+")";
	

	cursor2 *cur = empresaBase()->cargacursor(query);
	mui_totaltickets->setText(cur->valor("tot"));
	delete cur;

	Fixed total("0.00");
	Fixed min("0.00");

	for (int i=0; i < mui_listarefs->count(); i++) {

		QString query1 = "SELECT COALESCE(sum(pvpivainclalbaran*cantlalbaran)::NUMERIC(12,2), 0) AS tot FROM lalbaran LEFT JOIN albaran ON albaran.idalbaran= lalbaran.idalbaran WHERE upper(albaran.refalbaran) = upper('" + mui_listarefs->item(i)->text() + "')";
		cursor2 *cur1 = empresaBase()->cargacursor(query1);
		total = total + Fixed(cur1->valor("tot"));
		delete cur1;

		QString query2 = "SELECT COALESCE(min(pvpivainclalbaran),0) AS tot FROM lalbaran LEFT JOIN albaran ON albaran.idalbaran= lalbaran.idalbaran WHERE upper(albaran.refalbaran) = upper('" + mui_listarefs->item(i)->text() + "')";
		cursor2 *cur2 = empresaBase()->cargacursor(query2);
		min = min + Fixed(cur2->valor("tot"));
		delete cur2;
		
	} // end for

	Fixed traspasable = total - min;
	mui_totaltraspasable->setText(traspasable.toQString());

}




