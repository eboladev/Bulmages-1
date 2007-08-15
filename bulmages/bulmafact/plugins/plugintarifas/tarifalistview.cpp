/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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

#include "tarifalistview.h"
#include "tarifaview.h"
#include "pgimportfiles.h"
#include "company.h"
#include "funcaux.h"


TarifaListView::TarifaListView(Company *comp, QWidget *parent, edmode editmodo)
        : FichaBf(comp, parent), pgimportfiles(comp) {
    _depura("TarifaListView::INIT_TarifaListView()\n", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    m_modo = editmodo;
    setupUi(this);
    mui_list->setEmpresaBase(empresaBase());
    mui_list->cargar();
    meteWindow(tr("Tarifas"), this);
    _depura("TarifaListView::END_TarifaListView()\n", 0);
}


TarifaListView::~TarifaListView() {
    _depura("TarifaListView::INIT_destructor()\n", 0);
    empresaBase()->sacaWindow(this);
    _depura("TarifaListView::END_destructor()\n", 0);
}


void TarifaListView::on_mui_editar_clicked() {
    _depura("TarifaListView::INIT_s_editArticle()\n", 0);
    int a = mui_list->currentRow();
    if (a < 0) {
        _depura("Debe seleccionar una linea", 2);
        return;
    } // end if
    editar(a);
    _depura("TarifaListView::END_s_editArticle()\n", 0);
}


void TarifaListView::editar(int row) {
    _depura("TarifaListView::editar", 0);
    TarifaView *tar = new TarifaView(empresaBase(), 0);
    empresaBase()->m_pWorkspace->addWindow(tar);
    tar->cargar(mui_list->DBvalue(QString("idtarifa"), row));
    tar->show();
}


void TarifaListView::on_mui_crear_clicked() {
    _depura("TarifaListView::editar", 0);
    TarifaView *tar = new TarifaView(empresaBase(), parentWidget());
    /// Hacemos una carga "especial" para que se carguen todos los elementos.
    tar->cargar("0");
    empresaBase()->m_pWorkspace->addWindow(tar);
    tar->show();
}


void TarifaListView::on_mui_borrar_clicked() {
    _depura("TarifaListView::on_mui_borrar_clicked\n", 0);
    int a = mui_list->currentRow();
    TarifaView *tar = new TarifaView(empresaBase(), 0);
    tar->cargar(mui_list->DBvalue(QString("idtarifa"), a));
    tar->on_mui_borrar_clicked();
    delete tar;
    _depura("END TarifaListView::on_mui_borrar_clicked\n", 0);
}

