/***************************************************************************
 *   Copyright (C) 2004 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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

#include <QCloseEvent>

#include "tarifaview.h"
#include "company.h"
#include "familiasview.h"
#include "tiposarticuloview.h"
#include "busquedafamilia.h"
#include "busquedaalmacen.h"
#include "comparticulolistview.h"
#include "funcaux.h"
#include "plugins.h"


TarifaView::TarifaView(Company *comp, QWidget *parent)
        : FichaBf(comp, parent) {
    _depura("TarifaView::INIT_constructor()\n", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    setTitleName(tr("Tarifa"));
    setDBTableName("tarifa");
    setDBCampoId("idtarifa");
    addDBCampo("idtarifa", DBCampo::DBint, DBCampo::DBPrimaryKey, tr("ID tarifa"));
    addDBCampo("nomtarifa", DBCampo::DBvarchar, DBCampo::DBNotNull, tr("Nombre de la tarifa"));
    mui_idfamilia->setEmpresaBase(comp);
    mui_almacen->setEmpresaBase(comp);
    mui_almacen->setidalmacen("");
    mui_list->setEmpresaBase(comp);

    meteWindow(tr("Tarifa edicion"), this);

    dialogChanges_cargaInicial();
    _depura("TarifaView::END_constructor()\n", 0);
}


TarifaView::~TarifaView() {
    _depura("TarifaView::INIT_destructor()\n", 0);
    empresaBase()->sacaWindow(this);
    _depura("TarifaView::END_destructor()\n", 0);
}


void TarifaView::pintar() {
    _depura("TarifaView::pintar", 0);
    mui_nomtarifa->setText(DBvalue("nomtarifa"));
    setWindowTitle(tr("Tarifa") + " " + mui_nomtarifa->text());
    _depura("END TarifaView::pintar", 1);
}


QString TarifaView::formaQuery(QString idtarifa) {
    _depura("TarifaView::formaQuery", 0);
    QString wherearticulo = "";
    QString warticulo = " WHERE ";

    QString wherealmacen = "";
    QString walmacen= " WHERE ";

    QString idfamilia = mui_idfamilia->idfamilia();
    if (idfamilia != "") {
        wherearticulo += warticulo + " idfamilia = " + idfamilia;
        warticulo = " AND ";
    } // end if

    QString idalmacen = mui_almacen->idalmacen();
    if (idalmacen != "") {
        wherealmacen += walmacen + " idalmacen = " + idalmacen;
        walmacen = " AND ";
    } // end if

    QString SQLQuery = "SELECT * FROM (SELECT * FROM (SELECT * FROM almacen " + wherealmacen + ") AS t6, (SELECT * FROM articulo " + wherearticulo + ") AS t5, (SELECT * FROM tarifa WHERE idtarifa = " + idtarifa + ") AS t2 ) AS t3 ";
    SQLQuery += " LEFT JOIN (SELECT * FROM ltarifa WHERE idtarifa = " + idtarifa + ") as t1 ON t1.idtarifa = t3.idtarifa AND t1.idalmacen = t3.idalmacen AND t1.idarticulo = t3.idarticulo";
    return SQLQuery;
}


/// Esta funci&oacute;n carga un art&iacute;culo de la base de datos y lo presenta.
/// Si el par&aacute;metro pasado no es un identificador v&aacute;lido entonces se pone
/// la ventana de edici&oacute;n en modo de inserci&oacute;n.
int TarifaView::cargar(QString idtarifa) {
    _depura("TarifaView::cargar(" + idtarifa + ")\n", 0);
    int error = 0;
    setDBvalue("idtarifa", idtarifa);
    DBRecord::cargar(idtarifa);
    mui_list->cargar(formaQuery(idtarifa));

    setWindowTitle(tr("Tarifa") + " " + DBvalue("nomtarifa"));
    meteWindow(windowTitle(), this);

    dialogChanges_cargaInicial();
    /// Tratamiento de excepciones.
    if (error == 1) {
        _depura("TarifaView::END_chargeArticle Error en la carga del articulo()\n", 0);
        return -1;
    } // end if
    pintar();
    _depura("END TarifaView::cargar()\n", 0);
    return 0;
}


int TarifaView::guardar() {
    _depura("TarifaView::INIT_s_grabarClicked()\n", 0);
    setDBvalue("nomtarifa", mui_nomtarifa->text());
    DBRecord::guardar();
    /// Guardamos la lista de componentes.
    mui_list->setColumnValue("idtarifa", DBvalue("idtarifa"));
    mui_list->guardar();
    dialogChanges_cargaInicial();
    _depura("TarifaView::END_s_grabarClicked()\n", 0);
    return 0;
}


/// Esta funci&oacute;n se ejecuta cuando se ha pulsado sobre el bot&oacute;n de nuevo.
void TarifaView::on_mui_crear_clicked() {
    _depura("TarifaView::INIT_boton_nuevo()\n", 0);
    vaciar();
    pintar();
    _depura("TarifaView::END_boton_nuevo()\n", 0);
}


void TarifaView::on_mui_actualizar_clicked() {
    _depura("TarifaView::INIT_boton_nuevo()\n", 0);
    guardar();
    QString idtarifa = DBvalue("idtarifa");
    cargar(idtarifa);
    _depura("TarifaView::END_boton_nuevo()\n",0 );
}


/// Esta funci&oacute;n se ejecuta cuando se ha pulsado sobre el bot&oacute;n de borrar.
void TarifaView::on_mui_borrar_clicked() {
    _depura("TarifaView::INIT_boton_borrar()\n", 0);
    if (DBvalue("idtarifa") != "") {
        if (QMessageBox::question(this,
                                  tr("Borrar tarifa"),
                                  tr("Esta a punto de borrar una tarifa. Desea continuar?"),
                                  tr("&Si"), tr("&No"), 0, 1, 0) == 0) {
            empresaBase()->begin();
            int error = mui_list->borrar();
            error += borrar();
            if (error) {
                empresaBase()->rollback();
            } else
                empresaBase()->commit();
            dialogChanges_cargaInicial();
            close();
        } // end if
    } // end if
    _depura("TarifaView::END_boton_borrar()\n", 0);
}


void TarifaView::closeEvent(QCloseEvent *e) {
    _depura("closeEvent", 0);
    if (dialogChanges_hayCambios())  {
        int val = QMessageBox::warning(this,
                                       tr("Guardar tarifa"),
                                       tr("Desea guardar los cambios?"),
                                       tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
        if (val == 0)
            on_mui_guardar_clicked();
        if (val == 2)
            e->ignore();
    } // end if
}


void TarifaView::on_mui_aceptar_clicked() {
    on_mui_guardar_clicked();
    close();
}

