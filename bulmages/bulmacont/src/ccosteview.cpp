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

#include "ccosteview.h"
#include "empresa.h"
#include "selectccosteview.h"

#define COL_NOM_COSTE  0
#define COL_DESC_COSTE 1
#define COL_IDC_COSTE  2


/// Esta clase se encarga de presentar los centros de coste, la ventana, y
/// de controlar la inserci&oacute;n de nuevos centros de coste, borrarlos, etc.
ccosteview::ccosteview(Empresa  *emp, QWidget *parent)
        : FichaBc(emp, parent) {
    _depura("ccosteview::ccosteview", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    empresaactual = emp;
    conexionbase = empresaactual->bdempresa();
    idc_coste = 0;
    mui_list->setColumnCount(3);
    QStringList headers;
    headers << tr("Nombre") << tr("Descripcion") << tr("Id centro de coste");
    mui_list->setColumnWidth(0, 200);
    mui_list->setHeaderLabels(headers);
    mui_list->setColumnHidden(COL_IDC_COSTE, TRUE);
    dialogChanges_cargaInicial();
    empresaactual->meteWindow(windowTitle(), this);
    pintar();
    _depura("END ccosteview::ccosteview", 0);
}


ccosteview::~ccosteview() {
    _depura("ccosteview::~ccosteview", 0);
    empresaactual->sacaWindow(this);
    _depura("END ccosteview::~ccosteview", 0);
}


void ccosteview::pintar() {
  _depura("ccosteview::pintar", 0);
    QTreeWidgetItem *it;
    QMap <int, QTreeWidgetItem*> Lista;
    int padre;
    int idc_coste1 = 0;
    cursor2 *cursoraux1, *cursoraux2;

    /// Vaciamos el &aacute;rbol.
    while (mui_list->topLevelItemCount() > 0) {
        it = mui_list->takeTopLevelItem(0);
        delete it;
    } // end while

    cursoraux1 = conexionbase->cargacursor("SELECT * FROM c_coste WHERE padre ISNULL ORDER BY idc_coste");
    while (!cursoraux1->eof()) {
        padre = atoi( cursoraux1->valor("padre").toAscii());
        idc_coste1 = atoi( cursoraux1->valor("idc_coste").toAscii());
        it = new QTreeWidgetItem(mui_list);
        Lista[idc_coste1] = it;
        it->setText(COL_IDC_COSTE, cursoraux1->valor("idc_coste"));
        it->setText(COL_DESC_COSTE,cursoraux1->valor("descripcion"));
        it->setText(COL_NOM_COSTE, cursoraux1->valor("nombre"));
        mui_list->expandItem(it);
        cursoraux1->siguienteregistro();
    } // end while
    delete cursoraux1;

    cursoraux2= conexionbase->cargacursor("SELECT * FROM c_coste WHERE padre IS NOT NULL ORDER BY idc_coste");
    while (!cursoraux2->eof()) {
        padre = atoi(cursoraux2->valor("padre").toAscii());
        idc_coste1 = atoi(cursoraux2->valor("idc_coste").toAscii());
        fprintf(stderr, "Cuentas de subnivel: %d", padre);
        it = new QTreeWidgetItem(Lista[padre]);
        Lista[idc_coste1] = it;
        it->setText(COL_IDC_COSTE,cursoraux2->valor("idc_coste"));
        it->setText(COL_DESC_COSTE,cursoraux1->valor("descripcion"));
        it->setText(COL_NOM_COSTE,cursoraux1->valor("nombre"));
        mui_list->expandItem(it);
        cursoraux2->siguienteregistro();
    } // end while
    delete cursoraux2;

    if (idc_coste != 0) {
        mostrarplantilla();
    } // end if

    /// Ya que se han producido cambios en los centros de coste
    /// Se inicializa el selector de centros de coste.
    selectccosteview *scoste = empresaactual->getselccostes();
    scoste->cargacostes();
    _depura("END ccosteview::pintar", 0);
}


void ccosteview::on_mui_list_itemClicked(QTreeWidgetItem *it, int) {
  _depura("ccosteview::on_mui_list_itemClicked", 0);
  int previdccoste = it->text(COL_IDC_COSTE).toInt();
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar centro de coste"),
                                 tr("Desea guardar los cambios?"),
                                 tr("&Guardar"), tr("&Cancelar"), 0, 0, 1) == 0) {
            on_mui_guardar_clicked();
        } // end if
    } // end if
    idc_coste = previdccoste;
    mostrarplantilla();
    _depura("END ccosteview::on_mui_list_itemClicked", 0);
}


void ccosteview::mostrarplantilla() {
    _depura("ccosteview::mostrarplantilla", 0);
    QString query;
    query.sprintf("SELECT * from c_coste WHERE idc_coste = %d",idc_coste);
    cursor2 *cursorcoste = conexionbase->cargacursor(query);
    if (!cursorcoste->eof()) {
        nomcentro->setText(cursorcoste->valor("nombre"));
        desccoste->setPlainText(cursorcoste->valor("descripcion"));
    } // end if
    delete cursorcoste;
    dialogChanges_cargaInicial();
    _depura("END ccosteview::mostrarplantilla", 0);
}


void ccosteview::on_mui_guardar_clicked() {
    _depura("ccosteview::on_mui_guardar_clicked", 0);
    QString nom = nomcentro->text();
    QString desc = desccoste->toPlainText();
    QString query;
    query.sprintf("UPDATE c_coste SET nombre = '%s', descripcion = '%s' WHERE idc_coste = %d", nom.toAscii().constData(), desc.toAscii().constData(), idc_coste);
    conexionbase->begin();
    conexionbase->ejecuta(query);
    conexionbase->commit();
    fprintf(stderr,"Se ha guardado el centro de coste\n");
    dialogChanges_cargaInicial();
    pintar();
    _depura("END ccosteview::on_mui_guardar_clicked", 0);
}


void ccosteview::on_mui_crear_clicked() {
    _depura("ccosteview::on_mui_crear_clicked", 0);
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar centro de coste"),
                                 tr("Desea guardar los cambios?"),
                                 QMessageBox::Ok,
                                 QMessageBox::Cancel) == QMessageBox::Ok) {
            on_mui_guardar_clicked();
        } // end if
    } // end if

    QString query;
    QTreeWidgetItem *it;
    it = mui_list->currentItem();
    if (it) {
        idc_coste = atoi(it->text(COL_IDC_COSTE).toAscii());
        query.sprintf("INSERT INTO c_coste (padre, nombre, descripcion) VALUES (%d, 'Nuevo centro de coste', 'Escriba su descripcion')", idc_coste);
        conexionbase->begin();
        conexionbase->ejecuta(query);
    } else {
        query.sprintf("INSERT INTO c_coste (nombre, descripcion) VALUES ('Nuevo centro de coste', 'Escriba su descripcion')");
        conexionbase->begin();
        conexionbase->ejecuta(query);
    } // end if
    query.sprintf("SELECT MAX(idc_coste) AS id_coste FROM c_coste");
    cursor2 *cur = conexionbase->cargacursor(query, "queryy");
    idc_coste = atoi(cur->valor("id_coste").toAscii());
    delete cur;
    conexionbase->commit();
    pintar();
    _depura("END ccosteview::on_mui_crear_clicked", 0);
}


void ccosteview::on_mui_borrar_clicked() {
    _depura("ccosteview::on_mui_borrar_clicked", 0);
    switch (QMessageBox::warning(this,
                                 tr("Borrar centro de coste"),
                                 tr("Se va a borrar el centro de coste.\nEsta operacion puede ocasionar perdida de datos."),
                                 tr("&Borrar"), tr("&Cancelar"), 0, 0, 1)) {
    case 0: /// Retry clicked or Enter pressed.
        QString query;
        query.sprintf("DELETE FROM c_coste WHERE idc_coste = %d", idc_coste);
        conexionbase->begin();
        conexionbase->ejecuta(query);
        conexionbase->commit();
        idc_coste = 0;
        pintar();
    } // end switch
    _depura("END ccosteview::on_mui_borrar_clicked", 0);
}


void ccosteview::closeEvent(QCloseEvent *e) {
    _depura("ccosteview::closeEvent", 0);
    if (dialogChanges_hayCambios()) {
        int val = QMessageBox::warning(this,
                                       tr("Guardar centro de coste"),
                                       tr("Desea guardar los cambios?"),
                                       tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
        if (val == 0) {
            on_mui_guardar_clicked();
        } // end if
        if (val == 2) {
            e->ignore();
        } // end if
    } // end if
    _depura("END ccosteview::closeEvent", 0);
}

