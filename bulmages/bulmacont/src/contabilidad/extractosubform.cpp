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

#include <QMenu>

#include "extractosubform.h"


ExtractoSubForm::ExtractoSubForm(QWidget *parent, const char *) : SubForm2Bc(parent) {
    _depura("ExtractoSubForm::ExtractoSubForm", 0);
    setDBTableName("apunte");
    setFileConfig( "ExtractoSubForm");
    setDBCampoId("idapunte");
    addSHeader("punteo", DBCampo::DBboolean, DBCampo::DBNotNull,SHeader::DBNone, tr("punteo"));
    addSHeader("idapunte", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("idapunte"));
    addSHeader("codigoborrador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("codigoborrador"));
    addSHeader("idasiento", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idasiento"));
    addSHeader("iddiario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("iddiario"));
    addSHeader("fecha", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha"));
    addSHeader("conceptocontable", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Concepto"));
    addSHeader("idcuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idcuenta"));
    addSHeader("codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("codigo"));
    addSHeader("tipocuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("tipocuenta"));
    addSHeader("descripcion", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Cuenta"));
    addSHeader("debe", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Debe"));
    addSHeader("haber", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Haber"));
    addSHeader("contrapartida", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("contrapartida"));
    addSHeader("comentario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("comentario"));
    addSHeader("idcanal", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idcanal"));
    addSHeader("nombrecanal", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Canal"));
    addSHeader("marcaconciliacion", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("marcaconciliacion"));
    addSHeader("idc_coste", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idc_coste"));
    addSHeader("idtipoiva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idtipoiva"));
    addSHeader("orden", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("orden"));
    setinsercion(FALSE);
    _depura("END ExtractoSubForm::ExtractoSubForm", 0);
}


void ExtractoSubForm::contextMenuEvent (QContextMenuEvent *) {
    _depura("ExtractoSubForm::contextMenuEvent", 0);
    QAction *del = NULL;
    int row = currentRow();
    if (row < 0) {
        return;
    } // end if
    int col = currentColumn();
    if (row < 0) {
        return;
    } // end if
    QMenu *popup = new QMenu(this);

    QAction *mostapunte = popup->addAction("Mostrar asiento");
    popup->addSeparator();
    QAction *mostdiariodia = popup->addAction("Mostrar diario (dia)");
    QAction *mostdiariomes = popup->addAction("Mostrar diario (mes)");
    QAction *mostdiarioano = popup->addAction("Mostrar diario (ano)");
    popup->addSeparator();
    QAction *mostbalancedia = popup->addAction("Mostrar balance (dia)");
    QAction *mostbalancemes = popup->addAction("Mostrar balance (mes)");
    QAction *mostbalanceano = popup->addAction("Mostrar balance (ano)");
    popup->addSeparator();
    QAction *mostbalancejdia = popup->addAction("Mostrar balance jerarquico (dia)");
    QAction *mostbalancejmes = popup->addAction("Mostrar balance jerarquico (mes)");
    QAction *mostbalancejano = popup->addAction("Mostrar balance jerarquico (ano)");

    if (m_delete) {
        del = popup->addAction(tr("Borrar registro"));
    } // end if
    popup->addSeparator();
    QAction *ajustc = popup->addAction(tr("Ajustar columa"));
    QAction *ajustac = popup->addAction(tr("Ajustar altura"));
    QAction *ajust = popup->addAction(tr("Ajustar columnas"));
    QAction *ajusta = popup->addAction(tr("Ajustar alturas"));
    popup->addSeparator();
    QAction *verconfig = popup->addAction(tr("Ver configurador de subformulario"));
    QAction *opcion = popup->exec(QCursor::pos());

    if (opcion == mostapunte) {
        boton_asiento();
    } // end if
    if (opcion == del) {
        borrar(row);
    } // end if
    if (opcion == ajust) {
        resizeColumnsToContents();
    } // end if
    if (opcion == ajusta) {
        resizeRowsToContents();
    } // end if
    if (opcion == ajustc) {
        resizeColumnToContents(col);
    } // end if
    if (opcion == ajustac) {
        resizeRowToContents(row);
    } // end if
    if(opcion == verconfig) {
        showConfig();
    } // end if
    if (opcion == mostdiariodia) {
        boton_diario1(0);
    } // end if
    if (opcion == mostdiariomes) {
        boton_diario1(1);
    } // end if
    if (opcion == mostdiarioano) {
        boton_diario1(2);
    } // end if
    if (opcion == mostbalancedia) {
        boton_balance1(0);
    } // end if
    if (opcion == mostbalancemes) {
        boton_balance1(1);
    } // end if
    if (opcion == mostbalanceano) {
        boton_balance1(2);
    } // end if
    if (opcion == mostbalancejdia) {
        boton_balancetree(0);
    } // end if
    if (opcion == mostbalancejmes) {
        boton_balancetree(1);
    } // end if
    if (opcion == mostbalancejano) {
        boton_balancetree(2);
    } // end if
    delete popup;
    _depura("ExtractoSubForm::contextMenuEvent", 0);
}

