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
    m_comboFPago->clear();
    int i = 0;
    while (!m_curfpago->eof()) {
        m_comboFPago->insertItem(i, m_curfpago->valor("nomfpago"));
        if (idfpago == m_curfpago->valor("idfpago") )
            posicion = i;
        m_curfpago->siguienteregistro();
        i++;
    } // end while
    mostrarplantilla(posicion);
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
    if (m_comboFPago->count() > 0) {
        if (pos != 0) {
            m_comboFPago->setCurrentIndex(pos);
        } // end if
        m_posactual = m_comboFPago->currentIndex();
        m_nomFPago->setText(m_curfpago->valor("nomfpago", m_posactual));
        m_plazoPrimerPagoFPago->setText(m_curfpago->valor("plazoprimerpagofpago", m_posactual));
        m_nPlazosFPago->setText(m_curfpago->valor("nplazosfpago", m_posactual));
        m_tipoPlazoPrimerPago->setText(m_curfpago->valor("tipoplazoprimerpagofpago", m_posactual));
        m_plazoEntreReciboFPago->setText(m_curfpago->valor("plazoentrerecibofpago", m_posactual));
        m_tipoPlazoEntreReciboFPago->setText(m_curfpago->valor("tipoplazoentrerecibofpago", m_posactual));
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
void fpagoview::on_mui_guardar_clicked() {
    QString idfpago = m_curfpago->valor("idfpago", m_posactual);
    QString query = "UPDATE fpago SET nomfpago = '" + m_nomFPago->text() + "', nplazosfpago = " + m_nPlazosFPago->text() + " , plazoprimerpagofpago = " + m_plazoPrimerPagoFPago->text() + ", plazoentrerecibofpago = " + m_plazoEntreReciboFPago->text() + " WHERE idfpago = " + m_curfpago->valor("idfpago", m_posactual);
    empresaactual->ejecuta(query);
    dialogChanges_cargaInicial();
    pintar(m_curfpago->valor("idfpago", m_posactual));
}


/// SLOT que responde a la pulsación del botón de nuevo tipo de IVA. Inserta en la tabla
/// de IVAs.
void fpagoview::on_mui_crear_clicked() {
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
void fpagoview::on_mui_borrar_clicked() {
    switch (QMessageBox::warning(this,
                                 tr("Borrar forma de pago"),
                                 tr("Se va a borrar la forma de pago.\n \
                                    Esto puede ocasionar perdida de datos.\n \
                                    Tal vez deberia pensarselo mejor antes\n \
                                    porque igual su trabajo se pierde."),
                                 QMessageBox::Ok, QMessageBox::Cancel)) {
    case QMessageBox::Ok: /// Retry clicked or Enter pressed.
        empresaactual->ejecuta("DELETE FROM fpago WHERE idfpago = " + m_curfpago->valor("idfpago", m_comboFPago->currentIndex()));
        pintar();
        break;
    case QMessageBox::Cancel: /// Abort clicked or Escape pressed.
        break;
    } // end switch
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

