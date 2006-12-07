/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include <QMessageBox>
#include <QMenu>
#include <QKeyEvent>
#include <QEvent>

#include "subform3.h"
#include "subform2bc.h"
#include "funcaux.h"
#include "empresa.h"
#include "asiento1view.h"
#include "extractoview1.h"
#include "balance1view.h"

/// Incluimos las imagenes que catalogan los tipos de cuentas.
#include "images/cactivo.xpm"
#include "images/cpasivo.xpm"
#include "images/cneto.xpm"
#include "images/cingresos.xpm"
#include "images/cgastos.xpm"


SubForm2Bc::SubForm2Bc(QWidget *parent) : SubForm3(parent) {}


void SubForm2Bc::on_mui_list_pressedAsterisk(int row, int col) {
    _depura ("SubForm2Bc::on_mui_list_pressedAsterisk", 0);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row, col);
    /// Si no es un campo de tipo codigo salimos.
    if (camp->nomcampo() != "codigo" && camp->nomcampo() != "codigoctacliente")
        return;

    QDialog *diag = new QDialog(0);
    diag->setModal(true);
    listcuentasview1 *listcuentas = new listcuentasview1((empresa *)m_companyact, diag, 0, listcuentasview1::SelectMode);
    connect(listcuentas, SIGNAL(selected(QString)), diag, SLOT(accept()));
    diag->exec();
    if (listcuentas->codcuenta() != "") {
        cursor2 *cur = companyact()->cargacursor("SELECT * FROM cuenta WHERE codigo = '" + listcuentas->codcuenta() + "'");
        if (!cur->eof() ) {
            if (camp->nomcampo() == "codigo") {
                rec->setDBvalue("idcuenta", cur->valor("idcuenta"));
                rec->setDBvalue("codigo", cur->valor("codigo"));
                rec->setDBvalue("tipocuenta", cur->valor("tipocuenta"));
                rec->setDBvalue("descripcion", cur->valor("descripcion"));
            } // end if
            if (camp->nomcampo() == "codigoctacliente") {
                rec->setDBvalue("idctacliente", cur->valor("idcuenta"));
                rec->setDBvalue("codigoctacliente", cur->valor("codigo"));
                rec->setDBvalue("tipoctacliente", cur->valor("tipocuenta"));
                rec->setDBvalue("nomctacliente", cur->valor("descripcion"));
            } // end if
        } // end if
        delete cur;
    } // end if
    delete diag;
    _depura ("END SubForm2Bc::on_mui_list_pressedAsterisk", 0);
}


void SubForm2Bc::on_mui_list_pressedSlash(int row, int col) {
    _depura("SubForm2Bc::pressedSlash", 0);
    SDBCampo *camp = (SDBCampo *) item(row, col);
    if (camp->nomcampo() == "fecha")
        return;
    QString text = editaTexto(camp->text());
    camp->set
    (text);
}

void SubForm2Bc::on_mui_list_editFinished(int row, int col, int key) {
    _depura("SubForm2Bc::editFinished", 0);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row, col);
    camp->refresh();
    if (camp->nomcampo() == "codigo") {
        QString codigoext = extiendecodigo(camp->text(), ((empresa *) m_companyact)->numdigitosempresa());
        cursor2 *cur = companyact()->cargacursor("SELECT * FROM cuenta WHERE codigo = '" + codigoext + "'");
        if (!cur->eof() ) {
            rec->setDBvalue("idcuenta", cur->valor("idcuenta"));
            rec->setDBvalue("codigo", cur->valor("codigo"));
            rec->setDBvalue("tipocuenta", cur->valor("tipocuenta"));
            rec->setDBvalue("descripcioncuenta", cur->valor("descripcion"));
        } // end if
        delete cur;
    } // end if
    if (camp->nomcampo() == "fecha") {
        QString nfecha = normalizafecha( camp->text()).toString("dd/MM/yyyy");
        rec->setDBvalue( "fecha", nfecha);
    } // end if
    SubForm3::on_mui_list_editFinished(row, col, key);
    _depura("END SubForm2Bc::editFinished", 0);
}


void SubForm2Bc::boton_asiento() {
    _depura("SubForm2Bc::boton_asiento", 0);
    empresa *companyact = (empresa *) m_companyact;
    QString numasiento = DBvalue("idasiento");
    if (numasiento != "") {
        companyact->intapuntsempresa()->muestraasiento(numasiento.toInt());
        companyact->muestraapuntes1();
    } // end if
    _depura("END SubForm2Bc::boton_asiento", 0);
}



/// Si el parametro pasado es un:
/// 0 -> del d&iacute;a actual
/// 1 -> del mes actual
/// 2 -> del a&ntilde;o actual
void SubForm2Bc::boton_extracto1(int tipo) {
    _depura("SubForm2Bc::boton_extracto1", 0);
    empresa *companyact = (empresa *) m_companyact;
    QDate fecha1, fecha2, fechaact;
    QString fecha = DBvalue("fecha").left(10);
    QString codigo = DBvalue("codigo");
    if (fecha != "" && codigo != "") {
        fechaact = normalizafecha(fecha);
        switch (tipo) {
        case 0:
            fecha1.setYMD(fechaact.year(), fechaact.month(), fechaact.day());
            fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.day());
            break;
        case 1:
            fecha1.setYMD(fechaact.year(), fechaact.month(), 1);
            fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.daysInMonth());
            break;
        case 2:
            fecha1.setYMD(fechaact.year(), 1, 1);
            fecha2.setYMD(fechaact.year(), 12, 31);
            break;
        } // end switch
        companyact->extractoempresa()->inicializa1(codigo, codigo, fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
        companyact->extractoempresa()->accept();
        companyact->libromayor();
    } // end if
    _depura("END SubForm2Bc::boton_extracto1", 0);
}


/// Si el parametro pasado es un:
/// 0 -> del d&iacute;a actual
/// 1 -> del mes actual
/// 2 -> del a&ntilde;o actual
void SubForm2Bc::boton_diario1(int tipo) {
    _depura("SubForm2Bc::boton_diario1", 0);
    empresa *companyact = (empresa *) m_companyact;
    QDate fecha1, fecha2, fechaact, fechaact1;
    fechaact = normalizafecha(DBvalue("fecha").left(10));
    fechaact1 = normalizafecha(DBvalue("fecha").left(10));
    if (DBvalue("fecha").left(10) != "") {
        switch (tipo) {
        case 0:
            fecha1.setYMD(fechaact.year(), fechaact.month(), fechaact.day());
            fecha2.setYMD(fechaact1.year(), fechaact1.month(), fechaact1.day());
            break;
        case 1:
            fecha1.setYMD(fechaact.year(), fechaact.month(), 1);
            fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.daysInMonth());
            break;
        case 2:
            fecha1.setYMD(fechaact.year(), 1, 1);
            fecha2.setYMD(fechaact.year(), 12, 31);
            break;
        } // end switch
        companyact->diarioempresa()->inicializa1((char *) fecha1.toString("dd/MM/yyyy").toAscii().constData(), (char *) fecha2.toString("dd/MM/yyyy").toAscii().constData(), 0);
    } // end if
    companyact->diarioempresa()->accept();
    companyact->librodiario();
    _depura("SubForm2Bc::boton_diario1", 0);
}


/// Si el parametro pasado es un:
/// 0 -> del per&iacute;odo actual.
/// 1 -> del mes actual mirado a partir de la fecha de inicio.
/// 2 -> del a&ntilde;o actual mirado a partir de la fecha de inicio.
void SubForm2Bc::boton_balance1(int tipo) {
    _depura("SubForm2Bc::boton_balance1", 0);
    empresa *companyact = (empresa *) m_companyact;
    QString fecha = DBvalue("fecha").left(10);
    QString codigo = DBvalue("codigo");
    QDate fecha1, fecha2, fechaact, fechaact1;
    if (fecha != "" && codigo != "") {
        fechaact = normalizafecha(fecha);
        switch (tipo) {
        case 0:
            fecha1.setYMD(fechaact.year(), fechaact.month(), fechaact.day());
            fecha2.setYMD(fechaact1.year(), fechaact.month(), fechaact.day());
            break;
        case 1:
            fecha1.setYMD(fechaact.year(), fechaact.month(), 1);
            fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.daysInMonth());
            break;
        case 2:
            fecha1.setYMD(fechaact.year(), 1, 1);
            fecha2.setYMD(fechaact.year(), 12, 31);
            break;
        } // end switch
        companyact->balanceempresa()->inicializa1(codigo, codigo, fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
        companyact->balanceempresa()->accept();
        companyact->librobalance();
    } // end if
    _depura("END SubForm2Bc::boton_balance1", 0);
}


/// Si el parametro pasado es un:
/// 0 -> del per&iacute;odo actual.
/// 1 -> del mes actual mirado a partir de la fecha de inicio.
/// 2 -> del a&ntilde;o actual mirado a partir de la fecha de inicio.
void SubForm2Bc::boton_balancetree(int tipo) {
    _depura("SubForm2Bc::boton_balance2",0);
    empresa *companyact = (empresa *) m_companyact;
    QString fecha = DBvalue("fecha").left(10);
    QString codigo = DBvalue("codigo");
    QDate fecha1, fecha2, fechaact, fechaact1;
    if (fecha != "" && codigo != "") {
        fechaact = normalizafecha(fecha);
        switch (tipo) {
        case 0:
            fecha1.setYMD(fechaact.year(), fechaact.month(), fechaact.day());
            fecha2.setYMD(fechaact1.year(), fechaact.month(), fechaact.day());
            break;
        case 1:
            fecha1.setYMD(fechaact.year(), fechaact.month(), 1);
            fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.daysInMonth());
            break;
        case 2:
            fecha1.setYMD(fechaact.year(), 1, 1);
            fecha2.setYMD(fechaact.year(), 12, 31);
            break;
        } // end switch
        companyact->balance1empresa()->inicializa1(codigo, codigo, fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
        companyact->balance1empresa()->accept();
        companyact->librobalancetree();
    } // end if
    _depura("END SubForm2Bc::boton_balance2", 0);
}


void SubForm2Bc::creaMenu(QMenu *menu) {
    _depura("SubForm2Bc::pintaMenu", 0);
    QAction *ac = menu->addAction(tr("Submenu de contabilidad"));
    menu->addSeparator();
    _depura("END SubForm2Bc::pintaMenu", 0);
}


void SubForm2Bc::procesaMenu(QAction *ac) {
    _depura("SubForm2Bc::procesaMenu", 0);
    _depura("END SubForm2Bc::procesaMenu", 0);
}

