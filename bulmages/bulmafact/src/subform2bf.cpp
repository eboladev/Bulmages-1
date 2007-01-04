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
#include <QMenu>
#include <QKeyEvent>
#include <QEvent>

#include "subform2bf.h"
#include "funcaux.h"
#include "articulolist.h"


SubForm2Bf::SubForm2Bf(QWidget *parent) : SubForm3(parent) {
    _depura("SubForm2Bf::SubForm2Bf", 0);
    setDelete(TRUE);
    _depura("END SubForm2Bf::SubForm2Bf", 0);
}


int SubForm2Bf::addSHeader(QString nom, DBCampo::dbtype typ, int res, int opt, QString nomp) {
    _depura("SubForm2Bf::addSHeader", 0);
    SubForm3::addSHeader(nom, typ, res, opt, nomp);
    _depura("END SubForm2Bf::addSHeader", 0);
}


void SubForm2Bf::pintaCabeceras() {
    _depura("SubForm2Bf::pintaCabeceras", 0);
    SubForm3::pintaCabeceras();
    SHeader *linea;
    for (int i = 0; i < m_lcabecera.size(); ++i) {
        linea = m_lcabecera.at(i);
//        if (linea->nomcampo() == "descl"+tableName()) {
//            mui_list->setItemDelegate(new QTableItemTextDelegate(this));
            mui_list->setItemDelegateForColumn(i, new QTableItemTextDelegate(this));
//        } // end if
    } // end for
    _depura("END SubForm2Bf::pintaCabeceras", 0);
}

void SubForm2Bf::cargar(QString query) {
    _depura("SubForm2Bf::cargar", 0);
    SubForm3::cargar(query);
    _depura("END SubForm2Bf::cargar", 0);
}


void SubForm2Bf::on_mui_list_pressedAsterisk(int row, int col) {
    _depura("SubForm2Bf::pressedAsterisk", 0);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row, col);

    if (camp->nomcampo() != "codigocompletoarticulo")
        return;
    _depura("ListCompArticuloView::searchArticle", 0);
    ArticuloList *artlist = new ArticuloList((company *) companyact(), NULL, 0, ArticuloList::SelectMode);
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    this->setEnabled(false);
    artlist->show();
    while (!artlist->isHidden())
        theApp->processEvents();
    this->setEnabled(true);
    QString idArticle = artlist->idArticle();
    delete artlist;
    cursor2 *cur = companyact()->cargacursor("SELECT * FROM articulo WHERE idarticulo = " + idArticle);
    if (!cur->eof()) {
        rec->setDBvalue("idarticulo", idArticle);
        rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
        rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
        /// Invocamos la finalizacion de edicion para que todos los campos se actualicen.
        on_mui_list_editFinished(row, col, Qt::Key_Return);
    } // end if
    delete cur;
    _depura("END SubForm2Bf::pressedAsterisk", 0);
}


void SubForm2Bf::on_mui_list_pressedSlash(int row, int col) {
    _depura("SubForm2Bf::pressedSlash", 0);
    SDBCampo *camp = (SDBCampo *) item(row, col);
    QString text = editaTexto(camp->text());
    camp->set
    (text);
    _depura("END SubForm2Bf::pressedSlash", 0);
}


void SubForm2Bf::on_mui_list_pressedMinus(int row, int col) {
    _depura("SubForm2Bf::pressedMinus", 0);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row, col);
    cursor2 *cur = companyact()->cargacursor("SELECT * FROM articulo WHERE idarticulo = " + rec->DBvalue("idarticulo"));
    if (!cur->eof()) {
        rec->setDBvalue(camp->nomcampo(), cur->valor("obserarticulo"));
        /// Invocamos la finalizacion de edicion para que todos los campos se actualicen.
        on_mui_list_editFinished(row, col, Qt::Key_Return);
    } // end if
    delete cur;
    _depura("END SubForm2Bf::pressedMinus", 0);
}


void SubForm2Bf::on_mui_list_editFinished(int row, int col, int key) {
    _depura("SubForm2Bf::editFinished", 0);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row, col);
    camp->refresh();
    if (camp->nomcampo() == "codigocompletoarticulo") {
        cursor2 *cur = companyact()->cargacursor("SELECT * FROM articulo WHERE codigocompletoarticulo = '" + camp->text() + "'");
        if (!cur->eof()) {
            rec->setDBvalue("idarticulo", cur->valor("idarticulo"));
            rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
            rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
        }
        delete cur;
    }
    SubForm3::on_mui_list_editFinished(row, col, key);
    _depura("END SubForm2Bf::editFinished", 0);
}


void SubForm2Bf::contextMenuEvent(QContextMenuEvent *) {
    _depura("SubForm2Bf::contextMenuEvent", 0);
    QAction *del = NULL;
    int row = currentRow();
    if (row < 0)
        return;

    int col = currentColumn();
    if (row < 0)
        return;

    QMenu *popup = new QMenu(this);
    if (m_delete)
        del = popup->addAction(tr("Borrar registro"));
    popup->addSeparator();
    QAction *ajustc = popup->addAction(tr("Ajustar columna"));
    QAction *ajustac = popup->addAction(tr("Ajustar altura"));

    QAction *ajust = popup->addAction(tr("Ajustar columnas"));
    QAction *ajusta = popup->addAction(tr("Ajustar alturas"));

    popup->addSeparator();
    QAction *verconfig = popup->addAction(tr("Ver/Ocultar configurador de subformulario"));

    QAction *opcion = popup->exec(QCursor::pos());
    if (opcion == del && m_delete)
        borrar(row);
    if (opcion == ajust)
        resizeColumnsToContents();
    if (opcion == ajusta)
        resizeRowsToContents();
    if (opcion == ajustc)
        resizeColumnToContents(col);
    if (opcion == ajustac)
        resizeRowToContents(row);
    if (opcion == verconfig)
        toogleConfig();

    delete popup;
    _depura("END SubForm2Bf::contextMenuEvent", 0);
}

