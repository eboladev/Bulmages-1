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

#include <QLineEdit>
#include <QMessageBox>
#include <Q3Table>
#include <QComboBox>
#include <Q3TextEdit>
#include <QLabel>
#include <QPixmap>
#include <QCheckBox>
#include <Q3FileDialog>
#include <QCloseEvent>


#include "tarifaview.h"
#include "company.h"
#include "familiasview.h"
#include "tiposarticuloview.h"
#include "busquedafamilia.h"
#include "busquedaalmacen.h"
#include "comparticulolistview.h"
#include "comparticulolist.h"
#include "funcaux.h"
#include "plugins.h"


TarifaView::TarifaView(company *comp, QWidget *parent, const char *name)
        : QWidget(parent, name, Qt::WDestructiveClose) ,dialogChanges(this), DBRecord(comp) {
    _depura("TarifaView::INIT_constructor()\n", 0);
    m_companyact = comp;
    setupUi(this);
    setDBTableName("tarifa");
    setDBCampoId("idtarifa");
    addDBCampo("idtarifa", DBCampo::DBint, DBCampo::DBPrimaryKey, tr("Identificador"));
    addDBCampo("nomtarifa", DBCampo::DBvarchar, DBCampo::DBNotNull, tr("Nombre Tarifa"));
    mui_idfamilia->setcompany(comp);
    mui_almacen->setcompany(comp);
    mui_almacen->setidalmacen("0");
    mui_list->setcompany(comp);

    if (m_companyact->meteWindow("Tarifa Edicion", this))
        return;

    dialogChanges_cargaInicial();

    _depura("TarifaView::END_constructor()\n", 0);
}


TarifaView::~TarifaView() {
    _depura("TarifaView::INIT_destructor()\n", 0);
    m_companyact->sacaWindow(this);
    _depura("TarifaView::END_destructor()\n", 0);
}


void TarifaView::pintar() {
    _depura("TarifaView::pintar", 0);
    mui_nomtarifa->setText(DBvalue("nomtarifa"));
    setCaption(tr("Tarifa ") + mui_nomtarifa->text());
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
        wherearticulo += warticulo + " idfamilia=" + idfamilia;
        warticulo = " AND ";
    } // end if

    QString idalmacen = mui_almacen->idalmacen();
    if (idalmacen != "") {
        wherealmacen += walmacen + " idalmacen=" + idalmacen;
        walmacen = " AND ";
    } // end if

    QString SQLQuery = "SELECT * FROM (SELECT * FROM (SELECT * FROM almacen "+wherealmacen+") AS t6, (SELECT * FROM articulo " + wherearticulo + ") AS t5, (SELECT * FROM tarifa WHERE idtarifa= " + idtarifa + ") AS t2 ) AS t3 ";
    SQLQuery+= " LEFT JOIN (SELECT * FROM ltarifa WHERE idtarifa=" + idtarifa + ") as t1 ON t1.idtarifa=t3.idtarifa AND t1.idalmacen=t3.idalmacen AND t1.idarticulo=t3.idarticulo";
    return SQLQuery;
}


/// Esta funcion carga un articulo de la base de datos y lo presenta.
/// Si el parametro pasado no es un identificador válido entonces se pone
/// la ventana de edición en modo de inserción.
int TarifaView::cargar(QString idtarifa) {
    _depura("TarifaView::cargar(" + idtarifa + ")\n", 0);
    int error = 0;
    setDBvalue("idtarifa", idtarifa);
    DBRecord::cargar(idtarifa);
    mui_list->cargar(formaQuery(idtarifa));

    setCaption("Tarifa " + DBvalue("nomtarifa"));
    if (m_companyact->meteWindow(caption(), this))
        return -1;

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


/// Esta funcion se ejecuta cuando se ha pulsado sobre el boton de nuevo.
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


/// Esta funcion se ejecuta cuando se ha pulsado sobre el boton de borrar.
void TarifaView::on_mui_borrar_clicked() {
    _depura("TarifaView::INIT_boton_borrar()\n", 0);
    if (DBvalue("idtarifa") != "") {
        if (QMessageBox::question(this,
                                  tr("Borrar tarifa"),
                                  tr("Esta a punto de borrar una tarifa. Desea continuar?."),
                                  tr("&Si"), tr("&No"), 0, 1, 0) == 0) {
            m_companyact->begin();
            int error = mui_list->borrar();
            error += borrar();
            if (error) {
                m_companyact->rollback();
            } else
                m_companyact->commit();
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

