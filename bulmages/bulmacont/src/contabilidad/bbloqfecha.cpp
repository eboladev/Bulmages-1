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


miQTreeWidgetItem::miQTreeWidgetItem(QTreeWidget *parent)
        : QTreeWidgetItem(parent) {
    return;
}


miQTreeWidgetItem::miQTreeWidgetItem(QTreeWidgetItem *parent)
        : QTreeWidgetItem(parent) {
    return;
}


void BbloqFecha::inicializa() {
    _depura("BbloqFecha::inicializa", 0);
    miQTreeWidgetItem *itemlevel0;
    miQTreeWidgetItem *itemlevel1 = 0;

    QString consultabd;
    mui_treeWidget->setColumnCount(2);
    QStringList cabecera;
    cabecera << tr("Ejercicio") << tr("Estado");
    mui_treeWidget->setColumnWidth(0, 200);
    mui_treeWidget->setHeaderLabels(cabecera);

    mui_treeWidget->clear();
    mui_treeWidget->setSortingEnabled(FALSE);

    /// Consultamos a la base de datos.
    consultabd.sprintf("SELECT * FROM ejercicios WHERE periodo = 0 ORDER BY ejercicio DESC");
    cursor2 *curPeri, *curEjer = empresaactual->cargacursor(consultabd);

    while (!curEjer->eof()) {

        itemlevel0 = new miQTreeWidgetItem(mui_treeWidget);
        itemlevel0->setText(0, curEjer->valor("ejercicio")); /// Columna 0.

        if (curEjer->valor("bloqueado") == "t") {
            itemlevel0->setText(1, qsbloqueado);
        } else {
            itemlevel0->setText(1, qsabierto);
        } // end if

        itemlevel0->ej = curEjer->valor("ejercicio");
        itemlevel0->per = curEjer->valor("periodo");


        consultabd.sprintf("SELECT * FROM ejercicios WHERE ejercicio = '%s' ORDER BY periodo DESC", curEjer->valor("ejercicio").toAscii().constData());
        curPeri = empresaactual->cargacursor(consultabd);
        while (!curPeri->eof()) {
            switch (curPeri->valor("periodo").toInt()) {
            case 12:
                itemlevel1 = new miQTreeWidgetItem(itemlevel0);
                itemlevel1->setText(0, tr("Diciembre")); /// Columna 0.
                break;
            case 11:
                itemlevel1 = new miQTreeWidgetItem(itemlevel0);
                itemlevel1->setText(0, tr("Noviembre")); /// Columna 0.
                break;
            case 10:
                itemlevel1 = new miQTreeWidgetItem(itemlevel0);
                itemlevel1->setText(0, tr("Octubre")); /// Columna 0.
                break;
            case 9:
                itemlevel1 = new miQTreeWidgetItem(itemlevel0);
                itemlevel1->setText(0, tr("Septiembre")); /// Columna 0.
                break;
            case 8:
                itemlevel1 = new miQTreeWidgetItem(itemlevel0);
                itemlevel1->setText(0, tr("Agosto")); /// Columna 0.
                break;
            case 7:
                itemlevel1 = new miQTreeWidgetItem(itemlevel0);
                itemlevel1->setText(0, tr("Julio")); /// Columna 0.
                break;
            case 6:
                itemlevel1 = new miQTreeWidgetItem(itemlevel0);
                itemlevel1->setText(0, tr("Junio")); /// Columna 0.
                break;
            case 5:
                itemlevel1 = new miQTreeWidgetItem(itemlevel0);
                itemlevel1->setText(0, tr("Mayo")); /// Columna 0.
                break;
            case 4:
                itemlevel1 = new miQTreeWidgetItem(itemlevel0);
                itemlevel1->setText(0, tr("Abril")); /// Columna 0.
                break;
            case 3:
                itemlevel1 = new miQTreeWidgetItem(itemlevel0);
                itemlevel1->setText(0, tr("Marzo")); /// Columna 0.
                break;
            case 2:
                itemlevel1 = new miQTreeWidgetItem(itemlevel0);
                itemlevel1->setText(0, tr("Febrero")); /// Columna 0.
                break;
            case 1:
                itemlevel1 = new miQTreeWidgetItem(itemlevel0);
                itemlevel1->setText(0, tr("Enero")); /// Columna 0.
                break;
            } // end switch
            itemlevel1->ej = curEjer->valor("ejercicio");
            itemlevel1->per = curPeri->valor("periodo");
            curPeri->valor("bloqueado") == "t" ? itemlevel1->setText(1, qsbloqueado) : itemlevel1->setText(1, qsabierto);
            curPeri->siguienteregistro();
        } // end while
        curEjer->siguienteregistro();
    } // end while
    _depura("ENd BbloqFecha::inicializa", 0);
}


BbloqFecha::BbloqFecha(empresa *emp, QWidget *parent)
        : QWidget(parent) {
    _depura("BbloqFecha::BbloqFecha", 0);
    qsbloqueado = tr("Bloqueado");
    qsabierto = tr ("Abierto");
    setAttribute(Qt::WA_DeleteOnClose);
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


void BbloqFecha::on_mui_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int columna) {
    _depura("BbloqFecha::on_mui_treeWidget_doubleClicked", 0);
    int error;
    miQTreeWidgetItem *it = (miQTreeWidgetItem *) item;
    if (columna == 1) {
        if (item->text(1) == qsbloqueado) {
            item->setText(1, qsabierto);
            QString consultabd = "UPDATE ejercicios SET bloqueado = FALSE WHERE ejercicio = '" + it->ej + "' AND periodo = '" + it->per + "'";
            error = empresaactual->ejecuta(consultabd);
        } else {
            item->setText(1, qsbloqueado);
            QString consultabd = "UPDATE ejercicios SET bloqueado = TRUE WHERE ejercicio = '" + it->ej + "' AND periodo = '" + it->per + "'";
            error = empresaactual->ejecuta(consultabd);
        } // end if
    } // end if
    _depura("END BbloqFecha::on_mui_treeWidget_doubleClicked", 0);
}


void BbloqFecha::on_mui_crear_clicked() {
    _depura("BbloqFecha::on_mui_crear_clicked", 0);
    int ejer = 2004;

    QString consultabd = "SELECT max(ejercicio) AS ej FROM ejercicios";
    cursor2 *cur = empresaactual->cargacursor(consultabd);
    if (!cur->eof()) {
        ejer = cur->valor("ej").toInt();
    } // end if

    ejer++;

    for (int x = 0; x <= 12; x++) {
        QString consultabd = "INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES('" + QString::number(ejer) + "', '" + QString::number(x) + "', 'f')";
        empresaactual->ejecuta(consultabd);
    } // end for

    inicializa();

    _depura("BbloqFecha::on_mui_crear_clicked", 0);
}

