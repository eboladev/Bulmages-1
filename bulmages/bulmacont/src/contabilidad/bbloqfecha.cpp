/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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

#include "bbloqfecha.h"
#include "empresa.h"


myQListViewItem::myQListViewItem(myQListViewItem *parent, QString label1)
        : Q3ListViewItem(parent, label1) {
    return;
};


myQListViewItem::myQListViewItem(Q3ListView *parent, QString label1)
        : Q3ListViewItem(parent, label1) {
    return;
};


void BbloqFecha::inicializa() {
    _depura("BbloqFecha::inicializa", 0);
    myQListViewItem *listMain, *listAux = 0;
    QString query;
    listView1->setSorting(-1);

    listView1->clear();

    query.sprintf("SELECT * FROM ejercicios WHERE periodo = 0 ORDER BY ejercicio DESC");
    cursor2 *curPeri,*curEjer = empresaactual->cargacursor(query);
    while (!curEjer->eof()) {
        listMain = new myQListViewItem(listView1, curEjer->valor("ejercicio"));
        listMain->ej = curEjer->valor("ejercicio");
        listMain->per = "0";
        if (curEjer->valor("bloqueado") == "t") {
            listMain->setText(1, "Bloqueado");
        } else {
            listMain->setText(1, "Abierto");
        } // end if

        query.sprintf("SELECT * FROM ejercicios WHERE ejercicio = '%s' ORDER BY periodo DESC", curEjer->valor("ejercicio").toAscii().constData());
        curPeri = empresaactual->cargacursor(query);
        while (!curPeri->eof()) {
            switch (curPeri->valor("periodo").toInt()) {
            case 12:
                listAux = new myQListViewItem(listMain, tr("Diciembre "));
                break;
            case 11:
                listAux = new myQListViewItem(listMain, tr("Noviembre "));
                break;
            case 10:
                listAux = new myQListViewItem(listMain, tr("Octubre"   ));
                break;
            case  9:
                listAux = new myQListViewItem(listMain, tr("Septiembre"));
                break;
            case  8:
                listAux = new myQListViewItem(listMain, tr("Agosto    "));
                break;
            case  7:
                listAux = new myQListViewItem(listMain, tr("Julio     "));
                break;
            case  6:
                listAux = new myQListViewItem(listMain, tr("Junio     "));
                break;
            case  5:
                listAux = new myQListViewItem(listMain, tr("Mayo      "));
                break;
            case  4:
                listAux = new myQListViewItem(listMain, tr("Abril     "));
                break;
            case  3:
                listAux = new myQListViewItem(listMain, tr("Marzo     "));
                break;
            case  2:
                listAux = new myQListViewItem(listMain, tr("Febrero   "));
                break;
            case  1:
                listAux = new myQListViewItem(listMain, tr("Enero     "));
                break;
            }
            listAux->ej = curEjer->valor("ejercicio");
            listAux->per = curPeri->valor("periodo");
            curPeri->valor("bloqueado") == "t" ? listAux->setText(1, "Bloqueado") : listAux->setText(1, "Abierto");
            curPeri->siguienteregistro();
        }
        curEjer->siguienteregistro();
    }
    _depura("ENd BbloqFecha::inicializa", 0);

}


BbloqFecha::BbloqFecha(empresa *emp, QWidget *parent)
        : QWidget(parent, Qt::WDestructiveClose) {
    _depura("BbloqFecha::BbloqFecha", 0);
    setupUi(this);
    QString query;
    empresaactual = emp;
    inicializa();
    empresaactual->meteWindow(windowTitle(), this);
    _depura("ENd BbloqFecha::BbloqFecha", 0);
}


BbloqFecha::~BbloqFecha() {
    _depura("BbloqFecha::~BbloqFecha", 0);
    empresaactual->sacaWindow(this);
    _depura("ENd BbloqFecha::~BbloqFecha", 0);
}


void BbloqFecha::boto1_click() {
    delete this;
}


void BbloqFecha::on_listView1_doubleClicked (Q3ListViewItem *item) {
    _depura("BbloqFecha::on_listView1_itemDoubleClicked", 0);
    int error;
    myQListViewItem *it = (myQListViewItem *) item;
    if (item->text(1) == "Bloqueado") {
        item->setText(1, "Abierto");
        QString Query = "UPDATE ejercicios SET bloqueado = FALSE WHERE ejercicio = " + it->ej + " AND periodo = " + it->per;
        error = empresaactual->ejecuta(Query);
    } else {
        item->setText(1, "Bloqueado");
        QString Query = "UPDATE ejercicios SET bloqueado = TRUE WHERE ejercicio = " + it->ej + " AND periodo = " + it->per;
        error = empresaactual->ejecuta(Query);
    } // end if
    _depura("END BbloqFecha::on_listView1_itemDoubleClicked", 0);
}


void BbloqFecha::on_mui_crear_clicked() {
    _depura("BbloqFecha::on_mui_crear_clicked", 0);
    int ejer = 2004;

    QString query = "SELECT max(ejercicio) AS ej FROM ejercicios";
    cursor2 *cur = empresaactual->cargacursor( query );
    if (!cur->eof()) {
        ejer = cur->valor( "ej").toInt();
    }

    ejer++;

    for (int x=0; x<=12; x++) {
        QString query ="INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES('"+QString::number(ejer)+"', '"+QString::number(x)+"', 'f')";
        empresaactual->ejecuta(query);
    }// end for

    inicializa();

    _depura("BbloqFecha::on_mui_crear_clicked", 0);

}

