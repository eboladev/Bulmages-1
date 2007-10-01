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


///
/**
\param parent
**/
DiarioSubForm::DiarioSubForm(QWidget *parent, const char *) : SubForm2Bc(parent) {
    _depura("DiarioSubForm::DiarioSubForm", 0);
    setDBTableName("borrador");
    setFileConfig( "DiarioSubForm");
    setDBCampoId("idborrador");
    addSHeader("idasiento", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idasiento"));
    addSHeader("ordenasientoconfecha", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Asiento"));
    addSHeader("fecha", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha"));
    addSHeader("codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Cuenta"));
    addSHeader("descripcion", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion"));
    addSHeader("conceptocontable", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Concepto"));
    addSHeader("debe", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Debe"));
    addSHeader("haber", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Haber"));
    addSHeader("comentario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoWrite, tr("Comentario"));
    addSHeader("idborrador", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("idborrador"));
    addSHeader("codigoborrador", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("codigoborrador"));
    addSHeader("tipocuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("tipocuenta"));
    addSHeader("descripcioncuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion de Cuenta"));
    addSHeader("contrapartida", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Contrapartida"));
    addSHeader("codcontrapartida", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Contrapartida"));
    addSHeader("nombrecanal", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Canal"));
    addSHeader("marcaconciliacion", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("MarcaConciliacion"));
    addSHeader("nombrec_coste", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Centro Coste"));
    addSHeader("factura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Factura"));
    addSHeader("idapunte", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idapunte"));
    addSHeader("idtipoiva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idtipoiva"));
    addSHeader("idregistroiva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoView, tr("idregistroiva"));
    addSHeader("idcanal", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoView , tr("idcanal"));
    addSHeader("idc_coste", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("idc_coste"));
    addSHeader("iddiario", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("iddiario"));
    addSHeader("idcuenta", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("idcuenta"));
    addSHeader("orden", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("orden"));
    /// Van juntas. Desactivas un tipo de pijama y se activa otro personalizado al hacer 'rowspan'.
    setListadoPijama(FALSE);
    setColumnToRowSpan("ordenasientoconfecha");
    setColorFondo1(confpr->valor(CONF_BG_DIARIO));
    setColorFondo2(confpr->valor(CONF_BG_ALT_DIARIO));
    setinsercion(FALSE);
    setOrdenEnabled(FALSE);
    setOrdenPorQuery(TRUE);
    /// Preparamos el sistema de menus contextuales
    connect(this, SIGNAL(pintaMenu(QMenu *)), this, SLOT(s_pintaMenu(QMenu *)));
    connect(this, SIGNAL(trataMenu(QAction *)), this, SLOT(s_trataMenu(QAction *)));
    _depura("DiarioSubForm::DiarioSubForm", 0);
}


///
/**
**/
DiarioSubForm::~DiarioSubForm() {
    _depura("DiarioSubForm::~DiarioSubForm", 0);
    _depura("END DiarioSubForm::~DiarioSubForm", 0);
}


/// Slot que trata la solicitud de pintar el menu contextual sobre el subformulario.
/**
\param menu
**/
void DiarioSubForm::s_pintaMenu(QMenu *menu) {
    _depura("ListLinAsiento1View::s_pintaMenu", 0);
    menu->addSeparator();
    menu->addAction(tr("Mostrar asiento"));
    menu->addSeparator();
    menu->addAction(tr("Mostrar extracto (dia)"));
    menu->addAction(tr("Mostrar extracto (mes)"));
    menu->addAction(tr("Mostrar extracto (ano)"));
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
void DiarioSubForm::s_trataMenu(QAction *action) {
    _depura("ListLinAsiento1View::s_trataMenu", 0);
    if (!action) return;
    if (action->text() == tr("Mostrar asiento"))
        boton_asiento();
    if (action->text() == tr("Mostrar extracto (dia)"))
        boton_extracto1(0);
    if (action->text() == tr("Mostrar extracto (mes)"))
        boton_extracto1(1);
    if (action->text() == tr("Mostrar extracto (ano)"))
        boton_extracto1(2);
    if (action->text() == tr("Mostrar balance (dia)"))
        boton_balance1(0);
    if (action->text() == tr("Mostrar balance (mes)"))
        boton_balance1(1);
    if (action->text() == tr("Mostrar balance (ano)"))
        boton_balance1(2);
    _depura("END ListLinAsiento1View::s_trataMenu", 0);
}


