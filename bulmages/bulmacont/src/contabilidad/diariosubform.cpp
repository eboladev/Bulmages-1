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
#include "diariosubform.h"


DiarioSubForm::DiarioSubForm(QWidget *parent, const char *) : SubForm2Bc(parent) {
    _depura("DiarioSubForm::DiarioSubForm", 0);
    setDBTableName("borrador");
    setFileConfig( "DiarioSubForm");
    setDBCampoId("idborrador");
    addSHeader("idborrador", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("idborrador"));
    addSHeader("codigoborrador", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("codigoborrador"));
    addSHeader("ordenasiento", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("orden"));
    addSHeader("fecha", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha"));
    addSHeader("conceptocontable", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Concepto"));
    addSHeader("codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("codigo"));
    addSHeader("tipocuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("tipocuenta"));
    addSHeader("descripcion", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("descripcion"));
    addSHeader("debe", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Debe"));
    addSHeader("haber", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Haber"));
    addSHeader("contrapartida", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Contrapartida"));
    addSHeader("comentario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoWrite, tr("Comentario"));
    addSHeader("nombrecanal", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Canal"));
    addSHeader("marcaconciliacion", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("MarcaConciliacion"));
    addSHeader("nombrec_coste", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Centro Coste"));
    addSHeader("factura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Factura"));
    addSHeader("idapunte", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idapunte"));
    addSHeader("idtipoiva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idtipoiva"));
    addSHeader("idregistroiva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoView, tr("idregistroiva"));
    addSHeader("idasiento", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idasiento"));
    addSHeader("idcanal", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoView , tr("idcanal"));
    addSHeader("idc_coste", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("idc_coste"));
    addSHeader("iddiario", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("iddiario"));
    addSHeader("idcuenta", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idcuenta"));
    addSHeader("orden", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("orden"));
    setinsercion(FALSE);
    setOrdenEnabled(FALSE);
    setSortingEnabled(TRUE);
    _depura("DiarioSubForm::DiarioSubForm", 0);
}


void DiarioSubForm::contextMenuEvent(QContextMenuEvent *) {
    _depura("DiarioSubForm::contextMenuEvent", 0);
    QAction *del = NULL;
    int row = currentRow();
    if (row < 0) {
        return;
    } // end if
    int col = currentColumn();

    QMenu *popup = new QMenu(this);
    QAction *mostapunte = popup->addAction("Mostrar asiento");
    popup->addSeparator();
    QAction *mostextractodia = popup->addAction("Mostrar extracto (dia)");
    QAction *mostextractomes = popup->addAction("Mostrar extracto (mes)");
    QAction *mostextractoano = popup->addAction("Mostrar extracto (ano)");
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
    if (opcion == verconfig) {
        showConfig();
    } // end if
    if (opcion == mostextractodia) {
        boton_extracto1(0);
    } // end if
    if (opcion == mostextractomes) {
        boton_extracto1(1);
    } // end if
    if (opcion == mostextractoano) {
        boton_extracto1(2);
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
    _depura("END DiarioSubForm::contextMenuEvent", 0);
}

