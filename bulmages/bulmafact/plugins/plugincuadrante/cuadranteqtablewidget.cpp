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
#include "cuadrante1view.h"

/** Constructor de CuadranteQTableWidget clase derivada de QTableWidget con
un eventHandler especifico
*/
/**
\param parent
**/
CuadranteQTableWidget::CuadranteQTableWidget(QWidget *parent) : QTableWidget(parent) {
    _depura("CuadranteQTableWidget::CuadranteQTableWidget", 0);
     connect(this, SIGNAL(contextMenuRequested ( int, int, const QPoint &)), this, SLOT(on_contextMenuRequested ( int, int, const QPoint & )));
    _depura("END CuadranteQTableWidget::CuadranteQTableWidget", 0);
}


///
/**
**/
CuadranteQTableWidget::~CuadranteQTableWidget() {
    _depura("END ~CuadranteQTableWidget", 0);
}


///
/**
\param event
**/
void CuadranteQTableWidget::dragEnterEvent(QDragEnterEvent *event) {
    _depura("CuadranteQTableWidget::dragEnterEvent", 0);
    event->acceptProposedAction();
    _depura("END CuadranteQTableWidget::dragEnterEvent", 0);
}


///
/**
\param event
**/
void CuadranteQTableWidget::dragMoveEvent(QDragMoveEvent *event) {
    _depura("CuadranteQTableWidget::dragMoveEvent", 0);
    event->acceptProposedAction();
    _depura("END CuadranteQTableWidget::dragMoveEvent", 0);
}

///
/**
\param event
**/
void CuadranteQTableWidget::dropEvent(QDropEvent *event) {
    _depura("CuadranteQTableWidget::dropEvent", 0);
    QTableWidget::dropEvent(event);
    _depura("END CuadranteQTableWidget::dropEvent", 0);
}


///
/**
\param row
\param column
\param data
\return
**/
bool CuadranteQTableWidget::dropMimeData (int row, int column,const QMimeData *data, Qt::DropAction ) {
    _depura("CuadranteQTableWidget::dropMimeData", 0  );

    if (data->hasText()) {
        ((CuadranteQTextDocument *)cellWidget(row, column))->addTrabajador(data->text());
    }
    _depura("END CuadranteQTableWidget::dropMimeData", 0  );
    return (TRUE);
}


void CuadranteQTableWidget::on_contextMenuRequested ( int , int , const QPoint &  ) {
	_depura("CuadranteQTableWidget::contextMenuRequested", 2);
}


/// ======================================================================
/// ======================================================================


///
/**
\param emp
\param parent
**/
CuadranteQTextDocument::CuadranteQTextDocument(Company *emp, QWidget *parent) :QLabel(parent), QTableWidgetItem(QTableWidgetItem::UserType),  PEmpresaBase(emp) {
    _depura("CuadranteQTextDocument::CuadranteQTextDocument", 0);
//    setScaledContents(TRUE);
    _depura("END CuadranteQTextDocument::CuadranteQTextDocument", 0);
}


///
/**
\param e
**/
void CuadranteQTextDocument::contextMenuEvent ( QContextMenuEvent *  ) {
//        int currow = tableWidget()->row(this);
//	int currcol = tableWidget()->column(this);
	QTableWidgetItem::setSelected(TRUE);
//	tableWidget()->setRangeSelected( QTableWidgetSelectionRange(currow, currcol, currow, currcol ), TRUE );
}


///
/**
\param event
**/
void CuadranteQTextDocument::mouseDoubleClickEvent ( QMouseEvent *  ) {
    Cuadrante1View *cuad = new Cuadrante1View((Company *)empresaBase(), 0);
    empresaBase()->pWorkspace()->addWindow(cuad);
    cuad->show();
//    CuadranteQTextDocument *newItem = (CuadranteQTextDocument *) mui_cuadrante->cellWidget(mui_cuadrante->currentRow(), mui_cuadrante->currentColumn());
    connect (cuad, SIGNAL(save()), this, SLOT(refresh()));
    cuad->cargar(idcuadrante());
}



///
/**
**/
CuadranteQTextDocument::~CuadranteQTextDocument() {
    _depura("~CuadranteQTextDocument", 0);
}


///
/**
\param idtrabajador
**/
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


///
/**
\param idalmacen
\param date
\return
**/
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


///
/**
\param idalmacen
\param data
**/
void CuadranteQTextDocument::pintaCuadrante(QString idalmacen, const QDate &date) {
    _depura("CuadranteQTextDocument::pintaCuadrante", 0);
    QString html = "";
    cursor2 *cur = empresaBase()->cargacursor("SELECT * FROM cuadrante, almacen WHERE cuadrante.idalmacen = almacen.idalmacen AND almacen.idalmacen="+idalmacen+" AND cuadrante.fechacuadrante ='" +date.toString("dd/MM/yyyy")+ "'");
    if (!cur) throw -1;
    if (!cur->eof()) {
        if (cur->valor("fiestacuadrante") == "t") {
            html += "<table width=\"240\" height=\"300\" bgcolor=\"#999999\"><tr><td>";
        } else {
            html += "<table width=\"240\" height=\"300\" bgcolor=\"#FFFFFF\"><tr><td>";
        }
        html += "<font size=\"2\" color=\"#660000\"><B>" + cur->valor("nomalmacen") + "</B>: " + date.toString("dd/MM/yyyy")+"</font><BR>";
        mdb_idcuadrante = cur->valor("idcuadrante");
        mdb_idalmacen = idalmacen;
        mdb_fechacuadrante = date;
    } // end if

    QString oldnomtipotrabajo = "";

    cursor2 *cur1 = empresaBase()->cargacursor("SELECT * FROM horario, trabajador, tipotrabajo WHERE horario.idtrabajador = trabajador.idtrabajador AND trabajador.idtipotrabajo = tipotrabajo.idtipotrabajo AND idcuadrante = "+mdb_idcuadrante +" ORDER BY nomtipotrabajo, horainhorario, nomtrabajador");
    if (!cur1) throw -1;
    while (!cur1->eof()) {


        if (oldnomtipotrabajo != cur1->valor("nomtipotrabajo") ) {
            html += "<font size=\"2\" color=\"#00FF00\" >" + cur1->valor("nomtipotrabajo") + ":</font><BR>";
            oldnomtipotrabajo = cur1->valor("nomtipotrabajo");
        } // end if

	/// Si hay conflictos con el trabajador.
	if (buscaConflictos(cur1->valor("idtrabajador"), date, cur1->valor("horainhorario").left(5), cur1->valor("horafinhorario").left(5))) {
		html += "<font size=\"3\" color=\"#FF0000\">ERROR</FONT> ";
	}

        html += "<font size=\"3\" color=\"" + cur1->valor("colortipotrabajo") + "\">" + cur1->valor("nomtrabajador") + " " + cur1->valor("apellidostrabajador");
        html += " (" + cur1->valor("horainhorario").left(5) + "--" + cur1->valor("horafinhorario").left(5) + ") </font><BR>";

        cur1->siguienteregistro();
    } // end while
    delete cur1;

    if (cur->valor("comentcuadrante") != "") {
        html += "<HR><font size=\"2\" color=\"#000000\">" + cur->valor("comentcuadrante").replace("\n", "<BR>")+"</font>";
    } // end if
    delete cur;

    html += "</td></tr></table>";

    QLabel::setText(html);
    _depura("END CuadranteQTextDocument::pintaCuadrante", 0);
}


///
/**
\return
**/
QString CuadranteQTextDocument::idcuadrante() {
    _depura("CuadranteQTextDocument::idcuadrante", 0);
    _depura("END CuadranteQTextDocument::idcuadrante", 0);
    return mdb_idcuadrante;
}


///
/**
**/
void CuadranteQTextDocument::refresh() {
    _depura("CuadranteQTextDocument::refresh", 0);
    pintaCuadrante(mdb_idalmacen, mdb_fechacuadrante);
    _depura("END CuadranteQTextDocument::refresh", 0);
}


///
/**
\return
**/
const QString CuadranteQTextDocument::impresion() {
    _depura("CuadranteQTextDocument::impresion", 0);

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
            html +=  "<para "+style+"><font color=\"red\" size=\"5\">" + cur1->valor("nomtipotrabajo") + ":</font></para><spacer length=\"0.1cm\"/>\n";
            oldnomtipotrabajo = cur1->valor("nomtipotrabajo");
        } // end if
        html += "<para "+style+"><font face=\"Helvetica\" size=\"6\" color=\"" + cur1->valor("colortipotrabajo") + "\">" + cur1->valor("nomtrabajador") + " " + cur1->valor("apellidostrabajador");
        html += "<sup>(" + cur1->valor("horainhorario").left(5) + "--" + cur1->valor("horafinhorario").left(5) + ")</sup></font></para><spacer length=\"0.1cm\"/>\n";

        cur1->siguienteregistro();
    } // end while
    delete cur1;

    if (cur->valor("comentcuadrante") != "") {
        html += "<para "+style+"><font face=\"Helvetica\" size=\"5\" color=\"black\">" + cur->valor("comentcuadrante").replace("\n", "<spacer length=\"0.1cm\"/>\n") + "</font></para>";
    } // end if

    delete cur;

    html += "</td>\n";
    _depura("END CuadranteQTextDocument::impresion", 0);
    return html;
}


///
/**
\param idtrabajador
\param date
\param horain
\param horafin
\return
**/
bool CuadranteQTextDocument::buscaConflictos(QString idtrabajador, const QDate &date, QString horain, QString horafin) {
	_depura ("CuadranteQTextDocument::buscaConflictos", 0);
	bool conflicto = FALSE;
	QString query = "SELECT * FROM horario NATURAL LEFT JOIN cuadrante  WHERE idtrabajador = " + idtrabajador + " AND NOT (( horafinhorario < '" + horain + "') OR (horainhorario > '" + horafin +"')) AND fechacuadrante = '" + date.toString("dd/MM/yyyy") + "'";
	cursor2 *cur = empresaBase()->cargacursor(query);
	if (cur) {
		if (cur->numregistros() > 1) {
			conflicto = TRUE;
		} // end if
		delete cur;
	}

	query = "SELECT * FROM ausencia WHERE idtrabajador = " + idtrabajador + " AND fechainausencia <= '" + date.toString("dd/MM/yyyy") + "' AND fechafinausencia >= '" + date.toString("dd/MM/yyyy") + "'";
	cur = empresaBase()->cargacursor(query);
	if (cur) {
		if (!cur->eof()) {
			conflicto = TRUE;
		} // end if
		delete cur;
	} // end if

	_depura ("END CuadranteQTextDocument::buscaConflictos", 0);
	return conflicto;
}


