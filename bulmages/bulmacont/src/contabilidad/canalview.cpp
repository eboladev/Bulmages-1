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

#include "canalview.h"
#include "empresa.h"
#include "selectcanalview.h"


canalview::canalview(empresa *emp, QWidget *parent)
        : Ficha(parent) {
    _depura("canalview::canalview", 0);
    this->setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    empresaactual = emp;
    conexionbase = empresaactual->bdempresa();
    mui_idcanal->setcompany(emp);
    mui_idcanal->setidcanal("0");
    idcanal = 0;
    dialogChanges_cargaInicial();
    pintar();
    empresaactual->meteWindow(windowTitle(), this);
    _depura("END canalview::canalview", 0);
}


canalview::~canalview() {
    _depura("canalview::~canalview", 0);
    empresaactual->sacaWindow(this);
    _depura("END canalview::~canalview", 0);
}


void canalview::pintar() {
    _depura("canalview::pintar", 0);
    /// Si el combocoste no esta vacio se muestra el registro que contiene.
    if (idcanal != 0) {
        mostrarplantilla();
    } else {
        idcanal = mui_idcanal->idcanal().toInt();
        mostrarplantilla();
    } // end if

    /// Si se han cambiado los canales, se rehace el selector de canales.
    selectcanalview *scanal = empresaactual->getselcanales();
    scanal->cargacanales();
    _depura("END canalview::pintar", 0);
}


/// Esta funci&oacute;n sirve para hacer el cambio sobre un centro de coste.
void canalview::on_mui_idcanal_valueChanged(QString numcombo) {
    _depura("canalview::on_mui_idcanal_valueChanged", 0);
    int idcanal1 = numcombo.toInt();
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar canal"),
                                 tr("Desea guardar los cambios."),
                                 tr("&Guardar"), tr("&Cancelar"), 0 , 0, 1) == 0)
            on_mui_guardar_clicked();
    } // end if
    idcanal = idcanal1;
    mostrarplantilla();
    _depura("END canalview::on_mui_idcanal_valueChanged", 0);
}


/// Esta funci&oacute;n muestra el canal en la ventana.
void canalview::mostrarplantilla() {
    _depura("canalview::mostrarplantilla", 0);
    QString query;
    query.sprintf("SELECT * from canal WHERE idcanal = %d", idcanal);
    cursor2 *cursorcanal = conexionbase->cargacursor(query);
    if (!cursorcanal->eof()) {
        nomcanal->setText(cursorcanal->valor("nombre"));
        desccanal->setPlainText(cursorcanal->valor("descripcion"));
    } // end if
    mui_idcanal->setidcanal(QString::number(idcanal));
    dialogChanges_cargaInicial();
    _depura("END canalview::mostrarplantilla", 0);

}


void canalview::on_mui_guardar_clicked() {
    _depura("canalview::on_mui_guardar_clicked", 0);
    QString nom = nomcanal->text();
    QString desc = desccanal->toPlainText();
    QString query;
    query.sprintf ("UPDATE canal SET nombre ='%s', descripcion = '%s' WHERE idcanal = %d",
                   conexionbase->sanearCadena(nom).toAscii().constData(),
                   conexionbase->sanearCadena(desc).toAscii().constData(),
                   idcanal);
    conexionbase->ejecuta(query);
    dialogChanges_cargaInicial();
    pintar();
    _depura("END canalview::on_mui_guardar_clicked", 0);
}


void canalview::on_mui_crear_clicked() {
    _depura("canalview::on_mui_crear_clicked", 0);
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar canal"),
                                 tr("Desea guardar los cambios."),
                                 tr("&Guardar"), tr("&Cancelar"), 0 , 0, 1) == 0)
            on_mui_guardar_clicked();
    } // end if
    QString query;
    query.sprintf("INSERT INTO canal (nombre, descripcion) VALUES ('Nuevo Canal', 'Escriba su descripcion')");
    conexionbase->begin();
    conexionbase->ejecuta(query);
    query.sprintf("SELECT MAX(idcanal) AS id FROM canal");
    cursor2 *cur = conexionbase->cargacursor(query, "queryy");
    idcanal = atoi(cur->valor("id").toAscii());
    delete cur;
    conexionbase->commit();
    pintar();
    _depura("END canalview::on_mui_crear_clicked", 0);
}


void canalview::on_mui_borrar_clicked() {
    _depura("canalview::on_mui_borrar_clicked", 0);
    switch (QMessageBox::warning(this,
                                 tr("Borrar canal"),
                                 tr("Se va a borrar la forma de pago,\n \
                                    Esto puede ocasionar perdida de datos\n"),
                                 tr("&Borrar"), tr("&Cancelar"), 0 , 0, 1)) {
    case 0: /// Retry clicked or Enter pressed.
        QString query;
        query.sprintf("DELETE FROM canal WHERE idcanal = %d", idcanal);
        conexionbase->begin();
        conexionbase->ejecuta(query);
        conexionbase->commit();
        idcanal = 0;
        pintar();
    } // end switch
    _depura("END canalview::on_mui_borrar_clicked", 0);
}


void canalview::closeEvent(QCloseEvent *e) {
    _depura("ccosteview::closeEvent", 0);
    if (dialogChanges_hayCambios())	{
        int val = QMessageBox::warning(this,
                                       tr("Guardar canal"),
                                       tr("Desea guardar los cambios?"),
                                       tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
        if (val == 0)
            on_mui_guardar_clicked();
        if (val == 2)
            e->ignore();
    } // end if
    _depura("END ccosteview::closeEvent", 0);
}

