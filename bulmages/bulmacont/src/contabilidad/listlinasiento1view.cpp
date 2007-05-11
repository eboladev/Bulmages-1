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


/// Construye la cabecera del asiento (nombra cada columna de la tabla)
ListLinAsiento1View::ListLinAsiento1View(QWidget *parent, const char *)
        : SubForm2Bc(parent) {
    _depura("ListLinAsiento1View::ListLinAsiento1View", 0);
    setDBTableName("borrador");
    setFileConfig("asientotabla");
    setDBCampoId("idborrador");
    addSHeader("fecha", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, tr("Fecha"));
    addSHeader("codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Codigo"));
    addSHeader("descripcioncuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("Descripcion de la cuenta"));
    addSHeader("descripcion", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, tr("Descripcion"));
    addSHeader("conceptocontable", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, tr("Concepto contable"));
    addSHeader("debe", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Debe"));
    addSHeader("haber", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Haber"));
    addSHeader("contrapartida", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("Contrapartida"));
    addSHeader("comentario", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("Comentario"));
    addSHeader("idcanal", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("ID Canal"));
    addSHeader("marcaconciliacion", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("Conciliacion"));
    addSHeader("idc_coste", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("Id centro de coste"));
    addSHeader("idapunte", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("Id apunte"));
    addSHeader("idtipoiva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("Id tipo de IVA"));
    addSHeader("orden", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("Orden"));
    addSHeader("idborrador", DBCampo::DBint,  DBCampo::DBPrimaryKey, SHeader::DBNoWrite | SHeader::DBNoView, tr("Id borrador"));
    addSHeader("idasiento", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNoWrite | SHeader::DBNoView, tr("Id asiento"));
//    addSHeader("idcuenta", DBCampo::DBvarchar, DBCampo::DBNotNull | DBCampo::DBRequired, SHeader::DBNoWrite | SHeader::DBNoView, tr("Id cuenta"));
    addSHeader("idcuenta", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNoWrite | SHeader::DBNoView, tr("Id cuenta"));
    addSHeader("tipocuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("Tipo de cuenta"));
    setinsercion(TRUE);
    _depura("END ListLinAsiento1View::ListLinAsiento1View", 0);
}


ListLinAsiento1View::~ListLinAsiento1View() {
    _depura("ListLinAsiento1View::~ListLinAsiento1View"), 0;
}


/// Fabrica el men&uacute; contextual.
void ListLinAsiento1View::contextMenuEvent(QContextMenuEvent *) {
    _depura("SubForm2Bc::contextMenuEvent", 0);
    QAction *del = NULL;
    int row = currentRow();
    if (row < 0)
        return;

    int col = currentColumn();
    if (row < 0)
        return;

    QMenu *popup = new QMenu(this);

    QAction *mostapunte = popup->addAction(tr("Mostrar asiento"));
    popup->addSeparator();
    QAction *mostextractodia = popup->addAction(tr("Mostrar extracto (dia)"));
    QAction *mostextractomes = popup->addAction(tr("Mostrar extracto (mes)"));
    QAction *mostextractoano = popup->addAction(tr("Mostrar extracto (ano)"));
    popup->addSeparator();
    QAction *mostbalancedia = popup->addAction(tr("Mostrar balance (dia)"));
    QAction *mostbalancemes = popup->addAction(tr("Mostrar balance (mes)"));
    QAction *mostbalanceano = popup->addAction(tr("Mostrar balance (ano)"));
    popup->addSeparator();
    QAction *mostbalancejdia = popup->addAction(tr("Mostrar balance jerarquico (dia)"));
    QAction *mostbalancejmes = popup->addAction(tr("Mostrar balance jerarquico (mes)"));
    QAction *mostbalancejano = popup->addAction(tr("Mostrar balance jerarquico (ano)"));

    if (m_delete)
        del = popup->addAction(tr("Borrar registro"));
    popup->addSeparator();
    QAction *ajustc = popup->addAction(tr("Ajustar columa"));
    QAction *ajustac = popup->addAction(tr("Ajustar altura"));
    QAction *ajust = popup->addAction(tr("Ajustar columnas"));
    QAction *ajusta = popup->addAction(tr("Ajustar alturas"));
    popup->addSeparator();
    QAction *verconfig = popup->addAction(tr("Ver configurador de subformulario"));
    QAction *opcion = popup->exec(QCursor::pos());

    if (opcion == mostapunte)
        boton_asiento();
    if (opcion == del)
        borrar(row);
    if (opcion == ajust)
        resizeColumnsToContents();
    if (opcion == ajusta)
        resizeRowsToContents();
    if (opcion == ajustc)
        resizeColumnToContents(col);
    if (opcion == ajustac)
        resizeRowToContents(row);
    if (opcion == verconfig)
        showConfig();
    if (opcion == mostextractodia)
        boton_extracto1(0);
    if (opcion == mostextractomes)
        boton_extracto1(1);
    if (opcion == mostextractoano)
        boton_extracto1(2);
    if (opcion == mostbalancedia)
        boton_balance1(0);
    if (opcion == mostbalancemes)
        boton_balance1(1);
    if (opcion == mostbalanceano)
        boton_balance1(2);
    if (opcion == mostbalancejdia)
        boton_balancetree(0);
    if (opcion == mostbalancejmes)
        boton_balancetree(1);
    if (opcion == mostbalancejano)
        boton_balancetree(2);

    delete popup;
    _depura("END SubForm2Bc::contextMenuEvent", 0);
}


/// Carga lineas de asiento (apuntes).
void ListLinAsiento1View::cargar(QString idasiento) {
    _depura("AsientoSubForm::cargar", 0);
    QString SQLQuery = "SELECT * FROM borrador ";
    SQLQuery += " LEFT JOIN (SELECT codigo, descripcion AS descripcioncuenta, idcuenta, tipocuenta FROM cuenta) AS t1 ON t1.idcuenta = borrador.idcuenta ";
    SQLQuery += " LEFT JOIN (SELECT idcanal, nombre AS nombrecanal, descripcion AS descripcioncanal FROM canal) AS t2 ON borrador.idcanal = t2.idcanal ";
    SQLQuery += " LEFT JOIN (SELECT idc_coste, nombre AS nombrec_coste, descripcion AS descripcionc_coste FROM c_coste) AS t3 ON borrador.idc_coste = t3.idc_coste ";
    SQLQuery += " LEFT JOIN (SELECT idregistroiva, factura, ffactura, idborrador FROM registroiva) AS t4 ON borrador.idborrador = t4.idborrador ";
    SQLQuery += "WHERE idasiento = " + idasiento + " ORDER BY orden";
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    SubForm2Bc::cargar(cur);
    delete cur;
    _depura("END AsientoSubForm::cargar", 0);
}


void ListLinAsiento1View::boton_iva() {
    _depura("ListLinAsiento1View::boton_iva", 0);
    g_plugins->lanza("ListLinAsiento1View_boton_iva", this);
    _depura("END ListLinAsiento1View::boton_iva", 0);

}


/// Devuelve el DEBE de un asiento.
Fixed ListLinAsiento1View::totaldebe(QString idasiento) {
    _depura("ListLinAsiento1View::totaldebe", 0);
    QString SQLQuery = "SELECT sum(debe) FROM borrador LEFT JOIN (SELECT codigo, descripcion AS descripcioncuenta, idcuenta, tipocuenta FROM cuenta) AS t1 ON t1.idcuenta = borrador.idcuenta LEFT JOIN (SELECT idcanal, nombre AS nombrecanal, descripcion AS descripcioncanal FROM canal) AS t2 ON borrador.idcanal = t2.idcanal LEFT JOIN (SELECT idc_coste, nombre AS nombrec_coste, descripcion AS descripcionc_coste FROM c_coste) AS t3 ON borrador.idc_coste = t3.idc_coste LEFT JOIN (SELECT idregistroiva, factura, ffactura, idborrador FROM registroiva) AS t4 ON borrador.idborrador = t4.idborrador WHERE idasiento = " + idasiento;
    cursor2 *cur= m_companyact->cargacursor(SQLQuery);
    QString resultado = cur->valor(0);
    delete cur;
    _depura("END ListLinAsiento1View::totaldebe", 0);
    return Fixed(resultado);
}


/// Devuelve el HABER de un asiento.
Fixed ListLinAsiento1View::totalhaber(QString idasiento) {
    _depura("ListLinAsiento1View::totalhaber", 0);
    QString SQLQuery = "SELECT sum(haber) FROM borrador LEFT JOIN (SELECT codigo, descripcion AS descripcioncuenta, idcuenta, tipocuenta FROM cuenta) AS t1 ON t1.idcuenta=borrador.idcuenta LEFT JOIN (SELECT idcanal, nombre AS nombrecanal, descripcion AS descripcioncanal FROM canal) AS t2 ON borrador.idcanal = t2.idcanal LEFT JOIN (SELECT idc_coste, nombre AS nombrec_coste, descripcion AS descripcionc_coste FROM c_coste) AS t3 ON borrador.idc_coste = t3.idc_coste LEFT JOIN (SELECT idregistroiva, factura, ffactura, idborrador FROM registroiva) AS t4 ON borrador.idborrador = t4.idborrador WHERE idasiento = " + idasiento;
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    QString resultado = cur->valor(0);
    delete cur;
    _depura("END ListLinAsiento1View::totalhaber", 0);
    return Fixed(resultado);
}

