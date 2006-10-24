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

#include "duplicarasientoview.h"
#include "empresa.h"

#define NUMDIGITOS   empresaactual->numdigitosempresa()


duplicarasientoview::duplicarasientoview(empresa *emp, QWidget *parent, const char *name, bool flag)
        : QDialog(parent, name, flag, 0) {
    setupUi(this);
    empresaactual = emp;
    fdinicial->setText(QDate::currentDate().toString("dd/MM/yyyy"));
}


duplicarasientoview::~duplicarasientoview() {}


void duplicarasientoview::inicializa(QString ainicial, QString afinal) {
    aoinicial->setText(ainicial);
    aofinal->setText(afinal);
    QString query = "SELECT * FROM asiento WHERE ordenasiento = " + ainicial;
    empresaactual->begin();
    cursor2 *cur = empresaactual->cargacursor(query, "hola");
    empresaactual->commit();
    if (!cur->eof()) {
        foinicial->setText(cur->valor("fecha").left(10));
    } // end if
    delete cur;
    aoinicial->selectAll();
    aoinicial->setFocus();
}


void duplicarasientoview::lostFocus() {
    QString ainicial = aoinicial->text();
    QString query = "SELECT * FROM asiento WHERE ordenasiento = " + ainicial;
    empresaactual->begin();
    cursor2 *cur = empresaactual->cargacursor(query, "hola");
    empresaactual->commit();
    if (!cur->eof()) {
        foinicial->setText(cur->valor("fecha").left(10));
    } // end if
    delete cur;
}


void duplicarasientoview::accept() {
    QString asientoi = aoinicial->text();
    QString asientof = aofinal->text();
    QString query1, query2;
    QString textidasiento;
    int ordeninicial = 0;
    int idasientoinicial = 0;
    QString textordeninicial;
    QDate fedinicial = normalizafecha(fdinicial->text());

    /// Buscamos el orden asiento para la duplicaci&oacute;n.
    QString query = "SELECT max(ordenasiento) AS orden FROM asiento ";
    empresaactual->begin();
    cursor2 *cur = empresaactual->cargacursor(query, "hola");
    if (!cur->eof()) {
        ordeninicial = atoi(cur->valor("orden").ascii()) + 1;
    } // end if
    delete cur;

    query1 = "SELECT max(idasiento) AS maxim FROM asiento";
    cursor2 *cursaux = empresaactual->cargacursor(query1, "maximo");
    if (!cursaux->eof()) {
        idasiento = atoi(cursaux->valor("maxim").ascii());
        idasientoinicial = atoi(cursaux->valor("maxim").ascii()) + 1;
    } // end if
    delete cursaux;

    query1 = "SELECT * FROM asiento WHERE ordenasiento >= " + asientoi + " AND ordenasiento <= " + asientof;
    cursor2 *curasiento = empresaactual->cargacursor(query1, "mycursor");
    while (!curasiento->eof()) {
        textordeninicial.sprintf("%d", ordeninicial++);
        textidasiento.sprintf("%d", idasientoinicial);
        query1 = "INSERT INTO asiento (idasiento,descripcion, fecha, comentariosasiento,ordenasiento) VALUES(" + textidasiento + ",'" + curasiento->valor("descripcion") + "','" + fedinicial.toString("dd/MM/yyyy") + "','" + curasiento->valor("comentariosasiento") + "'," + textordeninicial + ")";
        empresaactual->ejecuta(query1);
        query2 = "SELECT * FROM borrador WHERE idasiento = " + curasiento->valor("idasiento");
        cursor2 *curborrador = empresaactual->cargacursor(query2, "mycursor2");
        while (!curborrador->eof()) {
            QString textiddiario = curborrador->valor("iddiario");
            if (textiddiario == "") {
                textiddiario = "NULL";
            } // end if
            QString textfecha = curborrador->valor("fecha");
            textfecha = fedinicial.toString("dd/MM/yyyy");
            QString textconceptocontable = curborrador->valor("conceptocontable");
            QString textidcuenta = curborrador->valor("idcuenta");
            if (textidcuenta == "") {
                textidcuenta = "NULL";
            } // end if
            QString textdescripcion = curborrador->valor("descripcion");
            QString textdebe = curborrador->valor("debe");
            QString texthaber = curborrador->valor("haber");
            QString textcontrapartida = curborrador->valor("contrapartida");
            if (textcontrapartida == "") {
                textcontrapartida = "NULL";
            } // end if
            QString textorden = curborrador->valor("orden");
            if (textorden == "") {
                textorden = "0";
            } // end if
            query2 = "INSERT INTO borrador (orden, idasiento, iddiario, fecha, conceptocontable, idcuenta, descripcion, debe, haber, contrapartida) VALUES (" + textorden + ","+textidasiento + "," + textiddiario + ",'" + textfecha + "','" + textconceptocontable + "'," + textidcuenta + ",'" + textdescripcion + "'," + textdebe + "," + texthaber + "," + textcontrapartida + ")";
            empresaactual->ejecuta(query2);
            curborrador->siguienteregistro();
        } // end while
        delete curborrador;
        empresaactual->cierraasiento(idasientoinicial++);
        curasiento->siguienteregistro();
    } // end while
    delete curasiento;
    empresaactual->commit();
    done(1);
}

