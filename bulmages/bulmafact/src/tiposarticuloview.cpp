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

#include <QMap>
#include <QLineEdit>
#include <QMessageBox>

#include "tiposarticuloview.h"
#include "company.h"
#include "funcaux.h"

#define COL_IDTIPOARTICULO    0
#define COL_CODTIPOARTICULO   1
#define COL_DESCTIPOARTICULO  2


TipoArticuloList::TipoArticuloList(company *comp, QWidget *parent, bool modoConsulta)
        : Ficha(parent) {
    _depura("TipoArticuloList::TipoArticuloList", 0);
    setupUi(this);
    companyact = comp;
    m_listTipos->setColumnCount(3);
    QStringList headers;
    headers << tr("Id") << tr("Codigo") << tr("Descripcion");
    m_listTipos->setHeaderLabels(headers);
    m_idtipo = "";
    if (modoConsulta) {
       setModoConsulta();
       groupBox1->hide();
       mui_detalles->hide();
       mui_crear->hide();
       mui_guardar->hide();
       mui_borrar->hide();
       mui_cancelar->hide();
       mui_aceptar->hide();
    } else {
        setModoEdicion();
       setAttribute(Qt::WA_DeleteOnClose);
       companyact->meteWindow(windowTitle(), this);
    } // end if
    pintar();
    _depura("END TipoArticuloList::TipoArticuloList", 0);
}


TipoArticuloList::~TipoArticuloList() {
    _depura("TipoArticuloList::~TipoArticuloList", 0);
    _depura("END TipoArticuloList::~TipoArticuloList", 0);
}


void TipoArticuloList::setModoConsulta() {
    m_modoConsulta = TRUE;
}


void TipoArticuloList::setModoEdicion() {
    m_modoConsulta = FALSE;
}


void TipoArticuloList::on_mui_aceptar_clicked() {
    close();
}


int TipoArticuloList::sacaWindow() {
    companyact->sacaWindow(this);
    return 0;
}


void TipoArticuloList::pintar() {
    QTreeWidgetItem * it;

    /// vaciamos el arbol.
    while (m_listTipos->topLevelItemCount() > 0) {
        it = m_listTipos->takeTopLevelItem(0);
        delete it;
    } // end while

    cursor2 *cursoraux1 = companyact->cargacursor("SELECT * FROM tipo_articulo ORDER BY codtipo_articulo");
    while (!cursoraux1->eof()) {
        it = new QTreeWidgetItem(m_listTipos);
        it->setText(COL_IDTIPOARTICULO, cursoraux1->valor("idtipo_articulo"));
        it->setText(COL_CODTIPOARTICULO,cursoraux1->valor("codtipo_articulo"));
        it->setText(COL_DESCTIPOARTICULO, cursoraux1->valor("desctipo_articulo"));
        //it->setOpen(true);
        cursoraux1->siguienteregistro();
    } // end while
    delete cursoraux1;
    m_idtipo = "";
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
}


QString TipoArticuloList::codtipo_articulo() {
    _depura("TipoArticuloList::codtipo_articulo", 0);
    QTreeWidgetItem *it = m_listTipos->currentItem();
    if (it) {
        return it->text(COL_CODTIPOARTICULO);
    } else {
       return "";
    } // end if
    _depura("END TipoArticuloList::codtipo_articulo", 0);
}

QString TipoArticuloList::idtipo_articulo() {
    _depura("TipoArticuloList::idtipo_articulo", 0);
    QTreeWidgetItem *it = m_listTipos->currentItem();
    if (it) {
        return it->text(COL_IDTIPOARTICULO);
    } else {
       return "";
    } // end if
    _depura("TipoArticuloList::idtipo_articulo", 0);
}

QString TipoArticuloList::desctipo_articulo() {
    _depura("TipoArticuloList::desctipo_articulo", 0);
    QTreeWidgetItem *it = m_listTipos->currentItem();
    if (it) {
       return it->text(COL_DESCTIPOARTICULO);
    } else {
       return "";
    } // end if
    _depura("END TipoArticuloList::desctipo_articulo", 0);
}


/// Se ha seleccionado un item en la lista
/// Lo que hacemos es mostar el elemento
/// Si el anterior ha sido modificado pedimos para actuar en consecuencia.
void TipoArticuloList::on_m_listTipos_itemDoubleClicked(QTreeWidgetItem *item, int) {
    if (m_modoConsulta) {
        m_idtipo = item->text(COL_IDTIPOARTICULO);
        emit selected(m_idtipo);
    } // end if
}


/// Se ha seleccionado un item en la lista
/// Lo que hacemos es mostar el elemento
/// Si el anterior ha sido modificado pedimos para actuar en consecuencia.
void TipoArticuloList::on_m_listTipos_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *) {
    _depura("TipoArticuloList::on_m_listTipos_itemChanged", 0);
    QString idtipoold = current->text(COL_IDTIPOARTICULO);
    if (idtipoold != "") {
        /// Si usamos el trataModificado peta porque si se guarda se sobreescribe el puntero it.
        trataModificado();
        m_idtipo = idtipoold;
        mostrarplantilla();
    } // end if
}


void TipoArticuloList::mostrarplantilla() {
    _depura("TipoArticuloList::mostrarplantilla", 0);
    QString query;
    query = "SELECT * from tipo_articulo WHERE idtipo_articulo = " + m_idtipo;
    cursor2 *cursortipo = companyact->cargacursor(query);
    if (!cursortipo->eof()) {
        m_codTipo->setText(cursortipo->valor("codtipo_articulo"));
        m_descTipo->setPlainText(cursortipo->valor("desctipo_articulo"));
    } // end if
    delete cursortipo;
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    _depura("END TipoArticuloList::mostrarplantilla", 0);
}


/// Antes de salir de la ventana debemos hacer la comprobacion de si se ha modificado algo
/// Esta funcion esta dedicada a Francina, Bienvenida al mundo
void TipoArticuloList::close() {
    trataModificado();
    QWidget::close();
}


bool TipoArticuloList::trataModificado() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar familia"),
                                 tr("Desea guardar los cambios?"),
                                 QMessageBox::Ok,
                                 QMessageBox::Cancel) == QMessageBox::Ok)
            on_mui_guardar_clicked();
        return TRUE;
    } // end if
    return FALSE;
}


/// SLOT que responde a la pulsacion del boton de guardar el tipo de iva que se esta editando.
/// Lo que hace es que se hace un update de todos los campos
void TipoArticuloList::on_mui_guardar_clicked() {
    QString query = "UPDATE tipo_articulo SET codtipo_articulo = '" +
                    companyact->sanearCadena(m_codTipo->text()) + "', desctipo_articulo = '" +
                    companyact->sanearCadena(m_descTipo->toPlainText()) + "' WHERE idtipo_articulo = " + m_idtipo;
    companyact->begin();
    int error = companyact->ejecuta(query);
    if (error) {
        companyact->rollback();
        return;
    } // end if
    companyact->commit();
    //pintar();

    /// Vamos a hacer algo no reentrante.
    QList<QTreeWidgetItem *> listit =  m_listTipos->findItems(m_idtipo, Qt::MatchExactly, COL_IDTIPOARTICULO);
    QTreeWidgetItem *it = listit.first();
    cursor2 *cursoraux1 = companyact->cargacursor("SELECT * FROM tipo_articulo WHERE idtipo_articulo = " + m_idtipo);
    if (!cursoraux1->eof()) {
        it->setText(COL_IDTIPOARTICULO, cursoraux1->valor("idtipo_articulo"));
        it->setText(COL_CODTIPOARTICULO,cursoraux1->valor("codtipo_articulo"));
        it->setText(COL_DESCTIPOARTICULO, cursoraux1->valor("desctipo_articulo"));
    } // end if
    delete cursoraux1;
}


/// SLOT que responde a la pulsacion del boton de nuevo tipo de iva
/// Inserta en la tabla de ivas
void TipoArticuloList::on_mui_crear_clicked() {
    _depura("TipoArticuloList::on_mui_crear_clicked", 0);
    /// Si se ha modificado el contenido advertimos y guardamos.
    trataModificado();

    QString query = "INSERT INTO tipo_articulo (codtipo_articulo, desctipo_articulo) VALUES ('XXXXXX', 'Descripcion del tipo')";
    companyact->begin();
    int error = companyact->ejecuta(query);
    if (error) {
        companyact->rollback();
        return;
    } // end if
    cursor2 *cur = companyact->cargacursor("SELECT max(idtipo_articulo) AS idtipo FROM tipo_articulo");
    companyact->commit();
    m_idtipo = cur->valor("idtipo");
    delete cur;
    pintar();
}

/// SLOT que responde a la pulsacion del boton de borrar la familia que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
void TipoArticuloList::on_mui_borrar_clicked() {
    _depura("TipoArticuloList::on_mui_borrar_clicked", 0);
    QTreeWidgetItem *it = m_listTipos->currentItem();
    if (!it)
        return;
    trataModificado();
    QString query = "DELETE FROM tipo_articulo WHERE idtipo_articulo = " + m_idtipo;
    int error = companyact->ejecuta(query);
    if (error) {
        return;
    } // end if
    it = m_listTipos->takeTopLevelItem(m_listTipos->indexOfTopLevelItem(m_listTipos->currentItem()));
    delete it;
    _depura("END TipoArticuloList::on_mui_borrar_clicked", 0);
}

