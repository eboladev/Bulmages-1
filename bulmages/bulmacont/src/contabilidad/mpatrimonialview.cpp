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
#include <q3listview.h>

#include "mpatrimonialview.h"
#include "mpatrimonialesview.h"


mpatrimonialview::mpatrimonialview(QWidget *parent, const char *name)
        : QDialog (parent, name) {
    setupUi(this);
    conexionbase = NULL;
    idmpatrimonial = "";
    componentessuma->addColumn(tr("identificador"), 0);
    componentessuma->addColumn(tr("codigo"), 0);
    componentessuma->addColumn(tr("descripcion"), -1);
    componentessuma->addColumn(tr("tipo"), 0);
    componentesresta->addColumn(tr("identificador"), 0);
    componentesresta->addColumn(tr("codigo"), 0);
    componentesresta->addColumn(tr("descripcion"), -1);
    componentesresta->addColumn(tr("tipo"), -1);
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
    idmpatrimonial = idmpatrimonial1;
    QString query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial = " + idmpatrimonial;
    conexionbase->begin();
    cursor2 *cursor = conexionbase->cargacursor(query, "micursor");
    conexionbase->commit();
    if (!cursor->eof() ) {
        descmpatrimonial->setText(cursor->valor("descmpatrimonial"));
    } // end if
    delete cursor;

    /// Leemos las cuentas y las metemos en la lista que corresponda.
    conexionbase->begin();
    query = "SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM compmasap WHERE masaperteneciente = " + idmpatrimonial + " AND signo = true)";
    cursor2 *cursoraux = conexionbase->cargacursor(query, "cursorusuario");
    conexionbase->commit();
    Q3ListViewItem *it;
    while (!cursoraux->eof()) {
        it = new Q3ListViewItem(componentessuma);
        it->setText(1, cursoraux->valor("codigo"));
        it->setText(2, cursoraux->valor("descripcion"));
        it->setText(3, "cuenta");
        it->setText(0, cursoraux->valor("idcuenta"));
        cursoraux->siguienteregistro();
    } // end while
    delete cursoraux;

    /// Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    conexionbase->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial IN (SELECT idmpatrimonial FROM compmasap WHERE masaperteneciente = " + idmpatrimonial + " AND signo=true)";
    cursoraux = conexionbase->cargacursor(query, "cursorusuario");
    conexionbase->commit();
    while (!cursoraux->eof()) {
        it = new Q3ListViewItem(componentessuma);
        it->setText(1, "");
        it->setText(2, cursoraux->valor("descmpatrimonial"));
        it->setText(3, "masa patrimonial");
        it->setText(0, cursoraux->valor("idmpatrimonial"));
        cursoraux->siguienteregistro();
    } // end while
    delete cursoraux;

    /// Leemos las cuentas y las metemos en la lista que corresponda.
    conexionbase->begin();
    query = "SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM compmasap WHERE masaperteneciente = " + idmpatrimonial + " AND signo = false)";
    cursor2 *cursoraux1 = conexionbase->cargacursor(query, "cursorusuario");
    conexionbase->commit();
    while (!cursoraux1->eof()) {
        it = new Q3ListViewItem(componentesresta);
        it->setText(1, cursoraux1->valor("codigo"));
        it->setText(2, cursoraux1->valor("descripcion"));
        it->setText(3, "cuenta");
        it->setText(0, cursoraux1->valor("idcuenta"));
        cursoraux1->siguienteregistro();
    } // end while
    delete cursoraux1;

    /// Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    conexionbase->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial IN (SELECT idmpatrimonial FROM compmasap WHERE masaperteneciente = " + idmpatrimonial + " AND signo = false)";
    cursoraux = conexionbase->cargacursor(query, "cursorusuario");
    conexionbase->commit();
    while (!cursoraux->eof()) {
        it = new Q3ListViewItem(componentesresta);
        it->setText(1, "");
        it->setText(2, cursoraux->valor("descmpatrimonial"));
        it->setText(3, "masa patrimonial");
        it->setText(0, cursoraux->valor("idmpatrimonial"));
        cursoraux->siguienteregistro();
    } // end while
    delete cursoraux;
}


/// Se ha pulsado sobre el bot&oacute; de agregar donde est&aacute; la suma.
/// Tenemos que a&ntilde;dir la masa patrimonial o la cuenta a la suma.
void mpatrimonialview::nuevasuma() {
    Q3ListViewItem *it;
    /// Lo primero de todo es coger el c&oacute;igo de cuenta.
    QString codcuenta = codigocta1->text();
    QString masapatrimonial = mpatrimonial->text();

    /// Leemos las cuentas y las metemos en la lista que corresponda.
    conexionbase->begin();
    QString query = "SELECT * FROM cuenta WHERE codigo = '" + codcuenta + "'";
    cursor2 *cursoraux1 = conexionbase->cargacursor(query, "cursorusuario");
    conexionbase->commit();
    while (!cursoraux1->eof()) {
        it = new Q3ListViewItem(componentessuma);
        it->setText(1, cursoraux1->valor("codigo"));
        it->setText(2, cursoraux1->valor("descripcion"));
        it->setText(3, "cuenta");
        it->setText(0, cursoraux1->valor("idcuenta"));
        cursoraux1->siguienteregistro();
    } // end while
    delete cursoraux1;

    /// Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    conexionbase->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial = " + masapatrimonial;
    cursoraux1 = conexionbase->cargacursor(query, "cursormpatrimonial");
    conexionbase->commit();
    while (!cursoraux1->eof()) {
        it = new Q3ListViewItem(componentessuma);
        it->setText(1, "");
        it->setText(2, cursoraux1->valor("descmpatrimonial"));
        it->setText(3, "masa patrimonial");
        it->setText(0, cursoraux1->valor("idmpatrimonial"));
        cursoraux1->siguienteregistro();
    } // end while
}


void mpatrimonialview::borrasuma() {
    Q3ListViewItem *it;
    it = componentessuma->currentItem();
    componentessuma->takeItem(it);
}


/// Se ha pulsado sobre el bot&oacute; de agregar donde est&aacute; la suma.
/// Tenemos que a&ntilde;dir la masa patrimonial o la cuenta a la suma.
void mpatrimonialview::nuevaresta() {
    Q3ListViewItem *it;
    /// Lo primero de todo es coger el c&oacute;igo de cuenta.
    QString codcuenta = codigocta1->text();
    QString masapatrimonial = mpatrimonial->text();

    /// Leemos las cuentas y las metemos en la lista que corresponda.
    conexionbase->begin();
    QString query = "SELECT * FROM cuenta WHERE codigo = '" + codcuenta + "'";
    cursor2 *cursoraux1 = conexionbase->cargacursor(query, "cursorusuario");
    conexionbase->commit();
    while (!cursoraux1->eof()) {
        it = new Q3ListViewItem(componentesresta);
        it->setText(1, cursoraux1->valor("codigo"));
        it->setText(2, cursoraux1->valor("descripcion"));
        it->setText(3, "cuenta");
        it->setText(0, cursoraux1->valor("idcuenta"));
        cursoraux1->siguienteregistro();
    } // end while
    delete cursoraux1;

    /// Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    conexionbase->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial = " + masapatrimonial;
    cursoraux1 = conexionbase->cargacursor(query, "cursormpatrimonial");
    conexionbase->commit();
    while (!cursoraux1->eof()) {
        it = new Q3ListViewItem(componentesresta);
        it->setText(1, "");
        it->setText(2, cursoraux1->valor("descmpatrimonial"));
        it->setText(3, "masa patrimonial");
        it->setText(0, cursoraux1->valor("idmpatrimonial"));
        cursoraux1->siguienteregistro();
    } // end while
}


void mpatrimonialview::borraresta() {
    Q3ListViewItem *it;
    it = componentesresta->currentItem();
    componentesresta->takeItem(it);
}


void mpatrimonialview::buscampatrimonial() {
    mpatrimonialesview *nuevae = new mpatrimonialesview(0, "mpatrimoniales", true);
    nuevae->inicializa(conexionbase);
    nuevae->setmodoselector();
    nuevae->exec();
    mpatrimonial->setText(nuevae->getidmasa());
    delete nuevae;
}


void mpatrimonialview::accept() {
    Q3ListViewItem *item;
    QString query;

    /// Si idmpatrimonial == NULL implica que es una inserci&oacute;n y no una modificaci&oacute;n
    if (idmpatrimonial == "") {
        conexionbase->begin();
        query.sprintf("INSERT INTO mpatrimonial (descmpatrimonial) VALUES ('nueva masa')");
        conexionbase->ejecuta(query.ascii());
        query.sprintf("SELECT MAX(idmpatrimonial) as id FROM mpatrimonial");
        cursor2 *curs = conexionbase->cargacursor(query, "cargaid");
        conexionbase->commit();
        idmpatrimonial = curs->valor("id").ascii();
    } // end if

    /// Ponemos los datos correctos sobre la masa patrimonial.
    QString text = descmpatrimonial->text();
    query.sprintf("UPDATE mpatrimonial SET descmpatrimonial = '%s' WHERE idmpatrimonial = %s", text.ascii(), idmpatrimonial.ascii());
    conexionbase->ejecuta(query);

    query.sprintf("DELETE FROM compmasap WHERE masaperteneciente = %s", idmpatrimonial.ascii());
    conexionbase->ejecuta(query);
    item = componentessuma->firstChild();
    while (item) {
        QString id = item->text(0);
        QString tipo = item->text(3);
        if (tipo == "cuenta") {
            query.sprintf("INSERT INTO compmasap(idcuenta, idmpatrimonial, masaperteneciente, signo) VALUES (%s, NULL, %s, true)",
                          conexionbase->sanearCadena(id).ascii(),
                          conexionbase->sanearCadena(idmpatrimonial).ascii());
        } else {
            query.sprintf("INSERT INTO compmasap(idcuenta, idmpatrimonial, masaperteneciente, signo) VALUES (NULL, %s, %s, true)",
                          conexionbase->sanearCadena(id).ascii(),
                          conexionbase->sanearCadena(idmpatrimonial).ascii());
        } // end if
        conexionbase->ejecuta(query);
        item = item->nextSibling();
    } // end while

    item = componentesresta->firstChild();
    while (item) {
        QString id = item->text(0).ascii();
        QString tipo = item->text(3).ascii();
        if (tipo == "cuenta") {
            query.sprintf("INSERT INTO compmasap(idcuenta, idmpatrimonial, masaperteneciente, signo) VALUES (%s, NULL, %s, false)",
                          conexionbase->sanearCadena(id).ascii(),
                          conexionbase->sanearCadena(idmpatrimonial).ascii());
        } else {
            query.sprintf("INSERT INTO compmasap(idcuenta, idmpatrimonial, masaperteneciente, signo) VALUES (NULL, %s, %s, false)",
                          conexionbase->sanearCadena(id).ascii(),
                          conexionbase->sanearCadena(idmpatrimonial).ascii());
        } // end if
        conexionbase->ejecuta(query);
        item = item->nextSibling();
    } // end while
    close();
}


QString mpatrimonialview::getnommasa() {
    return descmpatrimonial->text().ascii();
}

