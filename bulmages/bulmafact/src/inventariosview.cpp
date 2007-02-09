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

#include <QMessageBox>
#include <QFile>
#include <QTextStream>

#include "inventariosview.h"
#include "company.h"
#include "funcaux.h"
#include "inventarioview.h"

#define COL_IDINVENTARIO 0
#define COL_NOMINVENTARIO 1
#define COL_FECHAINVENTARIO 2


InventariosView::InventariosView(QWidget *parent, Qt::WFlags flag)
        : Ficha(parent, flag) {
    setupUi(this);
    companyact = NULL;
    meteWindow(windowTitle(), this);
}


InventariosView::InventariosView(company *comp, QWidget *parent)
        : Ficha(parent) {
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    companyact = comp;
    mui_listado->setcompany(comp);
    mui_listado->cargar();
    meteWindow(windowTitle(), this);
}


InventariosView::~InventariosView() {}


int InventariosView::sacaWindow() {
    companyact->sacaWindow(this);
    return 0;
}


void InventariosView::on_mui_editar_clicked() {
    int a = mui_listado->currentRow();
    if (a < 0) {
        mensajeInfo(tr("Tiene que seleccionar un inventario"));
        return;
    } else {
        QString idinventario = mui_listado->item(a, COL_IDINVENTARIO)->text();
        if (idinventario != "") {
            InventarioView *bud = new InventarioView(companyact, 0);
            if (bud->cargar(idinventario))
                return;
            companyact->m_pWorkspace->addWindow(bud);
            bud->show();
            bud->mui_nominventario->setFocus();
        } // end if
    } // end if
}


void InventariosView::on_mui_borrar2_clicked() {
    int a = mui_listado->currentRow();
    if (a < 0) {
        mensajeInfo(tr("Tiene que seleccionar un inventario"));
        return;
    } else {
        QString idinventario = mui_listado->item(a, COL_IDINVENTARIO)->text();
        if (idinventario != "") {
            InventarioView *inv = new InventarioView(companyact, 0);
            companyact->m_pWorkspace->addWindow(inv);
            inv->cargar(idinventario);
            /// Hacemos el borrado sin mostrar pantalla ni nada.
            inv->on_mui_borrar_clicked();
            mui_listado->cargar();
        } // end if
    } // end if
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
InventariosSubForm::InventariosSubForm(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("inventario");
    setDBCampoId("idinventario");
    addSHeader("idinventario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView, tr("Id inventario"));
    addSHeader("nominventario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre del inventario"));
    addSHeader("fechainventario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha del inventario"));
    setinsercion(FALSE);
}

