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

#include "amortizacionesview.h"
#include "amortizacionview.h"

#include <empresa.h>

#define COL_CODIGO 0
#define COL_NOMBRE 1


amortizacionesview::amortizacionesview(empresa *emp, QWidget *parent, const char *name)
        : QWidget(parent, name, Qt::WDestructiveClose) {
    _depura("amortizacionesview::amortizacionesview", 0);
    setupUi(this);
    m_companyact = emp;
    inicializatabla();
    modo = 0;
    m_companyact->meteWindow(caption(), this);
    _depura("END amortizacionesview::amortizacionesview", 0);
}


amortizacionesview::~amortizacionesview() {
    _depura("amortizacionesview::~amortizacionesview", 0);
    m_companyact->sacaWindow(this);
    _depura("END amortizacionesview::~amortizacionesview", 0);
}


void amortizacionesview::inicializatabla()  {
    _depura("amortizacionesview::inicializatabla", 0);
    /// Para el listado de columnas hacemos una inicializaci&oacute;n.
    QStringList headers;
    headers << "id" << "nombre" ;
    listado->setHorizontalHeaderLabels(headers);
    listado->setColumnCount(2);
    string query = "SELECT * FROM amortizacion ORDER BY nomamortizacion";
    cursor2 *cursoraux1 = m_companyact->cargacursor(query.c_str(), "elquery");
    listado->setRowCount(cursoraux1->numregistros());
    int i = 0;
    while (!cursoraux1->eof()) {
        QTableWidgetItem *it = new QTableWidgetItem(cursoraux1->valor("idamortizacion"));
        listado->setItem(i, COL_CODIGO, it);
        it = new QTableWidgetItem(cursoraux1->valor("nomamortizacion"));
        listado->setItem(i, COL_NOMBRE, it);
        cursoraux1->siguienteregistro ();
        i++;
    } // end while
    delete cursoraux1;
    _depura("END amortizacionesview::inicializatabla", 0);
}


void amortizacionesview::on_listado_cellDoubleClicked(int row, int) {
    _depura("amortizacionesview::on_listado_cellDoubleClicked", 0);
    /// Dependiendo del modo hacemos una cosa u otra.
    if (modo == 0) {
        idamortizacion = listado->item(row,COL_CODIGO)->text();
        /// Creamos el objeto mpatrimonialview, y lo lanzamos.
        amortizacionview *amor = new amortizacionview(m_companyact, 0, "");
        amor->inicializa(idamortizacion);
        m_companyact->pWorkspace()->addWindow(amor);
        amor->show();
    } else {
        idamortizacion = listado->item(listado->currentRow(), COL_CODIGO)->text();
        nomamortizacion = listado->item(listado->currentRow(), COL_NOMBRE)->text();
        close();
    } // end if
    _depura("END amortizacionesview::on_listado_cellDoubleClicked", 0);
}


void amortizacionesview::on_mui_crear_clicked() {
    _depura("amortizacionesview::on_mui_crear_clicked", 0);
    amortizacionview *amor = new amortizacionview(m_companyact, 0, "");
    m_companyact->pWorkspace()->addWindow(amor);
    amor->show();
    _depura("END amortizacionesview::on_mui_crear_clicked", 0);

}


/// Esta funci&oacute;n se encarga de borrar una amortizaci&oacute;n.
/// La que esta seleccionada en el listado.
void amortizacionesview::on_mui_borrar_clicked() {
    _depura("amortizacionesview::on_mui_borrar_clicked", 0);
    QString codigo = listado->item(listado->currentRow(), COL_CODIGO)->text();
    if (codigo != "") {
        QString query = "DELETE FROM linamortizacion WHERE idamortizacion = " + codigo;
        m_companyact->begin();
        m_companyact->ejecuta(query);
        m_companyact->commit();
        query = "DELETE FROM amortizacion WHERE idamortizacion = " + codigo;
        m_companyact->begin();
        m_companyact->ejecuta(query);
        m_companyact->commit();
        inicializatabla();
    } // end if
    _depura("END amortizacionesview::on_mui_borrar_clicked", 0);
}

