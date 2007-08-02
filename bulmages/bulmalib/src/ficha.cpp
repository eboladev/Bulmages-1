 /***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#include "ficha.h"


Ficha::Ficha(QWidget *parent, Qt::WFlags f, edmode modo) : BLWidget(parent, f), DBRecord(NULL), dialogChanges(this) {
    _depura("Ficha::Ficha", 0);
    m_modo = modo;
    dialogChanges_cargaInicial();
    _depura("END Ficha::Ficha", 0);
}


Ficha::Ficha(EmpresaBase *emp, QWidget *parent, Qt::WFlags f, edmode modo) : BLWidget(emp, parent, f), DBRecord(emp), dialogChanges(this) {
    _depura("Ficha::Ficha", 0);
    m_modo = modo;
    dialogChanges_cargaInicial();
    _depura("END Ficha::Ficha", 0);
}


Ficha::~Ficha() {
    _depura("Ficha::~Ficha", 0, this->windowTitle());
    sacaWindow();
    _depura("END Ficha::~Ficha", 0);
}


void Ficha::setModoConsulta() {
    m_modo = SelectMode;
}


void Ficha::setModoEdicion() {
    m_modo = EditMode;
}


bool Ficha::modoEdicion() {
    return m_modo == EditMode;
}


bool Ficha::modoConsulta() {
    return m_modo == SelectMode;
}


int Ficha::guardar() {
    _depura("Ficha::guardar existe solo para ser derivado", 2);
    return 0;
}


int Ficha::borrar() {
    _depura("Ficha::borrar existe solo para ser derivado", 2);
    return 0;
}


void Ficha::on_mui_cancelar_clicked() {
    _depura("Ficha::on_mui_cancelar_clicked", 0);
    close();
    _depura("END Ficha::on_mui_cancelar_clicked", 0);
}


void Ficha::on_mui_guardar_clicked() {
    _depura("Ficha::on_mui_guardar_clicked", 0);
    guardar();
    _depura("END Ficha::on_mui_guardar_clicked", 0);
}


void Ficha::on_mui_aceptar_clicked() {
    _depura("Ficha::on_mui_aceptar_clicked", 0);
    try {
        if (guardar()) {
            throw -1;
        } // end if
        close();
    } catch (...) {
	_depura("Error al guardar la ficha", 2);
    } // end try
    _depura("Ficha::on_mui_aceptar_clicked", 0);
}


void Ficha::on_mui_borrar_clicked() {
    _depura("Ficha::on_mui_borrar_clicked", 0);

    int val = QMessageBox::question(this,
                                    tr("Borrar") + " " + windowTitle(),
                                    tr("Desea eliminar '") + " " + windowTitle() + "' ?",
                                    QMessageBox::Yes,
                                    QMessageBox::Cancel | QMessageBox::Escape | QMessageBox::Default);

    if (val == QMessageBox::Yes) {
        if (!borrar()) {
            dialogChanges_cargaInicial();
            _depura(windowTitle() + " " + "borrado satisfactoriamente.", 10);
            close();
        } else {
            mensajeInfo(windowTitle() + tr("No se ha podido borrar"));
        }// end if
    } // end if
    _depura("END Ficha::on_mui_borrar_clicked", 0);
}


void Ficha::closeEvent(QCloseEvent *e) {
    _depura("Ficha::closeEvent", 0, windowTitle());
    try {
        if (dialogChanges_hayCambios()) {
            int val = QMessageBox::warning(this,
                                           tr("Guardar") + " " + windowTitle(),
                                           tr("Desea guardar los cambios?"),
                                           tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
            if (val == 0) {
                guardar();
            } // end if
            if (val == 2) {
                e->ignore();
                return;
            } // end if
        } // end if
        /// \TODO Este sacaWindow encubre un bug. Debe tratarse de otra forma el
        /// sacar las ventanas de listventanas.
//        sacaWindow();
    } catch (...) {
        mensajeInfo(tr("No se pudo cerrar la ventana debido a un error"));
        e->ignore();
    } // end try
    _depura("END Ficha::closeEvent", 0);
}


int Ficha::sacaWindow() {
    _depura("Ficha::sacaWindow", 0);
    if (empresaBase() != NULL) {
        empresaBase()->sacaWindow(this);
    } // end if
    _depura("END Ficha::sacaWindow", 0);
    return 0;
}


void Ficha::meteWindow(QString nom, QObject *obj, bool compdup) {
    _depura("Ficha::meteWindow", 0);
    if (empresaBase() != NULL) {
        empresaBase()->meteWindow(nom, obj, compdup);
    } // end if
    _depura("END Ficha::meteWindow", 0);
}

