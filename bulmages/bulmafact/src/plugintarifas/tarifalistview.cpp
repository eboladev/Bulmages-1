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

#include <QWidget>
#include <QMessageBox>
#include <QFile>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QTextStream>
#include <QMenu>
#include <Q3FileDialog>


#include "tarifalistview.h"
#include "tarifaview.h"
#include "pgimportfiles.h"
#include "company.h"
#include "qtable1.h"
#include "funcaux.h"



TarifaListView::TarifaListView(company *comp, QWidget *parent, const char *name, Qt::WFlags flag, edmode editmodo)
        : QWidget(parent, name, Qt::WDestructiveClose)  , pgimportfiles(comp) {
    _depura("TarifaListView::INIT_TarifaListView()\n",0);
    setupUi(this);
    m_companyact = comp;
    mui_list->setcompany(m_companyact);
    mui_list->cargar();
    _depura("TarifaListView::END_TarifaListView()\n",0);
}// end TarifaListView


TarifaListView::~TarifaListView() {
    _depura("TarifaListView::INIT_destructor()\n",0);
    _depura("TarifaListView::END_destructor()\n",0);
}// end ~TarifaListView


void TarifaListView::on_mui_editar_clicked() {
    _depura("TarifaListView::INIT_s_editArticle()\n",0);
    int a = mui_list->currentRow();
    if (a < 0) {
        _depura("Debe seleccionar una linea",2);
        return;
    }// end if
    editar(a);
    _depura("TarifaListView::END_s_editArticle()\n",0);
}

void TarifaListView::editar(int  row) {
    _depura("TarifaListView::editar",0);
    TarifaView *tar = new TarifaView(m_companyact, 0);
    m_companyact->m_pWorkspace->addWindow(tar);
    tar->cargar(mui_list->DBvalue(QString("idtarifa"),row));
    tar->show();
}


void TarifaListView::on_mui_crear_clicked() {
    _depura("TarifaListView::editar",0);
    TarifaView *tar = new TarifaView(m_companyact, parentWidget());
    /// Hacemos una carga "especial" para que se carguen todos los elementos.
    tar->cargar("0");
    m_companyact->m_pWorkspace->addWindow(tar);
    tar->show();
}



void TarifaListView::on_mui_borrar_clicked() {
    _depura("TarifaListView::on_mui_borrar_clicked\n",0);
    int a = mui_list->currentRow();
    TarifaView *tar = new TarifaView(m_companyact, 0);
    tar->cargar(mui_list->DBvalue(QString("idtarifa"),a));
    tar->on_mui_borrar_clicked();
    delete tar;
    _depura("END TarifaListView::on_mui_borrar_clicked\n",0);
}


/*
 
void TarifaListView::on_mui_list_cellDoubleClicked(int a, int ) {
 
    mdb_idarticulo = mui_list->item(a,COL_IDARTICULO)->text();
    if (m_modo ==0 && mdb_idarticulo != "") {
        ArticuloView *bud = new ArticuloView(m_companyact,0,theApp->translate("Edicion de Pagos", "company"));
        bud->cargar(mdb_idarticulo);
	m_companyact->m_pWorkspace->addWindow(bud);
        bud->show();
    } else {
        close();
    }// end if
 
}
 
void TarifaListView::on_mui_list_customContextMenuRequested(const QPoint &) {
 
    _depura("TarifaListView::on_mui_list_customContextMenuRequested",0);
    int a = mui_list->currentRow();
    if ( a < 0)
        return;
    QMenu *popup = new QMenu(this);
    QAction *edit = popup->addAction(tr("Editar Articulo"));
    QAction *del = popup->addAction(tr("Borrar Articulo"));
    QAction *opcion = popup->exec(QCursor::pos());
    if (opcion == del)
        on_mui_borrar_clicked();
    if (opcion == edit)
	on_mui_editar_clicked();
    delete popup;
 
}
 
*/

