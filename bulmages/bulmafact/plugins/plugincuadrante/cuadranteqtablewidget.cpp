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

#include <QKeyEvent>
#include <QEvent>
#include <QLineEdit>
#include <QTextEdit>

#include "cuadranteqtablewidget.h"
#include "configuracion.h"
#include "funcaux.h"
#include "company.h"


/** Constructor de CuadranteQTableWidget clase derivada de QTableWidget con
un eventHandler especifico
*/
CuadranteQTableWidget::CuadranteQTableWidget(QWidget *parent) : QTableWidget(parent) {
    _depura("CuadranteQTableWidget::CuadranteQTableWidget", 0);
    _depura("END CuadranteQTableWidget::CuadranteQTableWidget", 0);
}


CuadranteQTableWidget::~CuadranteQTableWidget() {
    _depura("END ~CuadranteQTableWidget", 0);
}




void CuadranteQTableWidget::dragEnterEvent(QDragEnterEvent *event) {
    _depura("CuadranteQTableWidget::dropMimeData", 0);
    event->acceptProposedAction();
}


void CuadranteQTableWidget::dragMoveEvent(QDragMoveEvent *event) {
    event->acceptProposedAction();
}

void CuadranteQTableWidget::dropEvent(QDropEvent *event) {
    _depura("CuadranteQTableWidget::dropEvent", 0);
    QTableWidget::dropEvent(event);
}


bool CuadranteQTableWidget::dropMimeData (int row, int column,const QMimeData *data, Qt::DropAction ) {
    _depura("CuadranteQTableWidget::dropMimeData", 0  );

    if (data->hasText()) {
        ((CuadranteQTextDocument *)cellWidget(row, column))->addTrabajador(data->text());
    }
    return (TRUE);
}



/// ======================================================================
/// ======================================================================


CuadranteQTextDocument::CuadranteQTextDocument(Company *emp, QWidget *parent) : QLabel(parent), PEmpresaBase(emp) {
    setScaledContents(TRUE);

}



CuadranteQTextDocument::~CuadranteQTextDocument() {
    _depura("~CuadranteQTextDocument", 0);
}

void CuadranteQTextDocument::addTrabajador(QString idtrabajador) {
    try {
        QString horain = "08:00";
        QString horafin = "19:00";

        QString querycuad = "SELECT * FROM cuadrante WHERE idcuadrante=" + mdb_idcuadrante;
        cursor2 *cur = empresaBase()->cargacursor(querycuad);
        if (!cur) throw -1;
        if (!cur->eof()) {
            if (cur->valor("aperturacuadrante") != "")
                horain = cur->valor("aperturacuadrante");
            if (cur->valor("cierre1cuadrante") != "")
                horafin = cur->valor("cierre1cuadrante");
        } // end if

        QString query = "INSERT INTO horario (idtrabajador, idcuadrante, horainhorario, horafinhorario) VALUES (" +idtrabajador +","+ mdb_idcuadrante +",'"+horain+"','"+horafin+"')";
        empresaBase()->begin();
        empresaBase()->ejecuta(query);
        empresaBase()->commit();
        pintaCuadrante(mdb_idalmacen, mdb_fechacuadrante);
    } catch (...) {
        empresaBase()->rollback();
        _depura("error", 2);
    } // end try
}

void CuadranteQTextDocument::setAlmFecha(QString idalmacen, const QDate &date) {
    _depura("CuadranteQTextDocument::setAlmFecha", 0);
    try {
        /// Comprobamos que exista el cuadrante y si no existe lo creamos.
        cursor2 *cur1 = empresaBase()->cargacursor("SELECT * FROM cuadrante WHERE fechacuadrante = '" + date.toString("dd/MM/yyyy") + "' AND idalmacen="+idalmacen);
        if (!cur1) throw -1;
        if (cur1->eof()) {
            QString query = "INSERT INTO cuadrante (idalmacen, fechacuadrante) VALUES ("+idalmacen+",'"+ date.toString("dd/MM/yyyy") +"')";
            empresaBase()->begin();
            empresaBase()->ejecuta(query);
            empresaBase()->commit();
        } // end if
        delete cur1;

        pintaCuadrante(idalmacen, date);


    } catch (...) {
        empresaBase()->rollback();
        _depura("Error en la carga", 2);
        return;
    } // end try
}




void CuadranteQTextDocument::pintaCuadrante(QString idalmacen, const QDate &date) {

    QString html = "";
    cursor2 *cur = empresaBase()->cargacursor("SELECT * FROM cuadrante, almacen WHERE cuadrante.idalmacen = almacen.idalmacen AND almacen.idalmacen="+idalmacen+" AND cuadrante.fechacuadrante ='" +date.toString("dd/MM/yyyy")+ "'");
    if (!cur) throw -1;
    if (!cur->eof()) {
        if (cur->valor("fiestacuadrante") == "t") {
            html += "<table width=\"240\" height=\"300\" bgcolor=\"#999999\"><tr><td>";
        } else {
            html += "<table width=\"240\" height=\"300\" bgcolor=\"#FFFFFF\"><tr><td>";
        }
        html += "<font size=\"-2\" color=\"#660000\"><B>" + cur->valor("nomalmacen") + "</B>: " + date.toString("dd/MM/yyyy")+"</font><BR>";
        mdb_idcuadrante = cur->valor("idcuadrante");
        mdb_idalmacen = idalmacen;
        mdb_fechacuadrante = date;
    } // end if



    QString oldnomtipotrabajo = "";

    cursor2 *cur1 = empresaBase()->cargacursor("SELECT * FROM horario, trabajador, tipotrabajo WHERE horario.idtrabajador = trabajador.idtrabajador AND trabajador.idtipotrabajo = tipotrabajo.idtipotrabajo AND idcuadrante = "+mdb_idcuadrante +" ORDER BY nomtipotrabajo, horainhorario, nomtrabajador");
    if (!cur1) throw -1;
    while (!cur1->eof()) {
        if (oldnomtipotrabajo != cur1->valor("nomtipotrabajo") ) {
            html += "<font size=\"-3\" color=\"#00FF00\" >" + cur1->valor("nomtipotrabajo") + ":</font><BR>";
            oldnomtipotrabajo = cur1->valor("nomtipotrabajo");
        } // end if
        html += "<font size=\"-3\" color=\"#0000FF\">" + cur1->valor("nomtrabajador") + " " + cur1->valor("apellidostrabajador");
        html += " (" + cur1->valor("horainhorario").left(5) + "--" + cur1->valor("horafinhorario").left(5) + ") </font><BR>";


        cur1->siguienteregistro();
    } // end while
    delete cur1;

    if (cur->valor("comentcuadrante") != "") {
        html += "<HR><font size=\"-4\" color=\"#000000\">" + cur->valor("comentcuadrante").replace("\n", "<BR>")+"</font>";
    } // end if
    delete cur;

    html += "</td></tr></table>";

    setText(html);
}

QString CuadranteQTextDocument::idcuadrante() {
    return mdb_idcuadrante;
}

void CuadranteQTextDocument::refresh() {
    pintaCuadrante(mdb_idalmacen, mdb_fechacuadrante);
}

const QString CuadranteQTextDocument::impresion() {

    QString html = "";
    html += "<td>\n";
    QString style="";
    cursor2 *cur = empresaBase()->cargacursor("SELECT * FROM cuadrante, almacen WHERE cuadrante.idalmacen = almacen.idalmacen AND almacen.idalmacen="+mdb_idalmacen+" AND cuadrante.fechacuadrante ='" +mdb_fechacuadrante.toString("dd/MM/yyyy")+ "'");
    if (!cur) throw -1;
    if (!cur->eof()) {
        if (cur->valor("fiestacuadrante") == "t") {
            style = " style=\"festivo\"";
        }
        html += "<para "+style+"> <b>" + cur->valor("nomalmacen") + "</b>: " + mdb_fechacuadrante.toString("dd/MM/yyyy")+"</para><spacer length=\"0.1cm\"/>\n";
    } // end if


    QString oldnomtipotrabajo = "";

    cursor2 *cur1 = empresaBase()->cargacursor("SELECT * FROM horario, trabajador, tipotrabajo WHERE horario.idtrabajador = trabajador.idtrabajador AND trabajador.idtipotrabajo = tipotrabajo.idtipotrabajo AND idcuadrante = "+mdb_idcuadrante +" ORDER BY nomtipotrabajo, horainhorario, nomtrabajador");
    if (!cur1) throw -1;
    while (!cur1->eof()) {
        if (oldnomtipotrabajo != cur1->valor("nomtipotrabajo") ) {
            html +=  "<para "+style+"><font color=\"red\">" + cur1->valor("nomtipotrabajo") + ":</font></para><spacer length=\"0.1cm\"/>\n";
            oldnomtipotrabajo = cur1->valor("nomtipotrabajo");
        } // end if
        html += "<para "+style+"><font face=\"Helvetica\" size=\"6\">" + cur1->valor("nomtrabajador") + " " + cur1->valor("apellidostrabajador");
        html += "<sup>(" + cur1->valor("horainhorario").left(5) + "--" + cur1->valor("horafinhorario").left(5) + ")</sup></font></para><spacer length=\"0.1cm\"/>\n";

        cur1->siguienteregistro();
    } // end while
    delete cur1;

    if (cur->valor("comentcuadrante") != "") {
        html += "<para "+style+"><font face=\"Helvetica\" size=\"5\" color=\"blue\">" + cur->valor("comentcuadrante").replace("\n", "<spacer length=\"0.1cm\"/>\n") + "</font></para>";
    } // end if

    delete cur;

    html += "</td>\n";
    return html;
}
