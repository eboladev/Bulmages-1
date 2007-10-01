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


///
/**
**/
ExtractoSubForm::~ExtractoSubForm() {
    _depura("ExtractoSubForm::~ExtractoSubForm", 0);
    _depura("END ExtractoSubForm::~ExtractoSubForm", 0);
}


///
/**
\param parent
**/
ExtractoSubForm::ExtractoSubForm(QWidget *parent, const char *) : SubForm2Bc(parent) {
    _depura("ExtractoSubForm::ExtractoSubForm", 0);
    setDBTableName("apunte");
    setFileConfig( "ExtractoSubForm");
    setDBCampoId("idapunte");
    addSHeader("punteo", DBCampo::DBboolean, DBCampo::DBNotNull,SHeader::DBNone, tr("Punteo"));
    addSHeader("fecha", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite | SHeader::DBBlockView, tr("Fecha"));
    addSHeader("codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Cuenta"));
    addSHeader("ordenasiento", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite | SHeader::DBBlockView, tr("Asiento"));
    addSHeader("descripcioncuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion"));
    addSHeader("conceptocontable", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Concepto"));
    addSHeader("debe", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite | SHeader::DBBlockView, tr("Debe"));
    addSHeader("haber", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite | SHeader::DBBlockView, tr("Haber"));
    addSHeader("saldo", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Saldo"));

    addSHeader("idapunte", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("idapunte"));
    addSHeader("codigoborrador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("codigoborrador"));
    addSHeader("idasiento", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idasiento"));
    addSHeader("iddiario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("iddiario"));
    addSHeader("idcuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idcuenta"));
    addSHeader("tipocuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("tipocuenta"));
    addSHeader("contrapartida", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Id contrapartida"));
    addSHeader("codcontrapartida", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("contrapartida"));
    addSHeader("comentario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentario"));
    addSHeader("idcanal", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idcanal"));
    addSHeader("nombrecanal", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Canal"));
    addSHeader("marcaconciliacion", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("marcaconciliacion"));
    addSHeader("idc_coste", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idc_coste"));
    addSHeader("nombrec_coste", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Centro Coste"));
    addSHeader("idtipoiva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idtipoiva"));
    addSHeader("orden", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Orden"));
    setinsercion(FALSE);
    setOrdenEnabled(TRUE);
    setOrdenPorQuery(FALSE);

    connect(this, SIGNAL(pintaMenu(QMenu *)), this, SLOT(s_pintaMenu(QMenu *)));
    connect(this, SIGNAL(trataMenu(QAction *)), this, SLOT(s_trataMenu(QAction *)));

    _depura("END ExtractoSubForm::ExtractoSubForm", 0);
}



/// Slot que trata la solicitud de pintar el menu contextual sobre el subformulario.
/**
\param menu
**/
void ExtractoSubForm::s_pintaMenu(QMenu *menu) {
    _depura("ListLinAsiento1View::s_pintaMenu", 0);
    menu->addSeparator();
    menu->addAction(tr("Mostrar asiento"));
    menu->addSeparator();
    menu->addAction(tr("Mostrar diario (dia)"));
    menu->addAction(tr("Mostrar diario (mes)"));
    menu->addAction(tr("Mostrar diario (ano)"));
    menu->addSeparator();
    menu->addAction(tr("Mostrar balance (dia)"));
    menu->addAction(tr("Mostrar balance (mes)"));
    menu->addAction(tr("Mostrar balance (ano)"));
    _depura("ListLinAsiento1View::s_pintaMenu", 0);
}

/// Slot que trata la activacion de un elemento en el menu contextual.
/**
\param action
/return
**/
void ExtractoSubForm::s_trataMenu(QAction *action) {
    _depura("ListLinAsiento1View::s_trataMenu", 0);
    if (!action) return;
    if (action->text() == tr("Mostrar asiento"))
        boton_asiento();
    if (action->text() == tr("Mostrar diario (dia)"))
        boton_diario1(0);
    if (action->text() == tr("Mostrar diario (mes)"))
        boton_diario1(1);
    if (action->text() == tr("Mostrar diario (ano)"))
        boton_diario1(2);
    if (action->text() == tr("Mostrar balance (dia)"))
        boton_balance1(0);
    if (action->text() == tr("Mostrar balance (mes)"))
        boton_balance1(1);
    if (action->text() == tr("Mostrar balance (ano)"))
        boton_balance1(2);
    _depura("END ListLinAsiento1View::s_trataMenu", 0);
}

