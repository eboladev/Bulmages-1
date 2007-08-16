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
#include "empresa.h"

#define COL_IDPAIS    0
#define COL_COD2PAIS  1
#define COL_COD3PAIS  2
#define COL_DESCPAIS  3


/// Esta clase se encarga de presentar los centros de coste, la ventana, y
/// de controlar la inserci&oacute;n de nuevos centros de coste, borrarlos, etc.
PaisView::PaisView(Empresa *emp, QWidget *parent)
        : FichaBc(emp, parent) {
    _depura("PaisView::PaisView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    empresaactual = emp;
    conexionbase = empresaactual->bdempresa();
    idpais = 0;
    mui_list->setColumnCount(4);
    //QStringList headers;
    //headers << tr("ID pais") << tr("Codigo 2 letras") << tr("Codigo 3 letras") << tr("Pais");
    mui_list->setColumnWidth(0, 200);
    //mui_list->setHeaderLabels(headers);
    mui_list->setColumnHidden(COL_IDPAIS, TRUE);
    dialogChanges_cargaInicial();
    empresaactual->meteWindow(windowTitle(), this);
    pintar();
    _depura("END PaisView::PaisView", 0);
}


PaisView::~PaisView() {
    _depura("PaisView::~PaisView", 0);
    empresaactual->sacaWindow(this);
    _depura("END PaisView::~PaisView", 0);
}


void PaisView::pintar() {
    _depura("PaisView::pintar", 10);
//    QMap <int, QTableWidgetItem*> Lista;
//    int idpais1 = 0;
    cursor2 *cursoraux1;

    /// Vaciamos el contenido de la tabla.
    mui_list->clear();
    mui_list->setRowCount(0);
    _depura("PaisView::pintar", 10);

    cursoraux1 = conexionbase->cargacursor("SELECT * FROM pais ORDER BY descpais");
    while (!cursoraux1->eof()) {
//        idpais1 = atoi(cursoraux1->valor("idpais").toAscii());
//        it = new QTableWidgetItem(cursoraux1->valor("idpais"));
//        Lista[idpais1] = it;
//        it->setText(COL_IDPAIS, cursoraux1->valor("idpais"));
//        it->setText(COL_COD2PAIS, cursoraux1->valor("cod2pais"));
//        it->setText(COL_COD3PAIS, cursoraux1->valor("cod3pais"));
//        it->setText(COL_DESCPAIS, cursoraux1->valor("descpais"));

        mui_list->insertRow ( mui_list->rowCount() );

        QTableWidgetItem *it1 = new QTableWidgetItem(cursoraux1->valor("idpais"));
        mui_list->setItem(mui_list->rowCount() - 1, COL_IDPAIS, it1);
        QTableWidgetItem *it2 = new QTableWidgetItem(cursoraux1->valor("cod2pais"));
        mui_list->setItem(mui_list->rowCount() - 1, COL_COD2PAIS, it2);
        QTableWidgetItem *it3 = new QTableWidgetItem(cursoraux1->valor("cod3pais"));
        mui_list->setItem(mui_list->rowCount() - 1, COL_COD3PAIS, it3);
        QTableWidgetItem *it4 = new QTableWidgetItem(cursoraux1->valor("descpais"));
        mui_list->setItem(mui_list->rowCount() - 1, COL_DESCPAIS, it4);


        cursoraux1->siguienteregistro();
    } // end while
    _depura("PaisView::pintar", 10);
    delete cursoraux1;

    if (idpais != 0) {
        mostrarplantilla();
    } // end if

    _depura("END PaisView::pintar", 0);
}


void PaisView::on_mui_list_itemSelectionChanged() {
    _depura("PaisView::on_mui_list_itemSelectionChanged", 0);
    /// Busca el item correcto.
    QTableWidgetItem *it2 = mui_list->item(mui_list->selectedItems().first()->row(), COL_IDPAIS);
    int previdpais = it2->text().toInt();

    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar pais"),
                                 tr("Desea guardar los cambios?"),
                                 tr("&Guardar"), tr("&Cancelar"), 0, 0, 1) == 0) {
            on_mui_guardar_clicked();
        } // end if
    } // end if
    idpais = previdpais;
    mostrarplantilla();
    _depura("END PaisView::on_mui_list_itemSelectionChanged", 0);
}


void PaisView::mostrarplantilla() {
    _depura("PaisView::mostrarplantilla", 0);
    QString query;

    query.sprintf("SELECT * FROM pais WHERE idpais = %d", idpais);
    cursor2 *cursorpais = conexionbase->cargacursor(query);

    if (!cursorpais->eof()) {
        mui_cod2pais->setText(cursorpais->valor("cod2pais"));
        mui_cod3pais->setText(cursorpais->valor("cod3pais"));
        mui_descpais->setText(cursorpais->valor("descpais"));
    } // end if
    delete cursorpais;
    dialogChanges_cargaInicial();
    _depura("END PaisView::mostrarplantilla", 0);
}


void PaisView::on_mui_guardar_clicked() {
    _depura("PaisView::on_mui_guardar_clicked", 0);
    QString descpais = mui_descpais->text();
    QString cod2pais = mui_cod2pais->text();
    QString cod3pais = mui_cod3pais->text();
    QString query;
    query.sprintf("UPDATE pais SET cod2pais = '%s', cod3pais = '%s', descpais ='%s' WHERE idpais = %d", cod2pais.toAscii().constData(), cod3pais.toAscii().constData(), descpais.toAscii().constData(), idpais);
    conexionbase->begin();
    conexionbase->ejecuta(query);
    conexionbase->commit();
    fprintf(stderr,"Se ha guardado el pais\n");
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
    conexionbase->begin();
    conexionbase->ejecuta(query);

    query.sprintf("SELECT COALESCE(MAX(idpais), 1) AS idultimopais FROM pais");
    cursor2 *cur = conexionbase->cargacursor(query, "queryy");
    idpais = atoi(cur->valor("idultimopais").toAscii());
    delete cur;
    conexionbase->commit();
    pintar();
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
        conexionbase->begin();
        conexionbase->ejecuta(query);
        conexionbase->commit();
        idpais = 0;
        pintar();
    } // end switch
    _depura("END PaisView::on_mui_borrar_clicked", 0);
}


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

