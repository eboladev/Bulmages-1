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

#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>
#include <QLabel>
#include <QDialog>
#include <QCheckBox>

#include "trabajadorview.h"
#include "company.h"
#include "funcaux.h"


/// Constructor de la clase inicializa la clase y llama a la clase de pintar para que pinte.
TrabajadorView::TrabajadorView(company *emp, QWidget *parent)
        : QWidget(parent, Qt::WDestructiveClose), dialogChanges(this) {
    _depura("TrabajadorView::TrabajadorView", 0);
    m_companyact = emp;
    setupUi(this);
    m_archivoimagen = "";
    setModoEdicion();
    m_cursortrabajadores = NULL;
    m_item = NULL;
    pintar();
    m_companyact->meteWindow(windowTitle(), this);
    _depura("END TrabajadorView::TrabajadorView", 0);
}


/// Carga el query de la base de datos y carga el qlistview.
void TrabajadorView::pintar() {
    mui_lista->clear();
    if (m_cursortrabajadores != NULL)
        delete m_cursortrabajadores;
    m_cursortrabajadores = m_companyact->cargacursor("SELECT * FROM trabajador ORDER BY apellidostrabajador");
    while (!m_cursortrabajadores->eof()) {
        new QListWidgetItem(m_cursortrabajadores->valor("apellidostrabajador") + " " + m_cursortrabajadores->valor("nomtrabajador"), mui_lista);
        m_cursortrabajadores->siguienteregistro();
    } // end while

    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
}


TrabajadorView::~TrabajadorView() {
    _depura("TrabajadorView::~TrabajadorView", 0);
    if (m_cursortrabajadores != NULL)
        delete m_cursortrabajadores;
    m_companyact->sacaWindow(this);
    _depura("END TrabajadorView::~TrabajadorView", 0);
}


void TrabajadorView::on_mui_lista_currentItemChanged(QListWidgetItem *cur, QListWidgetItem *) {
    _depura( "on_mui_lista_currentItemChanged", 0);

    int row = mui_lista->row(cur);
    trataModificado();
    m_nomtrabajador->setText(m_cursortrabajadores->valor("nomtrabajador", row));
    mdb_idtrabajador = m_cursortrabajadores->valor("idtrabajador", row);
    m_apellidostrabajador->setText(m_cursortrabajadores->valor("apellidostrabajador", row));
    m_nsstrabajador->setText(m_cursortrabajadores->valor("nsstrabajador", row));
    m_dirtrabajador->setText(m_cursortrabajadores->valor("dirtrabajador", row));
    m_teltrabajador->setText(m_cursortrabajadores->valor("teltrabajador", row));
    m_moviltrabajador->setText(m_cursortrabajadores->valor("moviltrabajador", row));
    m_emailtrabajador->setText(m_cursortrabajadores->valor("emailtrabajador", row));
    if ( m_cursortrabajadores->valor("activotrabajador", row) == "t") {
        m_activotrabajador->setChecked(TRUE);
    } else {
        m_activotrabajador->setChecked(FALSE);
    } // end if
    m_item = cur;

    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    m_imagen->setPixmap(QPixmap(confpr->valor(CONF_DIR_IMG_PERSONAL) + mdb_idtrabajador + ".jpg"));
}


void TrabajadorView::on_mui_guardar_clicked() {
    QString m_textactivotrabajador = "FALSE";
    if (m_activotrabajador->isChecked())
        m_textactivotrabajador = "TRUE";
    QString query = "UPDATE trabajador SET ";
    query += "  nomtrabajador='" + m_companyact->sanearCadena(m_nomtrabajador->text()) + "'";
    query += ", apellidostrabajador= '" + m_companyact->sanearCadena(m_apellidostrabajador->text()) + "'";
    query += ", nsstrabajador = '" + m_companyact->sanearCadena(m_nsstrabajador->text()) + "'";
    query += ", dirtrabajador = '" + m_companyact->sanearCadena(m_dirtrabajador->text()) + "'";
    query += ", teltrabajador = '" + m_companyact->sanearCadena(m_teltrabajador->text()) + "'";
    query += ", moviltrabajador = '" + m_companyact->sanearCadena(m_moviltrabajador->text()) + "'";
    query += ", emailtrabajador = '" + m_companyact->sanearCadena(m_emailtrabajador->text()) + "'";
    query += ", activotrabajador = " + m_companyact->sanearCadena(m_textactivotrabajador);
    query += " WHERE idtrabajador=" + m_companyact->sanearCadena(mdb_idtrabajador);

    int error = m_companyact->ejecuta(query);
    if (error) {
        m_companyact->rollback();
        return;
    } // end if

    if (m_cursortrabajadores != NULL)
        delete m_cursortrabajadores;
    m_cursortrabajadores = m_companyact->cargacursor("SELECT * FROM trabajador ORDER BY apellidostrabajador");

    if (m_item)
        m_item->setText(m_apellidostrabajador->text() + m_nomtrabajador->text());

    if (m_archivoimagen != "") {
        QString cadena = "cp " + m_archivoimagen + " " + confpr->valor(CONF_DIR_IMG_PERSONAL) + mdb_idtrabajador + ".jpg";
        system( cadena.toAscii().constData());
    } // end if

    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
}


bool TrabajadorView::trataModificado() {
    _depura( "TrabajadorView::trataModificado\n", 0);
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar datos del trabajador"),
                                 tr("Desea guardar los cambios?"),
                                 tr("&Si"), tr("&No"), 0, 0, 1 ) == 0)
            on_mui_guardar_clicked();
        return (TRUE);
    } // end if
    return (FALSE);
}


/// SLOT que responde a la pulsacion del boton de nuevo tipo de iva.
/// Inserta en la tabla de ivas
void TrabajadorView::on_mui_nuevo_clicked() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    trataModificado();
    QString query = "INSERT INTO trabajador (nomtrabajador, apellidostrabajador, nsstrabajador) VALUES ('NUEVO TRABAJADOR','NUEVO TRABAJADOR','000000000000')";
    m_companyact->begin();
    int error = m_companyact->ejecuta(query);
    if (error) {
        m_companyact->rollback();
        return;
    } // end if
    cursor2 *cur = m_companyact->cargacursor("SELECT max(idtrabajador) AS idtrabajador FROM trabajador");
    m_companyact->commit();
    mdb_idtrabajador = cur->valor("idtrabajador");
    delete cur;
    pintar();
}


/// SLOT que responde a la pulsacion del boton de borrar la familia que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
void TrabajadorView::on_mui_borrar_clicked() {
    trataModificado();
    m_companyact->begin();
    QString query = "DELETE FROM trabajador WHERE idtrabajador=" + mdb_idtrabajador;
    int error = m_companyact->ejecuta(query);
    if (error) {
        m_companyact->rollback();
        return;
    } // end if
    m_companyact->commit();
    pintar();
}


void TrabajadorView::on_mui_imagen_clicked() {
    m_archivoimagen = QFileDialog::getOpenFileName(this,
                      tr("Seleccione archivo"),
                      "",
                      tr("Imagenes (*.jpg)"));
    m_imagen->setPixmap(QPixmap(m_archivoimagen));
}

