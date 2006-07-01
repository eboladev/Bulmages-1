/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C. (Porting to QT4)              *
 *   fcojavmc@todo-redes.com                                               *
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

#include "listprovinciasview.h"
#include "company.h"

using namespace std;

#include "funcaux.h"

#define COL_PROVINCIA 0
#define COL_ORIGINALPROVINCIA 1


ListProvinciasView::ListProvinciasView(company *comp, QDialog *parent) : QDialog (parent) {
    _depura("INIT_ListProvinciasView::ListProvinciasView", 1);

    setupUi(this);
    companyact = comp;

    m_listado->setSelectionMode(QAbstractItemView::SingleSelection);
    m_listado->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_listado->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    inicializa();

    _depura("END_ListProvinciasView::ListProvinciasView", 1);
}


ListProvinciasView::~ListProvinciasView() {}


void ListProvinciasView::inicializa() {
    _depura("INIT_ListProvinciasView::inicializa", 1);

    m_listado->clear();
    m_listado->setColumnCount(2);

    QStringList headerlabels;
    headerlabels << tr("Provincia") << tr("Provincia");

    m_listado->setHorizontalHeaderLabels(headerlabels);

    m_listado->setColumnWidth(COL_PROVINCIA, 290);
    m_listado->hideColumn(COL_ORIGINALPROVINCIA);

    QString SQLQuery = "SELECT * FROM provincia";

    cursor2 *cur = companyact->cargacursor(SQLQuery);

    m_listado->setRowCount(cur->numregistros());

    int i = 0;
    while (!cur->eof()) {
        QTableWidgetItem *newItem1 = new QTableWidgetItem(cur->valor("provincia"), 0);
        QTableWidgetItem *newItem2 = new QTableWidgetItem(cur->valor("provincia"), 0);
        m_listado->setItem(i, COL_PROVINCIA, newItem1);
        m_listado->setItem(i, COL_ORIGINALPROVINCIA, newItem2);
        i++;
        cur->siguienteregistro();
    } // end while

    _depura("END_ListProvinciasView::inicializa", 1);
}


void ListProvinciasView::on_botonnew_clicked() {
    _depura("INIT_ListProvinciasView::on_botonnew_clicked", 1);

    QString SQLQuery = "INSERT INTO provincia (provincia) VALUES ('--')";

    int error = companyact->ejecuta(SQLQuery);
    if (error) {
        return;
    } // end if

    inicializa();

    _depura("END_ListProvinciasView::on_botonnew_clicked", 1);
}


void ListProvinciasView::on_botonsave_clicked() {
    _depura("INIT_ListProvinciasView::on_botonsave_clicked", 1);

    companyact->begin();

    int i = 0;
    while (i < m_listado->rowCount()) {
        int error = guardalinea(i);
        if (error) {
            companyact->rollback();
            return;
        } // end if
        i++;
    } // end while

    companyact->commit();
    inicializa();

    _depura("END_ListProvinciasView::on_botonsave_clicked", 1);
}


void ListProvinciasView::on_botondelete_clicked() {
    _depura("INIT_ListProvinciasView::on_botondelete_clicked", 1);

    QTableWidgetItem *newItem = new QTableWidgetItem("", 0);

    int row = m_listado->currentRow();

    if (row < 0)
        return;

    newItem = m_listado->item(row, COL_ORIGINALPROVINCIA);
    QString codigooriginal = newItem->text();
    QString SQLQuery = "DELETE FROM provincia WHERE provincia='" + codigooriginal + "'";

    int error = companyact->ejecuta(SQLQuery);
    if (error)
        return;

    inicializa();

    _depura("END_ListProvinciasView::on_botondelete_clicked", 1);
}


int ListProvinciasView::guardalinea(int row) {
    _depura("INIT_ListProvinciasView::guardalinea", 1);

    QTableWidgetItem *newItem1 = new QTableWidgetItem("", 0);
    QTableWidgetItem *newItem2 = new QTableWidgetItem("", 0);
    newItem1 = m_listado->item(row, COL_ORIGINALPROVINCIA);
    newItem2 = m_listado->item(row, COL_PROVINCIA);

    QString codigooriginal = newItem1->text();
    QString codigo = newItem2->text();

    QString SQLQuery = "UPDATE provincia SET provincia='" + codigo + "' WHERE provincia= '" + codigooriginal + "'";

    int error = companyact->ejecuta(SQLQuery);
    if (error)
        return 1;

    _depura("END_ListProvinciasView::guardalinea", 1);
    return 0;
}

