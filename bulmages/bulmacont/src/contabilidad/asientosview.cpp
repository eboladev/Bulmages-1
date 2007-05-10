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

#include <QWidget>
#include <QComboBox>
#include <QDateTime>

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
    _depura("asientosview::rellenaListaEjercicio", 0);
    /// Actualiza el contenido del combobox.
    mui_ejercicio->clear();
    mui_ejercicio->insertItem(0, tr("(todos)"));
    QString SQLQuery = "SELECT DISTINCT EXTRACT (YEAR FROM fecha) AS ano FROM borrador";
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    while (!cur->eof()) {
        mui_ejercicio->addItem(cur->valor("ano"));
        cur->siguienteregistro();
    } // end while
    delete cur;
    _depura("END asientosview::rellenaListaEjercicio", 0);
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
    QString saldototal = mui_saldoasiento->text();
    /// Pasamos el texto a minusculas para hacer la busqueda 'case insensitive'.
    QString nombreasiento = mui_nombreasiento->text().toLower();
    QString ejercicio = mui_ejercicio->currentText();
    QString apuntemayoroigual = mui_mayoroigual->text();
    QString apuntemenoroigual = mui_menoroigual->text();
    QString query;
    QString cadwhere;
    QString textsaldototal = "";
    QString textoparentesis = "";
    QString textapuntemayoroigual = "";
    QString textapuntemenoroigual = "";
    QString textnombreasiento = "";
    QString textejercicio = "";
    QString buscafechainicial = mui_fechaInicial->text();
    QString buscafechafinal = mui_fechaFinal->text();

    int pand = 0;

    /// Componemos la consulta a partir de la parte de filtrado.
    if (saldototal != "") {
        cadwhere = " WHERE ";
        textsaldototal = " asiento.idasiento IN (SELECT idasiento FROM (SELECT idasiento, SUM(debe) AS totaldebe, SUM(haber) as totalhaber FROM apunte GROUP BY idasiento) AS foo WHERE foo.totaldebe = " + saldototal + " OR foo.totalhaber = " + saldototal + ")";
        pand = 1;
    } // end if
    if (apuntemayoroigual != "") {
        cadwhere = " WHERE ";
        if (pand) {
            /// Ya existe un saldototal ('igual a') por lo que se hace un OR a la consulta.
            textapuntemayoroigual = " OR (";
        } // end if
        textapuntemayoroigual += " asiento.idasiento IN (SELECT idasiento FROM apunte WHERE debe >= " + apuntemayoroigual + " OR haber >= " + apuntemayoroigual + ")";
        pand = 1;
    } // end if
    if (apuntemenoroigual != "") {
        cadwhere = " WHERE ";
        if (pand) {
            if (apuntemayoroigual != "") {
                /// Hay definido un 'mayor o igual' y tambien un 'igual a'.
                textapuntemenoroigual = " AND ";
            } else {
                /// No se ha definido un 'mayor o igual' pero si un 'igual a'.
                textapuntemenoroigual = " OR (";
            }// end if
        } // end if
        textapuntemenoroigual += " asiento.idasiento IN (SELECT idasiento FROM apunte WHERE debe <= " + apuntemenoroigual + " OR haber <= " + apuntemenoroigual + ")";
        pand = 1;
    } // end if
    /// Se mira si se tiene que cerrar el parentesis en la consulta a la base de datos.
    if (saldototal != "" && (apuntemenoroigual != "" || apuntemayoroigual != "")) {
        textoparentesis = ")";
    } // end if

    if (nombreasiento != "") {
        cadwhere = " WHERE ";
        if (pand) {
            textnombreasiento = " AND ";
        } // end if
        textnombreasiento += " asiento.idasiento in (SELECT idasiento FROM apunte WHERE lower(conceptocontable) LIKE '%" + nombreasiento + "%' )";
        pand = 1;
    } // end if

    /// Comprobamos si faltan fechas de inicio o de fin en la busqueda y las establecemos
    /// automaticamente.

    /// Regla: Se prioriza la busqueda por fechas independientemente del ejercicio.
    ///         de esta manera si se ha especificado 'fecha inicial' y 'fecha final'
    ///         entonces el combobox de 'ejercicio' queda anulado.
    /// Regla: Si esta especificado 'ejercicio' pero solo una fecha 'inicial' o 'final'
    ///         se pone automaticamente la fecha que falta.
    if (buscafechainicial != "" && buscafechafinal == "") {
        /// Falta fecha final.
        if (mui_ejercicio->currentIndex() == 0) {
            /// Todos los ejercicios.
            if (pand) {
                textejercicio = " AND fecha >= '" + QDateTime::fromString(buscafechainicial, "dd/MM/yyyy").toString("yyyy-MM-dd") + "'";
            } else {
                textejercicio = " WHERE fecha >= '" + QDateTime::fromString(buscafechainicial, "dd/MM/yyyy").toString("yyyy-MM-dd") + "'";
            } // end if
        } else {
            /// Ejercicio seleccionado. Maximo hasta final de ese a&ntilde;o,
            buscafechafinal = "31/12/" + mui_ejercicio->currentText();
            mui_fechaFinal->setText(buscafechafinal);
            if (pand) {
                textejercicio = " AND fecha >= '" + QDateTime::fromString(buscafechainicial, "dd/MM/yyyy").toString("yyyy-MM-dd") + "' AND fecha <= '" + QDateTime::fromString(buscafechafinal, "dd/MM/yyyy").toString("yyyy-MM-dd") + "'";
            } else {
                textejercicio = " WHERE fecha >= '" + QDateTime::fromString(buscafechainicial, "dd/MM/yyyy").toString("yyyy-MM-dd") + "' AND fecha <= '" + QDateTime::fromString(buscafechafinal, "dd/MM/yyyy").toString("yyyy-MM-dd") + "'";
            } // end if
        } // end if
    } else if (buscafechainicial == "" && buscafechafinal != "") {
        /// Falta fecha inicial.
        if (mui_ejercicio->currentIndex() == 0) {
            /// Todos los ejercicios.
            if (pand) {
                textejercicio = " AND fecha <= '" + QDateTime::fromString(buscafechafinal, "dd/MM/yyyy").toString("yyyy-MM-dd") + "'";
            } else {
                textejercicio = " WHERE fecha <= '" + QDateTime::fromString(buscafechafinal, "dd/MM/yyyy").toString("yyyy-MM-dd") + "'";
            } // end if
        } else {
            /// Ejercicio seleccionado. Maximo hasta final de ese a&ntilde;o,
            buscafechainicial = "01/01/" + mui_ejercicio->currentText();
            mui_fechaInicial->setText(buscafechainicial);
            if (pand) {
                textejercicio = " AND fecha >= '" + QDateTime::fromString(buscafechainicial, "dd/MM/yyyy").toString("yyyy-MM-dd") + "' AND fecha <= '" + QDateTime::fromString(buscafechafinal, "dd/MM/yyyy").toString("yyyy-MM-dd") + "'";
            } else {
                textejercicio = " WHERE fecha >= '" + QDateTime::fromString(buscafechainicial, "dd/MM/yyyy").toString("yyyy-MM-dd") + "' AND fecha <= '" + QDateTime::fromString(buscafechafinal, "dd/MM/yyyy").toString("yyyy-MM-dd") + "'";
            } // end if
        } // end if
    } else if (buscafechainicial != "" && buscafechafinal != "") {
        /// Hay fecha inicial y fecha final.
        /// Primero se comprueba que la fecha final sea mayor que la fecha inicial.
        if (QDateTime::fromString(buscafechainicial, "dd/MM/yyyy") > QDateTime::fromString(buscafechafinal, "dd/MM/yyyy")) {
            /// Error de fechas.
            mensajeAviso(tr("La fecha inicial es posterior a la fecha final."));
            return;
        } else {
            if (pand) {
                textejercicio = " AND fecha >= '" + QDateTime::fromString(buscafechainicial, "dd/MM/yyyy").toString("yyyy-MM-dd") + "' AND fecha <= '" + QDateTime::fromString(buscafechafinal, "dd/MM/yyyy").toString("yyyy-MM-dd") + "'";
            } else {
                textejercicio = " WHERE fecha >= '" + QDateTime::fromString(buscafechainicial, "dd/MM/yyyy").toString("yyyy-MM-dd") + "' AND fecha <= '" + QDateTime::fromString(buscafechafinal, "dd/MM/yyyy").toString("yyyy-MM-dd") + "'";
            } // end if
        } // end if
    } else if (buscafechainicial == "" && buscafechafinal == "" && mui_ejercicio->currentIndex() != 0) {
        /// No hay ninguna fecha.
        if (mui_ejercicio->currentIndex() != 0) {
            /// Ejercicio seleccionado.
            if (pand) {
                textejercicio = " AND EXTRACT(YEAR FROM fecha) = '" + ejercicio + "'";
            } else {
                textejercicio = " WHERE EXTRACT(YEAR FROM fecha) = '" + ejercicio + "'";
            } // end if
        } // end if
    } // end if

    query = "SELECT asiento.ordenasiento, asiento.idasiento, asiento.fecha, totaldebe, totalhaber, numap, numborr, comentariosasiento, clase FROM asiento LEFT JOIN (SELECT count(idborrador) AS numborr, idasiento FROM borrador GROUP BY idasiento) AS foo1 ON foo1.idasiento = asiento.idasiento LEFT JOIN (SELECT SUM(debe) AS totaldebe, SUM(haber) AS totalhaber, count(idapunte) AS numap, idasiento FROM apunte GROUP BY idasiento) AS fula ON asiento.idasiento = fula.idasiento " + cadwhere + textsaldototal + textapuntemayoroigual + textapuntemenoroigual + textoparentesis + textnombreasiento + textejercicio + " ORDER BY EXTRACT (YEAR FROM asiento.fecha), asiento.ordenasiento";
    cursor2 *cursoraux = m_companyact->cargacursor(query);
    mui_list->cargar(cursoraux);
    delete cursoraux;

    /// Actualiza el contenido del combobox.
    rellenaListaEjercicio();

    /// Busca el texto guardado anteriormente del combobox y lo selecciona.
    int ejercicioIndice;
    ejercicioIndice = mui_ejercicio->findText(ejercicio);
    mui_ejercicio->setCurrentIndex(ejercicioIndice);

    /// Calculamos el total en el subformulario y lo presentamos.
    Fixed td= mui_list->sumarCampo("totaldebe");
    Fixed th = mui_list->sumarCampo("totalhaber");
    mui_totalDebe->setText(td.toQString());
    mui_totalHaber->setText(th.toQString());

    _depura("END asientosview::inicializa", 0);
}


void asientosview::on_mui_imprimir_clicked() {
    _depura("asientosview::on_mui_imprimir_clicked", 0);
    mui_list->imprimirPDF(tr("Asientos"));
    _depura("END asientosview::on_mui_imprimir_clicked", 0);
}


void asientosview::on_mui_configurar_toggled(bool checked) {
    _depura("asientosview::on_mui_configurar_toggled", 0);
    if (checked) {
        mui_list->showConfig();
    } else {
        mui_list->hideConfig();
    } // end if
    _depura("END asientosview::on_mui_configurar_toggled", 0);
}


void asientosview::on_mui_actualizar_clicked() {
    _depura("asientosview::on_mui_actualizar_clicked", 0);
    inicializa();
    _depura("END asientosview::on_mui_actualizar_clicked", 0);
}
