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

#include <stdio.h>

#include <QAction>
#include <QObject>
#include <QMessageBox>
#include <QLineEdit>
#include <QTabWidget>

#include "company.h"
#include "plugintrazabilidad.h"
#include "funcaux.h"
#include "movimientosview.h"


myplugin::myplugin() {}


myplugin::~myplugin() {}


void myplugin::elslot() {
    _depura("myplugin::elslot", 0);
	MovimientosView *mov = new MovimientosView((company *)empresaBase());
        empresaBase()->pWorkspace()->addWindow(mov);
	mov->show();
    _depura("END myplugin::elslot", 0);
}


void myplugin::inicializa(bulmafact *bges) {
    /// Creamos el men&uacute;.
    setEmpresaBase(bges->getcompany());
    m_bulmafact = bges;
    QAction *accion = new QAction("&Movimientos", 0);
    accion->setStatusTip("Ventana de Movimientos");
    accion->setWhatsThis("Movimientos");

    bges->menuMaestro->addSeparator();
    bges->menuMaestro->addAction(accion);

    connect(accion, SIGNAL(activated()), this, SLOT(elslot()));
}


int entryPoint(bulmafact *bges) {
    _depura("Punto de Entrada del plugin PluginTrazabilidad", 0);
    myplugin *plug = new myplugin();
    plug->inicializa(bges);
    _depura("END Punto de Entrada del plugin PluginTrazabilidad", 0);
    return 0;
}

int ListLinAlbaranProveedorView_ListLinAlbaranProveedorView(ListLinAlbaranProveedorView *subform) {
    _depura("ListLinAlbaranProveedorView_ListLinAlbaranProveedorView", 0);
    subform->addSHeader("lotelalbaranp", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, "Lote");
    _depura("END ListLinAlbaranProveedorView_ListLinAlbaranProveedorView", 0);
    return 0;
}

int ListLinAlbaranClienteView_ListLinAlbaranClienteView(ListLinAlbaranClienteView *subform) {
    _depura("ListLinAlbaranClienteView_ListLinAlbaranClienteView", 0);
    subform->addSHeader("lotelalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, "Lote");
    _depura("END ListLinAlbaranClienteView_ListLinAlbaranClienteView", 0);
    return 0;
}

int ListLinFacturaView_ListLinFacturaView(ListLinFacturaView *subform) {
    _depura("ListLinFacturaView_ListLinFacturaView", 0);
    subform->addSHeader("lotelfactura", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, "Lote");
    _depura("END ListLinFacturaView_ListLinFacturaView", 0);
    return 0;
}

int ListLinFacturaProveedorView_ListLinFacturaProveedorView(ListLinFacturaProveedorView *subform) {
    _depura("ListLinFacturaProveedorView_ListLinFacturaProveedorView", 0);
    subform->addSHeader("lotelfacturap", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, "Lote");
    _depura("END ListLinFacturaProveedorView_ListLinFacturaProveedorView", 0);
    return 0;
}

int SubForm2Bf_on_mui_list_editFinished(SubForm2Bf *) {
    _depura("SubForm2Bf_on_mui_list_editFinished", 0);

/*

    if(subform->m_prevRow == -1 || subform->m_prevCol == -1) {
	return 0;
    } // end if

    SDBRecord *rec = subform->lineaat(subform->m_prevRow);
    SDBCampo *camp = (SDBCampo *) subform->item(subform->m_prevRow, subform->m_prevCol);
    SDBCampo *campact = (SDBCampo *) subform->item(subform->currentRow(), subform->currentColumn());
    camp->refresh();



    if (camp->nomcampo() == "lote"+subform->tableName()) {
	QString query = "SELECT * FROM movimiento LEFT JOIN articulo ON movimiento.idarticulo = articulo.idarticulo WHERE lotemovimiento = '"+camp->valorcampo()+"'";
	cursor2 *cur = subform->empresaBase()->cargacursor(query);
	if (!cur->eof()) {
		if (campact->nomcampo() == "codigocompletoarticulo") {
			subform->situarse1(subform->m_prevRow, subform->m_prevCol);
			campact->setText(cur->valor("codigocompletoarticulo"));
		} else {
			rec->setDBvalue("idarticulo", cur->valor("idarticulo"));
			rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
			rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
			rec->refresh();
		} // end if
	} // end if
	delete cur;
    } // end if

*/
    return 0;

    _depura("END SubForm2Bf_on_mui_list_editFinished", 0);
}
