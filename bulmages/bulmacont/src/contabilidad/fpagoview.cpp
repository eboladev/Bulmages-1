/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include "fpagoview.h"
#include "empresa.h"


/// El constructor de la clase prepara las variables globales y llama a la funci&oacute;n
/// pintar.
fpagoview::fpagoview(empresa *emp, QWidget *parent)
        : Ficha(parent) {
    _depura("fpagoview::fpagoview", 0);
    this->setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    empresaactual = emp;
    m_curfpago = NULL;
    dialogChanges_cargaInicial();
    pintar();
    emp->meteWindow(windowTitle(), this);
    _depura("END fpagoview::fpagoview", 0);
}


/// El destructor de la clase guarda los datos (por si ha habido cambios) y libera
/// la memoria que se haya ocupado. */
fpagoview::~fpagoview() {
    on_mui_guardar_clicked();
    if (m_curfpago != NULL) {
        delete m_curfpago;
    } /// end if
    empresaactual->sacaWindow(this);
}


/// Pinta la ventana, recarga el combo y si se pasa el par&aacute;metro muestra
/// el identificador indicado. */
void fpagoview::pintar(QString idfpago) {
    int posicion = 0;
    /// Vamos a inicializar el combo de los tipos de IVA.
    if (m_curfpago != NULL)
        delete m_curfpago;
    QString query = "SELECT * from fpago ORDER BY nomfpago";
    m_curfpago = empresaactual->cargacursor(query);
    mui_comboFPago->clear();
    int i = 0;
    while (!m_curfpago->eof()) {
        mui_comboFPago->insertItem(i, m_curfpago->valor("nomfpago"));
        if (idfpago == m_curfpago->valor("idfpago"))
            posicion = i;
        m_curfpago->siguienteregistro();
        i++;
    } // end while

    if (mui_comboFPago->currentIndex() == -1) {
        mui_nombreFPago->setEnabled(FALSE);
        mui_plazoPrimerPago->setEnabled(FALSE);
        mui_numeroPlazos->setEnabled(FALSE);
        mui_tipoPlazoPrimerPago->setEnabled(FALSE);
        mui_plazoEntreRecibos->setEnabled(FALSE);
        mui_tipoPlazoEntreRecibos->setEnabled(FALSE);

        mui_nombreFPago->setText("");
        mui_plazoPrimerPago->setText("");
        mui_numeroPlazos->setText("");
        mui_tipoPlazoPrimerPago->setText("");
        mui_plazoEntreRecibos->setText("");
        mui_tipoPlazoEntreRecibos->setText("");

    } else {
        mui_nombreFPago->setEnabled(TRUE);
        mui_plazoPrimerPago->setEnabled(TRUE);
        mui_numeroPlazos->setEnabled(TRUE);
        mui_tipoPlazoPrimerPago->setEnabled(TRUE);
        mui_plazoEntreRecibos->setEnabled(TRUE);
        mui_tipoPlazoEntreRecibos->setEnabled(TRUE);

        mostrarplantilla(posicion);
    } // end if
}


/// Esta funci&oacute;n muestra la forma de pago en la ventana.
/** \param pos si es distinto de cero se busca en el combo la posici&oacute;n
    indicada sino se usa la posici&oacute;n actual del combo. */
void fpagoview::mostrarplantilla(int pos) {
    _depura("fpagoview::mostrarplantilla", 0);
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar forma de pago"),
                                 tr("Desea guardar los cambios?"),
                                 QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok)
            on_mui_guardar_clicked();
    } // end if
    if (mui_comboFPago->count() > 0) {
        if (pos != 0) {
            mui_comboFPago->setCurrentIndex(pos);
        } // end if
        m_posactual = mui_comboFPago->currentIndex();
        mui_nombreFPago->setText(m_curfpago->valor("nomfpago", m_posactual));
        mui_plazoPrimerPago->setText(m_curfpago->valor("plazoprimerpagofpago", m_posactual));
        mui_numeroPlazos->setText(m_curfpago->valor("nplazosfpago", m_posactual));
        mui_tipoPlazoPrimerPago->setText(m_curfpago->valor("tipoplazoprimerpagofpago", m_posactual));
        mui_plazoEntreRecibos->setText(m_curfpago->valor("plazoentrerecibofpago", m_posactual));
        mui_tipoPlazoEntreRecibos->setText(m_curfpago->valor("tipoplazoentrerecibofpago", m_posactual));
        /// Comprobamos cual es la cadena inicial.
        dialogChanges_cargaInicial();
    } // end if
    _depura("END fpagoview::mostrarplantilla", 0);
}


/// Esta funci&oacute;n sirve para hacer el cambio sobre un centro de coste.
void fpagoview::cambiacombo(int) {
    _depura("fpagoview::cambiacombo", 0);
    mostrarplantilla();
    _depura("END fpagoview::cambiacombo", 0);
}


/// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n de guardar el tipo de
/// IVA que se est&aacute; editando.
/** Lo que hace es que se hace un update de todos los campos. */
void fpagoview::on_mui_guardarFPago_clicked() {
    QString idfpago = m_curfpago->valor("idfpago", m_posactual);
    QString query = "UPDATE fpago SET nomfpago = '" + mui_nombreFPago->text() + "', nplazosfpago = " + mui_numeroPlazos->text() + " , plazoprimerpagofpago = " + mui_plazoPrimerPago->text() + ", plazoentrerecibofpago = " + mui_plazoEntreRecibos->text() + " WHERE idfpago = " + m_curfpago->valor("idfpago", m_posactual);
    empresaactual->ejecuta(query);
    dialogChanges_cargaInicial();
    pintar(m_curfpago->valor("idfpago", m_posactual));
}


/// SLOT que responde a la pulsación del botón de nuevo tipo de IVA. Inserta en la tabla
/// de IVAs.
void fpagoview::on_mui_crearFPago_clicked() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar forma de pago"),
                                 tr("Desea guardar los cambios?"),
                                 QMessageBox::Ok,
                                 QMessageBox::Cancel) == QMessageBox::Ok)
            on_mui_guardar_clicked();
    } // end if
    QString query = "INSERT INTO fpago (nomfpago, nplazosfpago, plazoprimerpagofpago, plazoentrerecibofpago) VALUES ('" + tr("Nueva forma de pago") + "', 0, 0, 0)";
    empresaactual->begin();
    empresaactual->ejecuta(query);
    cursor2 *cur = empresaactual->cargacursor("SELECT max(idfpago) AS idfpago FROM fpago");
    empresaactual->commit();
    pintar(cur->valor("idfpago"));
    delete cur;
}


/// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n de borrar un tipo de IVA.
/** Borra en la tabla de tiposiva el TIPO de IVA concreto. */
void fpagoview::on_mui_borrarFPago_clicked() {
    if (mui_comboFPago->currentIndex() == -1) {
        mensajeInfo(tr("Tiene que seleccionar una forma de pago antes de borrarla"));
        return;
    } else {
        switch (QMessageBox::warning(this,
                                     tr("Borrar forma de pago"),
                                     tr("Se va a borrar la forma de pago.\nEsto puede ocasionar perdida de datos.\nTal vez deberia pensarselo mejor antes\nporque igual su trabajo se pierde."),
                                     QMessageBox::Ok, QMessageBox::Cancel)) {
        case QMessageBox::Ok: /// Retry clicked or Enter pressed.
            empresaactual->ejecuta("DELETE FROM fpago WHERE idfpago = " + m_curfpago->valor("idfpago", mui_comboFPago->currentIndex()));
            pintar();
            break;
        case QMessageBox::Cancel: /// Abort clicked or Escape pressed.
            break;
        } // end switch
    } // end if
}


/// Antes de salir de la ventana debemos hacer la comprobaci&oacute;n de si se ha
/// modificado algo.
bool fpagoview::close() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar forma de pago"),
                                 tr("Desea guardar los cambios?"),
                                 QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok)
            on_mui_guardar_clicked();
    } // end if
    return QWidget::close();
}

