/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
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

#include "asientosview.h"
#include "asiento1view.h"
#include "empresa.h"


/// El constructor de la clase inicializa algunas estructuras y configura la visi&oacute;n
/// de la pantalla.
asientosview::asientosview(empresa *emp, QWidget *parent)
        : Ficha(parent) {
    _depura("asientosview::asientosview", 0);
    setupUi(this);
    m_companyact = emp;
    mui_list->setcompany(emp);

    rellenaListaEjercicio();

    mui_filtrar->toggle();
    m_companyact->meteWindow(windowTitle(), this);
    _depura("END asientosview::asientosview", 0);
}


void asientosview::rellenaListaEjercicio() {
    /// Actualiza el contenido del combobox.
    mui_ejercicio->clear();
    mui_ejercicio->insertItem(0, "--");
    QString SQLQuery = "SELECT DISTINCT EXTRACT (YEAR FROM fecha) AS ano FROM borrador";
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    while (!cur->eof()) {
        mui_ejercicio->addItem(cur->valor("ano"));
        cur->siguienteregistro();
    } // end while
    delete cur;
}


asientosview::~asientosview() {
    _depura("asientosview::~asientosview\n", 0);
    m_companyact->sacaWindow(this);
    _depura("END asientosview::~asientosview\n", 0);
}


void asientosview::on_mui_list_cellDoubleClicked(int, int) {
    _depura("asientosview::on_mui_list_cellDoubleClicked", 0);
    QString idasiento = mui_list->DBvalue("idasiento");
    m_companyact->intapuntsempresa()->muestraasiento(idasiento);
    m_companyact->intapuntsempresa()->show();
    m_companyact->intapuntsempresa()->setFocus();
    m_companyact->muestraapuntes1();
    _depura("END asientosview::on_mui_list_cellDoubleClicked", 0);
}


/// Inicializa la ventana, haciendo la consulta pertinente a la base de datos
/// y presentando los resultados en pantalla.
void asientosview::inicializa() {
    _depura("asientosview::inicializa", 0);
    QString cantapunt = mui_cantidadapunte->text();
    QString saldototal = mui_saldoasiento->text();
    QString nombreasiento = mui_nombreasiento->text();
    QString ejercicio = mui_ejercicio->currentText();
    QString query;
    QString cadwhere;
    QString textsaldototal = "";
    QString textcantapunt = "";
    QString textnombreasiento = "";
    QString textejercicio = "";
    int pand = 0;

    /// Componemos la consulta a partir de la parte de filtrado.
    if (saldototal != "") {
        cadwhere = " WHERE ";
        textsaldototal = " asiento.idasiento IN (SELECT idasiento FROM (SELECT idasiento, sum(debe) AS total from apunte GROUP BY idasiento) AS foo WHERE foo.total = " + saldototal + ")";
        pand = 1;
    } // end if
    if (cantapunt != "" ) {
        cadwhere = " WHERE ";
        if (pand) {
            textcantapunt = " AND ";
        } // end if
        textcantapunt += " asiento.idasiento IN (SELECT idasiento FROM apunte where debe = " + cantapunt + " OR haber = " + cantapunt + ")";
        pand = 1;
    } // end if
    if (nombreasiento != "") {
        cadwhere = " WHERE ";
        if (pand) {
            textnombreasiento = " AND ";
        } // end if
        textnombreasiento += " asiento.idasiento in (SELECT idasiento FROM apunte WHERE conceptocontable LIKE '%" + nombreasiento + "%' )";
        pand = 1;
    } // end if
    if (ejercicio != "--") {
        if (pand) {
            textejercicio = " AND EXTRACT(YEAR FROM fecha) = '" + ejercicio + "'";
        } else {
            textejercicio = " WHERE EXTRACT(YEAR FROM fecha) = '" + ejercicio + "'";
        } // end if
    } // end if

    query = "SELECT asiento.ordenasiento, asiento.idasiento, asiento.fecha,  totaldebe, totalhaber, numap, numborr, comentariosasiento, clase   from asiento  LEFT JOIN (SELECT count(idborrador) AS numborr, idasiento FROM borrador GROUP BY idasiento) as foo1 ON foo1.idasiento = asiento.idasiento LEFT JOIN (SELECT sum(debe) as totaldebe, sum(haber) as totalhaber, count(idapunte) as numap, idasiento from apunte group by idasiento) as fula ON asiento.idasiento = fula.idasiento " + cadwhere + textsaldototal + textcantapunt + textnombreasiento + textejercicio + " ORDER BY EXTRACT (YEAR FROM asiento.fecha), asiento.ordenasiento";
    cursor2 *cursoraux = m_companyact->cargacursor(query);
    mui_list->cargar(cursoraux);
    delete cursoraux;

    /// Actualiza el contenido del combobox.
    rellenaListaEjercicio();

    /// Busca el texto guardado anteriormente del combobox y lo selecciona.
    int ejercicioIndice;
    ejercicioIndice = mui_ejercicio->findText(ejercicio);
    mui_ejercicio->setCurrentIndex(ejercicioIndice);

    _depura("END asientosview::inicializa", 0);
}


void asientosview::on_mui_imprimir_clicked() {
    _depura("asientosview::on_mui_imprimir_clicked", 0);
    mui_list->imprimirPDF(tr("Asientos"));
    _depura("END asientosview::on_mui_imprimir_clicked", 0);
}

