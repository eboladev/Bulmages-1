/***************************************************************************
 *   Copyright (C) 2007 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include "paisview.h"
#include "empresabase.h"
#include "subform3.h"

/// Esta clase se encarga de presentar los centros de coste, la ventana, y
/// de controlar la inserci&oacute;n de nuevos centros de coste, borrarlos, etc.
PaisView::PaisView(EmpresaBase *emp, QWidget *parent)
        : Ficha(emp, parent) {
    _depura("PaisView::PaisView", 0);

    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);

    mui_list->setEmpresaBase(emp);
    mui_listprovincias->setEmpresaBase(emp);

    /// Preparamos la lista de paises.
    mui_list->setDBTableName("pais");
    mui_list->setDBCampoId("idpais");
    mui_list->addSHeader("idpais", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("Id. Pais"));
    mui_list->addSHeader("cod2pais", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo 2 Digitos"));
    mui_list->addSHeader("cod3pais", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo 3 Digitos"));
    mui_list->addSHeader("descpais", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre Pais"));
    mui_list->setinsercion(FALSE);
    mui_list->setDelete(FALSE);
    mui_list->setSortingEnabled(TRUE);

    /// Preparamos la lista de provincias.
    mui_listprovincias->setDBTableName("provincia");
    mui_listprovincias->setDBCampoId("idprovincia");
    mui_listprovincias->addSHeader("idprovincia", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("Id. Provincia"));
    mui_listprovincias->addSHeader("idpais", DBCampo::DBint, DBCampo::DBNotNull , SHeader::DBNoView | SHeader::DBNoWrite, tr("Id. Pais"));
    mui_listprovincias->addSHeader("provincia", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, tr("Provincia"));
    mui_listprovincias->setinsercion(TRUE);
    mui_listprovincias->setDelete(TRUE);
    mui_listprovincias->setSortingEnabled(TRUE);

    /// Establecemos cual es la tabla en la que basarse para los permisos
    setDBTableName("pais");
    setDBCampoId("idpais");
    addDBCampo("idpais", DBCampo::DBint, DBCampo::DBPrimaryKey, tr("idpais"));
    addDBCampo("descpais", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Pais"));
    addDBCampo("cod2pais", DBCampo::DBvarchar, DBCampo::DBNothing, tr("Codigo 2 digitos"));
    addDBCampo("cod3pais", DBCampo::DBvarchar, DBCampo::DBNothing,tr("Codigo 3 digitos"));


    idpais = 0;


    dialogChanges_cargaInicial();
    meteWindow(windowTitle(), this);
    pintar();
    _depura("END PaisView::PaisView", 0);
}


PaisView::~PaisView() {
    _depura("PaisView::~PaisView", 0);
    empresaBase()->sacaWindow(this);
    _depura("END PaisView::~PaisView", 0);
}


void PaisView::pintar() {
    _depura("PaisView::pintar", 10);
    /// Vaciamos el contenido de la tabla.
    _depura("PaisView::pintar", 10);
    mui_list->cargar("SELECT * FROM pais ORDER BY descpais");
    _depura("END PaisView::pintar", 0);
}


void PaisView::on_mui_list_itemClicked(QTableWidgetItem *) {
    _depura("PaisView::on_mui_list_itemSelectionChanged", 0);
    /// Busca el item correcto.
    QString previdpais = mui_list->DBvalue("idpais");
//    previdpais = (QTableWidgetItem2 *) item->
//    _depura(previdpais, 2);
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar pais"),
                                 tr("Desea guardar los cambios?"),
                                 tr("&Guardar"), tr("&Cancelar"), 0, 0, 1) == 0) {
            on_mui_guardar_clicked();
        } // end if
    } // end if
    idpais = previdpais.toInt();
    mostrarplantilla();
    _depura("END PaisView::on_mui_list_itemSelectionChanged", 0);
}


void PaisView::mostrarplantilla() {
    _depura("PaisView::mostrarplantilla", 0);

    if (idpais != 0) {
        cargar(QString::number(idpais));
	mui_descpais->setText(DBvalue("descpais"));
	mui_cod2pais->setText(DBvalue("cod2pais"));
	mui_cod3pais->setText(DBvalue("cod3pais"));

	mui_listprovincias->cargar("SELECT * FROM provincia WHERE idpais="+QString::number(idpais));

	dialogChanges_cargaInicial();
    } // end if
    _depura("END PaisView::mostrarplantilla", 0);
}


void PaisView::on_mui_guardar_clicked() {
    _depura("PaisView::on_mui_guardar_clicked", 0);
    QString id;
    setDBvalue("descpais", mui_descpais->text());
    setDBvalue("cod2pais", mui_cod2pais->text());
    setDBvalue("cod3pais", mui_cod3pais->text());
    empresaBase()->begin();
    DBsave(id);
    mui_listprovincias->setColumnValue("idpais", id);
    mui_listprovincias->guardar();
    empresaBase()->commit();
    dialogChanges_cargaInicial();
    pintar();
    _depura("END PaisView::on_mui_guardar_clicked", 0);
}


void PaisView::on_mui_crear_clicked() {
    _depura("PaisView::on_mui_crear_clicked", 0);
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar pais"),
                                 tr("Desea guardar los cambios?"),
                                 QMessageBox::Ok,
                                 QMessageBox::Cancel) == QMessageBox::Ok) {
            on_mui_guardar_clicked();
        } // end if
    } // end if

    QString query;
    query.sprintf("INSERT INTO pais (cod2pais, cod3pais, descpais) VALUES ('--', '---', 'Nuevo pais')");
    empresaBase()->begin();
    empresaBase()->ejecuta(query);
    empresaBase()->commit();
    pintar();
    mui_list->setCurrentItem(mui_list->rowCount(),1);
    mostrarplantilla();
    _depura("END PaisView::on_mui_crear_clicked", 0);
}


void PaisView::on_mui_borrar_clicked() {
    _depura("PaisView::on_mui_borrar_clicked", 0);
    switch (QMessageBox::warning(this,
                                 tr("Borrar pais"),
                                 tr("Se va a borrar el centro de coste.\nEsta operacion puede ocasionar perdida de datos."),
                                 tr("&Borrar"), tr("&Cancelar"), 0, 0, 1)) {
    case 0: /// Retry clicked or Enter pressed.
        QString query;
        query.sprintf("DELETE FROM pais WHERE idpais = %d", idpais);
        empresaBase()->begin();
        empresaBase()->ejecuta(query);
        empresaBase()->commit();
        idpais = 0;
        pintar();
    } // end switch
    _depura("END PaisView::on_mui_borrar_clicked", 0);
}

/*
void PaisView::closeEvent(QCloseEvent *e) {
    _depura("PaisView::closeEvent", 0);
    if (dialogChanges_hayCambios()) {
        int val = QMessageBox::warning(this,
                                       tr("Guardar pais"),
                                       tr("Desea guardar los cambios?"),
                                       tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
        if (val == 0) {
            on_mui_guardar_clicked();
        } // end if
        if (val == 2) {
            e->ignore();
        } // end if
    } // end if
    _depura("END PaisView::closeEvent", 0);
}
*/
