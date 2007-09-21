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

#include <QMenu>

#include "listlinasiento1view.h"
#include "plugins.h"
#include "empresa.h"


/// Construye la cabecera del asiento (nombra cada columna de la tabla)
ListLinAsiento1View::ListLinAsiento1View(QWidget *parent, const char *)
        : SubForm2Bc(parent) {
    _depura("ListLinAsiento1View::ListLinAsiento1View", 0);
    setDBTableName("borrador");
    setFileConfig("asientotabla");
    setDBCampoId("idborrador");
    addSHeader("idapunte", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("Id apunte"));
    addSHeader("fecha", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, tr("Fecha"));
    addSHeader("codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Codigo"));
    addSHeader("descripcioncuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("Nombre de la cuenta"));
    addSHeader("descripcion", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, tr("Descripcion del apunte"));
    addSHeader("conceptocontable", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, tr("Concepto contable"));
    addSHeader("debe", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Debe"));
    addSHeader("haber", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Haber"));
    addSHeader("contrapartida", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("Contrapartida"));
    addSHeader("comentario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("Comentario"));
    addSHeader("idcanal", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNoWrite | SHeader::DBNoView, tr("ID Canal"));
    addSHeader("nomcanal", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Canal"));
    addSHeader("marcaconciliacion", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("Conciliacion"));
    addSHeader("idc_coste", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNoWrite | SHeader::DBNoView, tr("Id centro de coste"));
    addSHeader("nomc_coste", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("Centro de coste"));
    addSHeader("idtipoiva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("Id tipo de IVA"));
    addSHeader("orden", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("Orden"));
    addSHeader("idborrador", DBCampo::DBint,  DBCampo::DBPrimaryKey, SHeader::DBNoWrite | SHeader::DBNoView, tr("Id borrador"));
    addSHeader("idasiento", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNoWrite | SHeader::DBNoView, tr("Id asiento"));
    addSHeader("idcuenta", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoWrite | SHeader::DBNoView, tr("Id cuenta"));
    addSHeader("tipocuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("Tipo de cuenta"));
    setinsercion(TRUE);
    setOrdenEnabled(FALSE);

    connect(this, SIGNAL(pintaMenu(QMenu *)), this, SLOT(s_pintaMenu(QMenu *)));
    connect(this, SIGNAL(trataMenu(QAction *)), this, SLOT(s_trataMenu(QAction *)));
    _depura("END ListLinAsiento1View::ListLinAsiento1View", 0);
}


/// Destructor de la clase. No hace nada especial.
ListLinAsiento1View::~ListLinAsiento1View() {
    _depura("ListLinAsiento1View::~ListLinAsiento1View", 0);
    _depura("END ListLinAsiento1View::~ListLinAsiento1View", 0);
}


/// Slot que trata la solicitud de pintar el menu contextual sobre el subformulario.
void ListLinAsiento1View::s_pintaMenu(QMenu *menu) {
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
void ListLinAsiento1View::s_trataMenu(QAction *action) {
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

/// Carga lineas de asiento (apuntes).
void ListLinAsiento1View::cargar(QString idasiento) {
    _depura("AsientoSubForm::cargar", 0);
    QString SQLQuery = "SELECT * FROM borrador ";
    SQLQuery += " LEFT JOIN (SELECT codigo, descripcion AS descripcioncuenta, idcuenta, tipocuenta FROM cuenta) AS t1 ON t1.idcuenta = borrador.idcuenta ";
    SQLQuery += " LEFT JOIN (SELECT idcanal, nombre AS nomcanal, descripcion AS descanal FROM canal) AS t2 ON borrador.idcanal = t2.idcanal ";
    SQLQuery += " LEFT JOIN (SELECT idc_coste, nombre AS nomc_coste, descripcion AS descc_coste FROM c_coste) AS t3 ON borrador.idc_coste = t3.idc_coste ";
    SQLQuery += " LEFT JOIN (SELECT idregistroiva, factura, ffactura, idborrador FROM registroiva) AS t4 ON borrador.idborrador = t4.idborrador ";
    SQLQuery += "WHERE idasiento = " + idasiento + " ORDER BY orden";
    SubForm2Bc::cargar(SQLQuery);
    _depura("END AsientoSubForm::cargar", 0);
}


/// Devuelve el DEBE de un asiento.
/// Si el asiento no existe devuelve 0
Fixed ListLinAsiento1View::totaldebe(QString idasiento) {
    _depura("ListLinAsiento1View::totaldebe", 0, idasiento);
    if (idasiento == "") return Fixed("0");
    QString SQLQuery = "SELECT sum(debe) FROM borrador LEFT JOIN (SELECT codigo, descripcion AS descripcioncuenta, idcuenta, tipocuenta FROM cuenta) AS t1 ON t1.idcuenta = borrador.idcuenta LEFT JOIN (SELECT idcanal, nombre AS nombrecanal, descripcion AS descripcioncanal FROM canal) AS t2 ON borrador.idcanal = t2.idcanal LEFT JOIN (SELECT idc_coste, nombre AS nombrec_coste, descripcion AS descripcionc_coste FROM c_coste) AS t3 ON borrador.idc_coste = t3.idc_coste LEFT JOIN (SELECT idregistroiva, factura, ffactura, idborrador FROM registroiva) AS t4 ON borrador.idborrador = t4.idborrador WHERE idasiento = " + idasiento;
    cursor2 *cur= empresaBase()->cargacursor(SQLQuery);
    if (!cur) return Fixed("0");
    QString resultado = cur->valor(0);
    delete cur;
    _depura("END ListLinAsiento1View::totaldebe", 0);
    return Fixed(resultado);
}


/// Devuelve el HABER de un asiento.
Fixed ListLinAsiento1View::totalhaber(QString idasiento) {
    _depura("ListLinAsiento1View::totalhaber", 0, idasiento);
    if (idasiento == "") return Fixed("0");
    QString SQLQuery = "SELECT sum(haber) FROM borrador LEFT JOIN (SELECT codigo, descripcion AS descripcioncuenta, idcuenta, tipocuenta FROM cuenta) AS t1 ON t1.idcuenta=borrador.idcuenta LEFT JOIN (SELECT idcanal, nombre AS nombrecanal, descripcion AS descripcioncanal FROM canal) AS t2 ON borrador.idcanal = t2.idcanal LEFT JOIN (SELECT idc_coste, nombre AS nombrec_coste, descripcion AS descripcionc_coste FROM c_coste) AS t3 ON borrador.idc_coste = t3.idc_coste LEFT JOIN (SELECT idregistroiva, factura, ffactura, idborrador FROM registroiva) AS t4 ON borrador.idborrador = t4.idborrador WHERE idasiento = " + idasiento;
    cursor2 *cur = empresaBase()->cargacursor(SQLQuery);
    if(!cur) return Fixed("0");
    QString resultado = cur->valor(0);
    delete cur;
    _depura("END ListLinAsiento1View::totalhaber", 0);
    return Fixed(resultado);
}

