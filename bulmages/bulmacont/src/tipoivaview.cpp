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

#include <QWidget>

#include "tipoivaview.h"
#include "empresa.h"
#include "busquedacuenta.h"


/// El constructor de la clase prepara las variables globales y llama a la función pintar.ç
tipoivaview::tipoivaview(Empresa *emp, QWidget *parent)
        : FichaBc(emp, parent) {
    _depura("tipoivaview::tipoivaview", 0);

    /// Establecemos cual es la tabla en la que basarse para los permisos
    setDBTableName("tipo_iva");

    this->setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    m_codigoCtaTipoIVA->setEmpresaBase(emp);
    m_curtipoiva = NULL;
    dialogChanges_cargaInicial();
    pintar();
    meteWindow(windowTitle(), this);
    _depura("END tipoivaview::tipoivaview", 0);
}


/// El destructor de la clase guarda los datos (por si ha habido cambios)
/// y libera la memoria que se haya ocupado.
tipoivaview::~tipoivaview() {
    _depura("tipoivaview::~tipoivaview", 0);
    on_mui_guardar2_clicked();
    if (m_curtipoiva != NULL)
        delete m_curtipoiva;
    empresaBase()->sacaWindow(this);
    _depura("END tipoivaview::~tipoivaview", 0);
}


/// Pinta la ventana, recarga el combo y si se pasa el parametro muestra el identificador
/// indicado.
void tipoivaview::pintar(QString idtipoiva) {
    _depura("tipoivaview::pintar", 0);
    int posicion = 0;
    /// Vamos a inicializar el combo de los tipos de IVA.
    if (m_curtipoiva != NULL)
        delete m_curtipoiva;
    QString query = "SELECT * from tipoiva left join cuenta ON tipoiva.idcuenta = cuenta.idcuenta ORDER BY nombretipoiva";
    m_curtipoiva = empresaBase()->cargacursor(query);
    mui_comboTipoIVA->clear();
    int i = 0;
    while (!m_curtipoiva->eof()) {
        mui_comboTipoIVA->insertItem(i, m_curtipoiva->valor("nombretipoiva"));
        if (idtipoiva == m_curtipoiva->valor("idtipoiva") )
            posicion = i;
        m_curtipoiva->siguienteregistro();
        i++;
    } // end while
    _depura("END tipoivaview::pintar", 0);
}


/// Esta funci&oacute;n muestra el tipo de IVA en la ventana.
/** \param pos, si es distinto de cero se busca en el combo la posici&oacute;n indicada
    sino se usa la posición actual del combo. */
void tipoivaview::mostrarplantilla(int pos) {
    _depura("tipoivaview::mostrarplantilla", 0);
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar tipo de IVA"),
                                 tr("Desea guardar los cambios?"),
                                 QMessageBox::Ok,
                                 QMessageBox::Cancel) == QMessageBox::Ok)
            on_mui_guardar2_clicked();
    } // end if
    if (mui_comboTipoIVA->count() > 0) {
        if (pos != 0)
            mui_comboTipoIVA->setCurrentIndex(pos);
        m_posactual = mui_comboTipoIVA->currentIndex();
        m_nombreTipoIVA->setText(m_curtipoiva->valor("nombretipoiva", m_posactual));
        m_codigoCtaTipoIVA->setText(m_curtipoiva->valor("codigo", m_posactual));
        m_porcentTipoIVA->setText(m_curtipoiva->valor("porcentajetipoiva", m_posactual));
        /// Comprobamos cual es la cadena inicial.
        dialogChanges_cargaInicial();
    } // end if
    _depura("END tipoivaview::mostrarplantilla", 0);
}


/// Esta funci&oacute;n sirve para hacer el cambio sobre un centro de coste .
void tipoivaview::on_mui_comboTipoIVA_currentIndexChanged(int) {
    _depura("tipoivaview::on_mui_comboTipoIVA_currentIndexChanged", 0);
    mostrarplantilla();
    _depura("END tipoivaview::on_mui_comboTipoIVA_currentIndexChanged", 0);
}


/// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n de guardar el tipo de IVA
/// que se est&aacute; editando.
/** Lo que hace es que se hace una actualizaci&oacute;n de todos los campos. */
void tipoivaview::on_mui_guardar2_clicked() {
    _depura("tipoivaview::on_mui_guardar2_clicked", 0);
    QString idtipoiva = m_curtipoiva->valor("idtipoiva", m_posactual);
    QString query = "UPDATE tipoiva SET nombretipoiva = '" + m_nombreTipoIVA->text() + "', porcentajetipoiva = " + m_porcentTipoIVA->text() + " , idcuenta = id_cuenta('" + m_codigoCtaTipoIVA->text() + "') WHERE idtipoiva = " + m_curtipoiva->valor("idtipoiva", m_posactual);
    empresaBase()->ejecuta(query);
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    pintar(m_curtipoiva->valor("idtipoiva", m_posactual));
    _depura("END tipoivaview::on_mui_guardar2_clicked", 0);
}


/// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n de nuevo tipo de IVA.
/** Inserta en la tabla de IVAs. */
void tipoivaview::on_mui_nuevo2_clicked() {
    _depura("tipoivaview::on_mui_nuevo2_clicked()", 0);
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar tipo de IVA"),
                                 tr("Desea guardar los cambios?"),
                                 QMessageBox::Ok,
                                 QMessageBox::Cancel ) == QMessageBox::Ok)
            on_mui_guardar2_clicked();
    } // end if
    QString query = "INSERT INTO tipoiva (nombretipoiva, porcentajetipoiva, idcuenta) VALUES ('NUEVO TIPO IVA', 0, id_cuenta('47'))";
    empresaBase()->begin();
    empresaBase()->ejecuta(query);
    cursor2 *cur = empresaBase()->cargacursor("SELECT max(idtipoiva) AS idtipoiva FROM tipoiva");
    empresaBase()->commit();
    pintar(cur->valor("idtipoiva"));
    delete cur;
    _depura("END tipoivaview::on_mui_nuevo2_clicked()", 0);
}


/// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n de borrar un tipo de IVA.
/** Borra en la tabla de tiposiva el TIPO de IVA concreto. */
void tipoivaview::on_mui_borrar2_clicked() {
    _depura("tipoivaview::on_mui_borrar2_clicked", 0);
    switch (QMessageBox::warning(this,
                                 tr("Borrar tipo de IVA"),
                                 tr("Se va a borrar el tipo de IVA. \nEsto puede ocasionar perdida de datos."),
                                 QMessageBox::Ok,
                                 QMessageBox::Cancel)) {
    case QMessageBox::Ok: /// Retry clicked or Enter pressed.
        empresaBase()->ejecuta("DELETE FROM tipoiva WHERE idtipoiva = " + m_curtipoiva->valor("idtipoiva", mui_comboTipoIVA->currentIndex()));
        pintar();
        break;
    case QMessageBox::Cancel: /// Abort clicked or Escape pressed.
        break;
    } // end switch
    _depura("END tipoivaview::on_mui_borrar2_clicked", 0);
}


/// Antes de salir de la ventana debemos hacer la comprobaci&oacute;n de si se ha
/// modificado algo.
/** Esta función está dedicada a Francina, Bienvenida al mundo. :) */
bool tipoivaview::close() {
    _depura("tipoivaview::close", 0);
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar tipo de IVA"),
                                 tr("Desea guardar los cambios?"),
                                 QMessageBox::Ok,
                                 QMessageBox::Cancel) == QMessageBox::Ok)
            on_mui_guardar2_clicked();
    } // end if
    _depura("END tipoivaview::close", 0);
    return QWidget::close();
}

