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
#include "fichacfg.h"
#include "qcombobox2.h"
#include "plugins.h"

#include <QMenu>
#include <QToolButton>
#include <QTextEdit>
#include <QCheckBox>


Ficha::Ficha(QWidget *parent, Qt::WFlags f, edmode modo) : BLWidget(parent, f), DBRecord(NULL), dialogChanges(this) {
    _depura("Ficha::Ficha", 0);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect (this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(on_customContextMenuRequested(const QPoint &)));
    m_modo = modo;
    dialogChanges_cargaInicial();
    _depura("END Ficha::Ficha", 0);
}


Ficha::Ficha(EmpresaBase *emp, QWidget *parent, Qt::WFlags f, edmode modo) : BLWidget(emp, parent, f), DBRecord(emp), dialogChanges(this) {
    _depura("Ficha::Ficha", 0);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect (this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(on_customContextMenuRequested(const QPoint &)));
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


void Ficha::on_mui_imprimir_clicked() {
    imprimir();
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

    /// De Forma rapida hacemos un tratamiento de los permisos
    setDBTableName(tableName());

    _depura("END Ficha::meteWindow", 0);
}

void Ficha::on_customContextMenuRequested(const QPoint &) {
    _depura("Ficha::on_customContextMenuRequested", 0);
    QMenu *popup = new QMenu(this);
    QAction *avconfig = popup->addAction(tr("Opciones Avanzadas de Ficha"));
    QAction *avprint = popup->addAction(tr("Imprimir Ficha"));
    QAction *opcion = popup->exec(QCursor::pos());
    if (opcion == avconfig) {
        new FichaCfg(empresaBase(), this, 0);
    } else if (opcion == avprint) {
        Ficha::imprimir();
    } // end if
    delete popup;
    _depura("END Ficha::on_customContextMenuRequested", 0);
}

void Ficha::setDBTableName(QString nom) {
    _depura("Ficha::setDBTableName", 0);
    DBRecord::setDBTableName(nom);


    if (!empresaBase()->has_table_privilege(nom, "INSERT")) {
        /// Buscamos los permisos que tiene el usuario y desactivamos botones.
        QToolButton *b = findChild<QToolButton *>("mui_guardar");
        if (b) b->setDisabled(TRUE);
        b = findChild<QToolButton *>("mui_borrar");
        if (b) b->setDisabled(TRUE);
    } // end if

    _depura("END Ficha::setDBTableName", 0);
}


void Ficha::pintar() {
    _depura("Ficha::pintar", 0);
    DBCampo *campo;
    /// Recorremos todos los campos definidos.
    for (int i = 0; i < m_lista.size(); ++i) {
        campo = m_lista.at(i);
        /// Buscamos un QLineEdit con nombre coincidente.
        QLineEdit *l = findChild<QLineEdit *>("mui_" + campo->nomcampo());
        if (l)
            l->setText(campo->valorcampo());

        /// Buscamos un QLineEdit con nombre coincidente.
        QTextEdit *l3 = findChild<QTextEdit *>("mui_" + campo->nomcampo());
        if (l3)
        l3->setText(campo->valorcampo());

    /// Buscamos BLWidgets que coincidan con el campo supuestamente sirve para los campos personales
        BLWidget *l1 = findChild<BLWidget *>("mui_" + campo->nomcampo());
    if (l1)
        l1->setValorCampo(campo->valorcampo());

    /// Buscamos QComboBox2 que coincidan con el campo supuestamente sirve para los campos personales
        QComboBox2 *l2 = findChild<QComboBox2 *>("mui_" + campo->nomcampo());
    if (l2)
        l2->setValorCampo(campo->valorcampo());

    QCheckBox *l5 = findChild<QCheckBox *>("mui_" + campo->nomcampo());
    if (l5) {
        if (campo->valorcampo() == "t") {
            l5->setChecked(TRUE);
        } else {
            l5->setChecked(FALSE);
        } // end if
    } // end if

    } // end for
}

/** Recoge de forma automatica los valores que tienen los QLineEdit y los pone en la estructura de DBRecord.
    para que funcione bien los QLineEdit deben tener como nombre el mismo campo que el correspondiente con la
    base de datos precedidos de mui_
*/
void Ficha::recogeValores() {
    _depura("Ficha::recogeValores", 0);
    DBCampo *campo;
    /// Recorremos todos los campos definidos.
    for (int i = 0; i < m_lista.size(); ++i) {
        campo = m_lista.at(i);
        /// Buscamos un QLineEdit con nombre coincidente.
        QLineEdit *l = findChild<QLineEdit *>("mui_" + campo->nomcampo());
        if (l)
        campo->set(l->text());

        /// Buscamos un QLineEdit con nombre coincidente.
        QTextEdit *l3 = findChild<QTextEdit *>("mui_" + campo->nomcampo());
        if (l3)
        campo->set(l3->toPlainText());

    /// Buscamos BLWidgets que coincidan con el campo supuestamente sirve para los campos personales
        BLWidget *l1 = findChild<BLWidget *>("mui_" + campo->nomcampo());
    if (l1)
        campo->set(l1->valorCampo());

    /// Buscamos QComboBox2 que coincidan con el campo supuestamente sirve para los campos personales
        QComboBox2 *l2 = findChild<QComboBox2 *>("mui_" + campo->nomcampo());
    if (l2)
        campo->set(l2->valorCampo());

    QCheckBox *l5 = findChild<QCheckBox *>("mui_" + campo->nomcampo());
    if (l5) {
        if (l5->isChecked()) {
            campo->set("TRUE");
        } else {
            campo->set("FALSE");
        } // end if
    } // end if

    } // end for
    _depura("END Ficha::recogeValores", 0);
}


int Ficha::cargar(QString id) {
    _depura("Ficha::cargar", 0);
    try {
        if (DBRecord::cargar(id))
            throw -1;

    /// Lanzamos los plugins.
    if (g_plugins->lanza("Ficha_cargar", this)) return 0;
    cargarPost(id);

        setWindowTitle(m_tablename + " " + DBvalue(m_campoid));
        pintar();
        dialogChanges_cargaInicial();
        meteWindow(windowTitle(), this);
    } catch (...) {
        return -1;
    } // end try
    _depura("END Ficha::cargar", 0);
    return 0;
}

/** Guarda los datos de la ficha en la base de datos.
*/
int Ficha::guardar() {
    _depura("Ficha::guardar", 0);

    try {
        QString id;
        recogeValores();
        empresaBase()->begin();
        DBsave(id);
        setDBvalue(m_campoid, id);
        empresaBase()->commit();

    /// Lanzamos los plugins.
    if (g_plugins->lanza("Ficha_guardar_Post", this)) return 0;
        guardarPost();

        /// Hacemos una carga para que se actualizen datos como la referencia.
        cargar(id);

        _depura("END Ficha::guardar", 0);
        return 0;
    } catch (...) {
        mensajeInfo("Error inesperado al guardar");
        empresaBase()->rollback();
        return -1;
    } // end try
}


int Ficha::borrarPre() {
    return 0;
}


/** Borra la ficha en la base de datos
    Este metodo puede ser invocado desde la clase ficha o desde la pantalla previa
*/
int Ficha::borrar() {
    _depura("Ficha::borrar existe solo para ser derivado", 0);
    borrarPre();
    return DBRecord::borrar();
}


int Ficha::guardarPost() {
    return 0;
}


int Ficha::cargarPost(QString id) {
    return 0;
}

