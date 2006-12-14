/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include <QLineEdit>
#include <Q3ListViewItem>

#include "mpatrimonialview.h"
#include "mpatrimonialesview.h"


mpatrimonialview::mpatrimonialview(QWidget *parent)
        : QDialog(parent) {
    setupUi(this);
    conexionbase = NULL;
    idmpatrimonial = "";

    componentessuma->setColumnCount(4);
    componentesresta->setColumnCount(4);

    QStringList etiquetas;
    etiquetas << tr("identificador") << tr("codigo") << tr("descripcion") << tr("tipo");
    componentessuma->setHorizontalHeaderLabels(etiquetas);
    componentesresta->setHorizontalHeaderLabels(etiquetas);
}


mpatrimonialview::~mpatrimonialview() {}


/// Esta funci&oacute;n se encarga de hacer las inicializaciones de todo el formulario.
/** Se llama as&iacute; y no desde el constructor porque as&iacute; la podemos llamar
    desde dentro de la misma clase, etc, etc, etc. */
int mpatrimonialview::inicializa(postgresiface2 *conn) {
    conexionbase = conn;
    return(0);
}


void mpatrimonialview::inicializa1(QString idmpatrimonial1) {
    QTableWidgetItem *it0, *it1, *it2, *it3;
    int i = 0;
    idmpatrimonial = idmpatrimonial1;
    QString query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial = " + idmpatrimonial;
    conexionbase->begin();
    cursor2 *cursor = conexionbase->cargacursor(query, "micursor");
    conexionbase->commit();
    if (!cursor->eof()) {
        descmpatrimonial->setText(cursor->valor("descmpatrimonial"));
    } // end if
    delete cursor;

    /// Leemos las cuentas y las metemos en la lista que corresponda.
    conexionbase->begin();
    query = "SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM compmasap WHERE masaperteneciente = " + idmpatrimonial + " AND signo = true)";
    cursor2 *cursoraux = conexionbase->cargacursor(query, "cursorusuario");
    conexionbase->commit();

    while (!cursoraux->eof()) {
        /// Anyade una nueva fila a la tabla.
        componentessuma->insertRow(i);
        it1 = new QTableWidgetItem(cursoraux->valor("codigo"));
        componentessuma->setItem(i, 1, it1);
        it2 = new QTableWidgetItem(cursoraux->valor("descripcion"));
        componentessuma->setItem(i, 2, it2);
        it3 = new QTableWidgetItem("cuenta");
        componentessuma->setItem(i, 3, it3);
        it0 = new QTableWidgetItem(cursoraux->valor("idcuenta"));
        componentessuma->setItem(i, 0, it0);
        cursoraux->siguienteregistro();
        i++;
    } // end while
    delete cursoraux;

    /// Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    conexionbase->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial IN (SELECT idmpatrimonial FROM compmasap WHERE masaperteneciente = " + idmpatrimonial + " AND signo = true)";
    cursoraux = conexionbase->cargacursor(query, "cursorusuario");
    conexionbase->commit();
    i = 0;
    while (!cursoraux->eof()) {
        componentessuma->insertRow(i);
        it1 = new QTableWidgetItem("");
        componentessuma->setItem(i, 1, it1);
        it2 = new QTableWidgetItem(cursoraux->valor("descmpatrimonial"));
        componentessuma->setItem(i, 2, it2);
        it3 = new QTableWidgetItem("masa patrimonial");
        componentessuma->setItem(i, 3, it3);
        it0 = new QTableWidgetItem(cursoraux->valor("idmpatrimonial"));
        componentessuma->setItem(i, 0, it0);
        i++;
        cursoraux->siguienteregistro();
    } // end while
    delete cursoraux;

    /// Leemos las cuentas y las metemos en la lista que corresponda.
    conexionbase->begin();
    query = "SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM compmasap WHERE masaperteneciente = " + idmpatrimonial + " AND signo = false)";
    cursor2 *cursoraux1 = conexionbase->cargacursor(query, "cursorusuario");
    conexionbase->commit();
    i = 0;
    while (!cursoraux1->eof()) {
        componentesresta->insertRow(i);
        it1 = new QTableWidgetItem(cursoraux1->valor("codigo"));
        componentesresta->setItem(i, 1, it1);
        it2 = new QTableWidgetItem(cursoraux1->valor("descripcion"));
        componentesresta->setItem(i, 2, it2);
        it3 = new QTableWidgetItem("cuenta");
        componentesresta->setItem(i, 3, it3);
        it0 = new QTableWidgetItem(cursoraux1->valor("idcuenta"));
        componentesresta->setItem(i, 0, it0);
        i++;
        cursoraux1->siguienteregistro();
    } // end while
    delete cursoraux1;

    /// Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    conexionbase->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial IN (SELECT idmpatrimonial FROM compmasap WHERE masaperteneciente = " + idmpatrimonial + " AND signo = false)";
    cursoraux = conexionbase->cargacursor(query, "cursorusuario");
    conexionbase->commit();
    i = 0;
    while (!cursoraux->eof()) {
        componentesresta->insertRow(i);
        it1 = new QTableWidgetItem("");
        componentesresta->setItem(i, 1, it1);
        it2 = new QTableWidgetItem(cursoraux->valor("descmpatrimonial"));
        componentesresta->setItem(i, 2, it2);
        it3 = new QTableWidgetItem("masa patrimonial");
        componentesresta->setItem(i, 3, it3);
        it0 = new QTableWidgetItem(cursoraux->valor("idmpatrimonial"));
        componentesresta->setItem(i, 0, it0);
        i++;
        cursoraux->siguienteregistro();
    } // end while
    delete cursoraux;
}


/// Se ha pulsado sobre el bot&oacute; de agregar donde est&aacute; la suma.
/// Tenemos que a&ntilde;dir la masa patrimonial o la cuenta a la suma.
void mpatrimonialview::on_mui_nuevasuma_clicked() {
    QTableWidgetItem *it0, *it1, *it2, *it3;
    int i;

    /// Lo primero de todo es coger el c&oacute;igo de cuenta.
    QString codcuenta = codigocta1->text();
    QString masapatrimonial = mpatrimonial->text();

    /// Leemos las cuentas y las metemos en la lista que corresponda.
    conexionbase->begin();
    QString query = "SELECT * FROM cuenta WHERE codigo = '" + codcuenta + "'";
    cursor2 *cursoraux1 = conexionbase->cargacursor(query, "cursorusuario");
    conexionbase->commit();
    i = componentessuma->rowCount();
    while (!cursoraux1->eof()) {
        componentessuma->insertRow(i);
        it1 = new QTableWidgetItem(cursoraux1->valor("codigo"));
        componentessuma->setItem(i, 1, it1);
        it2 = new QTableWidgetItem(cursoraux1->valor("descripcion"));
        componentessuma->setItem(i, 2, it2);
        it3 = new QTableWidgetItem("cuenta");
        componentessuma->setItem(i, 3, it3);
        it0 = new QTableWidgetItem(cursoraux1->valor("idcuenta"));
        componentessuma->setItem(i, 0, it0);
        i++;
        cursoraux1->siguienteregistro();
    } // end while
    delete cursoraux1;

    /// Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    conexionbase->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial = " + masapatrimonial;
    cursoraux1 = conexionbase->cargacursor(query, "cursormpatrimonial");
    conexionbase->commit();
    i = componentessuma->rowCount();
    while (!cursoraux1->eof()) {
        componentessuma->insertRow(i);
        it1 = new QTableWidgetItem("");
        componentessuma->setItem(i, 1, it1);
        it2 = new QTableWidgetItem(cursoraux1->valor("descmpatrimonial"));
        componentessuma->setItem(i, 2, it2);
        it3 = new QTableWidgetItem("masa patrimonial");
        componentessuma->setItem(i, 3, it3);
        it0 = new QTableWidgetItem(cursoraux1->valor("idmpatrimonial"));
        componentessuma->setItem(i, 0, it0);
        i++;
        cursoraux1->siguienteregistro();
    } // end while
}


void mpatrimonialview::on_mui_borrasuma_clicked() {
    componentessuma->removeRow(componentessuma->currentRow());
}


/// Se ha pulsado sobre el bot&oacute; de agregar donde est&aacute; la suma.
/// Tenemos que a&ntilde;dir la masa patrimonial o la cuenta a la suma.
void mpatrimonialview::on_mui_nuevaresta_clicked() {
    //Q3ListViewItem *it;
    QTableWidgetItem *item;

    /// Lo primero de todo es coger el c&oacute;igo de cuenta.
    QString codcuenta = codigocta1->text();
    QString masapatrimonial = mpatrimonial->text();

    /// Leemos las cuentas y las metemos en la lista que corresponda.
    conexionbase->begin();
    QString query = "SELECT * FROM cuenta WHERE codigo = '" + codcuenta + "'";
    cursor2 *cursoraux1 = conexionbase->cargacursor(query, "cursorusuario");
    conexionbase->commit();
    while (!cursoraux1->eof()) {
        //it = new Q3ListViewItem(componentesresta);
        //it->setText(1, cursoraux1->valor("codigo"));
        //it->setText(2, cursoraux1->valor("descripcion"));
        //it->setText(3, "cuenta");
        //it->setText(0, cursoraux1->valor("idcuenta"));
        cursoraux1->siguienteregistro();
    } // end while
    delete cursoraux1;

    /// Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    conexionbase->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial = " + masapatrimonial;
    cursoraux1 = conexionbase->cargacursor(query, "cursormpatrimonial");
    conexionbase->commit();
    while (!cursoraux1->eof()) {
        //it = new Q3ListViewItem(componentesresta);
        //it->setText(1, "");
        //it->setText(2, cursoraux1->valor("descmpatrimonial"));
        //it->setText(3, "masa patrimonial");
        //it->setText(0, cursoraux1->valor("idmpatrimonial"));
        cursoraux1->siguienteregistro();
    } // end while
}


void mpatrimonialview::on_mui_borraresta_clicked() {
    //Q3ListViewItem *it;
    QTableWidgetItem *item;

    //    it = componentesresta->currentItem();
    //    componentesresta->takeItem(it);
}


void mpatrimonialview::on_mui_buscampatrimonial_clicked() {
    mpatrimonialesview *nuevae = new mpatrimonialesview(0);
    nuevae->inicializa(conexionbase);
    nuevae->setmodoselector();
    nuevae->exec();
    mpatrimonial->setText(nuevae->getidmasa());
    delete nuevae;
}


void mpatrimonialview::on_mui_cancelar_clicked() {
    close();
}


void mpatrimonialview::on_mui_aceptar_clicked() {
    QTableWidgetItem *item;
    int i;
    QString query;

    /// Si idmpatrimonial == NULL implica que es una inserci&oacute;n y no una modificaci&oacute;n
    if (idmpatrimonial == "") {
        conexionbase->begin();
        query.sprintf("INSERT INTO mpatrimonial (descmpatrimonial) VALUES ('nueva masa')");
        conexionbase->ejecuta(query.toAscii());
        query.sprintf("SELECT MAX(idmpatrimonial) as id FROM mpatrimonial");
        cursor2 *curs = conexionbase->cargacursor(query, "cargaid");
        conexionbase->commit();
        idmpatrimonial = curs->valor("id").toAscii();
    } // end if

    /// Ponemos los datos correctos sobre la masa patrimonial.
    QString text = descmpatrimonial->text();
    query.sprintf("UPDATE mpatrimonial SET descmpatrimonial = '%s' WHERE idmpatrimonial = %s", text.toAscii().constData(), idmpatrimonial.toAscii().constData());
    conexionbase->ejecuta(query);

    query.sprintf("DELETE FROM compmasap WHERE masaperteneciente = %s", idmpatrimonial.toAscii().constData());
    conexionbase->ejecuta(query);

    for (i = 0; i < componentessuma->rowCount(); i++) {
        QString id = componentessuma->item(i, 0)->text();
        QString tipo = componentessuma->item(i, 3)->text();

        if (tipo == "cuenta") {
            query.sprintf("INSERT INTO compmasap(idcuenta, idmpatrimonial, masaperteneciente, signo) VALUES (%s, NULL, %s, true)",
                          conexionbase->sanearCadena(id).toAscii().constData(),
                          conexionbase->sanearCadena(idmpatrimonial).toAscii().constData());
        } else {
            query.sprintf("INSERT INTO compmasap(idcuenta, idmpatrimonial, masaperteneciente, signo) VALUES (NULL, %s, %s, true)",
                          conexionbase->sanearCadena(id).toAscii().constData(),
                          conexionbase->sanearCadena(idmpatrimonial).toAscii().constData());
        } // end if
        conexionbase->ejecuta(query);
    } // end for

    for (i = 0; i < componentesresta->rowCount(); i++) {
        QString id = componentesresta->item(i, 0)->text();
        QString tipo = componentesresta->item(i, 3)->text();

        if (tipo == "cuenta") {
            query.sprintf("INSERT INTO compmasap(idcuenta, idmpatrimonial, masaperteneciente, signo) VALUES (%s, NULL, %s, false)",
                          conexionbase->sanearCadena(id).toAscii().constData(),
                          conexionbase->sanearCadena(idmpatrimonial).toAscii().constData());
        } else {
            query.sprintf("INSERT INTO compmasap(idcuenta, idmpatrimonial, masaperteneciente, signo) VALUES (NULL, %s, %s, false)",
                          conexionbase->sanearCadena(id).toAscii().constData(),
                          conexionbase->sanearCadena(idmpatrimonial).toAscii().constData());
        } // end if
        conexionbase->ejecuta(query);
    } // end for
    close();
}


QString mpatrimonialview::getnommasa() {
    return descmpatrimonial->text().toAscii();
}

